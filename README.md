Multi threading drush
=====================

This provides a mechanism for creating concurrent applications using Drush for
Drupal 7.

mt.drush.inc is the main multi threading 'engine' which should be placed in
sites/all/drush directory for your site.

mtm.drush.inc is an example of using the engine to speed up Drupal Migrate
migrations

mtq.drush.inc is an example of using the engine to speed up processing the
Drupal Queue.

For instructions on how to use this, see:
http://deeson-online.co.uk/labs/multi-threading-part-1-how-make-drush-rush

For instructions on using this to make Drupal migrate faster, see:
http://deeson-online.co.uk/labs/multi-threading-part-2-how-make-migrate-move

For instructions on using this to make Drupal Queue processing faster, see:
http://deeson-online.co.uk/labs/multi-threading-part-3-jumping-drupal-queue
