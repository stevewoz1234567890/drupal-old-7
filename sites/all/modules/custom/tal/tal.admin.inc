<?php

function tal_archive_csv() {
  $episodes = array(
    '16',
    '17',
    '19',
    '24',
    '25',
    '29',
    '30',
    '33',
    '34',
    '35',
    '36',
    '39',
    '41',
    '43',
    '44',
    '48',
    '49',
    '50',
    '51',
    '52',
    '53',
    '55',
    '56',
    '57',
    '58',
    '59',
    '60',
    '62',
    '63',
    '64',
    '66',
    '67',
    '68',
    '70',
    '71',
    '73',
    '78',
    '79',
    '82',
    '83',
    '85',
    '86',
    '89',
    '91',
    '96',
    '97',
    '98',
    '100',
    '103',
    '105',
    '106',
    '108',
    '111',
    '112',
    '113',
    '114',
    '115',
    '116',
    '117',
    '118',
    '120',
    '122',
    '123',
    '124',
    '125',
    '126',
    '127',
    '128',
    '129',
    '130',
    '131',
    '132',
    '133',
    '134',
    '136',
    '138',
    '139',
    '142',
    '146',
    '147',
    '148',
    '149',
    '150',
    '151',
    '152',
    '153',
    '155',
    '156',
    '157',
    '158',
    '159',
    '160',
    '161',
    '162',
    '163',
    '167',
    '169',
    '170',
    '171',
    '173',
    '174',
    '176',
    '177',
    '178',
    '179',
    '180',
    '182',
    '183',
    '185',
    '187',
    '189',
    '190',
    '191',
    '193',
    '195',
    '196',
    '200',
    '201',
    '202',
    '207',
    '208',
    '211',
    '212',
    '213',
    '215',
    '216',
    '217',
    '219',
    '221',
    '222',
    '224',
    '227',
    '228',
    '229',
    '231',
    '232',
    '236',
    '237',
    '238',
    '240',
    '243',
    '244',
    '245',
    '247',
    '249',
    '250',
    '251',
    '255',
    '256',
    '257',
    '258',
    '260',
    '261',
    '262',
    '263',
    '264',
    '265',
    '267',
    '269',
    '270',
    '271',
    '272',
    '273',
    '274',
    '275',
    '276',
    '277',
    '278',
    '279',
    '280',
    '283',
    '284',
    '285',
    '286',
    '287',
    '288',
    '290',
    '293',
    '294',
    '295',
    '297',
    '298',
    '299',
    '300',
    '301',
    '302',
    '303',
    '305',
    '307',
    '308',
    '309',
    '311',
    '312',
    '313',
    '314',
    '315',
    '316',
    '318',
    '320',
    '321',
    '324',
    '325',
    '326',
    '327',
    '328',
    '329',
    '330',
    '331',
    '335',
    '336',
    '337',
    '338',
    '340',
    '341',
    '342',
    '344',
    '345',
    '346',
    '347',
    '349',
    '350',
    '353',
    '356',
    '357',
    '358',
    '359',
    '363',
    '364',
    '365',
    '366',
    '367',
    '368',
    '370',
    '371',
    '372',
    '373',
    '374',
    '375',
    '376',
    '377',
    '378',
    '379',
    '380',
    '381',
    '382',
    '384',
    '385',
    '386',
    '387',
    '397',
    '398',
    '399',
    '400',
    '401',
    '403',
    '404',
    '405',
    '406',
    '407',
    '408',
    '410',
    '411',
    '412',
    '413',
    '414',
    '415',
    '416',
    '417',
    '418',
    '420',
    '421',
    '422',
    '423',
    '424',
    '425',
    '426',
    '427',
    '428',
    '429',
    '431',
    '432',
    '433',
    '434',
    '435',
    '436',
    '437',
  );
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=TAL_archive.csv');
  $output = fopen('php://output', 'w');
  fputcsv($output, array('Episode number', 'Episode name', 'Episode URL', 'Audio'));
  $query = db_select('node', 'n');
  $query->join('field_data_field_audio_archive', 'a', "n.nid = a.entity_id");
  $query->join('field_data_field_episode_number', 'en', "n.nid = en.entity_id");
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('a', array('field_audio_archive_fid'))
    ->condition('n.type', 'episode')
    ->condition('field_episode_number_value', $episodes, 'IN')
    ->condition('n.status', 1)
    ->orderBy('en.field_episode_number_value', 'DESC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    $file = file_load($result->field_audio_archive_fid);
    $row = array(
      $result->field_episode_number_value,
      $result->title,
      url('node/'.$result->nid, array('absolute' => true)),
      file_create_url($file->uri),
    );
    fputcsv($output, $row);
  }
  exit;
}

