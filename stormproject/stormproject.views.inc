<?php
// $Id:

function stormproject_views_data() {
  $data['stormproject']['table']['group']  = 'Storm';
  $data['stormproject']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  
  $data['stormproject']['organization_nid'] = array(
    'title' => t('Project Node ID'),
    'help' => 'Storm Project Node ID',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['organization_title'] = array(
    'title' => t('Project Organization Title'),
    'help' => 'Storm Project Organization Title',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['projectstatus'] = array(
    'title' => t('Project Status'),
    'help' => 'Storm Project Status',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Project Status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['projectcategory'] = array(
    'title' => t('Project Category'),
    'help' => 'Storm Project Category',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Project Category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['projectpriority'] = array(
    'title' => t('Project Priority'),
    'help' => 'Storm Project Priority',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'stormattribute_handler_filter_attributes_domain',
      'domain' => 'Project Priority',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['pricemode'] = array(
    'title' => t('Project Price Mode'),
    'help' => 'Storm Project Price Mode',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['price'] = array(
    'title' => t('Project Price'),
    'help' => 'Storm Project Price',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['currency'] = array(
    'title' => t('Project Currency'),
    'help' => 'Storm Project Currency',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['datebegin'] = array(
    'title' => t('Project Date Begin'),
    'help' => 'Storm Project Date Begin',
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
  
  $data['stormproject']['dateend'] = array(
    'title' => t('Project Date End'),
    'help' => 'Storm Project Date End',
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
  
  $data['stormproject']['duration unit'] = array(
    'title' => t('Project Duration Unit'),
    'help' => 'Storm Project Duration Unit',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['stormproject']['duration'] = array(
    'title' => t('Project Duration'),
    'help' => 'Storm Project Duration',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

function stormproject_views_handlers() {
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


function stormproject_date_api_fields($field) {
  if ($field == 'stormproject.datebegin') {
    return array(
    // The type of date: DATE_UNIX, DATE_ISO, DATE_DATETIME.
    'sql_type' => DATE_UNIX, 
    // Timezone handling options: 'none', 'site', 'date', 'utc'.
    'tz_handling' => 'site',
    // Needed only for dates that use 'date' tz_handling.
    'timezone_field' => '', 
    // Needed only for dates that use 'date' tz_handling.
    'offset_field' => '', 
    // Array of "table.field" values for related fields that should be 
    // loaded automatically in the Views SQL.
    'related_fields' => array(),
    // Granularity of this date field's db data. 
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
   );
  }
}


function stormproject_date_api_tables() {
  return array('stormproject');
}