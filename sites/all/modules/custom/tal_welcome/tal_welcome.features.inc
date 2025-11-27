<?php
/**
 * @file
 * tal_welcome.features.inc
 */

/**
 * Implements hook_node_info().
 */
function tal_welcome_node_info() {
  $items = array(
    'welcome' => array(
      'name' => t('Welcome'),
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