function tal_nyt_report_page($episode_number = false) {
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=TAL_NYT.csv');
  $output = fopen('php://output', 'w');
  fputcsv($output, array('Episode number', 'Episode name', 'Episode URL', 'Audio'));
  $query = db_select('node', 'n');
  $query->join('field_data_field_audio_nyt', 'a', "n.nid = a.entity_id");
  $query->join('field_data_field_episode_number', 'en', "n.nid = en.entity_id");
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('a', array('field_audio_nyt_fid'))
    ->condition('n.type', 'episode')
    ->condition('n.status', 1)
    ->orderBy('en.field_episode_number_value', 'DESC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    $file = file_load($result->field_audio_nyt_fid);
    $row = array(
      $result->field_episode_number_value,
      $result->title,
      url('node/'.$result->nid, array('absolute' => true)),
      file_create_url($file->uri),
    );
    fputcsv($output, $row);
  }
  exit;
}

function tal_images_report_page($episode_number = false) {
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=TAL_images.csv');
  $output = fopen('php://output', 'w');
  fputcsv($output, array('Episode number', 'Episode name', 'Episode URL', 'Cleared for NYT', 'Image Notes'));
  $query = db_select('node', 'n');
  $query->join('field_data_field_image', 'i', "n.nid = i.entity_id");
  $query->join('field_data_field_episode_number', 'en', "n.nid = en.entity_id");
  $query->leftJoin('field_data_field_cleared', 'fc', "i.field_image_fid = fc.entity_id");
  $query->leftJoin('field_data_field_notes', 'fn', 'i.field_image_fid = fn.entity_id');
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('fc', array('field_cleared_value'))
    ->fields('fn', array('field_notes_value'))
    ->condition('n.type', 'episode')
    ->condition('n.status', 1)
    ->orderBy('en.field_episode_number_value', 'DESC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    //$episode = node_load($result->nid);
    $row = array(
      $result->field_episode_number_value,
      $result->title,
      url('node/'.$result->nid, array('absolute' => true)),
      $result->field_cleared_value,
      $result->field_notes_value,
    );
    fputcsv($output, $row);
  }
  exit;
}

