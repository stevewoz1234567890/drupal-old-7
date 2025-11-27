<?php
/**
 * Helper for episode RSS formatting.  Sets some rss element values and then sends to drupal to finish formatting and allow for hooks.
 *
 * @param object $node
 * @param string $description
 * @param int|boolean $timestamp
 * @return void
 */
function tal_episode_rss_build_item($node, $description = '', $timestamp = false) {
  $item_text = '';
  $title = $node->title;
  $timestamp = ($timestamp !== false) ? $timestamp : ($node->rss_timestamp ?: $node->release_timestamp);
  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');

  $node->link = 'https://www.thisamericanlife.org'.url('node/'.$node->nid);
  $node->rss_namespaces = array();
  $guid = $node->nid . ' at https://www.thisamericanlife.org';

  $node->rss_elements = array(
    array(
      'key' => 'pubDate',
      'value' => date('r', $timestamp),
    ),
    array(
      'key' => 'guid',
      'value' => $guid,
      'attributes' => array('isPermaLink' => 'false'),
    ),
    'itunes:episodeType' => 'full',
  );

  $build = node_view($node, 'rss');
  unset($build['#theme']);

  foreach ($node->rss_elements as $delta => $element) {
    if (!empty($element['key']) && $element['key'] == 'pubDate') {
      $node->rss_elements[$delta]['value'] = date('r', $timestamp);
    }
  }

  if (!empty($node->rss_namespaces)) {
    $namespaces = array_merge($namespaces, $node->rss_namespaces);
  }

  if (!empty($description)) {
    $item_text = $node->rss_elements['itunes:summary'] = $description;
  }
  else {
    $build['links']['#weight'] = 1000;
    $item_text .= drupal_render($build);
  }

  return format_rss_item($node->title, $node->link, $item_text, $node->rss_elements);
}

/**
 * Generate default RSS feed.
 *
 * @return void
 */
