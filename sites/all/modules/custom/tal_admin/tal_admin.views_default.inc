<?php

/**
 * @file
 * tal_admin.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tal_admin_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'content_advanced';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Content - Advanced';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Content Administration';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_episode' => 'field_episode',
    'changed' => 'changed',
    'status' => 'status',
    'type' => 'type',
    'field_contributor' => 'field_contributor',
    'field_release_date' => 'field_release_date',
    'field_radio_air_date' => 'field_radio_air_date',
    'nid' => 'nid',
  );
  $handler->display->display_options['style_options']['class'] = '';
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_episode' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_contributor' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_release_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'field_radio_air_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h1>Content Administration</h1>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_contributor_target_id']['id'] = 'field_contributor_target_id';
  $handler->display->display_options['relationships']['field_contributor_target_id']['table'] = 'field_data_field_contributor';
  $handler->display->display_options['relationships']['field_contributor_target_id']['field'] = 'field_contributor_target_id';
  /* Relationship: Content: Tags (field_tags) */
  $handler->display->display_options['relationships']['field_tags_tid']['id'] = 'field_tags_tid';
  $handler->display->display_options['relationships']['field_tags_tid']['table'] = 'field_data_field_tags';
  $handler->display->display_options['relationships']['field_tags_tid']['field'] = 'field_tags_tid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_episode_target_id']['id'] = 'field_episode_target_id';
  $handler->display->display_options['relationships']['field_episode_target_id']['table'] = 'field_data_field_episode';
  $handler->display->display_options['relationships']['field_episode_target_id']['field'] = 'field_episode_target_id';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Episode Number */
  $handler->display->display_options['fields']['field_episode_number']['id'] = 'field_episode_number';
  $handler->display->display_options['fields']['field_episode_number']['table'] = 'field_data_field_episode_number';
  $handler->display->display_options['fields']['field_episode_number']['field'] = 'field_episode_number';
  $handler->display->display_options['fields']['field_episode_number']['relationship'] = 'field_episode_target_id';
  $handler->display->display_options['fields']['field_episode_number']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_episode_number']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_episode_number']['type'] = 'number_unformatted';
  $handler->display->display_options['fields']['field_episode_number']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Episode */
  $handler->display->display_options['fields']['field_episode']['id'] = 'field_episode';
  $handler->display->display_options['fields']['field_episode']['table'] = 'field_data_field_episode';
  $handler->display->display_options['fields']['field_episode']['field'] = 'field_episode';
  $handler->display->display_options['fields']['field_episode']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_episode']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_episode']['alter']['text'] = '<br /><a href="/node/[field_episode-target_id]/edit">Episode #[field_episode_number]</a>';
  $handler->display->display_options['fields']['field_episode']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_episode']['settings'] = array(
    'bypass_access' => 0,
    'link' => 0,
  );
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'custom';
  $handler->display->display_options['fields']['changed']['custom_date_format'] = 'n/j/y H:i';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: Content: Published status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Title';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['status']['alter']['text'] = '[title][field_episode]
