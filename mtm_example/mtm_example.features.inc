<?php
/**
 * @file
 * mtm_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mtm_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function mtm_example_node_info() {
  $items = array(
    'mtm_example_content_type' => array(
      'name' => t('mtm_example Content Type'),
      'base' => 'node_content',
      'description' => t('A content type for use with the mtm_example example migration.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
