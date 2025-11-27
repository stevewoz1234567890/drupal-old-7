<?php
/**
 * Abstracted weekly one-time function execution.
 *
 * @param string $day
 * @param string $callback
 * @return void
 */
function tal_episode_megaphone_cron_weekly($day = 'Thursday', $callback = 'stub') {
    //  Once a week first thing on thursday.
    $now = date("l-Y.z");
    [$weekday, $yearday] = explode("-", $now);
    $lastExecution = variable_get("megaphone_cron_$callback", false);
    if (
        ($lastExecution === false) ||//                              If this has never been executed go ahead and do it now
        ( ($weekday == $day) && ($yearday > $lastExecution) )//      Otherwise wait until Thursday
    ) {
        // Run callback function.
        $func = "tal_episode_megaphone_cron_$callback";
        $func();
        // Save execution day so we only run once.
        // TODO: Do not set if callback fails and we have not tried a certain number of times
        variable_set("megaphone_cron_$callback", $yearday);
    }
}

/**
 * Episode Air Date & Megaphone episode stub creation.
 *    - Find all schedules in the future up until 1 year away
 *    - Step through each Friday during that time period and make sure a schedule node exists
 *    - If not, create a new Schedule Air Date node for that date
 *
 * @return void
 */
function tal_episode_megaphone_cron_stub() {

    // Load next year of Episode Air Date nodes
    $startTime = strtotime("today");
    $startDate = date("Y-m-d\TH:i:s", $startTime);
    $endTime = strtotime('today + 1 year + 1 week');
    $endDate = date("Y-m-d\TH:i:s", $endTime);
    $query = db_select('node', 'n');
    $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
    $query
        ->fields('n', array('nid'))
        ->addExpression("SUBSTRING(rad.field_radio_air_date_value, 1, 10)", "air_date");
    $query->condition('n.type', 'schedule')
        ->condition('rad.field_radio_air_date_value', $endDate, '<=')
        ->condition('rad.field_radio_air_date_value', $startDate, '>=');
    $allAirDates = $query->execute()->fetchAllKeyed();
    $allAirDates = array_flip($allAirDates);

    // Find any missed Fridays
    $fridayTime = strtotime('this friday');
    $friday = DateTime::createFromFormat('U', $fridayTime);
    $stubs = [];
    while ($fridayTime < $endTime) {
        $scheduleDate = $friday->format('Y-m-d');
        if (!isset($allAirDates[$scheduleDate])) $stubs[] = $scheduleDate;
        $friday->modify('+1 week');
        $fridayTime = $friday->format('U');
    }

    // Create stub queue items for each missed Friday
    if (count($stubs) > 0) {
        $stubQ = new TalEpisodeQueue('tal_episode_stub');
        foreach ($stubs as $date) {
            //  Create Schedule node
            $datetime = date_create($date);
            $schedule = new stdClass();
            $schedule->type = 'schedule';
            $schedule->status = 0;
            $schedule->title = "Scheduled Air Date: ". $datetime->format("D M j, Y");
            $schedule->field_radio_air_date[LANGUAGE_NONE][0]['value'] = $datetime->format("Y-m-d\\T") . "20:00:00";
            $schedule->language = LANGUAGE_NONE;
            node_object_prepare($schedule);
            node_save($schedule);
        }
    }
}

/**
 * Mark podcast episodes for transition to extended audio source in Megaphone.
 *   - Check if the associated Megaphone episode has successfully updated the audio source to extended.  If not, stop.
 *   - Create a new item in tal_episode_audio queue with the node id.
 *
 * @return void
 */