<br />[status] -- <em>Updated: [changed]</em>';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['type'] = 'published-notpublished';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: Field: Contributors */
  $handler->display->display_options['fields']['field_contributor']['id'] = 'field_contributor';
  $handler->display->display_options['fields']['field_contributor']['table'] = 'field_data_field_contributor';
  $handler->display->display_options['fields']['field_contributor']['field'] = 'field_contributor';
  $handler->display->display_options['fields']['field_contributor']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_contributor']['settings'] = array(
    'bypass_access' => 0,
    'link' => 0,
  );
  $handler->display->display_options['fields']['field_contributor']['delta_offset'] = '0';
  /* Field: Content: Early Release Date */
  $handler->display->display_options['fields']['field_release_date']['id'] = 'field_release_date';
  $handler->display->display_options['fields']['field_release_date']['table'] = 'field_data_field_release_date';
  $handler->display->display_options['fields']['field_release_date']['field'] = 'field_release_date';
  $handler->display->display_options['fields']['field_release_date']['label'] = 'Early Release';
  $handler->display->display_options['fields']['field_release_date']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_release_date']['alter']['text'] = '<br />[field_release_date]';
  $handler->display->display_options['fields']['field_release_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_release_date']['settings'] = array(
    'format_type' => 'custom',
    'custom_date_format' => 'n/j/y G:i',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_radio_air_date']['id'] = 'field_radio_air_date';
  $handler->display->display_options['fields']['field_radio_air_date']['table'] = 'field_data_field_radio_air_date';
  $handler->display->display_options['fields']['field_radio_air_date']['field'] = 'field_radio_air_date';
  $handler->display->display_options['fields']['field_radio_air_date']['label'] = 'Air Date';
  $handler->display->display_options['fields']['field_radio_air_date']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_radio_air_date']['alter']['text'] = '[field_radio_air_date][field_release_date]';
  $handler->display->display_options['fields']['field_radio_air_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_radio_air_date']['settings'] = array(
    'format_type' => 'custom',
    'custom_date_format' => 'n/j/y',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'Actions';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = '<a href="/node/[nid]/edit?destination=/admin/content/advanced">edit</a> | <a href="/node/[nid]/delete?destination=/admin/content/advanced">delete</a> ';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Post date';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Taxonomy term: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'field_contributor_target_id';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Contributor';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'contrib';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Published status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 'All';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Episode Number (field_episode_number) */
  $handler->display->display_options['filters']['field_episode_number_value']['id'] = 'field_episode_number_value';
  $handler->display->display_options['filters']['field_episode_number_value']['table'] = 'field_data_field_episode_number';
  $handler->display->display_options['filters']['field_episode_number_value']['field'] = 'field_episode_number_value';
  $handler->display->display_options['filters']['field_episode_number_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_episode_number_value']['expose']['operator_id'] = 'field_episode_number_value_op';
  $handler->display->display_options['filters']['field_episode_number_value']['expose']['label'] = 'Episode #';
  $handler->display->display_options['filters']['field_episode_number_value']['expose']['operator'] = 'field_episode_number_value_op';
  $handler->display->display_options['filters']['field_episode_number_value']['expose']['identifier'] = 'episode_num';
  $handler->display->display_options['filters']['field_episode_number_value']['expose']['remember_roles'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_episode_number_value']['group_info']['label'] = 'Episode Number (field_episode_number)';
  $handler->display->display_options['filters']['field_episode_number_value']['group_info']['identifier'] = 'field_episode_number_value';
  $handler->display->display_options['filters']['field_episode_number_value']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['field_episode_number_value']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: Content: Episode Number (field_episode_number) */
  $handler->display->display_options['filters']['field_episode_number_value_1']['id'] = 'field_episode_number_value_1';
  $handler->display->display_options['filters']['field_episode_number_value_1']['table'] = 'field_data_field_episode_number';
  $handler->display->display_options['filters']['field_episode_number_value_1']['field'] = 'field_episode_number_value';
  $handler->display->display_options['filters']['field_episode_number_value_1']['relationship'] = 'field_episode_target_id';
  $handler->display->display_options['filters']['field_episode_number_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_episode_number_value_1']['expose']['operator_id'] = 'field_episode_number_value_1_op';
  $handler->display->display_options['filters']['field_episode_number_value_1']['expose']['label'] = 'Parent Ep #';
  $handler->display->display_options['filters']['field_episode_number_value_1']['expose']['operator'] = 'field_episode_number_value_1_op';
  $handler->display->display_options['filters']['field_episode_number_value_1']['expose']['identifier'] = 'parent_num';
  $handler->display->display_options['filters']['field_episode_number_value_1']['expose']['remember_roles'] = array(
    2 => '2',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Content';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['content_advanced'] = $view;

  return $export;
}
