<?php

/**
* undocumented function
*/
function tal_goto_page() {
  $q = filter_xss($_GET['q'], array());
  $data = array(
    'original' => $q,
    'title' => '',
    'html' => '',
  );

  if (!arg(1)) {
    $path = variable_get('site_frontpage', 'node');
    $data['request_path'] = 'home';
  }
  elseif (arg(1) == 'radio-archives' && arg(2) == 'episode' && arg(3) && $node = tal_episode_number_load(arg(3))) {
    $path = 'node/'.$node->nid;
    $data['normal'] = $path;
    $data['request_path'] = url($path);
  }
  else {
    $path = str_replace('goto/', '', $q);
    list($section) = explode('/', $path);
    $data['section'] = drupal_html_class($section);
    $data['request_path'] = drupal_html_class($path);
    $path = drupal_get_normal_path($path);
    $data['normal'] = $path;
  }
  if (tal_is_ajax()) {
    menu_set_active_item($path);
    if ($router_item = menu_get_item($path)) {
      if ($router_item['access']) {
        if ($router_item['include_file']) {
          require_once DRUPAL_ROOT . '/' . $router_item['include_file'];
        }
        $page = call_user_func_array($router_item['page_callback'], $router_item['page_arguments']);
      }
      else {
        drupal_not_found();
        drupal_exit();
      }
    }
    else {
      drupal_not_found();
      drupal_exit();
    }

    /* Taken from drupal_render_page() */
    $main_content_display = &drupal_static('system_main_content_added', FALSE);

    // Allow menu callbacks to return strings or arbitrary arrays to render.
    // If the array returned is not of #type page directly, we need to fill
    // in the page with defaults.
    if (is_string($page) || (is_array($page) && (!isset($page['#type']) || ($page['#type'] != 'page')))) {
      drupal_set_page_content($page);
      $page = element_info('page');
    }

    // Modules can add elements to $page as needed in hook_page_build().
    foreach (module_implements('page_build') as $module) {
      $function = $module . '_page_build';
      $function($page);
    }
    // Modules alter the $page as needed. Blocks are populated into regions like
    // 'sidebar_first', 'footer', etc.
    drupal_alter('page', $page);

    // If no module has taken care of the main content, add it to the page now.
    // This allows the site to still be usable even if no modules that
    // control page regions (for example, the Block module) are enabled.
    if (!$main_content_display) {
      $page['content']['system_main'] = drupal_set_page_content();
    }

    if ($node = menu_get_object()) {
      $data['node_type'] = drupal_html_class($node->type);
      $data['facebook'] = tal_share_url('facebook', $node);
      $data['twitter'] = tal_share_url('twitter', $node);
      $data['mail'] = tal_share_url('mail', $node);
      if ($items = field_get_items('node', $node, 'field_background_color')) {
        $data['color'] = $items[0]['rgb'];
      }
      else {
        if ($node->type == 'episode') {
          $episode = $node;
        }
        else if ($node->type == 'act') {
          if ($items = field_get_items('node', $node, 'field_episode')) {
            $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
          }
        }
        else if ($items = field_get_items('node', $node, 'field_episodes')) {
          if ($episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
            if ($episode->type == 'act') {
              if ($items = field_get_items('node', $episode, 'field_episode')) {
                $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
              }
            }
          }
        }
        if (!empty($episode) && $episode->type == 'episode' && $items = field_get_items('node', $episode, 'field_background_color')) {
          $data['color'] = $items[0]['rgb'];
        }
      }
    }
    else {
      $data['facebook'] = tal_share_url('facebook');
      $data['twitter'] = tal_share_url('twitter');
      $data['mail'] = tal_share_url('mail');
      if (arg(0) == 'about') {
        $title = array(
          'title' => array(
            '#markup' => menu_get_active_title(),
            '#prefix' => '<h1 id="page-title"><span>',
            '#suffix' => '</span></h1>',
            '#weight' => 0,
          )
        );
        $page['content'] = $title + $page['content'];
      }
    }
    $data['title'] = menu_get_active_title();
    $data['main'] = drupal_render($page['content']);
    if (!empty($page['top'])) {
      $data['top'] = drupal_render($page['top']);
    }
    if (!empty($page['sidebar_first'])) {
      $data['sidebar'] = drupal_render($page['sidebar_first']);
    }

    drupal_add_http_header('Cache-Control', 'public, max-age=' . variable_get('cache_lifetime', 300));
    drupal_json_output($data);
    drupal_exit();
  }
  drupal_goto($path);
}

