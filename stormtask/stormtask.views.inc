<?php
// $Id:

function stormtask_views_data() {
  $data['stormtask']['table']['group']  = 'Storm';
  $data['stormtask']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormtask']['stepno'] = array(
    'title' => t('Task Step Number'),
    'help' => 'Storm Task Step Number',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormtask']['organization_nid'] = array(
    'title' => t('Task Organization Node ID'),
    'help' => 'Storm Task Organization Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormtask']['organization_title'] = array(
    'title' => t('Task Organization Title'),
    'help' => 'Storm Task Organization Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormtask']['project_nid'] = array(
    'title' => t('Task Project Node ID'),
    'help' => 'Storm Task Project Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['project_title'] = array(
    'title' => t('Task Project Title'),
    'help' => 'Storm Task Project Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['parent_nid'] = array(
    'title' => t('Task Parent Task Node ID'),
    'help' => 'Storm Task Patent Task Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['weight'] = array(
    'title' => t('Task Weight'),
    'help' => 'Storm Task Weight',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['taskstatus'] = array(
    'title' => t('Task Status'),
    'help' => 'Storm Task Status',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Task Status',
    ),
  );

  $data['stormtask']['taskcategory'] = array(
    'title' => t('Task Category'),
    'help' => 'Storm Task Category',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Task Category',
    ),
  );

  $data['stormtask']['taskpriority'] = array(
    'title' => t('Task Priority'),
    'help' => 'Storm Task Priority',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Task Priority',
    ),
  );

  $data['stormtask']['pricemode'] = array(
    'title' => t('Task Price Mode'),
    'help' => 'Storm Task Price Mode',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['price'] = array(
    'title' => t('Task Price'),
    'help' => 'Storm Task Price',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['currency'] = array(
    'title' => t('Task Currency'),
    'help' => 'Storm Task Currency',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['datebegin'] = array(
    'title' => t('Task Date Begin'),
    'help' => 'Storm Task Date Begin',
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['stormtask']['dateend'] = array(
    'title' => t('Task Date End'),
    'help' => 'Storm Task Date End',
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['stormtask']['durationunit'] = array(
    'title' => t('Task Duration Unit'),
    'help' => 'Storm Task Duration Unit',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtask']['duration'] = array(
    'title' => t('Task Duration'),
    'help' => 'Storm Task Duration',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

function stormtask_views_handlers() {
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
