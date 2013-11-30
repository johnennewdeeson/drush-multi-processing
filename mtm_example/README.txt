Multi Threaded Migrate Example
==============================

This example module demonstrates a multi threaded migration.

The module itself has no multi threaded components except that
it needs to be able to accept limit and offset arguments into
its constructor which allows the migration to be batched.

The example relies on the features module to create a content
type called mtm_example_content_type which data is migrated
into.

The example provides a hook_menu callback which generates a
list of source data in JSON format that is used as the
source for the migration.

Your site also needs a sites/all/drush folder containing the
files mt.drush.inc and mtm.drush.inc which can be downloaded
from github here:
https://github.com/johnennewdeeson/drush-multi-threading

To run the migration from Drush with one thread use the command:
drush mtm-import MTMExample 10 10 1

To run the migration with two threads try this command:
drush mtm-import MTMExample 10 5 2

Finally, try a larger migration with:
drush mtm-import MTMExample 100 50 2

On a local machine, running past 2 threads will likely cause you
issues. On a good web server 4 to 8 threads is probably ideal.

Details instructions on what is happening can be found here:
http://deeson-online.co.uk/labs/multi-threading-part-2-how-make-migrate-move
