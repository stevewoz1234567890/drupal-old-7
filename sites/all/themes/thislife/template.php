<?php

function thislife_js_alter(&$javascript) {
  global $user;
  if (empty($user->uid) && !empty($javascript['misc/drupal.js'])) {
    // Forked version of drupal.js to remove cookie for anonymous users
    $javascript['misc/drupal.js']['data'] = drupal_get_path('theme', 'thislife') . '/js/drupal.js';
  }
  if ($node = menu_get_object()) {
    if ($node->type == 'embed') {
      $scripts = array(
        'sites/all/themes/thislife/bower_components/cookieconsent/build/cookieconsent.min.js',
        //'sites/all/themes/thislife/js/main.js',
        'modules/contextual/contextual.js',
      );
      foreach ($scripts as $script) {
        unset($javascript[$script]);
      }
    }
  }
}

function thislife_preprocess_html(&$variables) {
  if ($node = menu_get_object()) {
    $path = explode('/', request_path());
    if ($path[0] == 'about') {
      $variables['classes_array'][] = 'page-about';
    }
    if ($node->type == 'episode') {
      if ($items = field_get_items('node', $node, 'field_episode_number')) {
        $variables['classes_array'][] = drupal_html_class('page-episode-number-'.$items[0]['value']);
      }
    }
    if ($node->type == 'embed') {
      $episode = false;
      if ($items = field_get_items('node', $node, 'field_episodes')) {
        if ($episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
          if ($episode->type == 'act') {
            if ($items = field_get_items('node', $episode, 'field_episode')) {
              $episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']));
            }
          }
        }
      }
      if (!empty($episode) && $episode->type == 'episode') {
        if ($items = field_get_items('node', $episode, 'field_background_color')) {
          $color = $items[0]['rgb'];
          $variables['attributes_array']['style'] = 'background-color: '.$color;
        }
      }
    }
  }
  if (!empty($_GET['app'])) {
    $variables['classes_array'][] = 'app';
  }
  if (!empty($_GET['mode'])) {
    $variables['classes_array'][] = drupal_html_class('mode-'.filter_xss($_GET['mode'], array()));
  }
  $status = drupal_get_http_header('status');
  $variables['classes_array'][] = 'page-status-'.drupal_html_class($status);
}

function thislife_fix_show_name($text) {
  return str_replace('This American Life', '<em>This American Life</em>', $text);
}

function thislife_fix_date($date) {
  $find = array('May. ', 'Jun. ', 'Jul. ', 'Sep. ');
  $replace = array('May ', 'June ', 'July ', 'Sept. ');
  return str_replace($find, $replace, $date);
}

function thislife_preprocess_page(&$variables) {
  drupal_add_feed(url('podcast/rss.xml', array('absolute' => true)), t('Podcast'));
  $variables['app'] = (!empty($_GET['app']));
  if (arg(0) == 'about') {
    $variables['show_title'] = true;
    if (arg(1) == 'announcements' && !empty($variables['page']['content']['system_main']['nodes']['nodes'])) {
      $children = element_children($variables['page']['content']['system_main']['nodes']['nodes']);
      if (empty($_GET['pager']) && !empty($children)) {
        foreach ($children as $delta => $nid) {
          if ($delta <= 1) {
            $variables['page']['content']['system_main']['nodes']['nodes'][$nid]['field_image'][0]['#image_style'] = 'collection';
          }
        }
      }
    }
  }
  if (!empty($variables['node'])) {
    $node = $variables['node'];
    if ($node->type == 'fullscreen') {
      $variables['theme_hook_suggestions'][] = 'page__node__fullscreen';
    }
    if ($node->type == 'embed') {
      $variables['theme_hook_suggestions'][] = 'page__node__embed';
    }
    if (in_array($node->type, array('tv', 'episode'))) {
      if ($items = field_get_items('node', $node, 'field_background_color')) {
        $color = $items[0]['rgb'];
        $variables['title_attributes_array']['style'] = 'background-color: '.$color;
      }
    }
    if ($node->type == 'act') {
      if ($items = field_get_items('node', $node, 'field_episode')) {
        if ($episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
          if ($items = field_get_items('node', $episode, 'field_background_color')) {
            $color = $items[0]['rgb'];
            $variables['title_attributes_array']['style'] = 'background-color: '.$color;
          }
        }
      }
    }
    if ($node->type == 'tv_act') {
      if ($items = field_get_items('node', $node, 'field_tv_episode')) {
        if ($episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
          if ($items = field_get_items('node', $episode, 'field_background_color')) {
            $color = $items[0]['rgb'];
            $variables['title_attributes_array']['style'] = 'background-color: '.$color;
          }
        }
      }
    }
    if ($node->type == 'video') {
      if ($items = field_get_items('node', $node, 'field_background_color')) {
        $color = $items[0]['rgb'];
        $variables['title_attributes_array']['style'] = 'background-color: '.$color;
      }
    }
    if (in_array($node->type, array('article', 'gallery', 'fullscreen'))) {
      if ($items = field_get_items('node', $node, 'field_episodes')) {
        $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
        if ($episode->type == 'act') {
          if ($items = field_get_items('node', $episode, 'field_episode')) {
            $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
          }
        }
        if (!empty($episode) && $episode->type == 'episode' && $items = field_get_items('node', $episode, 'field_background_color')) {
          $color = $items[0]['rgb'];
          $variables['title_attributes_array']['style'] = 'background-color: '.$color;
        }
      }
    }
  }
  $items = array();

  if ($tree = menu_tree_page_data('main-menu')) {
    foreach ($tree as $leaf) {
      $title = '';
      $active = false;
      $link = $leaf['link'];

      if (!$link['hidden']) {
        $href = $link['link_path'];
        if (empty($title)) {
          $title = $link['link_title'];
        }
        $item = array('data' => l($title, $href));
        $item['class'][] = drupal_html_class($title);
        $active_trail = false;
        if (!empty($link['in_active_trail']) || strpos(request_path(), drupal_get_path_alias($href)) === 0) {
          $item['class'][] = 'active-trail';
          $active_trail = true;
        }
        if (!empty($leaf['below'])) {
          $item['class'][] = 'has-children';
          foreach ($leaf['below'] as $kid) {
            $title = '';
            $link = $kid['link'];
            if (!$link['hidden']) {
              $href = $link['link_path'];
              if (empty($title)) {
                $title = $link['link_title'];
              }
              $item['children'][] = l($title, $href);
            }
          }
        }

        $items[] = $item;
      }
    }
  }
  //  Footer menu is the same as main menu minus top item
  $footer_items = $items;

  //  Top item only appears in mobile view
  //  Can be configured at in /admin/tal/config - see tal module
  $mobile_top_link_label = variable_get('tal_menu_mobile_label', '');
  $mobile_top_link = variable_get('tal_menu_mobile_link', '');
  if (!empty($mobile_top_link_label) && !empty($mobile_top_link)) {
    $top_item = array(
      'class' => array('partners','mobile'),
      'data' => l(t($mobile_top_link_label), $mobile_top_link),
    );
    $mobile_top_link_color = variable_get('tal_menu_mobile_color', '');
    if (!empty($mobile_top_link_color)) {
      $top_item['style'] = [
        "color:$mobile_top_link_color;",
      ];
    }
    array_unshift($items, $top_item);
  }

  /*
  $form = drupal_get_form('search_block_form');
  $items[] = array(
    'data' => render($form),
    'class' => array('search'),
  );
  */
  $items[] = array(
    'data' => l('<span class="icon-search"></span>', 'archive', array('fragment' => 'keyword', 'html' => 'true')),
    'class' => array('search'),
  );
  $items[] = array(
    'data' => '<h6>'.t('Follow Us').'</h6>',
    'children' => array(
      'facebook' => l('<span class="icon-facebook"></span>', 'https://www.facebook.com/thislife', array('html' => true)),
      'twitter' => l('<span class="icon-twitter"></span>', 'https://x.com/thisamerlife', array('html' => true)),
    ),
    'class' => array('social'),
  );

  //  Button link is styled as a button in the top nav and only visible on desktop
  $menu_button_label = variable_get('tal_menu_button_label', 'Merch');
  $menu_button_link = variable_get('tal_menu_button_link', 'https://store.thisamericanlife.org');
  if (!empty($menu_button_label) && !empty($menu_button_link)) {
    $items[] = array(
      'class' => array('store','desktop'),
      'data' => l(t($menu_button_label), $menu_button_link),
    );
  }

  $footer_items[] = array(
    'class' => array('contact'),
    'data' => l(t('Contact'), 'about/contact-us'),
  );

  //  Footer link is styled as a button in the top nav and just a link in the footer
  $footer_link_label = variable_get('tal_footer_link_label', '');
  $footer_link = variable_get('tal_footer_link', '');
  if (!empty($footer_link_label) && !empty($footer_link)) {
    $footer_items[] = array(
      'class' => array('footer-link'),
      'data' => l(t($footer_link_label), $footer_link),
    );
  }

  $form = '<div id="mc_embed_signup">
  <form action="//thisamericanlife.us2.list-manage.com/subscribe/post?u=231d7e24815c65f94bf421633&amp;id=09eaca450d" method="post" id="mc-embedded-subscribe-form" name="mc-embedded-subscribe-form" class="validate" target="_blank" novalidate>
  <div id="mc_embed_signup_scroll">
  <div class="mc-field-group">
  <label for="mce-EMAIL">Subscribe to our newsletter</label>
  <input type="email" value="" name="EMAIL" class="required email" id="mce-EMAIL" placeholder="Email Address">
  </div>

  <div style="position: absolute; left: -5000px;" aria-hidden="true"><input type="text" name="b_231d7e24815c65f94bf421633_09eaca450d" tabindex="-1" value=""></div>
  <div class="clear"><input type="submit" value="Go" name="subscribe" id="mc-embedded-subscribe" class="button form-submit"></div>
  </div>
  </form>
  </div>';
  $items[] = array(
    'data' => $form,
    'class' => array('signup'),
  );

  $variables['menu'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array(
      'class' => array(
        'menu',
      ),
    ),
  );
  $variables['footer_menu'] = array(
    '#theme' => 'item_list',
    '#items' => $footer_items,
    '#attributes' => array(
      'class' => array(
        'menu',
      ),
    ),
  );
}

