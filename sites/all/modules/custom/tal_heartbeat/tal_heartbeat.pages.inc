<?php

function tal_heartbeat_episodes_page() {
  $data = &drupal_static(__FUNCTION__);
  if (!isset($data)) {
    if ($cache = cache_get('tal_heartbeat_episodes')) {
      $data = $cache->data;
      $time = $cache->created;
    }
    else {
      $time = REQUEST_TIME;
      $episodes = array();
      $query = db_select('node', 'n');
      $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
      $query->join('field_data_field_episode_number', 'en', "(en.entity_id = n.nid AND en.entity_type ='node' AND en.deleted = 0)");
      $query
        ->fields('rad', array('field_radio_air_date_value'))
        ->fields('en', array('field_episode_number_value'))
        ->condition('n.type', 'episode')
        ->orderBy('rad.field_radio_air_date_value', 'DESC');
      $result = $query->execute();
      foreach ($result as $row) {
        $episodes[$row->field_episode_number_value] = $row->field_radio_air_date_value;
      }
      db_set_active('heartbeat');
      $result = db_query("SELECT episode, COUNT(*) AS plays, SUM(position) AS time FROM beats WHERE source IN ('', 'android', 'desktop', 'ios', 'mobile', 'thisamericanlife') GROUP BY episode ORDER BY episode DESC");
      while ($row = $result->fetchObject()) {
        $timestamp = strtotime($episodes[$row->episode]);
        $data[$row->episode] = array(
          date('m/d/Y', $timestamp),
          $row->episode,
          '',
          '',
          '',
          '',
          number_format($row->plays),
          number_format($row->time / 60 / 60),
          number_format($row->time / 60 / $row->plays),
        );
      }
      foreach ($episodes as $episode => $date) {
        if (!empty($data[$episode])) {
          $timestamp = strtotime($date) + (67 * 60 * 60);
          $result = db_query("SELECT episode, COUNT(*) AS plays, SUM(position) AS time FROM beats WHERE episode = :episode AND created <= :date ", array(':episode' => $episode, ':date' => ($timestamp + (7 * 24 * 60 * 60))));
          while ($row = $result->fetchObject()) {
            $data[$episode][2] = number_format($row->plays);
            $data[$episode][3] = number_format($row->time / 60 / 60);
          }
          $result = db_query("SELECT episode, COUNT(*) AS plays, SUM(position) AS time FROM beats WHERE episode = :episode AND created <= :date ", array(':episode' => $episode, ':date' => ($timestamp + (28 * 24 * 60 * 60))));
          while ($row = $result->fetchObject()) {
            $data[$episode][4] = number_format($row->plays);
            $data[$episode][5] = number_format($row->time / 60 / 60);
          }
        }
      }
      db_set_active();
      cache_set('tal_heartbeat_episodes', $data, 'cache', REQUEST_TIME + (60*60*24));

    }
  }
  $render = array();
  $render['time'] = array(
    '#prefix' => '<div class="last-update">',
    '#suffix' => '</div>',
    '#markup' => 'Last update: <strong>'.format_interval(REQUEST_TIME - $time, 1) . t(' ago').'</strong>',
  );
  $header = array(
    'Original Air Date',
    'Episode Number',
    '7-day Plays',
    '7-day Hours',
    '28-day Plays',
    '28-day Hours',
    'All-time Plays',
    'All-time Hours',
    'Average Minutes',
  );
  krsort($data);
  $render['table'] = array(
    '#theme' => 'table',
    '#rows' => $data,
    '#header' => $header,
    '#prefix' => '<div class="table-wrapper">',
    '#suffix' => '</div>',
  );
  return $render;
}

function tal_heartbeat_listening_page() {
  $data = &drupal_static(__FUNCTION__);
  if (!isset($data)) {
    if ($cache = cache_get('tal_heartbeat_listening')) {
      $data = $cache->data;
      $time = $cache->created;
    }
    else {
      $time = REQUEST_TIME;
      db_set_active('heartbeat');
      $result = db_query("SELECT MONTH(FROM_UNIXTIME(created)) AS month, YEAR(FROM_UNIXTIME(created)) AS year, COUNT(*) AS plays, SUM(position) AS time FROM beats WHERE source IN ('', 'android', 'desktop', 'ios', 'mobile', 'thisamericanlife') GROUP BY MONTH(FROM_UNIXTIME(created)), YEAR(FROM_UNIXTIME(created)) ORDER BY created DESC");
      while ($row = $result->fetchObject()) {
        $month = "$row->year-$row->month";
        $data[$month] = array(
          date('F Y', strtotime($month)),
          number_format($row->plays),
          number_format($row->time / 60 / 60),
          '',
          '',
          '',
          '',
        );
      }
      $result = db_query("SELECT MONTH(FROM_UNIXTIME(created)) AS month, YEAR(FROM_UNIXTIME(created)) AS year, COUNT(*) AS plays, SUM(position) AS time FROM beats WHERE source IN ('', 'desktop', 'mobile', 'thisamericanlife') GROUP BY MONTH(FROM_UNIXTIME(created)), YEAR(FROM_UNIXTIME(created)) ORDER BY created DESC");
      while ($row = $result->fetchObject()) {
        $month = "$row->year-$row->month";
        $data[$month][3] = number_format($row->plays);
        $data[$month][4] = number_format($row->time / 60 / 60);
      }
      $result = db_query("SELECT MONTH(FROM_UNIXTIME(created)) AS month, YEAR(FROM_UNIXTIME(created)) AS year, COUNT(*) AS plays, SUM(position) AS time FROM beats WHERE source IN ('android', 'ios') GROUP BY MONTH(FROM_UNIXTIME(created)), YEAR(FROM_UNIXTIME(created)) ORDER BY created DESC");
      while ($row = $result->fetchObject()) {
        $month = "$row->year-$row->month";
        $data[$month][5] = number_format($row->plays);
        $data[$month][6] = number_format($row->time / 60 / 60);
      }
      db_set_active();
      cache_set('tal_heartbeat_listening', $data, 'cache', REQUEST_TIME + (60*60*24));
    }
  }
  $render = array();
  $render['time'] = array(
    '#prefix' => '<div class="last-update">',
    '#suffix' => '</div>',
    '#markup' => 'Last update: <strong>'.format_interval(REQUEST_TIME - $time, 1) . t(' ago').'</strong>',
  );
  $header = array(
    'Month',
    'Total Plays',
    'Total Hours',
    'Site Plays',
    'Site Hours',
    'App Plays',
    'App Hours',
  );
  $render['table'] = array(
    '#theme' => 'table',
    '#rows' => $data,
    '#header' => $header,
    '#prefix' => '<div class="table-wrapper">',
    '#suffix' => '</div>',
  );
  return $render;
}