function tal_archive_page() {
  $date = REQUEST_TIME - (20*60*60);
  $render = array();
  $items = array();
  $filters = array();
  $per_page = 48;
  $page = (!empty($_GET['page']) && is_numeric($_GET['page']) ? (int) $_GET['page'] : 0);
  $types = array(
    'episodes' => t('Episodes'),
    'videos' => t('Videos'),
    'extras' => t('Extras'),
    'tv' => t('TV Show'),
  );

  // Test for term-based filters
  foreach (array('tag', 'contributor') as $key) {
    $params = drupal_get_query_parameters($_GET, array('q', $key, 'page'));
    if (!empty($_GET[$key]) && is_numeric($_GET[$key]) && $term = taxonomy_term_load($_GET[$key])) {
      $filters[$key] = $term;
      $items[] = l($term->name, current_path(), array('query' => $params));
    }
  }

  // Test for year filters
  $params = drupal_get_query_parameters($_GET, array('q', 'year', 'page'));
  if (!empty($_GET['year']) && is_numeric($_GET['year'])) {
    $items[] = l($_GET['year'], current_path(), array('query' => $params));
  }

  // Create filter chiclets
  if (!empty($items)) {
    $render['chiclet'] = array(
      '#weight' => -10,
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array(
        'class' => array('chiclets'),
      ),
    );
  }

  if (!empty($filters['contributor'])) {
    $term = $filters['contributor'];
    if (field_get_items('taxonomy_term', $term, 'field_image') || !empty($term->description)) {
      $render['contributor'] = taxonomy_term_view($filters['contributor']);
      $render['contributor']['#weight'] = -9;
    }
  }

  // Test for keyword
  if (!empty($_GET['keyword'])) {
    $filters['keyword'] = filter_xss($_GET['keyword'], array());
  }

  // Test for content type
  if (!empty($_GET['type'])) {
    $type = $_GET['type'];
    if (!empty($types[$type])) {
      $filters['type'] = $type;
    }
  }
  // Are we filtered?
  if (!empty($filters)) {
    // Just show episodes in grid
    if (count($filters) == 1 && !empty($filters['type']) && $filters['type'] == 'episodes') {
      $query = db_select('node', 'n')->extend('PagerDefault');
      $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND entity_type ='node' AND deleted = 0)");
      $query
        ->fields('n', array('nid'))
        ->condition('n.status', 1)
        ->condition('n.type', 'episode')
        ->condition('rad.field_radio_air_date_value', date('c', $date), '<')
        ->orderBy('rad.field_radio_air_date_value', 'DESC');
        if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
          $year = $_GET['year'];
          $query
            ->condition('rad.field_radio_air_date_value', date('c', gmmktime(0,0,0,1,1,$year)), '>=')
            ->condition('rad.field_radio_air_date_value', date('c', gmmktime(0,0,0,1,1,$year+1)), '<');
        }
        if (empty($_GET['page'])) {
          $params = drupal_get_query_parameters($_GET, array('q', 'year', 'page'));
          $class = array('year');
          if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
            $year = $_GET['year'];
          }
          else {
            $class[] = 'current';
          }
          $items = array(
            l(t('All'), 'archive', array('query' => $params, 'attributes' => array('class' => $class))),
          );
          for ($i = date('Y'); $i >= 1995; $i--) {
            $class = array('year');
            if (!empty($year) && $i == $year) {
              $class[] = 'current';
              unset($params['year']);
            }
            else {
              $params['year'] = $i;
            }
            $items[] = l($i, 'archive', array('query' => $params, 'attributes' => array('class' => $class)));
          }
          $render['years'] = array(
            '#theme' => 'item_list',
            '#items' => $items,
            '#attributes' => array('id' => 'year-filter'),
          );
        }

        if (empty($_GET['page']) && $num_rows = $query->countQuery()->execute()->fetchField()) {
          $render['meta'] = array(
            '#prefix' => '<div class="count-sort">',
            '#suffix' => '</div>',
          );
          $render['meta']['count'] = array(
            '#prefix' => '<div class="count">',
            '#suffix' => '</div>',
            '#markup' => format_plural($num_rows, 'There is 1 result', 'There are @count results'),
          );
        }
      $query->limit($per_page);
      if ($nids = $query->execute()->fetchCol()) {
        $nodes = node_load_multiple($nids);
        $render['nodes'][] = node_view_multiple($nodes, 'teaser');
        $render['nodes']['#prefix'] = '<div class="nodes clearfix">';
        $render['nodes']['#suffix'] = '</div>';
        $render['nodes']['#weight'] = 99;
      }
      $render['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 100,
        '#quantity' => 0,
        '#tags' => array(
          '', '', '', t('Load More'), '',
        ),
      );
    }
    // Just show episodes in grid
    else if (count($filters) == 1 && !empty($filters['type']) && $filters['type'] == 'tv') {
      $query = db_select('node', 'n')->extend('PagerDefault');
      $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND entity_type ='node' AND deleted = 0)");
      $query
        ->fields('n', array('nid'))
        ->condition('n.status', 1)
        ->condition('n.type', 'tv')
        ->condition('rad.field_radio_air_date_value', date('c', $date), '<')
        ->orderBy('rad.field_radio_air_date_value', 'DESC');
        if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
          $year = $_GET['year'];
          $query
            ->condition('rad.field_radio_air_date_value', date('c', gmmktime(0,0,0,1,1,$year)), '>=')
            ->condition('rad.field_radio_air_date_value', date('c', gmmktime(0,0,0,1,1,$year+1)), '<');
        }
        if (empty($_GET['page'])) {
          $params = drupal_get_query_parameters($_GET, array('q', 'year', 'page'));
          $class = array('year');
          if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
            $year = $_GET['year'];
          }
          else {
            $class[] = 'current';
          }
          $items = array(
            l(t('All'), 'archive', array('query' => $params, 'attributes' => array('class' => $class))),
          );
          for ($i = 2008; $i >= 2007; $i--) {
            $class = array('year');
            if (!empty($year) && $i == $year) {
              $class[] = 'current';
              unset($params['year']);
            }
            else {
              $params['year'] = $i;
            }
            $items[] = l($i, 'archive', array('query' => $params, 'attributes' => array('class' => $class)));
          }
          $render['years'] = array(
            '#theme' => 'item_list',
            '#items' => $items,
            '#attributes' => array('id' => 'year-filter'),
          );
        }

        if (empty($_GET['page']) && $num_rows = $query->countQuery()->execute()->fetchField()) {
          $render['meta'] = array(
            '#prefix' => '<div class="count-sort">',
            '#suffix' => '</div>',
          );
          $render['meta']['count'] = array(
            '#prefix' => '<div class="count">',
            '#suffix' => '</div>',
            '#markup' => format_plural($num_rows, 'There is 1 result', 'There are @count results'),
          );
        }
      $query->limit($per_page);
      if ($nids = $query->execute()->fetchCol()) {
        $nodes = node_load_multiple($nids);
        $render['nodes'][] = node_view_multiple($nodes, 'teaser');
        $render['nodes']['#prefix'] = '<div class="nodes clearfix">';
        $render['nodes']['#suffix'] = '</div>';
        $render['nodes']['#weight'] = 99;
      }
      $render['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 100,
        '#quantity' => 0,
        '#tags' => array(
          '', '', '', t('Load More'), '',
        ),
      );
    }
    else {
      $count = 0;
      if (empty($_GET['page'])) {
        $render['meta'] = array(
          '#prefix' => '<div class="count-sort">',
          '#suffix' => '</div>',
        );
      }
      if ($nodes = tal_search_results()) {
        $render['nodes'] = array(
          '#prefix' => '<div class="nodes clearfix">',
          '#suffix' => '</div>',
          '#weight' => 99,
        );
        $last_episode = false;
        foreach ($nodes as $node) {
          if ($node->type == 'act'){
            if ($items = field_get_items('node', $node, 'field_episode')) {
              $episode_nid = $items[0]['target_id'];
              if ($episode_nid == $last_episode) {
                $node->hide_episode = true;
              }
              $last_episode = $episode_nid;
            }
          }
          else {
            $last_episode = false;
          }
          $render['nodes'][] = node_view($node, 'archive');
        }
        $render['pager'] = array(
          '#weight' => 100,
          '#theme' => 'pager',
          '#quantity' => 0,
          '#tags' => array(
            '', '', '', t('Load More'), '',
          ),
        );
      }
      if (empty($_GET['page'])) {
        if (apachesolr_has_searched('solr')) {
          $current_query = apachesolr_current_query('solr');
          $searcher = $current_query->getSearcher();
          $response = apachesolr_static_response_cache($searcher);
          if (isset($response->response->numFound)) {
            if (empty($_GET['page']) && !empty($response->facet_counts->facet_fields->is_year)) {
              $years = (array) $response->facet_counts->facet_fields->is_year;
              krsort($years);
              $params = drupal_get_query_parameters($_GET, array('q', 'year', 'page'));
              $class = array('year');
              if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
                $year = $_GET['year'];
              }
              else {
                $class[] = 'current';
              }
              $items = array(
                l(t('All'), 'archive', array('query' => $params, 'attributes' => array('class' => $class))),
              );
              foreach ($years as $i => $count) {
                $class = array('year');
                if (!empty($_GET['year']) && $i == $_GET['year']) {
                  $class[] = 'current';
                  unset($params['year']);
                }
                else {
                  $params['year'] = $i;
                }
                $items[] = l($i, 'archive', array('query' => $params, 'attributes' => array('class' => $class)));
              }
              $render['years'] = array(
                '#theme' => 'item_list',
                '#items' => $items,
                '#attributes' => array('id' => 'year-filter'),
                '#weight' => -101,
              );
            }
            $render['meta']['count'] = array(
              '#prefix' => '<div class="count">',
              '#suffix' => '</div>',
              '#markup' => format_plural($response->response->numFound, 'There is 1 result', 'There are @count results'),
            );
          }
        }
        // Only show facet on count if we have one
        if (count($filters) == 1) {
          if (!empty($render['meta']['count'])) {
            if (!empty($filters['contributor'])) {
              $render['meta']['count']['#markup'] .= '<span>'.t(' for "!term"', array('!term' => $filters['contributor']->name)).'</span>';
            }
            if (!empty($filters['tag'])) {
              $render['meta']['count']['#markup'] .= '<span>'.t(' for "!term"', array('!term' => $filters['tag']->name)).'</span>';
            }
            if (!empty($filters['keyword'])) {
              $render['meta']['count']['#markup'] .= '<span>'.t(' for "!term"', array('!term' => $filters['keyword'])).'</span>';
            }
          }
        }
        $params = drupal_get_query_parameters($_GET, array('q', 'type', 'page'));
        $items = array();
        $class = array();
        if (empty($filters['type'])) {
          $class[] = 'current';
        }
        $items[] = array('class' => $class, 'data' => l(t('All'), current_path(), array('query' => $params)));
        foreach ($types as $key => $value) {
          $class = array();
          $params['type'] = $key;
          if (!empty($filters['type']) && $filters['type'] == $key) {
            $class[] = 'current';
          }
          if (!empty($response->facet_counts->facet_fields->ss_type->{$key})) {
            $items[] = array('class' => $class, 'data' => l($value.' ('.$response->facet_counts->facet_fields->ss_type->{$key}.')', current_path(), array('query' => $params)));
          }
        }

        if (count($items) > 2) {
          $render['meta']['types'] = array(
            '#theme' => 'item_list',
            '#items' => $items,
            '#attributes' => array('id' => 'type-sort'),
          );
        }
      }
    }
  }
  else {
    // Find recently aired
    $exclude = array();
    $recently = array();
    $date = REQUEST_TIME - (20*60*60);
    $query2 = db_select('node', 'n');
    $query2->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
    $query2->join('field_data_field_episode', 'e', "(e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)");
    $query2
      ->fields('e', array('field_episode_target_id'))
      ->fields('rad', array('field_radio_air_date_value'))
      ->condition('n.type', 'schedule')
      ->condition('rad.field_radio_air_date_value', date('c', $date), '<')
      ->orderBy('rad.field_radio_air_date_value', 'DESC')
      ->range(0,4);
    $result = $query2->execute();
    foreach ($result as $row) {
      if ($episode = node_load($row->field_episode_target_id)) {
        $ep_air_date = substr($episode->field_radio_air_date[LANGUAGE_NONE][0]['value'],0,10);
        $sched_air_date = substr($row->field_radio_air_date_value,0,10);
        if ($sched_air_date == $ep_air_date) {
          $exclude[] = $episode->nid;
        }
        $episode->field_radio_air_date[LANGUAGE_NONE][0]['value'] = $row->field_radio_air_date_value;
        $recently[] = $episode;
      }
    }


    // Year nav
    if (empty($_GET['page'])) {
      $params = drupal_get_query_parameters($_GET, array('q', 'year', 'page'));
      $class = array('year');
      if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
        $year = $_GET['year'];
      }
      else {
        $class[] = 'current';
      }
      $items = array(
        l(t('All'), 'archive', array('query' => $params, 'attributes' => array('class' => $class))),
      );
      for ($i = date('Y'); $i >= 1995; $i--) {
        $class = array('year');
        if (!empty($year) && $i == $year) {
          $class[] = 'current';
          unset($params['year']);
        }
        else {
          $params['year'] = $i;
        }
        $items[] = l($i, 'archive', array('query' => $params, 'attributes' => array('class' => $class)));
      }
      $render['years'] = array(
        '#theme' => 'item_list',
        '#items' => $items,
        '#attributes' => array('id' => 'year-filter'),
      );
    }

    // Load current page
    $query = db_select('node', 'n');
    $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND entity_type ='node' AND deleted = 0)");
    $query
      ->fields('n', array('nid'))
      ->condition('n.status', 1)
      ->condition('n.type', array('episode', 'article', 'gallery', 'video', 'fullscreen'))
      ->condition('rad.field_radio_air_date_value', date('c', $date), '<')
      ->orderBy('rad.field_radio_air_date_value', 'DESC');
      if (!empty($exclude)) {
        $query->condition('n.nid', $exclude, 'NOT IN');
      }
    if (!empty($_GET['year']) && is_numeric($_GET['year']) && $_GET['year'] >= 1995 && $_GET['year'] <= date('Y')) {
      $year = $_GET['year'];
      $query
        ->condition('rad.field_radio_air_date_value', date('c', mktime(0,0,0,1,1,$year)), '>=')
        ->condition('rad.field_radio_air_date_value', date('c', mktime(0,0,0,1,1,$year+1)), '<');
    }
    else {
      $query = $query->extend('PagerDefault')->limit($per_page);
      if (empty($_GET['page'])) {
        foreach ($recently as $node) {
          $render['recently'][] = node_view($node, 'teaser');
        }
        //$render['recently'][] = node_view_multiple($recently, 'teaser');
        $render['recently']['#prefix'] = '<h2 class="label">'.t('Recently Aired').'</h2><div class="nodes with-label clearfix">';
        $render['recently']['#suffix'] = '</div>';
        $render['recently']['#weight'] = 88;
        $render['nodes']['#prefix'] = '<h2 class="label">'.t('Archive').'</h2><div class="nodes with-label clearfix">';
      }
    }

    if ($nids = $query->execute()->fetchCol()) {
      $nodes = node_load_multiple($nids);
      $render['nodes'][] = node_view_multiple($nodes, 'teaser');
      if (empty($render['nodes']['#prefix'])) {
        $render['nodes']['#prefix'] = '<div class="nodes clearfix">';
      }
      $render['nodes']['#suffix'] = '</div>';
      $render['nodes']['#weight'] = 99;
    }
    $render['pager'] = array(
      '#theme' => 'pager',
      '#quantity' => 0,
      '#weight' => 100,
      '#tags' => array(
        '', '', '', t('Load More'), '',
      ),
    );
  }
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

