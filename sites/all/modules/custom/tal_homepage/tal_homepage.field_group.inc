<?php

/**
 * @file
 * tal_homepage.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tal_homepage_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_banner|node|homepage|form';
  $field_group->group_name = 'group_banner';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'homepage';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_banner',
      1 => 'field_link',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Banner',
    ),
    'format_type' => 'fieldset',
    'label' => 'Banner',
    'weight' => '5',
  );
  $field_groups['group_banner|node|homepage|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_featured|node|homepage|form';
  $field_group->group_name = 'group_featured';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'homepage';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_featured',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Featured',
    ),
    'format_type' => 'fieldset',
    'label' => 'Featured',
    'weight' => '4',
  );
  $field_groups['group_featured|node|homepage|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_promo|node|homepage|form';
  $field_group->group_name = 'group_promo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'homepage';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_background_color',
      1 => 'field_body',
      2 => 'field_call_to_action',
      3 => 'field_display_promo',
      4 => 'field_header',
      5 => 'field_image',
      6 => 'field_mobile_image',
      7 => 'field_text_color',
      8 => 'field_image_link',
      9 => 'field_cta_link',
    ),
    'format_settings' => array(
      'formatter' => 'collapsed',
      'instance_settings' => array(
        'classes' => 'group-promo field-group-fieldset',
        'description' => '',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Promo',
    ),
    'format_type' => 'fieldset',
    'label' => 'Promo',
    'weight' => '2',
  );
  $field_groups['group_promo|node|homepage|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_recommended|node|homepage|form';
  $field_group->group_name = 'group_recommended';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'homepage';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_collections',
      1 => 'field_notes',
    ),
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Recommended',
    ),
    'format_type' => 'fieldset',
    'label' => 'Recommended',
    'weight' => '6',
  );
  $field_groups['group_recommended|node|homepage|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Banner');
  t('Featured');
  t('Promo');
  t('Recommended');

  return $field_groups;
}
