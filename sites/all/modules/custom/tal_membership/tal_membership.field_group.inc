<?php

/**
 * @file
 * tal_membership.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tal_membership_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_break|node|life_partner|form';
  $field_group->group_name = 'group_break';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'life_partner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vert';
  $field_group->data = array(
    'children' => array(
      0 => 'field_break_header',
      1 => 'field_break_text',
      2 => 'field_button_link',
      3 => 'field_button_logo',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-break field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Break',
    'weight' => '3',
  );
  $field_groups['group_break|node|life_partner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_faq|node|life_partner|form';
  $field_group->group_name = 'group_faq';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'life_partner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vert';
  $field_group->data = array(
    'children' => array(
      0 => 'field_call_to_action',
      1 => 'field_cta_link',
      2 => 'field_faq_header',
      3 => 'field_questions',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-faq field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'FAQ',
    'weight' => '4',
  );
  $field_groups['group_faq|node|life_partner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_main|node|life_partner|form';
  $field_group->group_name = 'group_main';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'life_partner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vert';
  $field_group->data = array(
    'children' => array(
      0 => 'field_header',
      1 => 'field_membership_logo',
      2 => 'field_membership_logo_mobile',
      3 => 'field_sales_pitch',
      4 => 'field_sales_pitch_sub',
      5 => 'title',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
        'required_fields' => 1,
      ),
      'label' => 'Above the fold',
    ),
    'format_type' => 'tab',
    'label' => 'Above the fold',
    'weight' => '1',
  );
  $field_groups['group_main|node|life_partner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_meta|node|life_partner|form';
  $field_group->group_name = 'group_meta';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'life_partner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vert';
  $field_group->data = array(
    'children' => array(
      0 => 'field_short_description',
      1 => 'field_social_image',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-meta field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'tab',
    'label' => 'Meta fields',
    'weight' => '5',
  );
  $field_groups['group_meta|node|life_partner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sub_levels|node|life_partner|form';
  $field_group->group_name = 'group_sub_levels';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'life_partner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_vert';
  $field_group->data = array(
    'children' => array(
      0 => 'field_membership_level',
    ),
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => 'group-sub-levels field-group-tab',
        'description' => '',
        'required_fields' => 1,
      ),
      'label' => 'Membership Levels',
    ),
    'format_type' => 'tab',
    'label' => 'Membership Levels',
    'weight' => '2',
  );
  $field_groups['group_sub_levels|node|life_partner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_vert|node|life_partner|form';
  $field_group->group_name = 'group_vert';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'life_partner';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'group_break',
      1 => 'group_faq',
      2 => 'group_main',
      3 => 'group_sub_levels',
      4 => 'group_meta',
    ),
    'format_settings' => array(
      'instance_settings' => array(
        'classes' => 'group-vert field-group-tabs',
        'id' => '',
      ),
      'label' => '',
    ),
    'format_type' => 'tabs',
    'label' => '',
    'weight' => '0',
  );
  $field_groups['group_vert|node|life_partner|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Above the fold');
  t('Break');
  t('FAQ');
  t('Membership Levels');
  t('Meta fields');

  return $field_groups;
}
