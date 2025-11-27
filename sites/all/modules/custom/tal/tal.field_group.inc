<?php

/**
 * @file
 * tal.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tal_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_app|node|listen|form';
  $field_group->group_name = 'group_app';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'listen';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_app_body',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 0,
      ),
      'label' => 'Our Website',
    ),
    'format_type' => 'fieldset',
    'label' => 'Our Website',
    'weight' => '7',
  );
  $field_groups['group_app|node|listen|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_podcast|node|listen|form';
  $field_group->group_name = 'group_podcast';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'listen';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_podcast_body',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 0,
      ),
      'label' => 'Podcast Apps',
    ),
    'format_type' => 'fieldset',
    'label' => 'Podcast Apps',
    'weight' => '6',
  );
  $field_groups['group_podcast|node|listen|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_radio|node|listen|form';
  $field_group->group_name = 'group_radio';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'listen';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_radio_body',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 0,
      ),
      'label' => 'On The Radio',
    ),
    'format_type' => 'fieldset',
    'label' => 'On The Radio',
    'weight' => '8',
  );
  $field_groups['group_radio|node|listen|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('On The Radio');
  t('Our Website');
  t('Podcast Apps');

  return $field_groups;
}