function tal_episode_megaphone_cron_extended() {

    //  Load first schedule node four weeks out
    //  TODO - use tal_episode_podcast_episodes with offset + 1 day, try 5th episode
    //  TODO - watchdog log what steps are taken
    $four_weeks = 24*7*4;
    $endTime = strtotime("Friday 20:00 - $four_weeks hours");
    $endDate = date("c", $endTime);
    $query = db_select('node', 'n');
    $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
    $query
        ->fields('rad', array('field_radio_air_date_value'))
        ->fields('n', array('nid'))
        ->condition('n.type', 'schedule')
        ->condition('rad.field_radio_air_date_value', $endDate, '<')
        ->orderBy('rad.field_radio_air_date_value', 'DESC')
        ->range(0,4);
    $extendedNodes = $query->execute()->fetchAllKeyed();
    $nid = reset($extendedNodes);

    //  Stop if no results.
    if (!$nid) return;

    //  Stop if the associated Megaphone episode already has extended audio source.
    if (
        ($schedule = node_load($nid)) &&
        ($wrapper = entity_metadata_wrapper('node', $schedule)) &&
        ($mid = $wrapper->field_megaphone_episode_id->value()) &&
        ($file = $wrapper->field_audio_extended->value())
    ) {
        $api = new MegaRestClient();
        $api->setNid($nid);
        $megaEp = $api->episode($mid);
        $parts = pathinfo($file['uri']);
        $filename = $parts['filename'] .".". $parts['extension'];
        if (
            ($megaEp->originalFilename == $filename) &&
            (($megaEp->audioFileStatus == 'processing') || ($megaEp->audioFileStatus == 'success'))
        ) return;
    }

	// Create a new item in tal_episode_audio queue with the node id.
    tal_episode_megaphone_initiate_upload($schedule, "extended");
}

/**
 * Sends update about feed changes to NYT.
 *
 * @return void
 */
function tal_episode_megaphone_cron_nyt() {
    // Set up
    $updates = [//                                          Update headers
        'This Week' => false,
        'Falling out of feed' => false,
    ];
    $body = [];//                                           Email body
    $subject = "";//                                        Email subject
    $offset = 24*7;//                                       Offset in hours to get next weeks feed episodes
    $current_week = tal_episode_podcast_episodes();//       Current feed episodes
    $next_week = tal_episode_podcast_episodes($offset);//   Next feed episodes

    // Get current and next node ids
    $diff = array_diff($next_week, $current_week);
    $updates["This Week"] = array_pop($diff);
    $diff = array_diff($current_week, $next_week);
    $updates["Falling out of feed"] = array_pop($diff);

    // Build Email
    foreach ($updates as $head => $nid) {
        if (
            ($episode = node_load($nid)) &&
            ($wrapper = entity_metadata_wrapper('node', $episode)) &&
            ($episode_number = $wrapper->field_episode_number->value())
        ) {
            // Derive Audio URL
            //  use in_feed to determine link -- false means this is the new episode so we need prefix+mega
            if (
                !($episode->in_feed) &&
                ($schedule = tal_episode_get_latest_episode_schedule($nid)) &&
                ($schedule_wrapper = entity_metadata_wrapper('node', $schedule)) &&
                ($megaphone_url = $schedule_wrapper->field_megaphone_episode_url->value())
            ) {
                $audio_url = str_replace('https://', PODCAST_URL_PREFIX, $megaphone_url);
                $subject = "$episode_number: $episode->title";
                $link_before = "";
            } else if (
                ($file = $wrapper->field_audio_nyt->value()) &&
                ($url = file_create_url($file['uri']))
            ) {
                $audio_url = $url;
                $link_before = "New link: ";
            }

            // Compose email body
            if (isset($link_before) && isset($audio_url)) {
                $body[] = "<h2>$head</h2>";
                $body[] = "$episode_number: $episode->title";
                $body[] = "$link_before$audio_url";
                $body[] = "<p></p>";
                $body[] = "<p></p>";
            }
        }
    }

    // Do not complete if new episode doesn't have url
    if (!isset($megaphone_url)) {
        return false;
    }

    // Send Email
    if (
        !empty($body) &&
        ($mailto = variable_get('nyt_email_update_mailto', 'mark@thislife.org'))
    ) {
        $params = [
            'subject' => $subject,
            'body' => $body,
        ];
        drupal_mail('tal_episode', 'tal_episode_megaphone_cron_nyt', $mailto, language_default(), $params);
        $log_message = "':subject' email sent to :emails.";
        watchdog('tal_episode_megaphone_cron_nyt', t($log_message), [':subject' => $subject, ':emails' => $mailto], WATCHDOG_INFO);
    }
}

/**
 * Called by tal_episode_node_presave.
 *
 * 	- create megaphone episode with every new schedule node
 *  - update megaphone episode with every schedule node update
 *
 * @param object $node
 * @return void
 */
function tal_episode_megaphone_node_presave(&$node) {
	if (tal_episode_megaphone_node_sync_status($node)) {
        if ($node->is_new && !isset($node->nid)) {
            // Create Megaphone episode for each new schedule node
            $episode = tal_episode_megaphone_create($node);
        } else {
            // Update Megaphone episode
            $episode = tal_episode_megaphone_update($node);
        }
	}
}

