<?php
// $Id:

function stormperson_views_data() {
  $data['stormperson']['table']['group']  = 'Storm';
  $data['stormperson']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormperson']['organization_nid'] = array(
    'title' => t('Person Organization NID'),
    'help' => 'Storm Person Organization NID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['organization_title'] = array(
    'title' => t('Person Organization Title'),
    'help' => 'Storm Person Organization Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['prefix'] = array(
    'title' => t('Person Prefix'),
    'help' => 'Storm Person Prefix',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['fullname'] = array(
    'title' => t('Person Fullname'),
    'help' => 'Storm Person Fullname',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['email'] = array(
    'title' => t('Person Email'),
    'help' => 'Storm Person Email',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['www'] = array(
    'title' => t('Person WWW'),
    'help' => 'Storm Person WWW',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['phone'] = array(
    'title' => t('Person Phone'),
    'help' => 'Storm Person Phone',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['im'] = array(
    'title' => t('Person IM'),
    'help' => 'Storm Person IM',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormperson']['user_uid'] = array(
    'title' => t('Person Drupal User ID'),
    'help' => 'Storm Person Drupal User ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

  function stormperson_views_handlers() {
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
