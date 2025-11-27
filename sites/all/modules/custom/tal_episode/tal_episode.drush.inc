<?php

function tal_episode_drush_command() {
  $items['tal-queue-run'] = [
    'drupal dependencies' => array('tal_episode'),
    'description' => "Process TAL queues",
    'arguments' => array(
      'queue' => 'The Queue name.  If excluded process all TAL Episode queue items.',
    ),
    'aliases' => array('talq'),
  ];
  $items['tal-podcast'] = [
    'drupal dependencies' => array('tal_episode'),
    'description' => "List all current podcast feed episodes.",
    'aliases' => array('pod'),
  ];
  $items['tal-megaphone'] = [
    'drupal dependencies' => array('tal_episode'),
    'description' => "Connect to configured Megaphone API endpoint and run command",
    'arguments' => array(
      'method' => 'A method of the MegaRestClient class.  eg. "getEpisodes", "episode", ',
      'arguments' => 'As many arguments as needed by your method, surrounded by quotes and seperated by spaces.',
    ),
    'examples' => array(
      'drush mega' => 'Calls getEpisodes by default which loads all episodes.',
      'drush mega episode "548b790c-714c-11ef-8909-276ccac6f3e7"' => 'Get details for episode with id "548b790c-714c-11ef-8909-276ccac6f3e7".',
    ),
    'aliases' => array('mega'),
  ];
  return $items;
}

/**
 * Print current podcast feed episodes to the command line.
 *
 * @return void
 */
function drush_tal_episode_tal_podcast() {
  $podcast = tal_episode_podcast_episodes();
  drush_print("title (episode #) -- air date");
  foreach ($podcast as $nid) {
      $ep = node_load($nid);
      drush_print("$ep->title: (" . $ep->field_episode_number[LANGUAGE_NONE][0]['value'] . ") -- " . date('r', $ep->air_date));
  }
}

/**
 * Process Tal cron queue items.
 *
 * @param string $queue
 * @return void
 */
function drush_tal_episode_tal_queue_run($queue = 'all') {
  // Grab the defined cron queues.
  $queues = module_invoke_all('cron_queue_info');
  drupal_alter('cron_queue_info', $queues);

  if (isset($queues[$queue])) {
    $queues = [$queue => $queues[$queue]];
  } elseif (isset($queues["tal_episode_$queue"])) {
    $queues = ["tal_episode_$queue" => $queues["tal_episode_$queue"]];
  }

  // Work off queues.
  $items_processed = 0;
  $queues_processed = [];
  foreach ($queues as $queue_name => $info) {
    $function = $info['worker callback'];
    $end = time() + (isset($info['time']) ? $info['time'] : 15);
    $talq = new TalEpisodeQueue($queue_name);
    while (time() < $end && ($item = $talq->claimItem())) {
      $function($item->data);
      $talq->deleteItem($item);
      drush_print("Processed Item ID $item->item_id from queue $queue_name.");
      $items_processed++;
      if (!isset($queues_processed[$queue_name])) $queues_processed[] = $queue_name;
    }
  }
  $queues_processed = implode(' ', $queues_processed);
  drush_print("Processes a total of $items_processed items from queues: $queues_processed.\r");
}

function drush_tal_episode_tal_megaphone(...$args) {
  $api = new MegaRestClient();
  $method = (empty($args)) ? "getEpisodes" : array_shift($args);
  if (!in_array($method,[
    'podcast',
    'podcasts',
    'getEpisodes',
    'episode'
  ])) {
    drush_print("Method $method not allowed.");
    return;
  }
  $response = $api->{$method}(...$args);
  $print = json_encode($response,JSON_PRETTY_PRINT);

  drush_print($print);
}