function tal_songs_report_page($episode_number = false) {
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=TAL_songs.csv');
  $output = fopen('php://output', 'w');
  fputcsv($output, array('Episode number', 'Episode name', 'Episode URL', 'Act label', 'Act title', 'Song Title', 'Artist', 'Link'));
  $query = db_select('node', 'n');
  $query->join('field_data_field_episode', 'e', "n.nid = e.entity_id");
  $query->join('field_data_field_act_number', 'an', "n.nid = an.entity_id");
  $query->join('field_data_field_episode_number', 'en', "e.field_episode_target_id = en.entity_id");
  $query->join('node', 'n2', "e.field_episode_target_id = n2.nid");
  $query->join('field_data_field_song', 's', 'n.nid = s.entity_id');
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('n2', array('nid', 'title'))
    ->fields('an', array('field_act_number_value'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('s', array('field_song_value'))
    ->condition('n.type', 'act')
    ->condition('n2.type', 'episode')
    ->condition('n.status', 1)
    ->condition('field_episode_number_value', $episode_number, '>')
    ->orderBy('en.field_episode_number_value', 'DESC')
    ->orderBy('an.field_act_number_value', 'ASC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    $act = node_load($result->nid);
    $episode = node_load($result->n2_nid);
    $song = array(
      'title' => '',
      'artist' => '',
      'link' => '',
    );
    if ($collection_item = field_collection_item_load($result->field_song_value)) {
      if ($items = field_get_items('field_collection_item', $collection_item, 'field_title')) {
        $song['title'] = $items[0]['value'];
      }
      if ($items = field_get_items('field_collection_item', $collection_item, 'field_artist')) {
        $song['artist'] = $items[0]['value'];
      }
      if ($items = field_get_items('field_collection_item', $collection_item, 'field_link')) {
        $song['link'] = $items[0]['url'];
      }
    }
    $row = array(
      $result->field_episode_number_value,
      $result->n2_title,
      url('node/'.$result->n2_nid, array('absolute' => true)),
      $result->field_act_number_value,
      $result->title,
      $song['title'],
      $song['artist'],
      $song['link'],
    );
    fputcsv($output, $row);
  }
  exit;
}

function tal_producers_report_page($episode_number = false) {
  if (!empty($episode_number)) {
    header('Content-Type: text/csv; charset=utf-8');
    header('Content-Disposition: attachment; filename=TAL_producers.csv');
    $output = fopen('php://output', 'w');
    fputcsv($output, array('Air date', 'Episode number', 'Episode name', 'Episode URL', 'Act label', 'Act title', 'Contributors'));
    $query = db_select('node', 'n');
    $query->join('field_data_field_episode', 'e', "n.nid = e.entity_id");
    $query->join('field_data_field_act_number', 'an', "n.nid = an.entity_id");
    $query->join('field_data_field_episode_number', 'en', "e.field_episode_target_id = en.entity_id");
    $query->join('node', 'n2', "e.field_episode_target_id = n2.nid");
    $query
      ->fields('n', array('nid', 'title'))
      ->fields('n2', array('nid', 'title'))
      ->fields('an', array('field_act_number_value'))
      ->fields('en', array('field_episode_number_value'))
      ->condition('n.type', 'act')
      ->condition('n2.type', 'episode')
      ->condition('n.status', 1)
      ->condition('field_episode_number_value', $episode_number, '>')
      ->orderBy('en.field_episode_number_value', 'DESC')
      ->orderBy('an.field_act_number_value', 'ASC');
    $results = $query->execute();
    $rows = array();
    foreach ($results as $result) {
      $act = node_load($result->nid);
      $episode = node_load($result->n2_nid);
      $byline = array();
      if ($items = field_get_items('node', $act, 'field_contributor')) {
        foreach ($items as $item) {
          $term = taxonomy_term_load($item['target_id']);
          $byline[] = $term->name;
        }
      }
      $date = '';
      if ($items = field_get_items('node', $episode, 'field_radio_air_date')) {
  			$date = $items[0]['value'];
      }
      $row = array(
        $date,
        $result->field_episode_number_value,
        $result->n2_title,
        url('node/'.$result->n2_nid, array('absolute' => true)),
        $result->field_act_number_value,
        $result->title,
        implode(', ', $byline),
      );
      fputcsv($output, $row);
    }
  }
  exit;
}

function tal_acts_report_page() {
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=TAL_acts.csv');
  $output = fopen('php://output', 'w');
  fputcsv($output, array('Episode Number', 'Episode Number', 'Act Number', 'Act Name', 'Byline', 'Description'));
  $query = db_select('node', 'n');
  $query->join('field_data_field_episode', 'e', "n.nid = e.entity_id");
  $query->join('field_data_field_act_number', 'an', "n.nid = an.entity_id");
  $query->join('field_data_field_episode_number', 'en', "e.field_episode_target_id = en.entity_id");
  $query->join('node', 'n2', "e.field_episode_target_id = n2.nid");
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('n2', array('title'))
    ->fields('an', array('field_act_number_value'))
    ->fields('en', array('field_episode_number_value'))
    ->condition('n.type', 'act')
    ->condition('n2.type', 'episode')
    ->condition('n.status', 1)
    ->orderBy('en.field_episode_number_value', 'DESC')
    ->orderBy('an.field_act_number_value', 'ASC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    $node = node_load($result->nid);
    $byline = array();
    if ($items = field_get_items('node', $node, 'field_contributor')) {
      foreach ($items as $item) {
        $term = taxonomy_term_load($item['target_id']);
        $byline[] = $term->name;
      }
    }
    $description = '';
    if ($items = field_get_items('node', $node, 'body')) {
      $description = trim(html_entity_decode(strip_tags($items[0]['value'])));
    }
    $row = array(
      $result->field_episode_number_value,
      $result->field_act_number_value,
      $result->title,
      $result->n2_title,
      implode(', ', $byline),
      $description,
    );
    fputcsv($output, $row);
  }
  exit;
}

function tal_tags_report_page() {
  $render = array();
  $query = db_select('node', 'n');
  $query->join('taxonomy_index', 'ti', "n.nid = ti.nid");
  $query->join('taxonomy_term_data', 'td', "ti.tid = td.tid");
  $query->join('taxonomy_vocabulary', 'v', "td.vid = v.vid");
  $query->join('field_data_field_episode', 'e', "n.nid = e.entity_id");
  $query->join('field_data_field_episode_number', 'en', "e.field_episode_target_id = en.entity_id");
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('td', array('name'))
    ->fields('en', array('field_episode_number_value'))
    ->condition('n.type', 'act')
    ->condition('n.status', 1)
    ->condition('v.machine_name', 'tags')
    ->orderBy('td.name', 'ASC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    $rows[] = array(
      $result->name,
      $result->title,
      $result->field_episode_number_value,
    );
  }
  if (!empty($rows)) {
    header('Content-Type: text/csv; charset=utf-8');
    header('Content-Disposition: attachment; filename=tags.csv');
    $output = fopen('php://output', 'w');
    fputcsv($output, array('Tag Name', 'Act Name', 'Episode Number'));
    foreach ($rows as $row) {
      fputcsv($output, $row);
    }
    exit;
  }

  return $render;
}

function tal_content_advisory_form($form, &$form_state) {
  $form['episodes'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
    '#title' => t('Episodes'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Content Advisory'),
  );
  return $form;
}

function tal_content_advisory_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  if (!empty($values['episodes']) && $episodes = preg_split('/[\s,]+/', $values['episodes'])) {
    foreach ($episodes as $episode) {
      if (is_numeric($episode) && $node = tal_episode_number_load($episode)) {
        $node->field_advisory[$node->language][0]['value'] = 1;
        node_save($node);
        drupal_set_message(t('Content advisory was set for !episode.', array('!episode' => l($node->title, 'node/'.$node->nid))));
      }
    }
  }
}
