<?php
/**
 * @file
 * tal_recommendation.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function tal_recommendation_taxonomy_default_vocabularies() {
  return array(
    'collection_type' => array(
      'name' => 'Collection Type',
      'machine_name' => 'collection_type',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