function tal_broadcast_acts_csv() {
  $rows = array();
  $query = db_select('node', 'n');
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
  $query->join('field_data_field_episode', 'e', "(e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)");
  $query->join('node', 'n2', "e.field_episode_target_id = n2.nid");
  $query->join('field_data_field_episode_number', 'en', "(en.entity_id = n2.nid AND en.entity_type ='node' AND en.deleted = 0)");
  $query
    ->fields('n2', array('nid', 'title'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('rad', array('field_radio_air_date_value'))
    ->condition('n.type', 'schedule')
    ->condition('n2.type', 'episode')
    ->orderBy('rad.field_radio_air_date_value', 'ASC');
  $results = $query->execute();
  foreach ($results as $row) {
    $timestamp = strtotime($row->field_radio_air_date_value);
    $episode = $row->field_episode_number_value;
    $dates[$timestamp] = $episode;
    if (empty($episodes[$row->nid])) {
      $episodes[$row->nid] = array(
        'episode' => $episode,
        'title' => $row->title,
        'timestamp' => $timestamp,
        'dates' => array(date('Y', $timestamp)),
      );
    }
    else {
      $episodes[$row->nid]['dates'][] = date('Y', $timestamp);
    }
  }

  $query = db_select('node', 'n');
  $query->join('field_data_field_episode', 'e', "(e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)");
  $query->leftJoin('field_data_field_act_number', 'an', "(an.entity_id = n.nid AND an.entity_type ='node' AND e.deleted = 0)");
  $query->leftJoin('field_data_body', 'b', "(b.entity_id = n.nid AND b.entity_type ='node' AND e.deleted = 0)");
  $query->leftJoin('field_data_field_nonact', 'na', "(na.entity_id = n.nid AND na.entity_type ='node' AND e.deleted = 0)");
  $query->condition(db_or()
    ->isNull('na.field_nonact_value')
    ->condition('na.field_nonact_value', 0)
  );
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('e', array('field_episode_target_id'))
    ->fields('an', array('field_act_number_value'))
    ->fields('b', array('body_value'))
    ->condition('n.type', 'act')
    ->orderBy('an.field_act_number_value', 'ASC');
  $results = $query->execute();
  foreach ($results as $act) {
    if (!empty($episodes[$act->field_episode_target_id])) {
      $episode = $episodes[$act->field_episode_target_id];
      $row = array(
        date('m/d/Y', $episode['timestamp']),
        $episode['episode'],
        $episode['title'],
        implode(', ', $episode['dates']),
        $act->field_act_number_value,
        $act->title,
        filter_xss($act->body_value, array()),
      );
      if (preg_match('/\((.*)\s+minutes?\)/i', $act->body_value, $match)) {
        $row[] = floatval($match[1]);
      }
      else {
        $row[] = 'none';
      }
      // Contributors
      $contributors = array();
      $contributor_query = db_select('taxonomy_term_data', 'td');
      $contributor_query->join('field_data_field_contributor', 'c', "(td.tid = c.field_contributor_target_id AND c.bundle = 'act' AND c.entity_type ='node' AND c.deleted = 0)");
      $contributor_query
        ->fields('td', array('name'))
        ->condition('c.entity_id', $act->nid)
        ->orderBy('td.name', 'ASC');
      $contributor_results = $contributor_query->execute();
      foreach ($contributor_results as $contributor) {
        $contributors[] = $contributor->name;
      }
      $row[] = implode(', ', $contributors);
      // Tags
      $tags = array();
      $tag_query = db_select('taxonomy_term_data', 'td');
      $tag_query->join('field_data_field_tags', 'c', "(td.tid = c.field_tags_tid AND c.bundle = 'act' AND c.entity_type ='node' AND c.deleted = 0)");
      $tag_query
        ->fields('td', array('name'))
        ->condition('c.entity_id', $act->nid)
        ->orderBy('td.name', 'ASC');
      $tag_results = $tag_query->execute();
      foreach ($tag_results as $tag) {
        $tags[] = $tag->name;
      }
      $row[] = implode(', ', $tags);
      $row[] = url('node/'.$act->nid, array('absolute' => true));
      $rows[] = $row;
    }
  }
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=TAL_acts.csv');
  $output = fopen('php://output', 'w');
  $row = array(
    'Air date',
    'Episode number',
    'Episode name',
    'Rebroadcast',
    'Act number',
    'Act name',
    'Act description',
    'Act length',
    'Contributor(s)',
    'Tag(s)',
    'Act URL',
  );
  fputcsv($output, $row);
  foreach ($rows as $row) {
    fputcsv($output, $row);
  }
  fclose($output);
  exit;
}

function tal_broadcast_acts_page() {
  $render = array();
  $rows = array();
  $rows[] = array(
    array(
      'data' => l(t('Download Spreadsheet'), 'broadcast/acts.csv'),
      'colspan' => 4,
    ),
  );
  $query = db_select('node', 'n');
  $query->join('field_data_field_episode', 'e', "n.nid = e.entity_id");
  $query->join('field_data_field_act_label', 'al', "n.nid = al.entity_id");
  $query->join('field_data_field_act_number', 'an', "n.nid = an.entity_id");
  $query->join('field_data_field_episode_number', 'en', "e.field_episode_target_id = en.entity_id");
  $query->join('node', 'n2', "e.field_episode_target_id = n2.nid");
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n2.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('n2', array('nid', 'title'))
    ->fields('al', array('field_act_label_value'))
    ->fields('an', array('field_act_number_value'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('rad', array('field_radio_air_date_value'))
    ->condition('n.type', 'act')
    ->condition('n2.type', 'episode')
    ->condition('n.status', 1)
    ->orderBy('en.field_episode_number_value', 'DESC')
    ->orderBy('an.field_act_number_value', 'ASC');
  $results = $query->execute();
  foreach ($results as $result) {
    // Contributors
    $contributors = array();
    $contributor_query = db_select('taxonomy_term_data', 'td');
    $contributor_query->join('field_data_field_contributor', 'c', "(td.tid = c.field_contributor_target_id AND c.bundle = 'act' AND c.entity_type ='node' AND c.deleted = 0)");
    $contributor_query
      ->fields('td', array('name'))
      ->condition('c.entity_id', $result->nid)
      ->orderBy('td.name', 'ASC');
    $contributor_results = $contributor_query->execute();
    foreach ($contributor_results as $contributor) {
      $contributors[] = $contributor->name;
    }

    $act = array();
    if (!empty($result->field_act_label_value)) {
      $act[] = $result->field_act_label_value;
    }
    else if (!empty($result->field_act_number_value)) {
      $act[] = t('Act !number', array('!number' => $result->field_act_number_value));
    }
    $act[] = $result->title;

    $title = array(
      $result->n2_title,
      l(implode(': ', $act), 'node/'.$result->nid),
    );

    $row = array(
      date('m/d/Y', strtotime($result->field_radio_air_date_value)),
      l($result->field_episode_number_value, 'node/'.$result->n2_nid),
      implode('<br>', $title),
      implode(', ', $contributors),
    );
    $rows[] = $row;
  }
  $render[] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#attributes' => array(
      'id' => 'broadcast-episodes',
      'class' => array('acts'),
    ),
  );
  return $render;
}


function tal_broadcast_page() {
  $render = array();
  $nid_to_episode = array();
  $originals = array();
  $episodes = array();
  $dates = array();
  $years = array();
  $query = db_select('node', 'n');
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
  $query->join('field_data_field_episode', 'e', "(e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)");
  $query->join('node', 'n2', "e.field_episode_target_id = n2.nid");
  $query->leftJoin('field_data_field_episode_number', 'en', "(en.entity_id = n2.nid AND en.entity_type ='node' AND en.deleted = 0)");
  $query->leftJoin('field_data_field_advisory', 'a', "(a.entity_id = n2.nid AND a.entity_type ='node' AND a.deleted = 0)");
  $query->leftJoin('field_data_field_original', 'o', "(o.entity_id = n2.nid AND o.entity_type ='node' AND o.deleted = 0)");
  $query
    ->fields('n2', array('nid', 'title'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('rad', array('field_radio_air_date_value'))
    ->fields('a', array('field_advisory_value'))
    ->fields('o', array('field_original_target_id'))
    ->condition('n.type', 'schedule')
    ->condition('n2.type', 'episode')
    ->orderBy('rad.field_radio_air_date_value', 'DESC');
  $results = $query->execute();
  foreach ($results as $row) {
    $timestamp = strtotime($row->field_radio_air_date_value);
    $episode = $row->field_episode_number_value;
    $nid_to_episode[$row->nid] = $episode;
    $dates[$timestamp] = $episode;
    if (empty($episodes[$episode])) {
      $episodes[$episode] = array(
        'title' => $row->title,
        'nid' => $row->nid,
        'dates' => array($timestamp),
        'advisory' => $row->field_advisory_value,
      );
    }
    else {
      $episodes[$episode]['dates'][] = $timestamp;
    }
    if (!empty($row->field_original_target_id)) {
      $originals[$episode] = $row->field_original_target_id;
    }
  }

  foreach ($originals as $rerun_number => $original_nid) {
    if ($original_number = $nid_to_episode[$original_nid]) {
      $timestamps = array();
      if (!empty($episodes[$rerun_number])) {
        $timestamps = array_merge($timestamps, $episodes[$rerun_number]['dates']);
      }
      if (!empty($episodes[$original_number])) {
        $timestamps = array_merge($timestamps, $episodes[$original_number]['dates']);
      }
      rsort($timestamps);
      if (!empty($episodes[$rerun_number])) {
        $episodes[$rerun_number]['dates'] = $timestamps;
      }
      if (!empty($episodes[$original_number])) {
        $episodes[$original_number]['dates'] = $timestamps;
      }
    }
  }

  $items = array();

  $advisory = array();
  $advisory[] = '<span class="advisory" title="'.t('Content Advisory').'">';
  $advisory[] = '<span class="asterisk">*</span>';
  $advisory[] = ' = ';
  $advisory[] = t('content advisory');
  $advisory[] = '</span>';
  $items[] = array('', '', implode('', $advisory));

  foreach ($dates as $timestamp => $episode_number) {
    $this_year = date('Y', $timestamp);
    $episode = $episodes[$episode_number];
    $last = end($episode['dates']);

    //  Insert special data AFTER specific episodes
    if ($timestamp == $last) {
      switch ($episode_number) {
        case '16':
        $items[] = array(array('data' => 'Show name changed from Your Radio Playhouse to This American Life', 'colspan' => 3));
        break;
        case '68':
        $items[] = array(array('data' => 'PRI started distributing the show', 'colspan' => 3));
        break;
        case '137':
        $items[] = array(array('data' => 'Switched to Shure microphone', 'colspan' => 3));
        break;
        case '410':
        $items[] = array(array('data' => 'Switched from SDII to WAV', 'colspan' => 3));
        break;
      }
    }

    //  Add Episode air dates to timeline - link if 1st air date
    $item = array();
    $item[] = array(
      'data' => date('m/d/Y', $timestamp),
    );
    $item[] = array(
      'data' => ($timestamp == $last ? l($episode_number, 'node/'.$episode['nid']) : $episode_number),
      'class' => array('episode-number'),
    );
    $these_years = array();
    foreach ($episode['dates'] as $rerun) {
      if ($rerun != $timestamp) {
        $this_year = date('Y', $rerun);
        $these_years[$this_year] = $this_year;
      }
    }

    //  Mark content advisory
    $advisory = array();
    if (!empty($episode['advisory'])) {
      $advisory[] = '<span class="advisory" title="'.t('Content Advisory').'">';
      $advisory[] = '<span class="asterisk">*</span>';
      $advisory[] = '</span>';
    }
    $item[] = array(
      'data' => l($episode['title'], 'node/'.$episode['nid']).(!empty($these_years) ? ' ('.implode(', ', $these_years).')' : '').implode('', $advisory),
    );
    $items[] = $item;

    //  Insert special data BEFORE specific episodes
    if ($timestamp == $last) {
      switch ($episode_number) {
        case '837':
        $items[] = array(array('data' => 'Added 2nd ID break/midroll', 'colspan' => 3));
        break;
      }
    }
  }
  $render[] = array(
    '#theme' => 'table',
    '#rows' => $items,
    '#attributes' => array('id' => 'broadcast-episodes'),
  );
  return $render;
}

function tal_broadcast_nyt_page() {
  $items = array();
  $query = db_select('node', 'n');
  $query->join('field_data_field_audio_nyt', 'a', "n.nid = a.entity_id");
  $query->join('field_data_field_radio_air_date', 'rad', "(rad.entity_id = n.nid AND rad.entity_type ='node' AND rad.deleted = 0)");
  $query->join('field_data_field_episode_number', 'en', "n.nid = en.entity_id");
  $query->leftJoin('field_data_field_simplecast_nyt', 's', "(s.entity_id = n.nid AND s.entity_type ='node' AND s.deleted = 0)");
  $query
    ->fields('n', array('nid', 'title'))
    ->fields('rad', array('field_radio_air_date_value'))
    ->fields('en', array('field_episode_number_value'))
    ->fields('a', array('field_audio_nyt_fid'))
    ->fields('s', array('field_simplecast_nyt_value'))
    ->condition('n.type', 'episode')
    ->condition('n.status', 1)
    ->orderBy('en.field_episode_number_value', 'DESC');
  $results = $query->execute();
  $rows = array();
  foreach ($results as $result) {
    $file = file_load($result->field_audio_nyt_fid);
    $item = array();
    $item[] = array(
      'data' => date('m/d/Y', strtotime($result->field_radio_air_date_value)),
    );
    $item[] = array(
      'data' => $result->field_episode_number_value,
    );
    $item[] = array(
      'data' => l($result->title, 'node/'.$result->nid),
    );
    $item[] = array(
      'data' => l(t('Original'), file_create_url($file->uri)),
    );
    $item[] = array(
      'data' => l(t('Simplecast'), $result->field_simplecast_nyt_value),
    );
    $items[] = $item;
  }
  $prepend = array();
  $prepend[] = array(
    array(
      'data' => '<h2>NYT Episodes</h2>',
      'colspan' => 4,
    )
  );
  $prepend[] = array(
    array(
      'data' => '<strong>Total Episodes:</strong>',
      'colspan' => 2,
    ),
    array(
      'data' => '<strong>'.count($items).'</strong>',
      'colspan' => 3,
    )
  );
  $items = array_merge($prepend, $items);
  $render[] = array(
    '#theme' => 'table',
    '#rows' => $items,
    '#attributes' => array('id' => 'broadcast-episodes'),
  );
  //tal_debug($dates, false);
  //tal_debug($episodes);
  return $render;
}