/**
 * Called by tal_episode_node_insert.
 *
 *  This is where node->nid is available (it wasn't previously during hook_node_presave)
 * 	so this is where we'll update megaphone episodes externalId and update the initial
 * 	megaphone_log episodeCreate record with the correct nid
 *
 * @param object $node
 * @return void
 */
function tal_episode_megaphone_node_insert($node) {
    if (tal_episode_megaphone_node_sync_status($node)) {
        //  Update megaphone episode with external Id
        tal_episode_megaphone_update($node);
        //  Update log with nid
        if ($mid = $node->field_megaphone_episode_id[LANGUAGE_NONE][0]['value']) {
            $query = "update {tal_megaphone_log} set nid=:nid where nid=0 and mid=:mid and operation='episodeCreate'";
            $success = db_query($query,[':nid' => $node->nid, ':mid' => $mid])->fetchAll();
        }
    }
}

/**
 * Create new Megaphone episode when given a schedule node
 *
 *      Note: This is the only time that Megaphone pubdate will be written to or altered, all other times it is ignored.
 *
 * @param object $node
 * @return object - on success the Megaphone episode
 */
function tal_episode_megaphone_create(&$node) {
    //  Build Megaphone request
    $date = new DateTime($node->field_radio_air_date[LANGUAGE_NONE][0]['value']);
    $day = $date->format('D');
    if ($day !== 'Sun') $date->modify('Next Sunday');
    $titleDate = $date->format("m.d.Y");
    $attributes = [
        'title' => "This American Life-draft-$titleDate",
        'pubdate' => $date->format('Y-m-d\\TH:i:s.vp')
    ];

    //  Add any other metadata we can glean
    tal_episode_megaphone_extract_node_metadata($node, $attributes, false);

    //  Create Megaphone episode
    $api = new MegaRestClient();
    if (isset($node->nid)) $api->setNid($node->nid);
    $episode = $api->episodeCreate($attributes);
    $api->log();
    if ($episode->id) $node->field_megaphone_episode_id[LANGUAGE_NONE][0]['value'] = $episode->id;
    if ($episode->downloadUrl) $node->field_megaphone_episode_url[LANGUAGE_NONE][0]['value'] = $episode->downloadUrl;
    return $episode;
}

/**
 * Update Megaphone episode based upon changes in given schedule node
 *
 * @param object $node
 * @param string $mid
 * @param array $attributes
 * @return object - on success the Megaphone episode
 */
function tal_episode_megaphone_update($node, $attributes = []) {
    //  Do not attempt megaphone update if no megaphone episode id
    if (
        ($wrapper = entity_metadata_wrapper('node', $node)) &&
        ($mid = $wrapper->field_megaphone_episode_id->value())
    ) {
        //  Get anything that's been updated
        tal_episode_megaphone_extract_node_metadata($node, $attributes);

        //  Update Megaphone episode metadata
        if (count($attributes) > 0) {
            $api = new MegaRestClient();
            if (isset($node->nid)) $api->setNid($node->nid);
            $episode = $api->episodeUpdate($mid, $attributes);
            $api->log();
            return $episode;
        }

    }
}

/**
 * Update Megaphone episode with backgroundAudioFileUrl to initiate upload and create tal_episode_audio queue item to confirm success
 *
 *  - By default, do nothing if upload and processing already successful.
 *
 * @param object $node
 * @param string $fieldName
 * @param boolean $check - set to false to send update without checking for previous success
 * @return void
 */
function tal_episode_megaphone_initiate_upload($node, $fieldName, $check = true) {
    $sync_status = tal_episode_megaphone_node_sync_status($node);
    if (!$sync_status) return ['response' => (object)['error' => 'API sync is turned off.']];

    if (
        ($wrapper = entity_metadata_wrapper('node', $node)) &&
        ($mid = $wrapper->field_megaphone_episode_id->value()) &&
        ($file = $wrapper->{"field_audio_$fieldName"}->value()) &&
        ($file = file_load($file['fid'])) &&
        ($parts = pathinfo($file->uri)) &&
        ($filename = $parts['filename'] ."." . $parts['extension']) &&
        ($file_url = file_create_url($file->uri))
    ) {

        //  Get Megaphone episode
        $api = new MegaRestClient();
        if (isset($node->nid)) $api->setNid($node->nid);
        //  Check to make sure this specific audio file has not already successfully uploaded
        if (
            ($check) &&
            ($episode = $api->episode($mid)) &&
            ($episode->originalUrl == $file_url) &&
            (!$episode->audioFileProcessing) &&
            ($episode->audioFileStatus == 'success')
        ) {
            return [
                'episode' => $episode,
                'response' => (object)["error" => "Megaphone audio source already set to $filename and succesfully uploaded/processed."],
            ];
        }

        //  Send Url to Megaphone
        $episode = $api->episodeInitiateUpload($mid, $file_url);
        $api->log($node->nid, "sent new audio source $fieldName field");

        //  Create queue item to confirm success and update midroll
        if (
            ($episode) &&
            ($q = new TalEpisodeQueue('tal_episode_queue_audio')) &&
            ($qiid = $q->createItem(["nid" =>  $node->nid, "field" => "$fieldName"]))
        ) {
            return [
                'episode' => $episode,
                'response' => $api->response
            ];
        } else {
            return [
                'episode' => $episode,
                'response' => (object)["error" => "Megaphone audio source successfully updated to $filename BUT there was a problem starting the queue."]
            ];
        }

    }
}