function thislife_preprocess_taxonomy_term(&$variables) {
  $view_mode = $variables['view_mode'];
  $variables['classes_array'][] = drupal_html_class('view-'.$view_mode);
  if (!empty($variables['content']['field_image'])) {
    $variables['classes_array'][] = 'with-image';
  }
}

function thislife_preprocess_file_entity(&$variables) {
  $file = $variables['file'];
  if ($items = field_get_items('file', $file, 'field_caption')) {
    $variables['content']['figcaption']['caption'] = array(
      '#markup' => $items[0]['safe_value'],
      '#prefix' => '<span class="caption">',
      '#suffix' => '</span>',
    );
  }
  if ($items = field_get_items('file', $file, 'field_credit')) {
    $variables['content']['figcaption']['credit'] = array(
      '#markup' => $items[0]['safe_value'],
      '#prefix' => '<span class="credit">',
      '#suffix' => '</span>',
    );
  }
}

function thislife_preprocess_entity(&$variables) {
  $element = $variables['elements'];
  if ($element['#entity_type'] == 'field_collection_item') {
    $field_collection_item = $element['#entity'];
    if ($element['#bundle'] == 'field_episode_collection') {
      if ($items = field_get_items('field_collection_item', $field_collection_item, 'field_title')) {
        $variables['classes_array'][] = 'title-'.drupal_html_class($items[0]['value']);
      }
    }
    if ($element['#bundle'] == 'field_photo_gallery') {
      if ($items = field_get_items('field_collection_item', $field_collection_item, 'field_layout')) {
        $variables['classes_array'][] = 'layout-'.drupal_html_class($items[0]['value']);
      }
    }
    if ($element['#bundle'] == 'field_recommended_links') {
      if ($items = field_get_items('field_collection_item', $field_collection_item, 'field_link')) {
        $variables['url'] = $items[0]['url'];
      }
    }
    if ($element['#bundle'] == 'field_recommendations') {
      if (!empty($variables['content']['field_notes'][0]['#markup'])) {
        $variables['content']['field_notes'][0]['#markup'] = thislife_fix_show_name($variables['content']['field_notes'][0]['#markup']);
      }
    }
    if ($element['#bundle'] == 'field_questions') {
      if (!empty($variables['content']['field_title'][0]['#markup'])) {
        $variables['content']['field_title'][0]['#markup'] = thislife_fix_show_name($variables['content']['field_title'][0]['#markup']);
      }
    }
  }
}

