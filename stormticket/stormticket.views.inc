<?php
// $Id:

function stormticket_views_data() {
  $data['stormticket']['table']['group']  = 'Storm';
  $data['stormticket']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormticket']['ticketstatus'] = array(
    'title' => t('Ticket Status'),
    'help' => 'Storm Ticket status',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Ticket Status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['ticketcategory'] = array(
    'title' => t('Ticket Category'),
    'help' => t('Storm Ticket category'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Ticket Category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['ticketpriority'] = array(
    'title' => t('Ticket Priority'),
    'help' => t('Storm Ticket priority'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Ticket Priority',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['datebegin'] = array(
    'title' => t('Ticket Date Begin'),
    'help' => t('Storm Ticket begin date'),
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

  $data['stormticket']['dateend'] = array(
    'title' => t('Ticket Date End'),
    'help' => t('Storm Ticket end date'),
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

  $data['stormticket']['organization_title'] = array(
    'title' => t('Ticket Organization'),
    'help' => t('Storm Ticket organization'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['project_title'] = array(
    'title' => t('Ticket Project'),
    'help' => t('Storm Ticket project'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['task_title'] = array(
    'title' => t('Ticket Task'),
    'help' => t('Storm Task title'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

function stormticket_views_handlers() {
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