/**
 * Given a schedule node, update associated Megaphone episode with midroll values.
 *
 * @param object $node
 * @return void
 */
function tal_episode_megaphone_update_midroll( object $node ) {
    if (!tal_episode_megaphone_node_sync_status($node)) return ['response' => (object)['error' => 'API sync is turned off.']];

    if (
        ($wrapper = entity_metadata_wrapper('node', $node)) &&
        ($mid = $wrapper->field_megaphone_episode_id->value()) &&
        ($midroll = $wrapper->field_midroll->value())
    ) {
        $api = new MegaRestClient();
        if (isset($node->nid)) $api->setNid($node->nid);
        $episode = $api->episodeUpdateMidroll($mid, $midroll);
        $api->log();
        return ['episode' => $episode, 'response' => $api->response];
    }

    return false;
}

/**
 * Implements hook_FORMID_form_alter.
 *
 *    SCHEDULE Edit Form
 *      - Get timestamps and add to Audio tab
 *      - Hide/disable Megaphone fields
 *      - Add Megaphone manual action buttons
 *
 */
function tal_episode_megaphone_form_schedule_node_form_alter(&$form, &$form_state, $form_id) {
  global $user;

  if (
    !empty($form['#node']) &&
    ($nid = $form['#node']->nid)
  ) {


    /**
     * Megaphone API Log
     */
    $query = "select *
    from tal_megaphone_log l
    where l.nid= $nid
    order by l.timestamp desc";
    $rows = db_query($query)->fetchAll();
    $rows_transformed = [];
    foreach ($rows as $row) {
      $rows_transformed[] = [
        date('m/d/y H:i:s', $row->timestamp),
        str_replace("episode","",$row->operation),
        str_replace(":", " ", $row->response)
      ];
    }
    $form['tal_megaphone_log'] = [
      "children" => [
        [ "#markup" => "<h4>Megaphone API Log</h4>" ],
        [
          '#header' => [
            [
              'data' => 'Time',
              'field' => 'timestamp',
            ],
            [
              'data' => 'Operation',
              'field' => 'operation',
            ],
            [
              'data' => 'Response',
              'field' => 'response',
            ],
          ],
          "#rows" => $rows_transformed,
          "#empty" => "There have not been any Megaphone API transactions for this Air Date.",
          '#theme' => 'table',
        ],
      ],
      '#weight' => "85",
      "#parents" => ["group_api_log"],
    ];
    $form['#groups']['group_api_log']->children[] = 'tal_megaphone_log';
    $form['#fieldgroups']['group_api_log']->children[] = 'tal_megaphone_log';
    $form['#group_children']['tal_megaphone_log'] = 'group_api_log';



    /**
     * Megaphone manual buttons
     */
    drupal_add_css(drupal_get_path('module', 'tal_episode') . '/styles/edit-schedule.css');
    $form['tal_megaphone_manual'] = [
      "children" => [
        [ "#markup" => "<h4>Megaphone Manual Operations</h4>" ],
        [ "#markup" => "<hr />" ],
        [ "#markup" => "<div id='manual-operation-results'></div>" ],
        [ "#markup" => "<hr />" ],
        'sync_audio_group' => [
          'sync_audio' => [
            '#type' => 'button',
            '#value' => t('Initiate Upload'),
            '#ajax' => [
              'callback' => 'tal_episode_megaphone_sync_js',
              'wrapper' => 'manual-operation-results',
              'method' => 'prepend',
            ],
          ],
          'sync_audio_choice' => [
            '#type' => 'select',
            '#options' => ['podcast' => 'podcast','extended' => 'extended'],
          ],
          ['#markup' => '<p><em>Please choose an audio file source to begin Megaphone audio sync.</em></p>' ],
          '#prefix' => '<div class="manual-action-group">',
          '#suffix' => '</div>',
          '#tree' => true,
        ],
        [ "#markup" => "<hr />" ],
        'sync_midroll_group' => [
          'sync_midroll' => [
            '#type' => 'button',
            '#value' => t('Update Midroll'),
            '#ajax' => [
              'callback' => 'tal_episode_megaphone_sync_js',
              'wrapper' => 'manual-operation-results',
              'method' => 'prepend',
            ],
          ],
          ['#markup' => '<p><em>Sync saved midroll values with Megaphone. Note this will happen automatically when audio sync is complete.</em></p>' ],
          '#prefix' => '<div class="manual-action-group">',
          '#suffix' => '</div>',
          '#tree' => true,
        ],
      ],
      '#prefix' => '<div class="manual-actions-wrapper">',
      '#suffix' => '</div>',
    ];
    $form['#groups']['group_megaphone_manual']->children[] = 'tal_megaphone_manual';
    $form['#fieldgroups']['group_megaphone_manual']->children[] = 'tal_megaphone_manual';
    $form['#group_children']['tal_megaphone_manual'] = 'group_megaphone_manual';

    /**
     * Megaphone info and link
     */
    if (
      isset($form_state['node']->field_megaphone_episode_id) &&
      ($wrapper = entity_metadata_wrapper('node', $form_state['node'])) &&
      ($mid = $wrapper->field_megaphone_episode_id->value()) &&
      ($org = variable_get('megaphone_organization_id', false))
    ) {
      $api = new MegaRestClient();
      $megaEp = $api->episode($mid);
      $link = "https://cms.megaphone.fm/organizations/$org/podcasts/$megaEp->podcastId/episodes/$megaEp->id/edit";
    }
    $form['tal_megaphone_info'] = [
      "children" => [
        [ "#markup" => "<a href='$link' target='_blank'>Edit the Megaphone Episode here.</a>"],
        [ "#markup" => "<hr />" ],
        [ "#markup" => "<strong>Megaphone Episode Data:</strong>" ],
        [ "#markup" => "<pre>" ],
        [ "#markup" => var_export($megaEp, true) ],
        [ "#markup" => "<pre>" ],
        '#weight' => "20",
        "#parents" => ["group_megaphone"],
        '#prefix' => '<div class="megaphone-info-wrapper">',
        '#suffix' => '</div>',
      ]
    ];
    $form['#groups']['group_megaphone_info']->children[] = 'tal_megaphone_info';
    $form['#fieldgroups']['group_megaphone_info']->children[] = 'tal_megaphone_info';
    $form['#group_children']['tal_megaphone_info'] = 'group_megaphone_info';


  }

	//  Hide/Disable Megaphone API Ids
	$disabled_fields = ['title','field_megaphone_episode_id', 'field_megaphone_episode_url'];
	$disabled_fields_admin = ['title','field_megaphone_episode_id'];
	if (!array_intersect(['sysadmin','admin'], $user->roles)) {
		foreach ($disabled_fields_admin as $key) {
		  $form[$key][LANGUAGE_NONE][0]['value']['#attributes']['disabled'] = 'disabled';
		}
	}
}

