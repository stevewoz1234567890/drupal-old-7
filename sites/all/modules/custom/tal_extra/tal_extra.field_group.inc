<?php
/**
 * @file
 * tal_extra.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tal_extra_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_danger|node|fullscreen|form';
  $field_group->group_name = 'group_danger';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'fullscreen';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Danger Zone',
    'weight' => '11',
    'children' => array(
      0 => 'field_template',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Danger Zone',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_danger|node|fullscreen|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ratio|node|video|form';
  $field_group->group_name = 'group_ratio';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'video';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Aspect Ratio',
    'weight' => '10',
    'children' => array(
      0 => 'field_height',
      1 => 'field_width',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Aspect Ratio',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_ratio|node|video|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Aspect Ratio');
  t('Danger Zone');

  return $field_groups;
}
