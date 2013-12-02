Concurrent Applications with Drush
==================================

This provides a mechanism for creating concurrent applications using Drush for
Drupal 7. If a process can be split into chucks then this extension allows the
chucks to be processed in parrellel. Each process has been refered to as a
separate thread, though strictly speaking the term thread is incorrect.

This has been tested with Drush 5 and Drupal 7. Later versions of Drush will
probably work but have not been tested.

mt.drush.inc is the main multi threading 'engine' which should be placed in
sites/all/drush directory for your site.

mtm.drush.inc is an example of using the engine to speed up Drupal Migrate
migrations

mtq.drush.inc is an example of using the engine to speed up processing the
Drupal Queue.

For instructions on how to use this, see:
http://deeson-online.co.uk/labs/multi-processing-part-1-how-make-drush-rush

For instructions on using this to make Drupal migrate faster, see:
http://deeson-online.co.uk/labs/multi-processing-part-2-how-make-migrate-move

For instructions on using this to make Drupal Queue processing faster, see:
http://deeson-online.co.uk/labs/multi-processing-part-3-jumping-drupal-queue
