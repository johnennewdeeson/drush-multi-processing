<?php

/**
 * @file
 * An example of making migrate faster using the multi
 * threaded handler for drush.
 */

/**
 * Implements of hook_drush_command().
 */
function mtm_drush_command() {
  $items = array();

  $items['mtm-migrate-import'] = array(
    'description' => 'Multithreaded migration',
    'arguments' => array(
      'class' => 'The name of the migration base class',
      'limit' => 'Total number of jobs to migrate - use 0 for all.',
      'batch_size' => 'Number of jobs each thread will work on.',
      'threads' => 'Number of threads',
    ),
    'options' => array(
      'offset' => 'A starting offset should you want to start 1000 records in',
    ),
  );

  $items['mtm-migrate-cleanup'] = array(
    'description' => 'Clean up old mt migrations',
  );

  return $items;
}

/**
 * Multi threaded import.
 */
function drush_mtm_migrate_import($class, $limit = 10, $batch_size = 10, $threads = 1) {
  try {
    $GLOBALS['mtm-migrate-baseclass'] = $class;
    $GLOBALS['mtm-migrate-runtime'] = time();
    $starting_offset = drush_get_option('offset', 0);
    drush_thread_manager($limit, $batch_size, $threads, '_mtm_thread_setup', '_mtm_thread_teardown', $starting_offset);
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * Create a sub migration and the drush command to execute it.
 *
 * @param int $thread_id
 *   A thread identifier which is managing this migration.
 * @param int $limit
 *   Total number of items to migrate with this class.
 * @param int $offset
 *   Offset to start the migration at.
 *
 * @return string
 *   A command to run.
 */
function _mtm_thread_setup($thread_id, $limit, $offset) {
  $class_name = $GLOBALS['mtm-migrate-baseclass'];
  $run_time = $GLOBALS['mtm-migrate-runtime'];
  $machine_name = _mtm_migrate_generate_machine_name($class_name, $thread_id, $run_time);

  MigrationBase::registerMigration(
    $class_name,
    $machine_name,
    array(
      'limit' => $limit,
      'offset' => $offset,
      'machine_name' => $machine_name,
    )
  );

  $site_record = drush_sitealias_get_record('@self');
  $drush_command_path = drush_build_drush_command();
  $command_options = _drush_backend_get_global_contexts($site_record);
  $command_options[] = '--update';
  $command = 'migrate-import';
  $args = array($machine_name);
  $cmd = _drush_backend_generate_command($site_record, $drush_command_path . " " . $command, $args, $command_options) . ' 2>&1';

  return $cmd;
}

/**
 * Get the machine name for a migration based off the thread id.
 *
 * @return string
 *   The machine name for the thread based on the current run
 *   time, thread_id and name of the base migration.
 */
function _mtm_migrate_generate_machine_name($class_name, $thread_id, $run_time) {
  return $class_name . '_' . $thread_id . '_' . $run_time;
}

/**
 * Teardown function when a thread finishes.
 *
 * @param int $thread_id
 *   The id of the thread.
 */
function _mtm_thread_teardown($thread_id) {
  $class_name = $GLOBALS['mtm-migrate-baseclass'];
  $run_time = $GLOBALS['mtm-migrate-runtime'];
  $machine_name = _mtm_migrate_generate_machine_name($class_name, $thread_id, $run_time);
  _mtm_migrate_destroy_chuncked_migrations(array($machine_name => TRUE));
}

/**
 * Deregister a list of migrations at the end of the process.
 *
 * @param array $migrations
 *   An array of migration objects to be deregistered.
 */
function _mtm_migrate_destroy_chuncked_migrations($migrations) {
  foreach ($migrations as $machine_name => $migration) {
    try {
      MigrationBase::deregisterMigration($machine_name);
    }
    catch (Exception $e) {
      drush_set_error($e->getMessage());
    }
  }
}

/**
 * Remove old dynamically created migrations.
 */
function drush_mtm_mt_migrate_cleanup() {
  $removals = array();
  $migration_objects = migrate_migrations();

  foreach ($migration_objects as $machine_name => $migration) {
    if (preg_match('/[^_]+_\d+_\d+/i', $machine_name)) {
      $removals[$machine_name] = $migration;
    }
  }

  _mtm_migrate_destroy_chuncked_migrations($removals);
}
