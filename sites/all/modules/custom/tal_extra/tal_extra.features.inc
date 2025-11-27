<?php
/**
 * @file
 * tal_extra.features.inc
 */

/**
 * Implements hook_default_cer().
 */
function tal_extra_default_cer() {
  $items = array();
  $items['node:act:field_extras*node:article:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:act:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:article:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-37" } ] },
    "identifier" : "node:act:field_extras*node:article:field_episodes"
  }');
  $items['node:act:field_extras*node:fullscreen:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:act:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:fullscreen:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-40" } ] },
    "identifier" : "node:act:field_extras*node:fullscreen:field_episodes"
  }');
  $items['node:act:field_extras*node:gallery:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:act:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:gallery:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-38" } ] },
    "identifier" : "node:act:field_extras*node:gallery:field_episodes"
  }');
  $items['node:act:field_extras*node:video:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:act:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:video:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-39" } ] },
    "identifier" : "node:act:field_extras*node:video:field_episodes"
  }');
  $items['node:episode:field_extras*node:article:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:episode:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:article:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-41" } ] },
    "identifier" : "node:episode:field_extras*node:article:field_episodes"
  }');
  $items['node:episode:field_extras*node:fullscreen:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:episode:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:fullscreen:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-44" } ] },
    "identifier" : "node:episode:field_extras*node:fullscreen:field_episodes"
  }');
  $items['node:episode:field_extras*node:gallery:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:episode:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:gallery:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-42" } ] },
    "identifier" : "node:episode:field_extras*node:gallery:field_episodes"
  }');
  $items['node:episode:field_extras*node:video:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:episode:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:video:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-43" } ] },
    "identifier" : "node:episode:field_extras*node:video:field_episodes"
  }');
  $items['node:tv:field_extras*node:article:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:tv:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:article:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-50" } ] },
    "identifier" : "node:tv:field_extras*node:article:field_episodes"
  }');
  $items['node:tv:field_extras*node:fullscreen:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:tv:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:fullscreen:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-47" } ] },
    "identifier" : "node:tv:field_extras*node:fullscreen:field_episodes"
  }');
  $items['node:tv:field_extras*node:gallery:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:tv:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:gallery:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-49" } ] },
    "identifier" : "node:tv:field_extras*node:gallery:field_episodes"
  }');
  $items['node:tv:field_extras*node:video:field_episodes'] = entity_import('cer', '{
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_left" : { "und" : [ { "path" : "node:tv:field_extras" } ] },
    "cer_right" : { "und" : [ { "path" : "node:video:field_episodes" } ] },
    "cer_weight" : { "und" : [ { "value" : "-48" } ] },
    "identifier" : "node:tv:field_extras*node:video:field_episodes"
  }');
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function tal_extra_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function tal_extra_image_default_styles() {
  $styles = array();

  // Exported image style: collection.
  $styles['collection'] = array(
    'label' => 'Collection',
    'effects' => array(
      11 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1600,
          'height' => 900,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: extra.
  $styles['extra'] = array(
    'label' => 'Extra',
    'effects' => array(
      8 => array(
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

  // Exported image style: inset.
  $styles['inset'] = array(
    'label' => 'Inset',
    'effects' => array(
      10 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 520,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: vertical.
  $styles['vertical'] = array(
    'label' => 'Vertical',
    'effects' => array(
      9 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1080,
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
function tal_extra_node_info() {
  $items = array(
    'article' => array(
      'name' => t('Article'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'fullscreen' => array(
      'name' => t('Fullscreen'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'gallery' => array(
      'name' => t('Photo Gallery'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'video' => array(
      'name' => t('Video'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'video_collection' => array(
      'name' => t('Video Collection'),
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
