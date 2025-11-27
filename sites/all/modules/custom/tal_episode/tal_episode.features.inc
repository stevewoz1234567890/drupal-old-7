<?php

/**
 * @file
 * tal_episode.features.inc
 */

/**
 * Implements hook_default_cer().
 */
function tal_episode_default_cer() {
  $items = array();
  $items['node:episode:field_acts*node:act:field_episode'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "0" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:episode:field_acts" } ] },
    "cer_right" : { "und" : [ { "path" : "node:act:field_episode" } ] },
    "cer_weight" : { "und" : [ { "value" : "0" } ] },
    "identifier" : "node:episode:field_acts*node:act:field_episode"
  }');
  $items['node:tv:field_tv_acts*node:tv_act:field_tv_episode'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:tv:field_tv_acts" } ] },
    "cer_right" : { "und" : [ { "path" : "node:tv_act:field_tv_episode" } ] },
    "cer_weight" : { "und" : [ { "value" : "0" } ] },
    "identifier" : "node:tv:field_tv_acts*node:tv_act:field_tv_episode"
  }');
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function tal_episode_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function tal_episode_image_default_styles() {
  $styles = array();

  // Exported image style: archive.
  $styles['archive'] = array(
    'label' => 'Archive',
    'effects' => array(
      7 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => '',
          'height' => 200,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: column.
  $styles['column'] = array(
    'label' => 'Column',
    'effects' => array(
      5 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1440,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: contributor.
  $styles['contributor'] = array(
    'label' => 'Contributor',
    'effects' => array(
      6 => array(
        'name' => 'imagefield_focus_scale_and_crop',
        'data' => array(
          'width' => 308,
          'height' => 384,
          'strength' => 'high',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: height_thumbnail.
  $styles['height_thumbnail'] = array(
    'label' => 'Height Thumbnail',
    'effects' => array(
      4 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => '',
          'height' => 240,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: landscape.
  $styles['landscape'] = array(
    'label' => 'Landscape',
    'effects' => array(
      1 => array(
        'name' => 'imagefield_focus_scale_and_crop',
        'data' => array(
          'width' => 1600,
          'height' => 900,
          'strength' => 'high',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: large_landscape.
  $styles['large_landscape'] = array(
    'label' => 'Large Landscape',
    'effects' => array(
      12 => array(
        'name' => 'imagefield_focus_scale_and_crop',
        'data' => array(
          'width' => 2400,
          'height' => 1350,
          'strength' => 'high',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: large_portrait.
  $styles['large_portrait'] = array(
    'label' => 'Large Portrait',
    'effects' => array(
      13 => array(
        'name' => 'imagefield_focus_scale_and_crop',
        'data' => array(
          'width' => 1200,
          'height' => 1500,
          'strength' => 'low',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: portrait.
  $styles['portrait'] = array(
    'label' => 'Portrait',
    'effects' => array(
      2 => array(
        'name' => 'imagefield_focus_scale_and_crop',
        'data' => array(
          'width' => 800,
          'height' => 1000,
          'strength' => 'high',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: square.
  $styles['square'] = array(
    'label' => 'Square',
    'effects' => array(
      3 => array(
        'name' => 'imagefield_focus_scale_and_crop',
        'data' => array(
          'width' => 540,
          'height' => 540,
          'strength' => 'high',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: uncropped.
  $styles['uncropped'] = array(
    'label' => 'Uncropped',
    'effects' => array(
      2 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 2400,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function tal_episode_node_info() {
  $items = array(
    'act' => array(
      'name' => t('Act'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'episode' => array(
      'name' => t('Episode Page'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'schedule' => array(
      'name' => t('Episode Air Date'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'tv' => array(
      'name' => t('TV Episode'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'tv_act' => array(
      'name' => t('TV Act'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
