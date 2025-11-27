<?php

function tal_about_announcements_page() {
  $page = pager_find_page();
  $render = array();
  $query = db_select('node', 'n')->extend('PagerDefault');
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND entity_type ='node' AND deleted = 0)");
  $query
    ->fields('n', array('nid'))
    ->condition('n.status', 1)
    ->condition('n.type', 'announcement')
    ->condition('rad.field_radio_air_date_value', date('c'), '<')
    ->orderBy('rad.field_radio_air_date_value', 'DESC')
    ->limit(10);
  if ($nids = $query->execute()->fetchCol()) {
    $nodes = node_load_multiple($nids);
    $render['nodes'] = node_view_multiple($nodes, 'about');
    $render['nodes']['#prefix'] = '<div class="nodes clearfix page-'.$page.'">';
    $render['nodes']['#suffix'] = '</div>';;
  }
  $render['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 0,
    '#tags' => array(
      '', '', '', t('Load More'), '',
    ),
  );

  if (!empty($_GET['page']) && tal_is_ajax()) {
    $data = array(
      'path' => url(current_path(), array('query' => drupal_get_query_parameters($_GET, array('q', 'page')))),
      'html' => drupal_render($render),
    );
    drupal_add_http_header('Cache-Control', 'public, max-age=' . variable_get('cache_lifetime', 300));
    drupal_json_output($data);
    drupal_exit();
  }
  else {
    return $render;
  }
}

function tal_about_staff_page() {
  $render = array();
  $query = db_select('node', 'n');
  $query->join('field_data_field_last_name', 'ln', "(ln.entity_id = n.nid AND ln.entity_type ='node' AND ln.deleted = 0)");
  $query
    ->fields('n', array('nid'))
    ->condition('n.status', 1)
    ->condition('n.type', 'staff')
    ->orderBy('ln.field_last_name_value', 'ASC');
    $count = 0;
  if ($nids = $query->execute()->fetchCol()) {
    $nodes = node_load_multiple($nids);
    foreach ($nodes as $node) {
      $render['nodes'][$node->nid] = node_view($node);
      if ($node->title == 'Ira Glass') {
        $render['nodes'][$node->nid]['#weight'] = -101;
      }
      else {
        $render['nodes'][$node->nid]['#weight'] = $count;
      }
      $count++;
    }
    $render['nodes']['#prefix'] = '<div class="nodes clearfix">';
    $render['nodes']['#suffix'] = '</div>';
  }

  return $render;
}

/**
 * undocumented function
 */
 function tal_about_import_events() {
   if ($vocabulary = taxonomy_vocabulary_machine_name_load('events')) {
     if ($term = taxonomy_term_machine_name_load('ira_glass', $vocabulary)) {
       $result = drupal_http_request('http://www.barclayagency.com/site/speaker/ira-glass');

       if ($result->code == 200) {
         $events = array();
         $html = str_replace(array('&nbsp;'), ' ', $result->data);
         if (preg_match_all('/<tr[^>]*>((\s*<td>(.*?)<\/td>\s*){3})<\/tr>/si', $html, $rows)) {
           foreach ($rows[1] as $row) {
             if (preg_match_all('/<td*>(.*?)<\/td>/si', trim($row), $cells)) {
               $url = '';
               $location = trim($cells[1][2]);
               if (preg_match('/<a\s*href="([^"]+)"[^>]*>(.*?)<\/a>/si', $location, $venue)) {
                 $url = $venue[1];
                 $location = $venue[2];
               }
               else {
                 $location = filter_xss($location, array());
               }
               if (preg_match('/(\w{3})\s*(\d{1,2})\s*&\s*(\d{1,2})\s*,\s*(\d{4})/si', trim($cells[1][0]), $dates)) {
                 $date = date('Y-m-d', strtotime("$dates[1] $dates[2], $dates[4]")).'T00:00:00';
                 $events[] = array(
                   'date' => $date,
                   'url' => $url,
                   'city' => trim($cells[1][1]),
                   'venue' => trim($location),
                 );
                 $date = date('Y-m-d', strtotime("$dates[1] $dates[3], $dates[4]")).'T00:00:00';
                 $events[] = array(
                   'date' => $date,
                   'url' => $url,
                   'city' => trim($cells[1][1]),
                   'venue' => trim($location),
                 );
               }
               else {
                 $date = date('Y-m-d', strtotime(trim($cells[1][0]))).'T00:00:00';
                 $events[] = array(
                   'date' => $date,
                   'url' => $url,
                   'city' => trim($cells[1][1]),
                   'venue' => trim($location),
                 );
               }
             }
           }
         }


         foreach ($events as $event) {
           $query = db_select('node', 'n');
           $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND entity_type ='node' AND deleted = 0)");
           $query->join('taxonomy_index', 'ti', 'n.nid = ti.nid');
           $query
             ->fields('n', array('nid'))
             ->condition('ti.tid', $term->tid)
             ->condition('n.type', 'event')
             ->condition('rad.field_radio_air_date_value', $event['date']);
           if ($nid = $query->execute()->fetchField()) {
             $node = node_load($nid);
             $node->title = $event['city'];
             $node->field_radio_air_date[LANGUAGE_NONE][0]['value'] = $event['date'];
             $node->field_link[LANGUAGE_NONE][0]['url'] = $event['url'];
             $node->field_link[LANGUAGE_NONE][0]['title'] = $event['venue'];
           }
           else {
             $node = new stdClass();
             $node->language = LANGUAGE_NONE;
             $node->uid = 1;
             $node->name = 'rorris';
             $node->type = 'event';
             $node->title = $event['city'];
             node_object_prepare($node);
             $node->field_radio_air_date[LANGUAGE_NONE][0]['value'] = $event['date'];
             $node->created = REQUEST_TIME;
             $node->field_link[LANGUAGE_NONE][0]['url'] = $event['url'];
             $node->field_link[LANGUAGE_NONE][0]['title'] = $event['venue'];
             $node->field_event_type[LANGUAGE_NONE][0]['tid'] = $term->tid;
           }
           node_save($node);
         }
         if (!empty($_GET['destination'])) {
           drupal_set_message(t('Events has been refreshed.'));
           drupal_goto('');
         }
       }
     }
   }
 }