function tal_episode_podcast_feed() {
  $channel = array(
    'itunes:new-feed-url' => url('podcast/rss.xml', array('absolute' => true)),
    ['key' => 'atom:link', 'attributes' => ['href' => url('podcast/rss.xml', array('absolute' => true)), 'rel' => 'self', 'type' => 'application/rss+xml'] ],
    'copyright' => 'Copyright 1995-'.date('Y').' This American Life',
    'itunes:author' => 'This American Life',
    'itunes:subtitle' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'description' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'itunes:explicit' => 'false',
    'itunes:owner' => array(
      'itunes:email' => 'web@thislife.org'
      ),
      array(
        'key' => 'itunes:category',
        'attributes' => array(
          'text' => 'Society & Culture',
        ),
      ),
      array(
        'key' => 'itunes:category',
        'attributes' => array(
          'text' => 'Arts',
        ),
      ),
      array(
        'key' => 'itunes:category',
        'attributes' => array(
          'text' => 'News',
        ),
        'value' => array(
          array(
            'key' => 'itunes:category',
            'attributes' => array(
              'text' => 'Politics',
            ),
          ),
        ),
      ),
      array(
        'key' => 'itunes:image',
        'attributes' => array(
          'href' => 'https://thisamericanlife.org/sites/all/themes/thislife/img/tal-logo-3000x3000.png',
        ),
      ),
  );

  global $language_content;
  $base_url = url('podcast/rss.xml', array('absolute' => true));
  $item_length = variable_get('feed_item_length', 'fulltext');
  $namespaces = array(
    'xmlns:dc' => 'http://purl.org/dc/elements/1.1/',
    'xmlns:atom' => 'http://www.w3.org/2005/Atom',
    'xmlns:itunes' => 'http://www.itunes.com/dtds/podcast-1.0.dtd',
    'xmlns:media' => 'http://search.yahoo.com/mrss/',
  );
  $items =[];

  $exclude = array();

  //  Add classic episodes to the feed
  if ($episodes = variable_get('tal_classic_episodes', array())) {
    $descriptions = variable_get('tal_classic_episodes_descriptions', array());
    $date = variable_get('tal_classic_episodes_date', date('c'));
    $use_original_date = variable_get('tal_classic_episodes_use_original_date', false);
    $use_audio_field = variable_get('tal_classic_episodes_use_audio_field', false);
    foreach ($episodes as $delta => $episode_id) {
      $node = tal_episode_number_load($episode_id);
      $node_wrapper = entity_metadata_wrapper('node', $node);
      $item_text = '';
      $title = $node->title;
      $node->audio_type = 'nyt';
      $original_air_date = $node_wrapper->field_radio_air_date->value();
      $timestamp = $use_original_date ? $original_air_date : strtotime($date) - $delta;

      $node->link = 'https://www.thisamericanlife.org'.url('node/'.$node->nid);
      $node->rss_namespaces = array();
      $guid = $node->nid . ' at https://www.thisamericanlife.org';

      $node->rss_elements = array(
        array(
          'key' => 'pubDate',
          'value' => date('r', $timestamp),
        ),
        array(
          'key' => 'guid',
          'value' => $guid,
          'attributes' => array('isPermaLink' => 'false'),
        ),
        'itunes:episodeType' => 'full',
      );

      $build = node_view($node, 'rss');
      unset($build['#theme']);

      foreach ($node->rss_elements as $delta => $element) {
        if (!empty($element['key']) && $element['key'] == 'pubDate') {
          $node->rss_elements[$delta]['value'] = date('r', $timestamp);
        }

        //  Custom audio field - file field or text field
        if ($use_audio_field && !empty($element['key']) && $element['key'] == 'enclosure') {
          $url = $node_wrapper->{$use_audio_field}->value();
          // Get url from file field
          if (is_array($url) && isset($url['uri'])) $url = file_create_url($url['uri']);
          // Add tracking prefix
          $url = str_replace('https://', 'https://pfx.vpixl.com/6qj4J/dts.podtrac.com/redirect.mp3/chrt.fm/track/138C95/pdst.fm/e/prefix.up.audio/s/', $url);
          // Update url
          $node->rss_elements[$delta]['attributes']['url'] = $url;
        }
      }

      if (!empty($node->rss_namespaces)) {
        $namespaces = array_merge($namespaces, $node->rss_namespaces);
      }

      if (!empty($descriptions[$episode_id])) {
        $item_text = $descriptions[$episode_id];
        $node->rss_elements['itunes:summary'] = $descriptions[$episode_id];
      }
      else {
        $build['links']['#weight'] = 1000;
        $item_text .= drupal_render($build);
      }

      $items[$timestamp] = format_rss_item($node->title, $node->link, $item_text, $node->rss_elements);
    }
  }

  //  Build Regular feed episodes
  $episodes = tal_episode_podcast_episodes();
  foreach ($episodes as $nid) {
    if ($episode = node_load($nid)) {
      $items[$episode->rss_timestamp] = tal_episode_rss_build_item($episode);
    }
  }

  //  RSS extra
  if ($extras = tal_membership_check_for_all_nodes()) {
    foreach ($extras as $extra) {
      $timestamp = strtotime($extra->field_radio_air_date[LANGUAGE_NONE][0]['value']);
      $items[$timestamp] = theme('rss_extra', ['node' => $extra]);
    }
  }

  krsort($items);

  $channel_defaults = array(
    'version' => '2.0',
    'title' => variable_get('site_name', 'Drupal'),
    'link' => $base_url,
    'description' => variable_get('feed_description', ''),
    'language' => $language_content->language,
  );
  $channel_extras = array_diff_key($channel, $channel_defaults);
  $channel = array_merge($channel_defaults, $channel);

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"" . $channel["version"] . "\" xml:base=\"" . $base_url . "\" " . drupal_attributes($namespaces) . ">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], implode('', $items), $channel['language'], $channel_extras);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $output;
}

/**
 * RSS Feed for Youtube
 *
 * @return void
 */