/**
 * AJAX schedule node form logic.
 *
 * @param array $form
 * @param array $form_state
 * @return array - drupal render array
 */
function tal_episode_megaphone_sync_js($form, $form_state) {
    // Get needed form values and set defaults
    $output = $form_state['values']['op'];
    $op = str_replace(" ", "_", strtolower($output));
    $node = $form['#node'];
    $fieldName = $form_state['values']['sync_audio_group']['sync_audio_choice'];
    $value = false;
    $error = " has encountered a problem";

    //  Perform specified operation
    switch ($op) {
        case 'initiate_upload':
            if (['episode' => $episode, 'response' => $response] = tal_episode_megaphone_initiate_upload($node, $fieldName, false)) {
                if ($response->error) {
                    $error = "$response->error";
                } else {
                    $value = " new audio file sync initiated from $fieldName audio file.  Check log for updates.";
                }
            } else if (
                ($wrapper = entity_metadata_wrapper('node', $node)) &&
                (!$audio = $wrapper->{"field_audio_$fieldName"}->value())
            ) {
                $error = "There must be an audio file to proceed with sync.";
            }
            break;

        case 'update_midroll':
            if ( ['episode' => $episode, 'response' => $response] = tal_episode_megaphone_update_midroll($node) ) {
                if ($response->error) {
                    $error = "$response->error";
                } else {
                    $insertion = $episode->insertionPoints;
                    $insertion_formatted = implode(", ", $insertion);
                    $value = "new insertion point value: [$insertion_formatted]";
                }
            }
            break;
    }

    //  Compose output message
    $date = date_create($episode->updatedAt);
    $date_formatted = date_format($date, 'H:i:s');
    $output .= ": $date_formatted";
    if ($value) {
        $status_message = $response->status_message;
        $code = $response->code;
        $output .= " - $code $status_message - $value";
    } else {
        $output .= " -- ERROR -- $error";
    }

    //  Send to browser
    return [
        "#markup" => "$output",
        '#prefix' => '<div class="result">',
        '#suffix' => '</div>',
    ];
}