function thislife_preprocess_node(&$variables) {
  $variables['app'] = (!empty($_GET['app']));
  $node = $variables['node'];
  $view_mode = $variables['view_mode'];
  $variables['classes_array'][] = drupal_html_class('view-'.$view_mode);
  if (!in_array($node->type, array('event'))) {
    $variables['classes_array'][] = 'clearfix';
  }
  if ($view_mode == 'heartbeat') {
    $variables['classes_array'][] = 'view-full';
    $view_mode = 'full';
  }

  $variables['title'] = thislife_fix_show_name($variables['title']);

  if (in_array($view_mode, array('full', 'homepage'))) {
    $words = preg_split('/ +/', $variables['title']);
    if (count($words) > 3) {
      $variables['title'] = implode(' ', array_slice($words, 0, (count($words)-2))).' '.implode('&nbsp;', array_slice($words, -2, 2));
    }
  }

  if (!empty($variables['content']['field_radio_air_date'])) {
    foreach (element_children($variables['content']['field_radio_air_date']) as $delta) {
      $variables['content']['field_radio_air_date'][$delta] = thislife_fix_date($variables['content']['field_radio_air_date'][$delta]);
    }
  }

  if ($node->type == 'embed') {
    $data = tal_episode_playlist_json($node, true);
    $variables['playlist_json'] = json_encode($data);
    $episode = false;
    if ($items = field_get_items('node', $node, 'field_episodes')) {
      if ($episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
        $variables['episode_url'] = url('node/'.$episode->nid, array('absolute' => true));
        if ($episode->type == 'act') {
          if ($items = field_get_items('node', $episode, 'field_episode')) {
            $episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']));
          }
        }
      }
    }
    if (!empty($episode) && $episode->type == 'episode') {
      $variables['episode_name'] = $episode->title;
      if ($items = field_get_items('node', $episode, 'field_episode_number')) {
        $variables['episode_name'] = t('@label: @title', array('@label' => $items[0]['value'], '@title' => $episode->title));
      }
      if (empty($variables['content']['field_image']) && $items = field_get_items('node', $episode, 'field_image')) {
        $file = $items[0];
        $variables['content']['field_image'] = array(
          '#theme' => 'image_style',
          '#style_name' => 'archive',
          '#path' => $file['uri'],
        );
      }
    }
  }

  if ($node->type == 'announcement') {
    $byline = array();
    if ($items = field_get_items('node', $node, 'field_author')) {
      foreach ($items as $item) {
        $byline[] = $item['safe_value'];
      }
    }
    if ($items = field_get_items('node', $node, 'field_contributor')) {
      foreach ($items as $item) {
        if ($contributor = (!empty($item['entity']) ? $item['entity'] : taxonomy_term_load($item['target_id']))) {
          $byline[] = l($contributor->name, 'archive', array('query' => array('contributor' => $contributor->tid)));
        }
      }
    }
    if (!empty($byline)) {
      $variables['content']['byline'] = array(
        '#markup' => implode(', ', $byline),
        '#prefix' => t('By '),
      );
    }
    $variables['kicker'] = l(t('Announcements'), 'node/'.$node->nid);
    if ($items = field_get_items('node', $node, 'field_kicker')) {
      if (!empty($items[0]['safe_value'])) {
        $variables['kicker'] = l($items[0]['safe_value'], 'node/'.$node->nid);
      }
    }
    if ($view_mode == 'full') {
      $variables['social_links']['facebook'] = tal_share_url('facebook', $node);
      $variables['social_links']['twitter'] = tal_share_url('twitter', $node);
      $variables['social_links']['mail'] = tal_share_url('mail', $node);
    }
  }

  if ($node->type == 'homepage') {
    if (!empty($variables['content']['field_banner'][0])) {
      if ($items = field_get_items('node', $node, 'field_link')) {
        $variables['content']['field_banner'][0]['#prefix'] = "<a href='" . $items[0]['url'] . "'>";
        $variables['content']['field_banner'][0]['#suffix'] = "</a>";
      }
    }
    if (!empty($variables['content']['field_notes'][0])) {
      $variables['content']['field_notes'][0]['#suffix'] = l(t('View all'), 'recommended', array('attributes' => array('class' => array('view-all'))));
    }
    if (!empty($variables['content']['field_featured'])) {
      $variables['classes_array'][] = 'has-featured';
      $variables['classes_array'][] = 'featured-'.count(element_children($variables['content']['field_featured']));
    }
  }

  if ($node->type == 'video_collection') {
    if ($view_mode == 'collection') {
      if (!empty($variables['content']['body'][0])) {
        $variables['content']['body'][0]['#markup'] .= ' '.l(t('View this list'), 'node/'.$node->nid, array('attributes' => array('class' => array('details'))));
      }
    }
    if ($view_mode == 'full') {
      if ($items = field_get_items('node', $node, 'field_banner')) {
        $image = (object) $items[0];
        $url = file_create_url($image->uri);
        $variables['title_attributes_array']['style'] = 'background-image: url('.$url.')';
        $variables['title_attributes_array']['class'][] = 'banner';
        if ($items = field_get_items('file', $image, 'field_credit')) {
          if (!empty($items[0]['value'])) {
            $variables['content']['credit'] = array(
              '#prefix' => '<div class="credit">',
              '#suffix' => '</div>',
              '#markup' => check_markup($items[0]['value'], 'simple'),
            );
          }
        }
      }
    }
  }

  if ($node->type == 'landing') {
    if ($view_mode == 'full') {
      if ($items = field_get_items('node', $node, 'field_image')) {
        $image = (object) $items[0];
        $url = file_create_url($image->uri);
        //$variables['title_attributes_array']['id'] = 'top';
        $variables['title_attributes_array']['style'] = 'background-image: url('.$url.')';
        $variables['title_attributes_array']['class'][] = 'banner';
        $variables['classes_array'][] = 'has-banner';
        if ($items = field_get_items('file', $image, 'field_credit')) {
          if (!empty($items[0]['value'])) {
            $variables['content']['credit'] = array(
              '#prefix' => '<div class="credit">',
              '#suffix' => '</div>',
              '#markup' => check_markup($items[0]['value'], 'simple'),
            );
          }
        }
        if ($items = field_get_items('node', $node, 'field_mobile_image')) {
          $image = (object) $items[0];
          $url = file_create_url($image->uri);
          $variables['title_attributes_array']['class'][] = 'has-mobile';
          $variables['mobile_banner'] = '<div class="banner__mobile" style="background-image: url('.$url.')"></div>';
        }
      }
    }
  }

  if ($node->type == 'pick') {
    $variables['attributes_array']['data-type'] = 'collection';
    $variables['attributes_array']['data-id'] = $node->nid;
    if (!empty($variables['content']['field_image'][0]['#path'])) {
      $variables['content']['field_image'][0]['#path']['options']['attributes']['class'] = array('goto', 'goto-collection');
    }
    if ($view_mode == 'collection') {
      if (!empty($variables['content']['body'][0])) {
        $variables['content']['body'][0]['#markup'] .= ' '.l(t('View this list'), 'node/'.$node->nid, array('attributes' => array('class' => array('details', 'goto', 'goto-collection'))));
      }
    }
  }

  if ($node->type == 'collection') {
    $variables['attributes_array']['data-type'] = 'collection';
    $variables['attributes_array']['data-id'] = $node->nid;
    if (!empty($variables['content']['field_image'][0]['#path'])) {
      $variables['content']['field_image'][0]['#path']['options']['attributes']['class'] = array('goto', 'goto-collection');
    }
    if ($view_mode == 'collection') {
      if (!empty($variables['content']['body'][0])) {
        $variables['content']['body'][0]['#markup'] .= ' '.l(t('View this list'), 'node/'.$node->nid, array('attributes' => array('class' => array('details', 'goto', 'goto-collection'))));
      }
    }
    if ($view_mode == 'full') {
      if ($items = field_get_items('node', $node, 'field_banner')) {
        $image = (object) $items[0];
        $url = file_create_url($image->uri);
        $variables['title_attributes_array']['style'] = 'background-image: url('.$url.')';
        $variables['title_attributes_array']['class'][] = 'banner';
        if ($items = field_get_items('file', $image, 'field_credit')) {
          if (!empty($items[0]['value'])) {
            $variables['content']['credit'] = array(
              '#prefix' => '<div class="credit">',
              '#suffix' => '</div>',
              '#markup' => check_markup($items[0]['value'], 'simple'),
            );
          }
        }
      }
      $is_contributors = false;
      if ($items = field_get_items('node', $node, 'field_collection_type')) {
        if (!empty($items[0]['tid'])) {
          if ($collection_type = taxonomy_term_load($items[0]['tid'])) {
            $variables['classes_array'][] = drupal_html_class('type-'.$collection_type->machine_name);
            if ($collection_type->machine_name == 'contributors') {
              $is_contributors = true;
            }
          }
        }
      }
      $variables['content']['field_image']['#access'] = $is_contributors;
    }
  }

  if (in_array($node->type, array('article', 'gallery', 'video', 'announcement', 'fullscreen'))) {
    if ($view_mode == 'teaser') {
      if ($items = field_get_items('node', $node, 'field_image')) {
        $file = $items[0];
        $variables['classes_array'][] = 'with-image';
        $variables['content']['field_image'] = array(
          '#theme' => 'image_style',
          '#style_name' => 'archive',
          '#path' => $file['uri'],
          '#prefix' => '<figure class="episode-image">',
          '#suffix' => '</figure>',
        );
      }
      else if ($items = field_get_items('node', $node, 'field_photo_gallery')) {
        $slide = field_collection_item_load($items[0]['value']);
        if ($items = field_get_items('field_collection_item', $slide, 'field_photo')) {
          $file = $items[0];
          $variables['classes_array'][] = 'with-image';
          $variables['content']['field_image'] = array(
            '#theme' => 'image_style',
            '#style_name' => 'archive',
            '#path' => $file['uri'],
            '#prefix' => '<figure class="episode-image">',
            '#suffix' => '</figure>',
          );
        }
      }
    }
    if ($view_mode == 'teaser' || $view_mode == 'related' || $view_mode == 'featured') {

      if (!empty($variables['content']['field_radio_air_date']) && $items = field_get_items('node', $node, 'field_radio_air_date')) {
        foreach ($items as $delta => $item) {
          $variables['content']['field_radio_air_date'][$delta]['#markup'] = l($variables['content']['field_radio_air_date'][$delta]['#markup'], 'node/'.$node->nid, array('html' => true));
        }
      }
    }
    if ($view_mode == 'full') {
      $byline = array();
      if ($items = field_get_items('node', $node, 'field_author')) {
        foreach ($items as $item) {
          $byline[] = $item['safe_value'];
        }
      }
      if ($items = field_get_items('node', $node, 'field_contributor')) {
        foreach ($items as $item) {
          if ($contributor = (!empty($item['entity']) ? $item['entity'] : taxonomy_term_load($item['target_id']))) {
            $byline[] = l($contributor->name, 'archive', array('query' => array('contributor' => $contributor->tid)));
          }
        }
      }
      if (!empty($byline)) {
        $variables['content']['byline'] = array(
          '#markup' => implode(', ', $byline),
          '#prefix' => t('By '),
        );
      }
      if (!empty($variables['content']['field_episodes'])) {
        if (count(element_children($variables['content']['field_episodes'])) == 1) {
          $variables['content']['field_episodes']['#title'] = t('Related Episode');
        }
      }
      $variables['social_links']['facebook'] = tal_share_url('facebook', $node);
      $variables['social_links']['twitter'] = tal_share_url('twitter', $node);
      $variables['social_links']['mail'] = tal_share_url('mail', $node);
      $variables['attributes_array']['data-background'] = '#020D70';
      $variables['title_attributes_array']['class'][] = 'extra-header';
      if ($node->type == 'video') {
        if ($items = field_get_items('node', $node, 'field_background_color')) {
          $color = $items[0]['rgb'];
          $variables['attributes_array']['data-background'] = $color;
          $variables['title_attributes_array']['style'] = 'background-color: '.$color;
        }
        if ($items = field_get_items('node', $node, 'field_episodes')) {
          $variables['classes_array'][] = 'with-episodes';
        }
        if ($items = field_get_items('node', $node, 'field_width')) {
          $width = $items[0]['value'];
          if ($items = field_get_items('node', $node, 'field_height')) {
            $height = $items[0]['value'];
            $padding = $height/$width*100;
            $variables['content_attributes_array']['style'] = 'padding-top: '.$padding.'%';
          }
        }
      }
      elseif ($node->type != 'announcement') {
        if ($items = field_get_items('node', $node, 'field_episodes')) {
          $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
          $variables['episode_url'] = url('node/'.$episode->nid);
          if ($episode->type == 'act') {
            if ($items = field_get_items('node', $episode, 'field_episode')) {
              $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
            }
          }
          if (!empty($episode) && $episode->type == 'episode' && $items = field_get_items('node', $episode, 'field_background_color')) {
            $color = $items[0]['rgb'];
            $variables['attributes_array']['data-background'] = $color;
            $variables['title_attributes_array']['style'] = 'background-color: '.$color;
          }
        }
      }
    }
    if (!empty($variables['content']['field_author'])) {
      $variables['content']['field_author']['#title'] = t('By');
    }
    if ($view_mode == 'featured') {
      $variables['kicker'] = l(t('Extra'), 'node/'.$node->nid);
      if ($node->type == 'gallery') {
        $variables['kicker'] = l(t('Photos'), 'node/'.$node->nid);
        if ($items = field_get_items('node', $node, 'field_photo_gallery')) {
          $slide = field_collection_item_load($items[0]['value']);
          if ($items = field_get_items('field_collection_item', $slide, 'field_photo')) {
            $file = $items[0];
            $variables['content']['field_image'] = array(
              '#theme' => 'image_style',
              '#style_name' => 'collection',
              '#path' => $file['uri'],
              '#prefix' => '<div class="field-name-field-image"><div class="field-item"><a href="'.url('node/'.$node->nid).'">',
              '#suffix' => '</a></div></div>',
            );
          }
        }
      }
      if ($items = field_get_items('node', $node, 'field_kicker')) {
        if (!empty($items[0]['safe_value'])) {
          $variables['kicker'] = l($items[0]['safe_value'], 'node/'.$node->nid);
        }
      }
    }
    if ($view_mode == 'episode') {
      $variables['kicker'] = t('Episode Extra');
      if ($items = field_get_items('node', $node, 'field_kicker')) {
        if (!empty($items[0]['safe_value'])) {
          $variables['kicker'] = $items[0]['safe_value'];
        }
      }
      if ($episode = menu_get_object()) {
        if ($items = field_get_items('node', $episode, 'field_background_color')) {
          $color = $items[0]['rgb'];
          $variables['title_attributes_array']['style'] = 'background-color: '.$color;
          $variables['content_attributes_array']['class'][] = 'background-color';
        }
        else if ($episode->type == 'homepage') {
          if ($episode = tal_episode_this_week()) {
            if ($items = field_get_items('node', $episode, 'field_background_color')) {
              $color = $items[0]['rgb'];
              $variables['title_attributes_array']['style'] = 'background-color: '.$color;
              $variables['content_attributes_array']['class'][] = 'background-color';
            }
          }
        }
      }
      if ($items = field_get_items('node', $node, 'field_image')) {
        $image = $items[0];
        $url = image_style_url('column', $image['uri']);
        $variables['content_attributes_array']['class'][] = 'thumbnail';
        $variables['content_attributes_array']['style'] = 'background-image: url('.$url.')';
      }
      elseif ($items = field_get_items('node', $node, 'field_photo_gallery')) {
        $slide = field_collection_item_load($items[0]['value']);
        if ($items = field_get_items('field_collection_item', $slide, 'field_photo')) {
          $image = $items[0];
          $url = image_style_url('column', $image['uri']);
          $variables['content_attributes_array']['class'][] = 'thumbnail';
          $variables['content_attributes_array']['style'] = 'background-image: url('.$url.')';
        }
      }
      else {
        $variables['content_attributes_array']['class'][] = 'thumbnail';
        $variables['content_attributes_array']['class'][] = 'no-thumbnail';
      }
      if (!empty($variables['content']['body'][0]['#markup'])) {
        $variables['content']['body'][0]['#markup'] = filter_xss($variables['content']['body'][0]['#markup'], array('em', 'strong'));
      }
    }
    if ($view_mode == 'act') {
      $variables['kicker'] = t('Act Extra');
      if ($items = field_get_items('node', $node, 'field_kicker')) {
        if (!empty($items[0]['safe_value'])) {
          $variables['kicker'] = $items[0]['safe_value'];
        }
      }
      if ($items = field_get_items('node', $node, 'field_importance')) {
        if (!empty($items[0]['value'])) {
          $variables['classes_array'][] = 'important';
          if ($parent = menu_get_object()) {
            if ($parent->type == 'episode') {
              $episode = $parent;
            }
            else if ($parent->type == 'act') {
              if ($items = field_get_items('node', $parent, 'field_episode')) {
                $episode = ($items[0]['entity'] ? $items[0]['entity'] : node_load($items[0]['target_id']));
              }
            }
            if (
              isset($episode) &&
              ($items = field_get_items('node', $episode, 'field_background_color'))
            ) {
              $color = $items[0]['rgb'];
              $variables['title_attributes_array']['style'] = 'background-color: '.$color;
              $variables['content_attributes_array']['class'][] = 'background-color';
            }
          }
        }
      }
      if ($items = field_get_items('node', $node, 'field_image')) {
        $image = $items[0];
        $url = image_style_url('column', $image['uri']);
        $variables['content_attributes_array']['class'][] = 'thumbnail';
        $variables['content_attributes_array']['style'] = 'background-image: url('.$url.')';
      }
      elseif ($items = field_get_items('node', $node, 'field_photo_gallery')) {
        $slide = field_collection_item_load($items[0]['value']);
        if ($items = field_get_items('field_collection_item', $slide, 'field_photo')) {
          $image = $items[0];
          $url = image_style_url('column', $image['uri']);
          $variables['content_attributes_array']['class'][] = 'thumbnail';
          $variables['content_attributes_array']['style'] = 'background-image: url('.$url.')';
        }
      }
      else {
        $variables['content_attributes_array']['class'][] = 'thumbnail';
        $variables['content_attributes_array']['class'][] = 'no-thumbnail';
      }
    }
    if ($view_mode == 'archive') {
      $variables['kicker'] = t('Episode Extra');
      if ($node->type == 'announcement') {
        $variables['kicker'] = l(t('Announcements'), 'node/'.$node->nid);
      }
      if ($items = field_get_items('node', $node, 'field_episodes')) {
        if ($episode = node_load($items[0]['target_id'])) {
          if ($episode->type == 'act') {
            $variables['kicker'] = t('Act Extra');
            if ($items = field_get_items('node', $episode, 'field_act_label')) {
              $variables['kicker'] = t('@label Extra', array('@label' => $items[0]['value']));
            }

            if ($items = field_get_items('node', $episode, 'field_episode')) {
              $episode = node_load($items[0]['target_id']);
            }
          }
          if (!empty($episode) && $episode->type == 'episode') {
            $variables['episode_title'] = $episode->title;
            if ($items = field_get_items('node', $episode, 'field_episode_number')) {
              $variables['content']['episode_number'] = array(
                '#prefix' => '<div class="field field-name-field-episode-number">',
                '#suffix' => '</div>',
                '#markup' => l($items[0]['value'].': '.$episode->title, 'node/'.$episode->nid, array('attributes' => array('class' => array('goto-episode', 'goto')))),
              );
            }
          }
        }
      }
      if ($items = field_get_items('node', $node, 'field_kicker')) {
        if (!empty($items[0]['safe_value'])) {
          if ($node->type == 'announcement') {
            $variables['kicker'] = l($items[0]['safe_value'], 'node/'.$node->nid);
          }
          else {
            $variables['kicker'] = $items[0]['safe_value'];
          }
        }
      }
      if ($items = field_get_items('node', $node, 'field_image')) {
        $image = $items[0];
        $url = image_style_url('column', $image['uri']);
        $variables['content_attributes_array']['style'] = 'background-image: url('.$url.')';
        $variables['content_attributes_array']['class'][] = 'thumbnail';
      }
      elseif ($items = field_get_items('node', $node, 'field_photo_gallery')) {
        $slide = field_collection_item_load($items[0]['value']);
        if ($items = field_get_items('field_collection_item', $slide, 'field_photo')) {
          $image = $items[0];
          $url = image_style_url('column', $image['uri']);
          $variables['content_attributes_array']['style'] = 'background-image: url('.$url.')';
          $variables['content_attributes_array']['class'][] = 'thumbnail';
        }
      }
      else {
        $variables['content_attributes_array']['class'][] = 'thumbnail';
        $variables['content_attributes_array']['class'][] = 'no-thumbnail';
      }
      if ($items = field_get_items('node', $node, 'field_radio_air_date')) {
        $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
        $variables['content']['radio_air_date'] = array(
          '#prefix' => '<div class="field field-name-field-radio-air-date">',
          '#suffix' => '</div>',
          '#markup' => l($date, 'node/'.$node->nid),
        );
      }
    }
  }

  if ($node->type == 'gallery') {
    $variables['label'] = t('Photo Gallery');
    if ($items = field_get_items('node', $node, 'field_kicker')) {
      if (!empty($items[0]['safe_value'])) {
        $variables['label'] = $items[0]['safe_value'];
      }
    }
  }

  if ($node->type == 'transcript') {
    if ($view_mode == 'full') {
      if ($items = field_get_items('node', $node, 'field_episode')) {
        $episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']));
        $variables['episode_url'] = url('node/'.$episode->nid);

        if ($items = field_get_items('node', $episode, 'field_episode_number')) {
          $episode_number = $variables['episode_number'] = $items[0]['value'];
          if ($episode->is_live) {
            $attr = array(
              'data-episode' => $episode_number,
              'data-type' => $episode->type,
              'data-id' => $episode->nid,
              'class' => array('play', 'play-'.$episode_number, 'play-transcript'),
            );
            $variables['content']['play'] = array(
              '#markup' => l('<span class="icon icon-play"></span><span class="icon icon-pause"></span>', current_path(), array('html' => true, 'attributes' => $attr)),
            );

            $data = tal_episode_playlist_json($episode, true);
            $variables['playlist_json'] = json_encode($data);
          }
        }

        if ($items = field_get_items('node', $episode, 'field_episode_number')) {
          $episode_number = $variables['episode_number'] = $items[0]['value'];
          $variables['attributes_array']['data-episode'] = $episode_number;
          $variables['attributes_array']['data-episode-id'] = $episode->nid;
        }

        if ($items = field_get_items('node', $episode, 'field_image')) {
          foreach ($items as $delta => $item) {
            $variables['content']['field_episode_image'][$delta] = array(
              '#theme' => 'image_style',
              '#style_name' => 'height_thumbnail',
              '#path' => $item['uri'],
            );
          }
        }
      }
      if (empty($variables['content']['field_notes'])) {
        $variables['content']['field_notes'] = array(
          '#prefix' => '<div class="field-name-field-notes">',
          '#suffix' => '</div>',
          '#markup' => "<p>Note: This American Life is produced for the ear and designed to be heard. If you are able, we strongly encourage you to listen to the audio, which includes emotion and emphasis that's not on the page. Transcripts are generated using a combination of speech recognition software and human transcribers, and may contain errors. Please check the corresponding audio before quoting in print.",
        );
      }
    }
  }

  if ($node->type == 'act') {
    $variables['attributes_array']['data-type'] = 'act';
    $variables['attributes_array']['data-id'] = $node->nid;
    if (!empty($variables['content']['field_image'])) {
      if ($items = field_get_items('node', $node, 'field_image_display')) {
        if (!empty($items[0]['value'])) {
          $variables['classes_array'][] = 'with-image';
        }
      }
    }
    $variables['title_attributes_array']['class'][] = 'act-header';
    if (!empty($variables['content']['field_image'][0]['#path'])) {
      $variables['content']['field_image'][0]['#path']['options']['attributes']['class'] = array('goto', 'goto-act');
    }
    if ($items = field_get_items('node', $node, 'field_nonact')) {
      if (!empty($items[0]['value'])) {
        $variables['nonact'] = true;
        $variables['classes_array'][] = 'nonact';
      }
    }
    if ($items = field_get_items('node', $node, 'field_episode')) {
      if ($episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
        if (in_array($view_mode, array('full', 'landing'))) {
          $data = tal_episode_playlist_json($episode, true);
          $variables['playlist_json'] = json_encode($data);
        }
        $variables['episode_url'] = url('node/'.$episode->nid);
        if ($items = field_get_items('node', $episode, 'field_episode_number')) {
          $episode_number = $variables['episode_number'] = $items[0]['value'];
          $variables['attributes_array']['data-episode'] = $episode_number;
          $variables['attributes_array']['data-episode-id'] = $episode->nid;
          if (empty($variables['nonact']) && $episode->is_live) {
            $attr = array(
              'data-episode' => $episode_number,
              'data-type' => $node->type,
              'data-id' => $node->nid,
              'class' => array('play', 'play-act'),
            );
            if ($items = field_get_items('node', $node, 'field_act_number')) {
              $attr['data-act'] = $items[0]['value'];
              $attr['class'][] = 'play-'.$episode_number.'-'.$items[0]['value'];
            }
            $variables['content']['play'] = array(
              '#markup' => l('<span class="icon icon-play"></span><span class="icon icon-pause"></span>', 'node/'.$node->nid, array('html' => true, 'attributes' => $attr)),
            );
            $offline = variable_get('tal_shortcut_offline', array());
            // Remove Shortcut
            if (false && !in_array($episode_number, $offline)) {
              $url = 'https://shortcut.thisamericanlife.org/#/clipping/'.$episode_number;
              if ($items = field_get_items('node', $node, 'field_timestamp')) {
                $url = 'https://shortcut.thisamericanlife.org/#/clipping/'.$episode_number.'/'.$items[0]['value'];
              }
              $attr = array(
                'data-type' => $node->type,
                'data-id' => $node->nid,
                'class' => array('cut'),
              );
              $variables['content']['shortcut'] = array(
                '#markup' => l('<span class="icon icon-cut"></span><span class="label">Share a clip</span></a>', $url, array('html' => true, 'attributes' => $attr)),
              );
            }
          }
        }
      }
    }

    if ($credits = field_get_items('node', $node, 'field_credits')) {
      if ($vocab = taxonomy_vocabulary_machine_name_load('roles')) {
        if ($terms = taxonomy_term_load_multiple(array(), array('vid' => $vocab->vid))) {
          $roles = array(
            0 => array(
              'name' => 'By',
            )
          );
          foreach ($terms as $term) {
            $roles[$term->tid] = array(
              'name' => $term->name,
            );
          }
          foreach ($credits as $credit) {
            if ($field_collection_item = field_collection_item_load($credit['value'])) {
              if ($items = field_get_items('field_collection_item', $field_collection_item, 'field_contributor')) {
                $tid = 0;
                if ($role = field_get_items('field_collection_item', $field_collection_item, 'field_role')) {
                  $tid = $role[0]['tid'];
                }
                foreach ($items as $item) {
                  if ($contributor = (!empty($item['entity']) ? $item['entity'] : taxonomy_term_load($item['target_id']))) {
                    $roles[$tid]['items'][] = l($contributor->name, 'archive', array('query' => array('contributor' => $contributor->tid)));
                  }
                }
              }
            }
          }
        }
      }
      $byline = array();
      foreach ($roles as $role) {
        if (!empty($role['items'])) {
          if (count($role['items']) > 2) {
            $byline[] = $role['name'].' '.implode(', ', array_slice($role['items'], 0, -1)).', and '.implode('', array_slice($role['items'], -1, 1));
          }
          else {
            $byline[] = $role['name'].' '.implode(' and ', $role['items']);
          }
        }
      }
      if (!empty($byline)) {
        $variables['byline'] = implode('; ', $byline);
        unset($variables['content']['field_contributor']);
      }
    }
    if (!empty($variables['content']['field_contributor'])) {
      if ($items = field_get_items('node', $node, 'field_contributor')) {
        foreach ($items as $delta => $item) {
          if ($contributor = (!empty($item['entity']) ? $item['entity'] : taxonomy_term_load($item['target_id']))) {
            $variables['content']['field_contributor'][$delta] = array(
              '#markup' => l($contributor->name, 'archive', array('query' => array('contributor' => $contributor->tid))),
            );
          }
        }
      }
      $variables['content']['field_contributor']['#title'] = t('By');
    }

    if (!empty($variables['content']['field_song'])) {
      if ($items = field_get_items('node', $node, 'field_song')) {
        $songs = array();
        foreach ($items as $delta => $item) {
          $field_collection_item = field_collection_item_load($item['value']);
          if ($title = field_get_items('field_collection_item', $field_collection_item, 'field_title')) {
            $song = format_string('&ldquo;!song&rdquo;', array('!song' => $title[0]['safe_value']));
            if ($artist = field_get_items('field_collection_item', $field_collection_item, 'field_artist')) {
              $song .= format_string(' by !artist', array('!artist' => $artist[0]['safe_value']));
            }
            if ($link = field_get_items('field_collection_item', $field_collection_item, 'field_link')) {
              $song = l($song, $link[0]['url'], array('html' => true));
            }
            $songs[] = $song;
          }
          unset($variables['content']['field_song'][$delta]);
        }
        if (!empty($songs)) {
          $variables['content']['field_song']['#title'] .= ':';
          $variables['content']['field_song'][0] = array(
            '#markup' => implode(' & ', $songs),
          );
        }
      }
    }
    if ($view_mode == 'full') {
      if (!empty($variables['content']['field_image'])) {
        $access = false;
        $variables['content']['field_image'] = array(
          '#theme' => 'tal_episode_image',
          '#node' => $node,
        );
        if ($items = field_get_items('node', $node, 'field_image_display')) {
          $access = $items[0]['value'];
        }
        $variables['content']['field_image']['#access'] = $access;
      }
      if (!empty($episode)) {
        // Check that espiode is live before adding transcript link
        if ($episode->is_live) {
          $query = db_select('node', 'n');
          $query->join('field_data_field_episode', 'e', "(e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)");
          $query
            ->fields('n', array('nid'))
            ->condition('e.field_episode_target_id', $episode->nid)
            ->condition('n.status', 1)
            ->condition('n.type', 'transcript');
          if ($nid = $query->execute()->fetchField()) {
            $variables['transcript_url'] = url('node/'.$nid);
          }
        }
        if ($items = field_get_items('node', $episode, 'field_image')) {
          foreach ($items as $delta => $item) {
            $variables['content']['field_episode_image'][$delta] = array(
              '#theme' => 'image_style',
              '#style_name' => 'height_thumbnail',
              '#path' => $item['uri'],
            );
          }
        }
        if ($items = field_get_items('node', $episode, 'field_background_color')) {
          $color = $items[0]['rgb'];
          $variables['title_attributes_array']['style'] = 'background-color: '.$color;
        }
        if (!empty($episode_number)) {
          if (!empty($variables['content']['field_episode'])) {
            foreach (element_children($variables['content']['field_episode']) as $delta) {
              $variables['content']['field_episode'][$delta]['#prefix'] = $variables['episode_number'].': ';
            }
          }
        }
      }
    }
    else if ($view_mode == 'episode') {
      if (!empty($variables['content']['field_image'])) {
        $access = false;
        if ($items = field_get_items('node', $node, 'field_image_display')) {
          $access = $items[0]['value'];
          $variables['content']['field_image']['#access'] = $access;
        }
        if ($access) {
          $variables['content']['field_image'] = array(
            '#theme' => 'tal_episode_image',
            '#node' => $node,
            '#link' => url('node/'.$node->nid),
            '#access' => $access,
          );
        }
      }
    }
    elseif ($view_mode == 'archive' || $view_mode == 'footer') {
      if (!empty($variables['content']['body'][0])) {
        $variables['content']['body'][0]['#markup'] = tal_episode_act_summary($node, $view_mode);
      }
      if ($items = field_get_items('node', $node, 'field_act_label')) {
        if ($items[0]['value'] != $node->title) {
          $variables['title'] = t('@label: @title', array('@label' => $items[0]['value'], '@title' => $node->title));
        }
      }

      if (!empty($node->hide_episode)) {
        $variables['classes_array'][] = 'hide-episode';
        $variables['hide_episode'] = true;
      }
      else {
        if ($items = field_get_items('node', $node, 'field_episode')) {
          $episode = node_load($items[0]['target_id']);
          $variables['episode_title'] = $episode->title;
          if ($items = field_get_items('node', $episode, 'field_episode_number')) {
            $variables['content']['episode_number'] = array(
              '#prefix' => '<div class="field field-name-field-episode-number">',
              '#suffix' => '</div>',
              '#markup' => l($items[0]['value'].': '.$episode->title, 'node/'.$episode->nid, array('attributes' => array('class' => array('goto-episode', 'goto')))),
            );
          }
          if ($items = field_get_items('node', $episode, 'field_radio_air_date')) {
            $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
            $variables['content']['radio_air_date'] = array(
              '#prefix' => '<div class="field field-name-field-radio-air-date">',
              '#suffix' => '</div>',
              '#markup' => l($date, 'node/'.$node->nid, array('attributes' => array('class' => array('goto-act')))),
            );
          }
        }
      }
    }
    elseif (in_array($view_mode, array('landing'))) {
      if ($items = field_get_items('node', $node, 'field_episode')) {
        $episode = node_load($items[0]['target_id']);
        $variables['episode_title'] = $episode->title;
        if ($items = field_get_items('node', $episode, 'field_episode_number')) {
          $episode_number = $items[0]['value'];
          $variables['content']['episode_number'] = array(
            '#prefix' => '<div class="field field-name-field-episode-number">',
            '#suffix' => '</div>',
            '#markup' => $episode_number,
          );
          if (!empty($episode_number)) {
            $awards = variable_get('tal_awards', array());
            $variables['awards'] = (!empty($awards[$episode_number]) ? $awards[$episode_number] : false);
          }
        }
        if ($items = field_get_items('node', $episode, 'field_radio_air_date')) {
          $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
          $variables['content']['radio_air_date'] = array(
            '#prefix' => '<div class="field field-name-field-radio-air-date">',
            '#suffix' => '</div>',
            '#markup' => $date,
          );
        }
      }
    }
    elseif (in_array($view_mode, array('collection'))) {
      $variables['title'] = tal_episode_act_name($node);
      if ($items = field_get_items('node', $node, 'field_episode')) {
        $episode = node_load($items[0]['target_id']);
        $variables['episode_title'] = $episode->title;
        if ($items = field_get_items('node', $episode, 'field_episode_number')) {
          $variables['content']['episode_number'] = array(
            '#prefix' => '<div class="field field-name-field-episode-number">',
            '#suffix' => '</div>',
            '#markup' => l($items[0]['value'], 'node/'.$episode->nid, array('attributes' => array('class' => array('goto-episode', 'goto')))),
          );
        }
        if ($items = field_get_items('node', $episode, 'field_radio_air_date')) {
          $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
          $variables['content']['radio_air_date'] = array(
            '#prefix' => '<div class="field field-name-field-radio-air-date">',
            '#suffix' => '</div>',
            '#markup' => l($date, 'node/'.$node->nid),
          );
        }
        $image = false;
        if ($items = field_get_items('node', $node, 'field_image')) {
          $image = $items[0];
        }
        elseif ($items = field_get_items('node', $episode, 'field_image')) {
          $image = $items[0];
        }
        if (!empty($image)) {
          $variables['content']['image'] = array(
            '#prefix' => '<div class="field field-name-field-image">',
            '#suffix' => '</div>',
            '#theme' => 'image_formatter',
            '#item' => $image,
            '#path' => array('path' => 'node/'.$node->nid, 'options' => array('attributes' => array('class' => array('goto', 'goto-act')))),
          );

        }
      }
    }
    else {
      if (!empty($variables['content']['field_act_label']) && $items = field_get_items('node', $node, 'field_act_label')) {
        foreach ($items as $delta => $item) {
          $variables['content']['field_act_label'][$delta]['#markup'] = l($item['safe_value'], 'node/'.$node->nid, array('attributes' => array('class' => array('goto-act', 'goto'))));
        }
      }
    }
  }

  if ($node->type == 'tv_act') {
    $variables['title_attributes_array']['class'][] = 'act-header';
    if ($items = field_get_items('node', $node, 'field_tv_episode')) {
      if ($episode = (!empty($items[0]['entity']) ? $items[0]['entity'] : node_load($items[0]['target_id']))) {
        $variables['episode_url'] = url('node/'.$episode->nid);
      }
    }

    if (!empty($variables['content']['field_contributor'])) {
      if ($items = field_get_items('node', $node, 'field_contributor')) {
        foreach ($items as $delta => $item) {
          if ($contributor = (!empty($item['entity']) ? $item['entity'] : taxonomy_term_load($item['target_id']))) {
            $variables['content']['field_contributor'][$delta] = array(
              '#markup' => l($contributor->name, 'archive', array('query' => array('contributor' => $contributor->tid))),
            );
          }
        }
      }
      $variables['content']['field_contributor']['#title'] = t('By');
    }
    if ($view_mode == 'full') {

      if (!empty($variables['content']['field_image'])) {
        $access = false;
        if ($items = field_get_items('node', $node, 'field_image_display')) {
          $access = $items[0]['value'];
        }
        $variables['content']['field_image']['#access'] = $access;
      }

      if (!empty($episode)) {
        if ($items = field_get_items('node', $episode, 'field_image')) {
          foreach ($items as $delta => $item) {
            $variables['content']['field_episode_image'][$delta] = array(
              '#theme' => 'image_style',
              '#style_name' => 'height_thumbnail',
              '#path' => $item['uri'],
            );
          }
        }
        if ($items = field_get_items('node', $episode, 'field_background_color')) {
          $color = $items[0]['rgb'];
          $variables['title_attributes_array']['style'] = 'background-color: '.$color;
        }
        if (!empty($episode_number)) {
          if (!empty($variables['content']['field_episode'])) {
            foreach (element_children($variables['content']['field_episode']) as $delta) {
              $variables['content']['field_episode'][$delta]['#prefix'] = $variables['episode_number'].': ';
            }
          }
        }
      }
    }
    elseif ($view_mode == 'archive') {
      if (!empty($variables['content']['body'][0])) {
        $variables['content']['body'][0]['#markup'] = tal_episode_act_summary($node);
      }
      if ($items = field_get_items('node', $node, 'field_act_label')) {
        if ($items[0]['value'] != $node->title) {
          $variables['title'] = t('@label: @title', array('@label' => $items[0]['value'], '@title' => $node->title));
        }
      }

      if (!empty($node->hide_episode)) {
        $variables['classes_array'][] = 'hide-episode';
        $variables['hide_episode'] = true;
      }
      else {
        if ($items = field_get_items('node', $node, 'field_episode')) {
          $episode = node_load($items[0]['target_id']);
          $variables['episode_title'] = $episode->title;
          if ($items = field_get_items('node', $episode, 'field_episode_number')) {
            $variables['content']['episode_number'] = array(
              '#prefix' => '<div class="field field-name-field-episode-number">',
              '#suffix' => '</div>',
              '#markup' => l($items[0]['value'].': '.$episode->title, 'node/'.$episode->nid, array('attributes' => array('class' => array('goto-episode', 'goto')))),
            );
          }
          if ($items = field_get_items('node', $episode, 'field_radio_air_date')) {
            $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
            $variables['content']['radio_air_date'] = array(
              '#prefix' => '<div class="field field-name-field-radio-air-date">',
              '#suffix' => '</div>',
              '#markup' => l($date, 'node/'.$node->nid, array('attributes' => array('class' => array('goto-act')))),
            );
          }
        }
      }
    }
    elseif ($view_mode == 'related' || $view_mode == 'collection') {
      if ($items = field_get_items('node', $node, 'field_act_label')) {
        if ($items[0]['value'] != $node->title) {
          $variables['title'] = t('@label: @title', array('@label' => $items[0]['value'], '@title' => $node->title));
        }
      }

      if ($items = field_get_items('node', $node, 'field_episode')) {
        $episode = node_load($items[0]['target_id']);
        $variables['episode_title'] = $episode->title;
        if ($items = field_get_items('node', $episode, 'field_episode_number')) {
          $variables['content']['episode_number'] = array(
            '#prefix' => '<div class="field field-name-field-episode-number">',
            '#suffix' => '</div>',
            '#markup' => l($items[0]['value'], 'node/'.$episode->nid, array('attributes' => array('class' => array('goto-episode', 'goto')))),
          );
        }
        if ($items = field_get_items('node', $episode, 'field_radio_air_date')) {
          $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
          $variables['content']['radio_air_date'] = array(
            '#prefix' => '<div class="field field-name-field-radio-air-date">',
            '#suffix' => '</div>',
            '#markup' => l($date, 'node/'.$node->nid),
          );
        }
        $image = false;
        if ($items = field_get_items('node', $node, 'field_image')) {
          $image = $items[0];
        }
        elseif ($items = field_get_items('node', $episode, 'field_image')) {
          $image = $items[0];
        }
        if (!empty($image)) {
          $variables['content']['image'] = array(
            '#prefix' => '<div class="field field-name-field-image">',
            '#suffix' => '</div>',
            '#theme' => 'image_formatter',
            '#item' => $image,
            '#path' => array('path' => 'node/'.$node->nid),
          );

        }
      }
    }
    else {
      if (!empty($variables['content']['field_act_label']) && $items = field_get_items('node', $node, 'field_act_label')) {
        foreach ($items as $delta => $item) {
          $variables['content']['field_act_label'][$delta]['#markup'] = l($item['safe_value'], 'node/'.$node->nid, array('attributes' => array('class' => array('goto-act', 'goto'))));
        }
      }
    }
  }

  if ($node->type == 'article') {
    if ($view_mode == 'full') {
      if ($items = field_get_items('node', $node, 'field_audio')) {
        $variables['title_attributes_array']['class'][] = 'with-play';
        $data = tal_episode_playlist_json($node, true);
        $variables['playlist_json'] = json_encode($data);
        $attr = array(
          'data-type' => 'extra',
          'data-id' => $node->nid,
          'class' => array('extra', 'play', 'play-extra-'.$node->nid),
        );
        $variables['content']['play'] = array(
          '#markup' => l('<span class="icon icon-play"></span><span class="icon icon-pause"></span>', 'node/'.$node->nid, array('html' => true, 'attributes' => $attr)),
        );
      }
    }
  }

  if ($node->type == 'episode') {
    $schedule = tal_episode_get_latest_episode_schedule($node->nid);
    $variables['attributes_array']['data-type'] = 'episode';
    $variables['attributes_array']['data-id'] = $node->nid;
    if ($items = field_get_items('node', $node, 'field_episode_number')) {
      $episode_number = $variables['episode_number'] = $items[0]['value'];
      $variables['attributes_array']['data-episode'] = $episode_number;
      $variables['classes_array'][] = 'episode-number-'.$episode_number;
      $variables['theme_hook_suggestions'][] = 'node__episode__number_'.$episode_number;

      if ($node->is_live) {
        $attr = array(
          'data-episode' => $episode_number,
          'data-type' => $node->type,
          'data-id' => $node->nid,
          'class' => array('play', 'play-'.$episode_number),
        );
        $variables['content']['play'] = array(
          '#markup' => l('<span class="icon icon-play"></span><span class="icon icon-pause"></span>', 'node/'.$node->nid, array('html' => true, 'attributes' => $attr)),
        );
      }
      elseif (empty($variables['app']) && $items = field_get_items('node', $node, 'field_audio_promo')) {
        $attr = array(
          'data-episode' => $episode_number,
          'data-type' => $node->type,
          'data-id' => $node->nid,
          'class' => array('promo', 'play', 'play-'.$episode_number),
        );
        $variables['content']['play'] = array(
          '#markup' => l('<span class="icon icon-play"></span><span class="icon icon-pause"></span><div class="label">Preview</div>', 'node/'.$node->nid, array('html' => true, 'attributes' => $attr)),
        );
      }
    }
    if (!empty($variables['content']['field_image'][0]['#path'])) {
      $variables['content']['field_image'][0]['#path']['options']['attributes']['class'] = array('goto', 'goto-episode');
    }

    if ($view_mode == 'landing') {
      $data = tal_episode_playlist_json($node, true);
      $title = preg_replace('/\(\d{4}\)/', '', $variables['title']);
      $title = explode(' - ', $title);
      $variables['title'] = trim(reset($title));
      $variables['playlist_json'] = json_encode($data);
      if (!empty($episode_number)) {
        $awards = variable_get('tal_awards', array());
        $variables['awards'] = (!empty($awards[$episode_number]) ? $awards[$episode_number] : false);
      }
    }

    if ($view_mode == 'teaser') {
      if ($items = field_get_items('node', $node, 'field_image')) {
        $file = $items[0];
        $variables['classes_array'][] = 'with-image';
        $variables['content']['field_image'] = array(
          '#theme' => 'image_style',
          '#style_name' => 'archive',
          '#path' => $file['uri'],
          '#prefix' => '<figure class="episode-image">',
          '#suffix' => '</figure>',
        );
      }
    }

    if ($view_mode == 'recently' || $view_mode == 'series') {
      if ($items = field_get_items('node', $node, 'field_image')) {
        $file = $items[0];
        $variables['classes_array'][] = 'with-image';
        $variables['content']['field_image'] = array(
          '#theme' => 'image_style',
          '#style_name' => 'inset',
          '#path' => $file['uri'],
          '#prefix' => '<figure>',
          '#suffix' => '</figure>',
        );
      }
    }

    if ($view_mode == 'homepage') {
      if ($items = field_get_items('node', $node, 'field_hide_notch')) {
        if (!empty($items[0]['value'])) {
          $variables['classes_array'][] = 'hide-notch';
        }
      }
      if (!empty($variables['content']['body'][0])) {
        $variables['content']['body'][0]['#markup'] .= l(t('View !episode details', array('!episode' => '<span>'.t('episode').'</span>')), 'node/'.$node->nid, array('html' => true, 'attributes' => array('class' => array('details', 'goto', 'goto-episode'))));
      }
      if (!empty($variables['content']['field_episode_number']) && $items = field_get_items('node', $node, 'field_episode_number')) {
        foreach ($items as $delta => $item) {
          $variables['content']['field_episode_number'][$delta]['#markup'] = l(t('This Week: !number', array('!number' => $item['value'])), 'node/'.$node->nid, array('attributes' => array('class' => array('goto', 'goto-episode'))));
        }
      }
      if (!empty($schedule->field_radio_air_date[LANGUAGE_NONE][0]['value'])) {
        $date = date_create($schedule->field_radio_air_date[LANGUAGE_NONE][0]['value']);
        $air_date = $date->format('F j, Y');
        $air_date = "<div class='field field-name-field-radio-air-date'><span class='date-display-single'>$air_date</span></div>";
        $variables['content']['field_radio_air_date']['#markup'] = l($air_date, 'node/'.$node->nid, array('html' => true, 'attributes' => array('class' => array('goto', 'goto-episode'))));
      }
      $variables['title_attributes_array']['class'][] = 'episode-header';
      $variables['title_attributes_array']['class'][] = 'clearfix';
      if (empty($variables['content']['play'])) {
        $variables['title_attributes_array']['class'][] = 'without-play';
      }
      $data = tal_episode_playlist_json($node, true);
      $variables['playlist_json'] = json_encode($data);

      if (!$node->is_live) {
        $text = variable_get('tal_sunday_release', 'This episode will be available Sunday 7 p.m. CT.');
        $variables['content']['download'] = array(
          '#markup' => t($text),
          '#prefix' => '<div class="download">',
          '#suffix' => '</div>',
        );
      }

      if ($items = field_get_items('node', $node, 'field_hero_video')) {
        $variables['content']['video'] = array(
          '#theme' => 'tal_episode_video',
          '#node' => $node,
          '#link' => true,
        );
      }

      if ($items = field_get_items('node', $node, 'field_image')) {
        $variables['content']['field_image'] = array(
          '#theme' => 'tal_episode_image',
          '#node' => $node,
          '#link' => true,
        );

        $file = (object) $items[0];
        $variables['classes_array'][] = 'with-image';
        if ($items = field_get_items('node', $node, 'field_image_layout')) {
          $landscape = false;
          switch ($items[0]['value']) {
            case 'auto':
            if ($info = image_get_info($file->uri)) {
              if ($info['width'] > $info['height']) {
                $landscape = true;
                $variables['classes_array'][] = 'image-landscape';
              }
              else {
                $variables['classes_array'][] = 'image-portrait';
                $variables['is_portrait'] = true;
              }
            }
            break;

            case 'special':
            $variables['classes_array'][] = 'image-special';
            break;

            case 'portrait':
            $variables['is_portrait'] = true;
            $variables['classes_array'][] = 'image-portrait';
            break;

            case 'full':
            $landscape = true;
            $variables['classes_array'][] = 'image-full';
            break;

            case 'uncropped':
            case 'landscape':
            $landscape = true;
            $variables['classes_array'][] = 'image-landscape';
            break;

            case 'tile':
            $variables['classes_array'][] = 'image-tile';
            break;
          }
          if ($landscape && $items = field_get_items('node', $node, 'field_mobile_image')) {
            $variables['classes_array'][] = 'image-shifter';
          }
        }
      }
      if ($next_week = tal_episode_next_week()) {
        $variables['content']['next_week'] = array(
          '#markup' => l(t('Next week preview'), 'node/'.$next_week->nid),
          '#prefix' => '<div class="next-week">',
          '#suffix' => '</div>',
        );
      }
    }

    if ($view_mode == 'full') {
      $longest = array_reduce(str_word_count($node->title, 1), 'thislife_reduce');
      if (strlen($longest) > 10) {
        $variables['classes_array'][] = 'long-title';
      }
      $data = tal_episode_playlist_json($node, true);
      $variables['playlist_json'] = json_encode($data);
      if ($node->is_live) {
        $offline = variable_get('tal_shortcut_offline', array());

        if ($download_file = $node->download_url) {
          $variables['content']['download'] = array(
            '#markup' => l('<span class="icon icon-download"></span><span class="label">Download</span></a>', $download_file, array('html' => true, 'attributes' => array('download' => $episode_number.'.mp3'))),
          );
        }
      } else {
        $text = variable_get('tal_sunday_release', 'Episode available Sunday 7 p.m. CT');
        $variables['content']['download'] = array(
          '#markup' => t($text),
        );
        $variables['title_attributes_array']['class'][] = 'with-message';
      }
      $variables['title_attributes_array']['class'][] = 'episode-header';
      if ($items = field_get_items('node', $node, 'field_background_color')) {
        $color = $items[0]['rgb'];
        $variables['title_attributes_array']['style'] = 'background-color: '.$color;
      }
      if ($items = field_get_items('node', $node, 'field_hero_video')) {
        $variables['content']['video'] = array(
          '#theme' => 'tal_episode_video',
          '#node' => $node,
        );
      }
      if ($items = field_get_items('node', $node, 'field_image')) {
        $variables['content']['field_image'] = array(
          '#theme' => 'tal_episode_image',
          '#node' => $node,
        );
        $file = (object) $items[0];
        $variables['classes_array'][] = 'with-image';
        if ($items = field_get_items('node', $node, 'field_image_layout')) {
          $landscape = false;
          switch ($items[0]['value']) {
            case 'auto':
            if ($info = image_get_info($file->uri)) {
              if ($info['width'] > $info['height']) {
                $landscape = true;
                $variables['classes_array'][] = 'image-landscape';
              }
              else {
                $variables['classes_array'][] = 'image-portrait';
              }
            }
            break;

            case 'special':
            $variables['classes_array'][] = 'image-special';

            break;

            case 'portrait':
            $variables['classes_array'][] = 'image-portrait';
            break;

            case 'full':
            $landscape = true;
            $variables['classes_array'][] = 'image-full';
            break;

            case 'uncropped':
            case 'landscape':
            $landscape = true;
            $variables['classes_array'][] = 'image-landscape';
            break;

            case 'tile':
            $variables['classes_array'][] = 'image-tile';
            break;
          }
          if ($landscape && $items = field_get_items('node', $node, 'field_mobile_image')) {
            $variables['classes_array'][] = 'image-shifter';
          }
        }

      }

      // Check that episode is live before adding transcript link
      if ($node->is_live) {
        $query = db_select('node', 'n');
        $query->join('field_data_field_episode', 'e', "(e.entity_id = n.nid AND e.entity_type ='node' AND e.deleted = 0)");
        $query
          ->fields('n', array('nid'))
          ->condition('e.field_episode_target_id', $node->nid)
          ->condition('n.status', 1)
          ->condition('n.type', 'transcript');
        if ($nid = $query->execute()->fetchField()) {
          $variables['transcript_url'] = url('node/'.$nid);
        }
      }
    }

    if (in_array($view_mode, array('teaser', 'related', 'recently', 'featured', 'collection'))) {
      if (!empty($variables['content']['field_episode_number']) && $items = field_get_items('node', $node, 'field_episode_number')) {
        foreach ($items as $delta => $item) {
          $variables['content']['field_episode_number'][$delta]['#markup'] = l($item['value'], 'node/'.$node->nid, array('attributes' => array('class' => array('goto', 'goto-episode'))));
        }
      }
      if (!empty($variables['content']['field_radio_air_date']) && $items = field_get_items('node', $node, 'field_radio_air_date')) {
        foreach ($items as $delta => $item) {
          $variables['content']['field_radio_air_date'][$delta]['#markup'] = l($variables['content']['field_radio_air_date'][$delta]['#markup'], 'node/'.$node->nid, array('html' => true, 'attributes' => array('class' => array('goto', 'goto-episode'))));
        }
      }
    }

    if (!empty($variables['content']['field_series'])) {
      $variables['content']['field_series']['#title'] = t('More in this Series');
    }
  }

  if ($node->type == 'tv') {
    if ($items = field_get_items('node', $node, 'field_episode_number')) {
      $episode_number = t('E').$items[0]['value'];
      if ($items = field_get_items('node', $node, 'field_season_number')) {
        $episode_number = t('S').$items[0]['value'].$episode_number;
      }
      $episode_number = t('TV').' '.$episode_number;
    }
    if ($items = field_get_items('node', $node, 'field_link')) {
      $variables['watch'] = $items[0]['url'];
    }
    if ($view_mode == 'teaser') {
      if ($items = field_get_items('node', $node, 'field_image')) {
        $file = $items[0];
        $variables['classes_array'][] = 'with-image';
        $variables['content']['field_image'] = array(
          '#theme' => 'image_style',
          '#style_name' => 'archive',
          '#path' => $file['uri'],
          '#prefix' => '<figure class="episode-image">',
          '#suffix' => '</figure>',
        );
      }
      $variables['content']['field_episode_number'][0]['#markup'] = l($episode_number, 'node/'.$node->nid, array('attributes' => array('class' => array())));
    }
    if ($view_mode == 'recently' || $view_mode == 'series') {
      if ($items = field_get_items('node', $node, 'field_image')) {
        $file = $items[0];
        $variables['classes_array'][] = 'with-image';
        $variables['content']['field_image'] = array(
          '#theme' => 'image_style',
          '#style_name' => 'inset',
          '#path' => $file['uri'],
          '#prefix' => '<figure>',
          '#suffix' => '</figure>',
        );
      }
    }


    if ($view_mode == 'full') {
      $variables['title_attributes_array']['class'][] = 'episode-header';
      if ($items = field_get_items('node', $node, 'field_background_color')) {
        $color = $items[0]['rgb'];
        $variables['title_attributes_array']['style'] = 'background-color: '.$color;
      }
      if (!empty($episode_number) && !empty($variables['content']['field_episode_number'][0])) {
        $variables['content']['field_episode_number'][0]['#markup'] = $episode_number;
      }

      if ($items = field_get_items('node', $node, 'field_image')) {
        $file = (object) $items[0];
        $caption = array();
        $alt = '';
        // Caption
        if ($items = field_get_items('file', $file, 'field_caption')) {
          if (!empty($items[0]['value'])) {
            $caption['caption'] = array(
              '#markup' => check_markup($items[0]['value'], 'simple'),
            );
          }
        }
        if ($items = field_get_items('file', $file, 'field_credit')) {
          if (!empty($items[0]['value'])) {
            $caption['credit'] = array(
              '#prefix' => '<div class="credit">',
              '#suffix' => '</div>',
              '#markup' => check_markup($items[0]['value'], 'simple'),
            );
          }
        }
        $suffix = '</figure>';
        if (!empty($caption)) {
          $suffix = '<figcaption>'.render($caption).'</figcaption></figure>';
        }
        if ($items = field_get_items('file', $file, 'field_file_image_alt_text')) {
          $alt = strip_tags($items[0]['safe_value']);
        }

        $variables['classes_array'][] = 'with-image';
        if ($items = field_get_items('node', $node, 'field_image_layout')) {
          switch ($items[0]['value']) {
            case 'auto':
            if ($info = image_get_info($file->uri)) {
              if ($info['width'] > $info['height']) {
                $variables['classes_array'][] = 'image-landscape';
                $variables['content']['field_image'] = array(
                  '#theme' => 'image_style',
                  '#style_name' => 'landscape',
                  '#path' => $file->uri,
                  '#prefix' => '<figure class="episode-image landscape">',
                  '#suffix' => $suffix,
                  '#alt' => $alt,
                );
              }
              else {
                $variables['classes_array'][] = 'image-portrait';
                $variables['content']['field_image'] = array(
                  '#theme' => 'image_style',
                  '#style_name' => 'portrait',
                  '#path' => $file->uri,
                  '#prefix' => '<figure class="episode-image portrait">',
                  '#suffix' => $suffix,
                  '#alt' => $alt,
                );
              }
            }
            break;

            case 'special':
            $variables['classes_array'][] = 'image-special';

            break;

            case 'portrait':
            $variables['classes_array'][] = 'image-portrait';
            $variables['content']['field_image'] = array(
              '#theme' => 'image_style',
              '#style_name' => 'portrait',
              '#path' => $file->uri,
              '#prefix' => '<figure class="episode-image portrait">',
              '#suffix' => $suffix,
              '#alt' => $alt,
            );
            break;

            case 'full':
            $variables['classes_array'][] = 'image-full';
            $variables['content']['field_image'] = array(
              '#theme' => 'image_style',
              '#style_name' => 'landscape',
              '#path' => $file->uri,
              '#prefix' => '<figure class="episode-image full">',
              '#suffix' => $suffix,
              '#alt' => $alt,
            );
            break;

            case 'uncropped':
            $variables['classes_array'][] = 'image-landscape';
            $variables['content']['field_image'] = array(
              '#theme' => 'image_style',
              '#style_name' => 'uncropped',
              '#path' => $file->uri,
              '#prefix' => '<figure class="episode-image landscape">',
              '#suffix' => $suffix,
              '#alt' => $alt,
            );
            break;

            case 'landscape':
            $variables['classes_array'][] = 'image-landscape';
            $variables['content']['field_image'] = array(
              '#theme' => 'image_style',
              '#style_name' => 'landscape',
              '#path' => $file->uri,
              '#prefix' => '<figure class="episode-image landscape">',
              '#suffix' => $suffix,
              '#alt' => $alt,
            );
            break;

            case 'tile':
            $variables['classes_array'][] = 'image-tile';
            $variables['content']['field_image'] = array(
              '#prefix' => '<figure class="episode-image tile clearfix">',
              '#suffix' => $suffix,
            );
            for ($i = 1; $i <= 4; $i++) {
              $variables['content']['field_image'][] = array(
                '#theme' => 'image_style',
                '#style_name' => 'square',
                '#path' => $file->uri,
                '#alt' => $alt,
                '#attributes' => array(
                  'class' => array(
                    'image-'.$i,
                  ),
                ),
              );
            }
            break;
          }
        }
      }

    }
    if ($view_mode == 'related' || $view_mode == 'collection') {
      $variables['content']['field_episode_number'][0]['#markup'] = l($episode_number, 'node/'.$node->nid, array('attributes' => array('class' => array())));

      if ($items = field_get_items('node', $node, 'field_radio_air_date')) {
        $date = thislife_fix_date(date('M. j, Y', strtotime($items[0]['value'])));
        $variables['content']['radio_air_date'] = array(
          '#prefix' => '<div class="field field-name-field-radio-air-date">',
          '#suffix' => '</div>',
          '#markup' => l($date, 'node/'.$node->nid),
        );
      }
      if ($items = field_get_items('node', $node, 'field_image')) {
        $image = $items[0];
        $variables['content']['image'] = array(
          '#prefix' => '<div class="field field-name-field-image">',
          '#suffix' => '</div>',
          '#theme' => 'image_formatter',
          '#item' => $image,
          '#path' => array('path' => 'node/'.$node->nid),
        );

      }

    }

  }

}