function tal_episode_youtube_feed() {
  $channel = array(
    'itunes:new-feed-url' => url('podcast/youtube.xml', array('absolute' => true)),
    ['key' => 'atom:link', 'attributes' => ['href' => url('podcast/youtube.xml', array('absolute' => true)), 'rel' => 'self', 'type' => 'application/rss+xml'] ],
    'copyright' => 'Copyright 1995-'.date('Y').' This American Life',
    'itunes:author' => 'This American Life',
    'itunes:subtitle' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'description' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'itunes:explicit' => 'false',
    'itunes:owner' => array(
      'itunes:email' => 'web@thislife.org'
      ),
      array(
        'key' => 'itunes:category',
        'attributes' => array(
          'text' => 'Society & Culture',
        ),
      ),
      array(
        'key' => 'itunes:category',
        'attributes' => array(
          'text' => 'Arts',
        ),
      ),
      array(
        'key' => 'itunes:category',
        'attributes' => array(
          'text' => 'News',
        ),
        'value' => array(
          array(
            'key' => 'itunes:category',
            'attributes' => array(
              'text' => 'Politics',
            ),
          ),
        ),
      ),
      array(
        'key' => 'itunes:image',
        'attributes' => array(
          'href' => 'https://thisamericanlife.org/sites/all/themes/thislife/img/tal-logo-3000x3000.png',
        ),
      ),
  );

  global $language_content;
  $base_url = url('podcast/youtube.xml', array('absolute' => true));

  $item_length = variable_get('feed_item_length', 'fulltext');
  $namespaces = array(
    'xmlns:dc' => 'http://purl.org/dc/elements/1.1/',
    'xmlns:atom' => 'http://www.w3.org/2005/Atom',
    'xmlns:itunes' => 'http://www.itunes.com/dtds/podcast-1.0.dtd',
    'xmlns:media' => 'http://search.yahoo.com/mrss/',
  );
  $items = [];

  //  Filter out episodes which have "Exclude from Youtube Feed" checked
  $query = db_select('node', 'n');
  $query->join('field_data_field_publish_to_youtube_rss', 'pub', "(pub.entity_id = n.nid AND pub.entity_type ='node' AND pub.deleted = 0)");
  $query
    ->fields('n', array('nid'))
    ->condition('pub.field_publish_to_youtube_rss_value', 1)
    ->condition('n.status', 1)
    ->condition('n.type', 'episode');
  $result = $query->execute();
  $excluded_nids = $result->fetchAllAssoc('nid');
  $excluded_nids = array_keys($excluded_nids);

  //  Build query
  $now = date('c', REQUEST_TIME - (AIR_DATE_LIVE_OFFSET*60*60));
  $start =  date('c', strtotime("November 14 2024"));
  $nid_string = implode(",",$excluded_nids);
  $nid_clause = count($excluded_nids) ? "and e.field_episode_target_id not in ($nid_string)" : "";
  $query = "select e.field_episode_target_id, rad.field_radio_air_date_value
  from node n
  join field_data_field_radio_air_date as rad on (rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)
  left join field_data_field_release_date as rd on (rd.entity_id = n.nid AND rd.entity_type ='node' AND rd.deleted = 0)
  join field_data_field_episode as e on (e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)
  where
  n.type='schedule'
  and n.status=1
  $nid_clause
  and (
    (
      (rd.field_release_date_value is not null)
      and (rd.field_release_date_value > '$start')
      and (rd.field_release_date_value < '$now')
    )
  or
    (
      (rad.field_radio_air_date_value is not null)
      and (rad.field_radio_air_date_value < '$now')
      and (rad.field_radio_air_date_value > '$start')
    )
  )
  order by rad.field_radio_air_date_value desc
  ";
  $rows = db_query($query)->fetchAll();

  //  Render results
  $nids = [];
  foreach ($rows as $row) {
    if (
      ($node = node_load($row->field_episode_target_id)) &&
      (!in_array($node->nid, $nids))
    ) {
      $nids[] = $node->nid;
      $node->field_radio_air_date[LANGUAGE_NONE][0]['value'] = $row->field_radio_air_date_value;
      $timestamp = isset($node->rss_timestamp) ? $node->rss_timestamp : strtotime($row->field_radio_air_date_value);
      $podcast_nids[$timestamp] = $node->nid;
      $item_text = '';

      $node->link = 'https://www.thisamericanlife.org'.url('node/'.$node->nid);
      $node->rss_namespaces = array();
      $guid = $node->nid . ' at https://www.thisamericanlife.org';

      $node->rss_elements = array(
        array(
          'key' => 'pubDate',
          'value' => date('r', $timestamp),
        ),
        array(
          'key' => 'guid',
          'value' => $guid,
          'attributes' => array('isPermaLink' => 'false'),
        ),
        'itunes:episodeType' => 'full',
      );

      // The node gets built and modules add to or modify $node->rss_elements
      // and $node->rss_namespaces.
      $build = node_view($node, 'rss');
      unset($build['#theme']);

      if (!empty($node->rss_namespaces)) {
        $namespaces = array_merge($namespaces, $node->rss_namespaces);
      }

      if ($item_length != 'title') {
        // We render node contents and force links to be last.
        $build['links']['#weight'] = 1000;
        $item_text .= drupal_render($build);
      }

      $items[$timestamp] = format_rss_item($node->title, $node->link, $item_text, $node->rss_elements);

    }
  }

  krsort($items);

  $channel_defaults = array(
    'version' => '2.0',
    'title' => variable_get('site_name', 'Drupal'),
    'link' => $base_url,
    'description' => variable_get('feed_description', ''),
    'language' => $language_content->language,
  );
  $channel_extras = array_diff_key($channel, $channel_defaults);
  $channel = array_merge($channel_defaults, $channel);

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"" . $channel["version"] . "\" xml:base=\"" . $base_url . "\" " . drupal_attributes($namespaces) . ">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], implode('', $items), $channel['language'], $channel_extras);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $output;
}

