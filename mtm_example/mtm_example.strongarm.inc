<?php
/**
 * @file
 * mtm_example.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function mtm_example_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'additional_settings__active_tab_mtm_example_content_type';
  $strongarm->value = 'edit-menu';
  $export['additional_settings__active_tab_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_mtm_example_content_type';
  $strongarm->value = 0;
  $export['comment_anonymous_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_mtm_example_content_type';
  $strongarm->value = 0;
  $export['comment_default_mode_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_mtm_example_content_type';
  $strongarm->value = '50';
  $export['comment_default_per_page_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_mtm_example_content_type';
  $strongarm->value = 0;
  $export['comment_form_location_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_mtm_example_content_type';
  $strongarm->value = '0';
  $export['comment_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_mtm_example_content_type';
  $strongarm->value = '0';
  $export['comment_preview_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_mtm_example_content_type';
  $strongarm->value = 0;
  $export['comment_subject_field_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__mtm_example_content_type';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_mtm_example_content_type';
  $strongarm->value = array();
  $export['menu_options_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_mtm_example_content_type';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_mtm_example_content_type';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_mtm_example_content_type';
  $strongarm->value = '0';
  $export['node_preview_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_mtm_example_content_type';
  $strongarm->value = 0;
  $export['node_submitted_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'publishcontent_mtm_example_content_type';
  $strongarm->value = 0;
  $export['publishcontent_mtm_example_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'save_continue_mtm_example_content_type';
  $strongarm->value = 'Save and add fields';
  $export['save_continue_mtm_example_content_type'] = $strongarm;

  return $export;
}
