<?php

/**
 * @file
 * Multi threading drush.
 */

/**
 * Implements of hook_drush_command().
 */
function mt_drush_command() {
  $items = array();

  $items['mt-test'] = array(
    'description' => 'A test command for testing multi threading.',
    'arguments' => array(
      'limit' => 'The total number of jobs',
    ),
    'options' => array(
      'threads' => 'The number of threads to use',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );

  $items['mt-pause'] = array(
    'description' => 'A test command for use with drush mt-test. Waits between 1 - 5 seconds before finishing',
    'arguments' => array(
      'name' => 'The name of this process, this will be the thread id',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );

  $items['mt-dining-philosophers'] = array(
    'description' => 'Run an example of the dining philosphers problem',
    'arguments' => array(
      'number_of_philosophers' => 'The number of philosophers, must be at least 2',
      'total_mouthfulls' => 'The number of times a philosopher eats before finishing',
    ),
  );

  $items['mt-philosopher'] = array(
    'description' => 'A single philosopher. This is used by the drush command mt-dining-philosophers and is not intended to be run on its own',
    'arguments' => array(
      'name' => 'The name of this process, this will be the thread id',
      'number_of_philosophers' => 'The number of philosophers, must be at least 2',
      'total_mouthfulls' => 'The number of times the philosopher eats before finishing',
    ),
  );

  return $items;
}

/**
 * Multi threaded example application.
 *
 * This is a very simple example of multi threaded management.
 * Each thread will be told to run the drush command:
 * drush mt-pause
 * This waits between 1 and 5 seconds before finishing.
 *
 * @param int $limit
 *   The total number of jobs to process. Each job is an
 *   execution of drush mt-pause
 */
function drush_mt_test($limit = 10) {
  $time = time();
  $threads = drush_get_option('threads', 1);

  try {

    if ($limit < 1 || $threads < 1) {
      throw new Exception('Threads, limit and batch size must be greater than 0!');
    }

    drush_print("Going to work on {$limit} jobs with {$threads} threads...");

    drush_thread_manager($limit, 1, $threads, '_mt_test_setup', '_mt_test_teardown');
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }

  $time = time() - $time;
  drush_print("Time taken with {$threads} threads: {$time} seconds");
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
function _mt_test_setup($thread_id, $batch_size, $offset) {
  return "drush mt-pause $thread_id";
}

/**
 * A test multi threaded teardown function.
 *
 * @param int $thread_id
 *   The thread which just completed.
 */
function _mt_test_teardown($thread_id) {
  // Nothing to do in this example.
}

/**
 * Pause for between 1 and 5 seconds.
 */
function drush_mt_pause($name) {
  $pause_time = rand(1, 5);
  drush_print("Thread $name is working for $pause_time seconds ...");
  sleep($pause_time);
}

/**
 * An implementation of the dining philosophers problem.
 *
 * This is to demonstrate Drupal's locking mechanism and how
 * it can be used with the multi threaded drush script. For
 * information of the dining philosphers see:
 * http://en.wikipedia.org/wiki/Dining_philosophers_problem
 */
function drush_mt_dining_philosophers($number_of_philosophers, $total_mouthfulls) {
  try {

    if ($number_of_philosophers < 2 || $total_mouthfulls < 2) {
      throw new Exception('Number of philosophers and total mouthfulls must be greater than 1');
    }

    $GLOBALS['number_of_philosophers'] = $number_of_philosophers;
    $GLOBALS['total_mouthfulls'] = $total_mouthfulls;

    drush_thread_manager($number_of_philosophers, 1, $number_of_philosophers, '_mt_philosophers_setup');
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * Thread setup function for dining philosophers.
 *
 * For this program we are not using batching or offset.
 * The threads do their work and then die.
 */
function _mt_philosophers_setup($thread_id, $batch_size, $offset) {
  $number_of_philosophers = $GLOBALS['number_of_philosophers'];
  $total_mouthfulls = $GLOBALS['total_mouthfulls'];
  $cmd = "drush mt-philosopher $thread_id $number_of_philosophers $total_mouthfulls";
  return $cmd;
}

/**
 * A single philosopher.
 */
function drush_mt_philosopher($name, $number_of_philosophers, $total_mouthfulls) {
  $last_philosopher = $name == ($number_of_philosophers - 1);
  $low_fork = $last_philosopher ? 0 : "df_fork_{$name}";
  $high_fork = $last_philosopher ? "df_fork_{$name}" : 'df_fork_' . ($name + 1);

  try {
    for ($count = 1; $count <= $total_mouthfulls; $count++) {
      _mt_philosopher_think($name, $count, $total_mouthfulls);
      _mt_philosopher_eat($name, $low_fork, $high_fork, $count, $total_mouthfulls);
    }

    drush_print("Philosopher $name has eaten enough!");
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * A philosopher thinks.
 */
function _mt_philosopher_think($name, $count, $total_mouthfulls) {
  $think_time = rand(1, 3);
  drush_print("Philosopher $name is thinking... ($count/$total_mouthfulls)");
  sleep($think_time);
}

/**
 * Philosopher tries to pick up two forks and eat.
 */
function _mt_philosopher_eat($name, $low_fork, $high_fork, $count, $total_mouthfulls) {
  $eat_time = rand(1, 3);
  _mt_philosopher_get_fork($name, $low_fork);
  _mt_philosopher_get_fork($name, $high_fork);

  drush_print("Philosopher $name is eating... ($count/$total_mouthfulls)");
  sleep($eat_time);

  lock_release($low_fork);
  lock_release($high_fork);
}

/**
 * A philosopher picks up a fork.
 */
function _mt_philosopher_get_fork($name, $fork_name) {
  $waiting = TRUE;
  $count = 0;

  while ($waiting) {
    if (lock_acquire($fork_name)) {
      $waiting = FALSE;
    }
    else {
      $count++;
      if ($count > 30) {
        throw new Exception("Philosopher $name starved to death waiting for fork $fork_name");
      }
      sleep(1);
    }
  }
}

/**
 * A multithreading manager for drush.
 *
 * @param int $job_count
 *   The total number of jobs to process
 * @param int $job_batch_size
 *   The number of jobs handed to each thread
 * @param int $thread_count
 *   The maximum number of active threads
 * @param string $setup_function
 *   The function to generate the thread command. This is written for your
 *   application its signature is:
 *   my_setup_function($thread_id, $limit, $offset) : string;
 *   Where $thread_id is the unique thread identifier, $limit is the total
 *   number of records to process and offset is the starting point. The
 *   function should return a string which is a shell command that the thread
 *   will execute in a separate process.
 * @param string $teardown_function
 *   A function which runs when a thread completes. It's signature is:
 *   my_teardown_function($thread_id);
 *   Your business logic can then run any cleanup which occurs when the
 *   thread completes.
 * @param int $starting_offset
 *   (optional) Start x records into the process.
 */
function drush_thread_manager($job_count, $job_batch_size, $thread_count, $setup_function, $teardown_function = '', $starting_offset = 0) {

  if (!function_exists('proc_open')) {
    throw new Exception('proc_open command is needed to use multi threading.');
  }

  $jobs_remaining = $job_count;
  $processes = array();
  $running = TRUE;
  $offset = $starting_offset;
  $start = time();

  $descriptor_spec = array(
    0 => array("pipe", "r"),
    1 => array("pipe", "w"),
    2 => array("pipe", "w"),
  );

  while ($running) {

    // Fill all empty threads.
    for ($thread_id = 0; $thread_id < $thread_count; $thread_id++) {
      if ($jobs_remaining > 0 && !isset($processes[$thread_id])) {
        $pipes = array();
        $cmd = $setup_function($thread_id, $job_batch_size, $offset);
        $process = proc_open($cmd, $descriptor_spec, $pipes);

        drush_print("Thread {$thread_id} starts at offset $offset. Jobs remaining: $jobs_remaining");

        foreach ($pipes as $pipe) {
          stream_set_blocking($pipe, FALSE);
        }

        $offset += $job_batch_size;
        $jobs_remaining -= $job_batch_size;

        $processes[$thread_id] = array(
          'process' => $process,
          'pipes' => $pipes,
        );

        //sleep(1);
      }
    }

    // Monitor running threads.
    for ($thread_id = 0; $thread_id < $thread_count; $thread_id++) {
      if (!isset($processes[$thread_id])) {
        continue;
      }

      $process_running = _mt_monitor_process($thread_id, $processes[$thread_id]['process'], $processes[$thread_id]['pipes']);

      if (!$process_running) {
        unset($processes[$thread_id]);

        if (!empty($teardown_function)) {
          $teardown_function($thread_id);
        }

        $speed = ($job_count - $jobs_remaining) / (time() - $start);
        $end_date = date('G:i:s d/m/y', ($job_count / $speed) + $start);
        $speed = floor($speed * 60);

        drush_print("Thread {$thread_id} finished. Average speed is {$speed} jobs/minute. Estimated completion at {$end_date}");
      }
    }

    // Check for endgame.
    if ($jobs_remaining < 1 && empty($processes)) {
      $running = FALSE;
    }
    else {
      // Pause the master process.
      sleep(1);
    }
  }
}

/**
 * Monitor a process.
 *
 * This function checks a running process.
 * If it has finished it is closed down safely.
 *
 * @return bool
 *   TRUE if the process is still running
 *   FALSE if it is not running.
 */
function _mt_monitor_process($thread_id, $process, $pipes) {
  $status = proc_get_status($process);

  foreach ($pipes as $id => $pipe) {
    if ($id == 0) {
      // Don't read from stdin!
      continue;
    }

    $messages = stream_get_contents($pipe);

    if (!empty($messages)) {
      foreach (explode("\n", $messages) as $message) {
        $message = str_replace('[error]', '', $message);
        $message = str_replace('[completed]', '', $message);
        $message = trim($message);
        if (!empty($message)) {
          if ($id == 1) {
            // Message from stdout.
            drush_print(" -> T$thread_id: $message");
          }
          else {
            // Message from stderr.
            drush_set_error(" -> T$thread_id: $message");
          }
        }
      }
    }
  }

  if (!$status['running']) {
    foreach ($pipes as $pipe) {
      fclose($pipe);
    }

    proc_close($process);
  }

  return $status['running'];
}