function tal_episode_all_feed() {
  $channel = array(
    'itunes:new-feed-url' => url('podcast/rss.xml', array('absolute' => true)),
    ['key' => 'atom:link', 'attributes' => ['href' => url('podcast/rss.xml', array('absolute' => true)), 'rel' => 'self', 'type' => 'application/rss+xml'] ],
    'copyright' => 'Copyright 1995-'.date('Y').' This American Life',
    'itunes:author' => 'This American Life',
    'itunes:subtitle' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'description' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'itunes:explicit' => 'false',
    'itunes:owner' => array(
      'itunes:email' => 'web@thislife.org'
    ),
    array(
      'key' => 'itunes:category',
      'attributes' => array(
        'text' => 'Society & Culture',
      ),
    ),
    array(
      'key' => 'itunes:category',
      'attributes' => array(
        'text' => 'Arts',
      ),
    ),
    array(
      'key' => 'itunes:category',
      'attributes' => array(
        'text' => 'News',
      ),
      'value' => array(
        array(
          'key' => 'itunes:category',
          'attributes' => array(
            'text' => 'Politics',
          ),
        ),
      ),
    ),
    array(
      'key' => 'itunes:image',
      'attributes' => array(
        'href' => 'https://thisamericanlife.org/sites/all/themes/thislife/img/tal-logo-3000x3000.png',
      ),
    ),
  );

  global $language_content;
  $base_url = 'https://www.thisamericanlife.org';

  $item_length = variable_get('feed_item_length', 'fulltext');
  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');

  $query = db_select('node', 'n');
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
  $query
    ->fields('n', array('nid'))
    ->condition('n.status', 1)
    ->condition('n.type', 'episode')
    ->condition('rad.field_radio_air_date_value', date('c', REQUEST_TIME), '<')
    ->orderBy('rad.field_radio_air_date_value', 'DESC');
  $result = $query->execute();
  foreach ($result as $row) {
    if ($node = node_load($row->nid)) {
      if ($dates = field_get_items('node', $node, 'field_radio_air_date')) {
        $timestamp = strtotime($dates[0]['value']);
      }
      $item_text = '';

      $node->link = 'https://www.thisamericanlife.org'.url('node/'.$node->nid);
      $node->rss_namespaces = array();
      $guid = $node->nid . ' at https://www.thisamericanlife.org';

      $node->rss_elements = array(
        array(
          'key' => 'pubDate',
          'value' => date('r', $timestamp),
        ),
        array(
          'key' => 'guid',
          'value' => $guid,
          'attributes' => array('isPermaLink' => 'false'),
        ),
        'itunes:episodeType' => 'full',
      );

      // The node gets built and modules add to or modify $node->rss_elements
      // and $node->rss_namespaces.
      $build = node_view($node, 'rss');
      unset($build['#theme']);

      if (!empty($node->rss_namespaces)) {
        $namespaces = array_merge($namespaces, $node->rss_namespaces);
      }

      if ($item_length != 'title') {
        // We render node contents and force links to be last.
        $build['links']['#weight'] = 1000;
        $item_text .= drupal_render($build);
      }

      $items[] = format_rss_item($node->title, $node->link, $item_text, $node->rss_elements);
    }
  }

  $channel_defaults = array(
    'version' => '2.0',
    'title' => variable_get('site_name', 'Drupal'),
    'link' => $base_url,
    'description' => variable_get('feed_description', ''),
    'language' => $language_content->language,
  );
  $channel_extras = array_diff_key($channel, $channel_defaults);
  $channel = array_merge($channel_defaults, $channel);

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"" . $channel["version"] . "\" xml:base=\"" . $base_url . "\" " . drupal_attributes($namespaces) . ">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], implode('', $items), $channel['language'], $channel_extras);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $output;
}

