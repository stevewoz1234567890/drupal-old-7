<?php

/**
 * @file
 * tal_membership.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tal_membership_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function tal_membership_node_info() {
  $items = array(
    'eyebrow' => array(
      'name' => t('Eyebrow'),
      'base' => 'node_content',
      'description' => t('A published eyebrow will appear in the #eyebrow div above the main content.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'life_partner' => array(
      'name' => t('Membership Landing Page'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'modal' => array(
      'name' => t('Modal Takeover'),
      'base' => 'node_content',
      'description' => t('The newest published modal will appear on any page of the site, on first visit until dismissed.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'rss_extra' => array(
      'name' => t('RSS extra'),
      'base' => 'node_content',
      'description' => t('Add an item to the feed that is not an episode.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
