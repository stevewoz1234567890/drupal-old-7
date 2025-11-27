<?php
/**
 * @file
 * tal_transcript.features.inc
 */

/**
 * Implements hook_node_info().
 */
function tal_transcript_node_info() {
  $items = array(
    'transcript' => array(
      'name' => t('Transcript'),
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
