<?php

function welcome_preprocess_node(&$variables) {
  $variables['app'] = (!empty($_GET['app']));
  $node = $variables['node'];
  $view_mode = $variables['view_mode'];
  $variables['node_url'] = url('node/'.$node->nid, array('absolute' => true));

  if ($node->type == 'episode') {
    $variables['theme_hook_suggestions'] = array(
      'node__episode',
    );
    $data = tal_episode_playlist_json($node, true);
    $variables['playlist_json'] = json_encode($data);
    if ($download_file = $node->download_url) {
      $variables['content']['download'] = array(
        '#markup' => l('<span class="icon icon-download"></span>', $download_file, array('html' => true, 'attributes' => array('title' => t('Download'), 'class' => array('download')))),
      );
    }
  }
}

function welcome_link($variables) {
  $variables['options']['absolute'] = true;
  return '<a href="' . check_plain(url($variables['path'], $variables['options'])) . '"' . drupal_attributes($variables['options']['attributes']) . '>' . ($variables['options']['html'] ? $variables['text'] : check_plain($variables['text'])) . '</a>';
}

function welcome_preprocess_page(&$variables) {
  $variables['front_page'] = url('<front>', array('absolute' => true));
}