function tal_episode_archive_feed() {
  $channel = array(
    'copyright' => 'Copyright 1995-'.date('Y').' This American Life',
    'itunes:author' => 'This American Life',
    'itunes:subtitle' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'description' => "Each week we choose a theme. Then anything can happen. This American Life is true stories that unfold like little movies for radio. Personal stories with funny moments, big feelings, and surprising plot twists. Newsy stories that try to capture what it’s like to be alive right now. It’s the most popular weekly podcast in the world, and winner of the first ever Pulitzer Prize for a radio show or podcast. Hosted by Ira Glass and produced in collaboration with WBEZ Chicago.",
    'itunes:owner' => array(
      'itunes:email' => 'web@thislife.org'
    ),
    array(
      'key' => 'itunes:category',
      'attributes' => array(
        'text' => 'Society & Culture',
      ),
    ),
    array(
      'key' => 'itunes:category',
      'attributes' => array(
        'text' => 'Arts',
      ),
    ),
    array(
      'key' => 'itunes:category',
      'attributes' => array(
        'text' => 'News',
      ),
      'value' => array(
        array(
          'key' => 'itunes:category',
          'attributes' => array(
            'text' => 'Politics',
          ),
        ),
      ),
    ),
    array(
      'key' => 'itunes:image',
      'attributes' => array(
        'href' => 'https://thisamericanlife.org/sites/all/themes/thislife/img/tal-logo-3000x3000.png',
      ),
    ),
  );

  global $language_content;
  $base_url = 'https://www.thisamericanlife.org';

  $item_length = variable_get('feed_item_length', 'fulltext');
  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');

  $query = db_select('node', 'n');
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
  $query
    ->fields('n', array('nid'))
    ->condition('n.status', 1)
    ->condition('n.type', 'episode')
    ->condition('rad.field_radio_air_date_value', date('c', REQUEST_TIME), '<')
    ->orderBy('rad.field_radio_air_date_value', 'DESC');
  $result = $query->execute();
  foreach ($result as $row) {
    if ($node = node_load($row->nid)) {
      unset($node->audio_type);
      if ($dates = field_get_items('node', $node, 'field_radio_air_date')) {
        $timestamp = strtotime($dates[0]['value']);
      }
      $item_text = '';

      $node->link = 'https://www.thisamericanlife.org'.url('node/'.$node->nid);
      $node->rss_namespaces = array();
      $guid = $node->nid . ' at https://www.thisamericanlife.org';

      $node->rss_elements = array(
        array(
          'key' => 'pubDate',
          'value' => date('r', $timestamp),
        ),
        array(
          'key' => 'guid',
          'value' => $guid,
          'attributes' => array('isPermaLink' => 'false'),
        ),
      );

      // The node gets built and modules add to or modify $node->rss_elements
      // and $node->rss_namespaces.
      $build = node_view($node, 'rss');
      unset($build['#theme']);

      if (!empty($node->rss_namespaces)) {
        $namespaces = array_merge($namespaces, $node->rss_namespaces);
      }

      if ($item_length != 'title') {
        // We render node contents and force links to be last.
        $build['links']['#weight'] = 1000;
        $item_text .= drupal_render($build);
      }

      $items[] = format_rss_item($node->title, $node->link, $item_text, $node->rss_elements);
    }
  }

  $channel_defaults = array(
    'version' => '2.0',
    'title' => variable_get('site_name', 'Drupal'),
    'link' => $base_url,
    'description' => variable_get('feed_description', ''),
    'language' => $language_content->language,
  );
  $channel_extras = array_diff_key($channel, $channel_defaults);
  $channel = array_merge($channel_defaults, $channel);

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"" . $channel["version"] . "\" xml:base=\"" . $base_url . "\" " . drupal_attributes($namespaces) . ">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], implode('', $items), $channel['language'], $channel_extras);
  $output .= "</rss>\n";

  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  print $output;
}
