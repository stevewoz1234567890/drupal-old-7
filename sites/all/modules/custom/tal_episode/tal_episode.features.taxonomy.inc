<?php

/**
 * @file
 * tal_episode.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function tal_episode_taxonomy_default_vocabularies() {
  return array(
    'contributor' => array(
      'name' => 'Contributor',
      'machine_name' => 'contributor',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'roles' => array(
      'name' => 'Roles',
      'machine_name' => 'roles',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'search_keywords' => array(
      'name' => 'Search Keywords',
      'machine_name' => 'search_keywords',
      'description' => 'Content can be tagged with keywords to ensure inclusion in specific search results.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'series' => array(
      'name' => 'Series',
      'machine_name' => 'series',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tags' => array(
      'name' => 'Tags',
      'machine_name' => 'tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
