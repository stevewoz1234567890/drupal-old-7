<?php

/**
 * @file
 * tal_episode.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tal_episode_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_acts|node|episode|form';
  $field_group->group_name = 'group_acts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_acts',
      1 => 'field_extras',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-acts field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Acts & Extras',
    'weight' => '64',
  );
  $field_groups['group_acts|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_api_log|node|schedule|form';
  $field_group->group_name = 'group_api_log';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-api-log field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Megaphone API Log',
    'weight' => '8',
  );
  $field_groups['group_api_log|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_megaphone_manual|node|schedule|form';
  $field_group->group_name = 'group_megaphone_manual';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-megaphone-manual field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Megaphone Manual Ops',
    'weight' => '8',
  );
  $field_groups['group_megaphone_manual|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_megaphone_info|node|schedule|form';
  $field_group->group_name = 'group_megaphone_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-megaphone-info field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Megaphone Episode Info',
    'weight' => '10',
  );
  $field_groups['group_megaphone_info|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_audio|node|episode|form';
  $field_group->group_name = 'group_audio';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_audio_archive',
      1 => 'field_audio_promo',
      2 => 'field_audio_nyt',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Audio',
    'weight' => '65',
  );
  $field_groups['group_audio|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_audio|node|schedule|form';
  $field_group->group_name = 'group_audio';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_audio_clean',
      1 => 'field_audio_extended',
      2 => 'field_audio_podcast',
      3 => 'field_midroll',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'required_fields' => 1,
      ),
      'label' => 'Audio',
    ),
    'format_type' => 'tab',
    'label' => 'Audio',
    'weight' => '4',
  );
  $field_groups['group_audio|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_dates|node|schedule|form';
  $field_group->group_name = 'group_dates';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_radio_air_date',
      1 => 'field_release_date',
      2 => 'field_rerun',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'required_fields' => 1,
      ),
      'label' => 'Dates',
    ),
    'format_type' => 'tab',
    'label' => 'Dates',
    'weight' => '5',
  );
  $field_groups['group_dates|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_display|node|episode|form';
  $field_group->group_name = 'group_display';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_image',
      1 => 'field_image_layout',
      2 => 'field_background_color',
      3 => 'field_social_image',
      4 => 'field_mobile_image',
      5 => 'field_hide_notch',
      6 => 'field_hero_video',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-display field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Display',
    'weight' => '63',
  );
  $field_groups['group_display|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_dovetail|node|episode|form';
  $field_group->group_name = 'group_dovetail';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_retired';
  $field_group->data = array(
    'children' => array(
      0 => 'field_audio_a',
      1 => 'field_audio_b',
      2 => 'field_dovetail',
      3 => 'field_dovetail_status',
      4 => 'field_audio_b_extended',
      5 => 'field_dovetail_id',
    ),
    'format_settings' => array(
      'formatter' => 'collapsed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 0,
      ),
      'label' => 'Feed',
    ),
    'format_type' => 'fieldset',
    'label' => 'Feed',
    'weight' => '50',
  );
  $field_groups['group_dovetail|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_download|node|episode|form';
  $field_group->group_name = 'group_download';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_retired';
  $field_group->data = array(
    'children' => array(
      0 => 'field_amazon',
      1 => 'field_itunes',
    ),
    'format_settings' => array(
      'formatter' => 'collapsed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Download',
    ),
    'format_type' => 'fieldset',
    'label' => 'Download',
    'weight' => '48',
  );
  $field_groups['group_download|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_megaphone|node|schedule|form';
  $field_group->group_name = 'group_megaphone';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_megaphone_episode_id',
      1 => 'field_megaphone_episode_url',
      2 => 'field_do_not_sync',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-megaphone field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Megaphone',
    'weight' => '6',
  );
  $field_groups['group_megaphone|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_meta|node|episode|form';
  $field_group->group_name = 'group_meta';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'body',
      1 => 'field_episode_number',
      2 => 'field_radio_air_date',
      3 => 'field_explicit',
      4 => 'field_import_id',
      5 => 'field_notes',
      6 => 'field_series',
      7 => 'field_retracted',
      8 => 'field_advisory',
      9 => 'field_original',
      10 => 'field_notes_lower',
      11 => 'title',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => 'group-meta field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
      'label' => 'Details',
    ),
    'format_type' => 'tab',
    'label' => 'Details',
    'weight' => '62',
  );
  $field_groups['group_meta|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_retired|node|episode|form';
  $field_group->group_name = 'group_retired';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_audio_clean',
      1 => 'field_audio_extended',
      2 => 'field_audio_podcast',
      3 => 'field_stream_archive',
      4 => 'field_stream_extended',
      5 => 'field_stream_podcast',
      6 => 'field_audio_pandora',
      7 => 'field_pandora_adbreak',
      8 => 'field_midroll',
      9 => 'group_dovetail',
      10 => 'group_download',
      11 => 'group_simplecast',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Retired',
    'weight' => '67',
  );
  $field_groups['group_retired|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_rss|node|episode|form';
  $field_group->group_name = 'group_rss';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_tabs';
  $field_group->data = array(
    'children' => array(
      0 => 'field_rss_promo_copy',
      1 => 'field_rss_image',
      2 => 'field_youtube_image',
      3 => 'field_publish_to_youtube_rss',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-rss field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'RSS',
    'weight' => '66',
  );
  $field_groups['group_rss|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_simplecast|node|episode|form';
  $field_group->group_name = 'group_simplecast';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_retired';
  $field_group->data = array(
    'children' => array(
      0 => 'field_simplecast',
      1 => 'field_simplecast_episode',
      2 => 'field_simplecast_audio',
      3 => 'field_simplecast_status',
      4 => 'field_simplecast_nyt',
      5 => 'field_simplecast_nyt_episode',
      6 => 'field_simplecast_nyt_audio',
      7 => 'field_simplecast_promo',
      8 => 'field_simplecast_promo_episode',
      9 => 'field_simplecast_promo_audio',
    ),
    'format_settings' => array(
      'formatter' => 'collapsed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 0,
      ),
      'label' => 'Simplecast',
    ),
    'format_type' => 'fieldset',
    'label' => 'Simplecast',
    'weight' => '49',
  );
  $field_groups['group_simplecast|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|episode|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'episode';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'group_acts',
      1 => 'group_audio',
      2 => 'group_display',
      3 => 'group_meta',
      4 => 'group_retired',
      5 => 'group_rss',
    ),
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-tabs field-group-tabs',
        'id' => '',
      ),
    ),
    'format_type' => 'tabs',
    'label' => 'Tabs',
    'weight' => '0',
  );
  $field_groups['group_tabs|node|episode|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tabs|node|schedule|form';
  $field_group->group_name = 'group_tabs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'schedule';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'group_audio',
      1 => 'group_dates',
      2 => 'group_megaphone',
      3 => 'group_megaphone_manual',
      4 => 'group_api_log',
      5 => 'group_megaphone_info',
    ),
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-tabs field-group-tabs',
        'id' => '',
      ),
    ),
    'format_type' => 'tabs',
    'label' => 'Tabs',
    'weight' => '2',
  );
  $field_groups['group_tabs|node|schedule|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_timestamps|node|act|form';
  $field_group->group_name = 'group_timestamps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'act';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_timestamp',
      1 => 'field_timestamp_extended',
      2 => 'field_timestamp_podcast',
      3 => 'field_timestamp_dovetail',
    ),
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Timestamps',
    ),
    'format_type' => 'fieldset',
    'label' => 'Timestamps',
    'weight' => '5',
  );
  $field_groups['group_timestamps|node|act|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Acts & Extras');
  t('Audio');
  t('Dates');
  t('Details');
  t('Display');
  t('Download');
  t('Feed');
  t('Megaphone');
  t('Megaphone API Log');
  t('RSS');
  t('Retired');
  t('Simplecast');
  t('Tabs');
  t('Timestamps');

  return $field_groups;
}
