<?php
// $Id:

function stormbudget_views_data() {
  $data['stormbudget']['table']['group']  = 'Storm';
  $data['stormbudget']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormbudget']['stepno'] = array(
    'title' => t('Budget item Step Number'),
    'help' => 'Storm Budget item Step Number',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormbudget']['organization_nid'] = array(
    'title' => t('Budget Organization Node ID'),
    'help' => 'Storm Budget Organization Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormbudget']['organization_title'] = array(
    'title' => t('Budget Organization Title'),
    'help' => 'Storm Budget Organization Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormbudget']['project_nid'] = array(
    'title' => t('Budget Project Node ID'),
    'help' => 'Storm Budget Project Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['project_title'] = array(
    'title' => t('Budget Project Title'),
    'help' => 'Storm Budget Project Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['parent_nid'] = array(
    'title' => t('Budget item Parent Budget item Node ID'),
    'help' => 'Storm Budget Patent Budget item Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['weight'] = array(
    'title' => t('Budget Weight'),
    'help' => 'Storm item Weight',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['budgetitemstatus'] = array(
    'title' => t('Budget item Status'),
    'help' => 'Storm Budget item Status',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Budget Status',
    ),
  );

  $data['stormbudget']['budgetitemcategory'] = array(
    'title' => t('Budget item Category'),
    'help' => 'Storm Budget item Category',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Budget Category',
    ),
  );

  $data['stormbudget']['budgetitempriority'] = array(
    'title' => t('Budget item Priority'),
    'help' => 'Storm Budget item Priority',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Budget item Priority',
    ),
  );

  $data['stormbudget']['pricemode'] = array(
    'title' => t('Budget item Price Mode'),
    'help' => 'Storm Budget item Price Mode',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['price'] = array(
    'title' => t('Budget item Price'),
    'help' => 'Storm Budget item Price',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['currency'] = array(
    'title' => t('Budget item Currency'),
    'help' => 'Storm Budget item Currency',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['datebegin'] = array(
    'title' => t('Budget item Date Begin'),
    'help' => 'Storm Budget item Date Begin',
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

  $data['stormbudget']['dateend'] = array(
    'title' => t('Budget item Date End'),
    'help' => 'Storm Budget item Date End',
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

  $data['stormbudget']['durationunit'] = array(
    'title' => t('Budget item Duration Unit'),
    'help' => 'Storm Budget item Duration Unit',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormbudget']['duration'] = array(
    'title' => t('Budget item Duration'),
    'help' => 'Storm Budget item Duration',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

function stormbudget_views_handlers() {
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

/**
 * Implementation of hook_date_api_fields().
 * on behalf of core fields.
 * 
 * All modules that create custom fields that use the 
 * 'views_handler_field_date' handler can provide
 * additional information here about the type of
 * date they create so the date can be used by
 * the Date API views date argument and date filter.
 */
function stormbudget_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX, 
    'tz_handling' => 'site',
    'timezone_field' => '', 
    'offset_field' => '', 
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
  );

  switch ($field) {
    case 'stormbudget.datebegin':
    case 'stormbudget.dateend':
      return $values;
  }

}

/**
 * Implement hook_date_api_tables().
 */
function stormbudget_date_api_tables() {
  return array('stormbudget');
}