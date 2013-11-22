<?php

/**
 * @file
 * A multi threading Drupal Queue.
 */

/**
 * Implements hook_drush_command().
 */
function mtq_drush_command() {
  $items = array();

  $items['mtq-process'] = array(
    'description' => 'A command to process items on the queue.',
    'options' => array(
      'threads' => 'The number of threads to use',
      'limit' => 'The total number of jobs to put on the queue',
      'batch_size' => 'How many items from the queue to process in each thread. Default is 10.',
    ),
  );

  $items['mtq-consumer'] = array(
    'description' => 'A single consumer process.',
    'arguments' => array(
      'limit' => 'The number of jobs to process on the queue',
    ),
  );

  return $items;
}

/**
 * Process the queue, multi threader.
 */
function drush_mtq_process() {

  $time = time();
  $threads = drush_get_option('threads', 1);
  $limit = drush_get_option('limit', 100);
  $batch_size = drush_get_option('batch_size', 10);

  _mtq_populate_queue($limit);

  $queue = DrupalQueue::get('mtq', TRUE);
  $queue_size = $queue->numberOfItems();

  try {
    drush_print("Going to work on {$queue_size} items from the queue with {$threads} threads...");
    drush_thread_manager($queue_size, $batch_size, $threads, '_mtq_setup');
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }

  $time = time() - $time;
  drush_print("Time taken with {$threads} threads: {$time} seconds");
}

/**
 * Consume items from the queue.
 *
 * @param int $limit
 *   The maximum number of items to consume.
 */
function drush_mtq_consumer($limit = 10) {
  $queue = DrupalQueue::get('mtq', TRUE);

  for ($count = 0; $count < $limit; $count++) {

    if ($item = $queue->claimItem()) {
      $transaction = db_transaction(__FUNCTION__);

      try {
        // Do something with the item.
        _mtq_process_item($item->data);
      }
      catch (Exception $e) {
        $transaction->rollback();
        drush_set_error($e->getMessage());
        return;
      }

      // Unset the transaction to force a commit.
      unset($transaction);
      $queue->deleteItem($item);
    }

  }
}

/**
 * Feed the queue with dummy items.
 *
 * You will have your own code which populates the
 * queue. This is here as an example.
 */
function _mtq_populate_queue($limit) {
  $queue = DrupalQueue::get('mtq', TRUE);
  $queue_size = $queue->numberOfItems();

  $limit = $limit - $queue_size;

  if ($limit > 0) {

    for ($i = 0; $i < $limit; $i++) {
      $record = array(
        'id' => $i,
      );

      $queue->createItem($record);
    }

  }

  drush_print("added $limit items to mtq");
  drush_print("mtq has $queue_size items to process");
}

/**
 * A test multi threaded setup function.
 *
 * @param int $thread_id
 *   An identifier for the thread which will execute this command.
 * @param int $batch_size
 *   How many tasks this command should work on.
 * @param int $offset
 *   The position in a queue of jobs for the first job this command
 *   should work on.
 *
 * @return string
 *   A command which can be executed on the command line.
 */
function _mtq_setup($thread_id, $batch_size, $offset) {
  return "drush mtq-consumer $batch_size";
}

/**
 * Process an item on the queue.
 *
 * The rest of this code is boiler plate for this
 * function which is actually doing something useful.
 * Replace this with your own processing code.
 */
function _mtq_process_item($data) {
  drush_print("Processing {$data['id']}");
  usleep(rand(100000, 1000000));
}
