<?php
// $Id:

function stormnote_views_data() {
  $data['stormnote']['table']['group']  = 'Storm';
  $data['stormnote']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormnote']['organization_nid'] = array(
    'title' => t('Note Organization Node ID'),
    'help' => 'Storm Note Organization Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormnote']['organization_title'] = array(
    'title' => t('Note Organization Title'),
    'help' => 'Storm Note Organization Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormnote']['project_nid'] = array(
    'title' => t('Note Project Node ID'),
    'help' => 'Storm Note Project Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormnote']['project_title'] = array(
    'title' => t('Note Project Title'),
    'help' => 'Storm Note Project Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormnote']['task_nid'] = array(
    'title' => t('Note Task Node ID'),
    'help' => 'Storm Note Task Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormnote']['task_stepno'] = array(
    'title' => t('Note Task Step Number'),
    'help' => 'Storm Note Task Step Number',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormnote']['task_title'] = array(
    'title' => t('Note Task Title'),
    'help' => 'Storm Note Task Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

function stormnote_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'stormattribute'),
    ),
    'handlers' => array(
      'stormattribute_handler_filter_attributes_domain' => array(
        'parent' => 'views_handler_filter_in_operator',
    ),
    ),
  );
}