function thislife_css_alter(&$css) {
  $path = path_to_theme().'/css/icomoon.css';
  if (!empty($css[$path])) {
    $css[$path]['preprocess'] = false;
  }
  foreach ($css as $path => $item) {
    if (strpos($path, 'sites/all/modules/contrib') === 0) {
      unset($css[$path]);
    }
  }
}

function thislife_field($variables) {
  $output = '';
  $element = $variables['element'];
  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h2 class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . '</h2>';
  }

  if (in_array($element['#field_name'], array('field_featured_collections', 'field_collections', 'field_secondary_collections'))) {
    $variables['classes'] .= ' clearfix';
  }

  if ($element['#field_name'] == 'field_featured_video') {
    $variables['classes'] .= ' clearfix';
  }

  if ($element['#field_name'] == 'field_title' && $element['#entity_type'] == 'field_collection_item' && in_array($element['#bundle'], array('field_videos', 'field_picks', 'field_episode_collection'))) {
    if ($element['#view_mode'] == 'full') {
      // Render the items.
      $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
      foreach ($variables['items'] as $delta => $item) {
        $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
        $output .= '<h2 class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</h2>';
      }
      $output .= '</div>';
    }
  }
  else {
    // Render the items.
    $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
    foreach ($variables['items'] as $delta => $item) {
      $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
      $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
    }
    $output .= '</div>';
  }
  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

function thislife_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;
  $items = array();
  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = false;
  $li_previous = false;
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('next ›')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = false;

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {

      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current'),
            'data' => $i,
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }
    if (!empty($items)) {
      return theme('item_list', array(
        'items' => $items,
        'attributes' => array('class' => array('pager')),
      ));
    }
  }
}

function thislife_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $attributes['class'][] = 'pager';
  $attributes['class'][] = 'ignore';

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $attributes['href'] = url($_GET['q'], array('query' => $query));
  return '<a' . drupal_attributes($attributes) . '>' . check_plain($text) . '</a>';
}


function thislife_image_style($variables) {
  // Determine the dimensions of the styled image.
  $dimensions = array(
    'width' => $variables['width'],
    'height' => $variables['height'],
  );

  $uri = image_style_path($variables['style_name'], $variables['path']);

  // Determine the URL for the styled image.
  $variables['path'] = image_style_url($variables['style_name'], $variables['path']);
  if ($info = image_get_info($uri)) {
    $variables['width'] = $info['width'];
    $variables['height'] = $info['height'];
  }
  return theme('image', $variables);
}

function thislife_reduce($v, $p) {
    return strlen($v) > strlen($p) ? $v : $p;
}
