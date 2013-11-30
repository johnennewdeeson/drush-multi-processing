<?php

/**
 * @file
 * Migration classes.
 */

/**
 * My Migration base class.
 */
class MyMigrate extends Migration {

  /**
   * Constructor.
   *
   * @param array $args
   *   Arguments to the constructor.
   *   This migration supports 2 additional arguments to normal
   *   'limit' int is the total number of migration items to retrieve
   *   'offset' int is the number into the list of items to retrieve
   */
  public function __construct(array $args = array()) {
    parent::__construct($args);

    $this->description = t('MyMigrate migration');

    $this->team = array(
      new MigrateTeamMember('ceng', 'johne@deeson.co.uk', t('Cheif Engineer')),
    );

    $this->map = new MigrateSQLMap('mymigrate',
      array(
        'reference' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Unique content reference',
          'alias' => 'o',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $fields = array(
      'reference' => 'Unique content id',
      'valuea' => 'Some content',
      'valueb' => 'Some more content',
    );

    // Get the limit and offset values from the arguments.
    $limit = empty($args['limit']) ? 100 : $args['limit'];
    $offset = empty($args['offset']) ? 0 : $args['offset'];
    $source_url = url('mymigrate/source', array('absolute' => TRUE));

    // We create our own source handler capable of
    // returning a subset of results.
    $list_class = new MyMigrateListJSON($source_url, $limit, $offset);
    $item_class = new MigrateItemJSON($source_url . '/:id');
    $this->source = new MigrateSourceList($list_class, $item_class, $fields);

    $this->destination = new MigrateDestinationNode('mymigrate_content_type');

    $this->addFieldMapping('field_mymigrate_reference', 'reference');
    $this->addFieldMapping('title', 'valuea');
    $this->addFieldMapping('field_mymigrate_valueb', 'valueb');

    $this->addUnmigratedDestinations(array('created', 'changed', 'promote', 'status', 'revision', 'language'));
  }
}

/**
 * Define our own source handler which can deal with limits and offsets.
 *
 * For the purposes of the example, the source is the site itself
 * which has a generate function which returns random JSON encoded data.
 */
class MyMigrateListJSON extends MigrateListJSON {

  /**
   * Constructor.
   */
  public function __construct($source_url, $limit = 100, $offset = 0) {
    $query = array(
      'limit' => $limit,
      'offset' => $offset,
    );

    $list_url = url($source_url, array('query' => $query));

    parent::__construct($list_url, array());
  }

}