/**
 * Check to make sure node is set to sync with Megaphone API.
 *
 *  There's two ways to turn off sync:
 *      - programmatically add object property skipMegaphoneSync to schedule node before save
 *      - check field_do_not_sync through drupal node edit screen
 *
 * @param object $node
 * @return boolean
 */
function tal_episode_megaphone_node_sync_status($node) {
    if (
        ($node->type == 'schedule') &&
        (!isset($node->skipMegaphoneSync)) &&
        (
            empty($node->field_do_not_sync) ||
            !isset($node->field_do_not_sync[LANGUAGE_NONE][0]['value']) ||
            (!$node->field_do_not_sync[LANGUAGE_NONE][0]['value'])
        )
    ) {
        return true;
    }
    return false;
}

/**
 * Get metadata relevant to Megaphone episode from schedule node.
 *
 *  - newly created nodes - return all relevant metadata
 *  - updated nodes - only return updated metadata
 *  - comparison is made to megaphone episode data
 *
 * @param object $node
 * @param array $attributes
 * @param boolean $compare - when false, do not filter out unchanged values
 * @return array
 */
function tal_episode_megaphone_extract_node_metadata($node, &$attributes = [], $compare = true) {
    //  Set up
    //   - meta storage
    $extracts = ['current' => [], 'old' => []];

    //   - current state
    $wrapper = entity_metadata_wrapper('node', $node);
    $episode = ($ep = $wrapper->field_episode->value()) ? $ep : null;
    $epNum = ($episode && ($num = $episode->field_episode_number[LANGUAGE_NONE][0]['value'])) ? $num : null;
    // title
    if ($episode && $episode->title) $extracts['current']['title'] = $episode->title;
    if ($episode && $episode->title && $epNum) $extracts['current']['title'] .= " (#$epNum)";
    // draft
    $extracts['current']['draft'] = !$node->status;
    // externalId
    if (isset($node->nid)) $extracts['current']['externalId'] = $node->nid;
    if (isset($audioFile)) $extracts['current']['externalId'] .= "--" . $audioFile->fid;
    // episodeNumber
    if ($epNum) $extracts['current']['episodeNumber'] = (int) $epNum;

    //   - old state
    if (
        $compare
        && $wrapper
        && ($mid = $wrapper->field_megaphone_episode_id->value())
    ) {
        $api = new MegaRestClient();
        $original = $api->episode($mid);
        $extracts['old'] = (array) $original;
    }

    //  Loop through current and add changed, non-false values to attributes
    foreach ($extracts['current'] as $attr => $value) {
        $currentVal = isset($extracts['current'][$attr]) ? $value : null;
        $oldVal = isset($extracts['old'][$attr]) ? $extracts['old'][$attr] : null;
        if (isset($currentVal) && !($currentVal === $oldVal)) $attributes[$attr] = $currentVal;
    }

    return $attributes;
}

/**
 * Send an email to configured notification reciptients.
 *
 * @param string $op
 * @param string $subject
 * @param string $message
 * @return void
 */
function tal_episode_megaphone_email_notification(string $op, string $subject, string $message) {
    $op = strtolower($op);
    $mailto = variable_get("megaphone_client_mailto_$op", false);
    if ($mailto) {
        $params = [
          'subject' => $subject,
          'body' => $message,
        ];
        drupal_mail('tal_episode', "megaphone_client_mailto_$op", $mailto, language_default(), $params);
    }
}