function tal_heartbeat_episode_page($node) {
  $render = array();
  $path = drupal_get_path('module', 'tal_heartbeat');

  drupal_add_js($path.'/js/Chart.bundle.min.js', array('weight' => 99, 'group' => JS_THEME));
  drupal_add_js($path.'/js/tal_heartbeat.js', array('weight' => 99, 'group' => JS_THEME));

  if ($items = field_get_items('node', $node, 'field_episode_number')) {
    $episode_number = (int) $items[0]['value'];
    $acts = array();
    if ($items = field_get_items('node', $node, 'field_acts')) {
      foreach ($items as $item) {
        $act = node_load($item['target_id']);
        $time = tal_episode_get_timestamp($node, $act);
        $time = round($time/15)*15;
        $name = '';
        if ($names = field_get_items('node', $act, 'field_act_label')) {
          $name = $names[0]['value'];
        }
        else if ($names = field_get_items('node', $act, 'field_act_number')) {
          if (!empty($names[0]['value'])) {
            $name = t('Act !number', array('!number' => $items[0]['value']));
          }
        }
        if (!empty($name)) {
          $acts[$time] = $name;
        }
      }
    }
    if (!empty($node->field_acts)) {
      foreach ($node->field_acts as $act) {
        if (!empty($act['time'])) {
           if (preg_match('/^(Act ([^\.]*)|Prologue)\.(.*)/i', $act['title'], $matches)) {
            $title = $matches[1];
          }
          $time = round($act['time']/15)*15;
          $acts[$time] = $title;
        }
      }
    }
    $data = array();
    $start = time();
    $end = time();
    $percentages = array();
    if ($duration = $node->audio_source_file['metadata']['duration']) {
      $length = tal_time_to_seconds($duration);
    }
    db_set_active('heartbeat');
    $result = db_query("SELECT position, created, updated FROM {beats} WHERE episode = :episode ORDER BY position ASC", array(':episode' => $episode_number));
    while ($row = $result->fetchAssoc()) {
      if ($row['created'] < $start) {
        $start = $row['created'];
      }
      for ($i = 0; $i <= $row['position']; $i += 15) {
        if ($i <= $length) {
          $data[$i]++;
        }
      }
    }
    db_set_active();
    /*
    if (!empty($_GET['start']) && !empty($_GET['end'])) {
      $start = strtotime(filter_xss($_GET['start'], array()));
      $end = strtotime(filter_xss($_GET['end'], array()));
      $result = db_query("SELECT position, created, updated FROM {beats} WHERE episode = %d AND created >= %d AND created < %d ORDER BY position ASC", $_GET['episode'], $start, ($end + (24*60*60)));
      while ($row = db_fetch_object($result)) {
        for ($i = 0; $i <= $row->position; $i+=15) {
          if ($i <= $length) {
          $data[$i]++;
        }
        }
      }
    }
    else {
      $result = db_query("SELECT position, created, updated FROM {beats} WHERE episode = %d ORDER BY position ASC", $_GET['episode']);
      while ($row = db_fetch_object($result)) {
        if ($row->created < $start) {
          $start = $row->created;
        }
        for ($i = 0; $i <= $row->position; $i+=15) {
          if ($i <= $length) {
            $data[$i]++;
          }
        }
      }
    }
    */
    $stop = 0;
    $total = $data[0];
    foreach ($data as $k => $v) {
      $stop = $k;
      $percentages[$k] = round($v/$total*10000)/100;
    }
  }
  /*
  $averages = false;
  $filename = 'heartbeat-nDJ6Q2i9nYwR6hB8G2X2Pt2DAx5Vwo0M/data/totals.json';
  if ($json = file_get_contents($filename)) {
    $averages = (array) json_decode($json);
    $averages_percentages = array();
    $averages_total = reset($averages);
    foreach ($averages as $k => $v) {
      if ($k <= $stop) {
        $averages_percentages[$k] = round($v/$averages_total*10000)/100;
      }
    }
  }
  */
  $heartbeat = array(
    'totals' => $data,
    'acts' => $acts,
    'labels' => array_keys($data),
    'percentages' => array_values($percentages),
    //'averages' => (!empty($averages) ? $averages_percentages : array()),
  );

  drupal_add_js('var heartbeat = '.drupal_json_encode($heartbeat).';', 'inline');

  $render['node'] = node_view($node, 'heartbeat');

  $render['canvas'] = array(
    '#prefix' => '<div id="heartbeat-wrapper">',
    '#suffix' => '</div>',
    '#markup' => '<canvas id="myChart" width="1600" height="820"></canvas>',
  );

  return $render;
}
