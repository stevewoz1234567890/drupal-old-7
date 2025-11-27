<?php
/**
 * @file
 * tal_about.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function tal_about_taxonomy_default_vocabularies() {
  return array(
    'events' => array(
      'name' => 'Events',
      'machine_name' => 'events',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
