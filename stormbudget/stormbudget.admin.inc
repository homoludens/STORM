<?php
// $Id: stormbudget.admin.inc,v 1.8.4.16 2009/04/16 19:15:29 magnity Exp $

/**
 * @file
 */

function stormbudget_budgetitems($project) {
  $node_invoice = node_load($invoice->nid);
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  $breadcrumb[] = l(t('Projects'), 'storm/projects');
  $breadcrumb[] = l(t('Project : '. $project->title), 'node/'. $project->nid);
  drupal_set_breadcrumb($breadcrumb);

  $i = new stdClass();
  $i->type = 'stormbudget';

  $o = drupal_get_form('stormbudget_budgetitems_filter');

  $params = $_GET;
  $params['organization_nid'] = $project->organization_nid;
  $params['project_nid'] = $project->nid;

  $header = array(
    array(
      'data' => t('Title'),
    ),
    array(
      'data' => t('Category'),
    ),
    array(
      'data' => t('Status'),
    ),
    array(
      'data' => t('Priority'),
    ),
    array(
      'data' => t('Price'),
    ),
//     array(
//       'data' => t('Parent'),
//     ),
//     array(
//       'data' => t('Weight'),
//     ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  if ($_SESSION['stormbudget_budgetitems_filter']['budgetcategory'] && $_SESSION['stormbudget_budgetitems_filter']['budgetcategory'] != '-') {
    $category = split(',', $_SESSION['stormbudget_budgetitemss_filter']['budgetcategory']);
    $v = array();
    foreach ($category as $item) $v[] = '%s';
    $where[] = "sta.budgetcategory IN ('". implode("','",$v) ."')";
    foreach ($category as $key => $value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormbudget_budgetitems_filter']['budgetstatus'] && $_SESSION['stormbudget_budgetitems_filter']['budgetitemstatus'] != '-') {
    $status = split(',', $_SESSION['stormbudget_budgetitems_filter']['budgetstatus']);
    $v = array();
    foreach ($status as $item) $v[] = '%s';
    $where[] = "sta.budgetstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key => $value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormbudget_budgetitems_filter']['budgetpriority'] && $_SESSION['stormbudget_budgetitems_filter']['budgetpriority'] != '-') {
    $priority = split(',', $_SESSION['stormbudget_budgetitems_filter']['budgetpriority']);
    $v = array();
    foreach ($priority as $item) $v[] = '%s';
    $where[] = "sta.budgetpriority IN ('". implode("','",$v) ."')";
    foreach ($priority as $key => $value) {
      $args[] = $value;
    }
  }

  $budgetitemstree = _stormbudget_get_tree($project->nid, 0, -1, NULL, $where, $args);
  $o .= drupal_get_form('stormbudget_budgetitems_form', $header, $budgetitemstree, $project);
  return $o;
}

function stormbudget_budgetitems_form($form_id, $header, $budgetitems, $project) {
  $form = array();

  $form['budgetitems'] = array(
    '#theme' => 'stormbudget_budgetitems',
  );

  $form['budgetitems']['header'] = array(
    '#value' => $header,
  );

  $tree = _stormbudget_get_tree($project->nid);
  $parent_budgetitems = _stormbudget_plain_tree($tree);

  $params = $_GET;
  $params['project_nid'] = $project->nid;

  foreach ($budgetitems as $budgetitem) {
    $n = new stdClass();
    $n->nid = $budgetitem->nid;
    $n->uid = $budgetitem->uid;
    $n->type = 'stormbudget';

    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_depth_'. $budgetitem->nid] = array(
      '#value' => $budgetitem->depth,
    );

    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_description_'. $budgetitem->nid] = array(
      '#value' => l($budgetitem->stepno .' '. $budgetitem->title, 'node/'. $budgetitem->nid, array('html' => TRUE)),
    );

    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_category_'. $budgetitem->nid] = array(
      '#value' => storm_icon('category_'. $budgetitem->budgetcategory, $budgetitem->budgetcategory),
    );

    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_status_'. $budgetitem->nid] = array(
      '#value' => storm_icon('status_'. $budgetitem->budgetstatus, $budgetitem->budgetstatus),
    );

    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_priority_'. $budgetitem->nid] = array(
      '#value' => storm_icon('priority_'. $budgetitem->budgetpriority, $budgetitem->budgetpriority),
    );

    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_price_'. $budgetitem->nid] = array(
      '#value' => $budgetitem->price,
    );

// /*    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_parent-nid_'. $budgetitem->nid] = array(
//       '#type' => 'hidden',
//       '#options' => array(0 => '-') + $parent_budgetitems,
//       '#default_value' => $budgetitem->parent_nid,
//     );*/

//     $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_nid_'. $budgetitem->nid] = array(
//       '#type' => 'hidden',
//       '#default_value' => $budgetitem->nid,
//     );

//     $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_weight_'. $budgetitem->nid] = array(
//       '#type' => 'hidden',
//       '#default_value' => $budgetitem->weight,
//     );

    $v = storm_icon_edit_node($n, $params);
    $v .= '&nbsp;'. storm_icon_delete_node($n, $params);
    $params['parent_nid'] = $budgetitem->nid;
    $v .= '&nbsp;'. storm_icon_add_node($n, $params);
    $form['budgetitems']['budgetitems'][$budgetitem->nid]['budgetitem_operations_'. $budgetitem->nid] = array(
      '#value' => $v,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('stormbudget_budgetitems_submit'),
    '#value' => t('Save'),
  );

  return $form;
}

function stormbudget_budgetitems_submit($form, &$form_state) {
  $budgetitems = array();
  foreach ($form_state['values'] as $key => $value) {
    $ar = explode('_', $key);
    if ($ar[0]=='budgetitem') {
      if ($ar[1]=='weight') $budgetitems[$ar[2]]['weight'] = $value;
      if ($ar[1]=='parent-nid') $budgetitems[$ar[2]]['parent-nid'] = $value;
    }
  }

  $s = "UPDATE {stormbudget} SET weight=%d, parent_nid=%d WHERE nid=%d";  
  foreach ($budgetitems as $nid=>$values) {
    db_query($s, $values['weight'], $values['parent-nid'], $nid);
  }

  drupal_set_message(t('Budgetitems saved'));
}

function stormbudget_budgetitems_filter() {
  $category_list = stormattribute_attributes_bydomain('Budget item category search');
  $budgetitemcategory = $_SESSION['stormbudget_budgetitems_filter']['budgetcategory'];
  if (!$budgetitemcategory) {
    $budgetitemcategory = $category_list['default'];
    $_SESSION['stormbudget_budgetitems_filter']['budgetitemcategory'] = $budgetitemcategory;
  }

  $status_list = stormattribute_attributes_bydomain('Budget item status search');
  $budgetitemstatus = $_SESSION['stormbudget_budgetitems_filter']['budgetstatus'];
  if (!$budgetitemstatus) {
    $budgetitemstatus = $status_list['default'];
    $_SESSION['stormtbudget_budgetitems_filter']['budgetitemstatus'] = $budgetitemstatus;
  }

  $priority_list = stormattribute_attributes_bydomain('Budget item priority search');
  $budgetitempriority = $_SESSION['stormbudget_budgetitems_filter']['budgetpriority'];
  if (!$budgetitempriority) {
    $budgetitempriority = $priority_list['default'];
    $_SESSION['stormbudget_budgetitems_filter']['budgetitempriority'] = $budgetitempriority;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['budgetitemcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $budgetitemcategory,
    '#options' => $category_list['values'],
  );

  $form['filter']['group1']['budgetitemstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $budgetitemstatus,
    '#options' => $status_list['values'],
  );

  $form['filter']['group1']['budgetitempriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $budgetitempriority,
    '#options' => $priority_list['values'],
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormbudget_budgetitems_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormbudget_budgetitems_filter_reset'),
  );

  return $form;
}

function stormbudget_budgetitems_filter_filter($form, &$form_state) {
  $_SESSION['stormbudget_budgetitems_filter']['budgetitemcategory'] = $form_state['values']['budgetitemcategory'];
  $_SESSION['stormbudget_budgetitems_filter']['budgetitemstatus'] = $form_state['values']['budgetitemstatus'];
  $_SESSION['stormbudget_budgetitems_filter']['budgetitempriority'] = $form_state['values']['budgetitempriority'];
}

function stormbudget_budgetitems_filter_reset($form, &$form_state) {
  unset($_SESSION['stormbudget_budgetitems_filter']);
}

function _stormbudget_project_budgetitems_js($project_nid) {
  $tree = _stormbudget_get_tree($project_nid);
  $budgetitems = _stormbudget_plain_tree($tree);
  print drupal_to_js($budgetitems);
  exit();
}

function stormbudget_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormbudget_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormbudget_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormbudget_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormbudget_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormbudget_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormbudget_list_filter']['organization_nid'] = $p->organization_nid;

  }

  $o = drupal_get_form('stormbudget_list_filter');

  $i = new stdClass();
  $i->type = 'stormbudget';

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'sta.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sta.project_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Category'),
      'field' => 'sta.budgetcategory',
    ),
    array(
      'data' => t('Status'),
      'field' => 'sta.budgetstatus',
    ),
    array(
      'data' => t('Priority'),
      'field' => 'sta.budgetpriority',
    ),
    array(
      'data' => t('Price'),
      'field' => 'sta.price',
    ),    

    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  $s  = "SELECT n.*, sta.* FROM {node} AS n INNER JOIN {stormbudget} AS sta ON n.vid=sta.vid WHERE n.status=1 AND n.type='stormbudget' ";
  if ($_SESSION['stormbudgetitem_list_filter']['organization_nid']) {
    $where[] = 'sta.organization_nid=%d';
    $args[] = $_SESSION['stormbudget_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormbudgetitem_list_filter']['project_nid']) {
    $where[] = 'sta.project_nid=%d';
    $args[] = $_SESSION['stormbudget_list_filter']['project_nid'];
  }

  if ($_SESSION['stormbudget_list_filter']['budgetitemcategory'] && $_SESSION['stormbudget_list_filter']['budgetitemcategory'] != '-') {
    $category = split(',', $_SESSION['stormbudget_list_filter']['budgetitemcategory']);
    $v = array();
    foreach ($category as $item) $v[] = '%s';
    $where[] = "sta.budgetitemcategory IN ('". implode("','",$v) ."')";
    foreach ($category as $key => $value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormbudget_list_filter']['budgetitemstatus'] && $_SESSION['stormbudget_list_filter']['budgetitemstatus'] != '-') {
    $status = split(',', $_SESSION['stormbudget_list_filter']['budgetitemstatus']);
    $v = array();
    foreach ($status as $item) $v[] = '%s';
    $where[] = "sta.budgetitemstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key => $value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormbudget_list_filter']['budgetitempriority'] && $_SESSION['stormbudget_list_filter']['budgetitempriority'] != '-') {
    $priority = split(',', $_SESSION['stormbudget_list_filter']['budgetitempriority']);
    $v = array();
    foreach ($priority as $item) $v[] = '%s';
    $where[] = "sta.budgetitempriority IN ('". implode("','",$v) ."')";
    foreach ($priority as $key => $value) {
      $args[] = $value;
    }
  }

  $itemsperpage = $_SESSION['stormbudget_list_filter']['itemsperpage'];

  $s = stormbudget_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $budgetitems = array();
  while ($item = db_fetch_object($r)) {
    $budgetitems[] = $item;
  }

  $o .= theme('stormbudget_list', $header, $budgetitems);
  $o .= theme('pager', NULL, $itemperpage, 0);
  print theme('page', $o);
}

function stormbudget_list_filter() {
  $category_list = stormattribute_attributes_bydomain('Budget item category search');
  $budgetitemcategory = $_SESSION['stormbudget_list_filter']['budgetitemcategory'];
  if (!$budgetitemcategory) {
    $budgetitemcategory = $category_list['default'];
    $_SESSION['stormbudget_list_filter']['budgetitemcategory'] = $budgetitemcategory;
  }

  $status_list = stormattribute_attributes_bydomain('Budget item status search');
  $budgetitemstatus = $_SESSION['stormbudget_list_filter']['budgetitemstatus'];
  if (!$budgetitemstatus) {
    $budgetitemstatus = $status_list['default'];
    $_SESSION['stormbudget_list_filter']['budgetitemstatus'] = $budgetitemstatus;
  }

  $priority_list = stormattribute_attributes_bydomain('Budget item priority search');
  $budgetitempriority = $_SESSION['stormbudget_list_filter']['budgetitempriority'];
  if (!$budgetitempriority) {
    $budgetitempriority = $priority_list['default'];
    $_SESSION['stormbudget_list_filter']['budgetitempriority'] = $budgetitempriority;
  }

  $organization_nid = $_SESSION['stormbudget_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormbudget_list_filter']['project_nid'];

 $itemsperpage = $_SESSION['stormbudget_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormbudget_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => t('All')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormproject_organization_projects(this, 'edit-project-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormproject} AS spr ON spr.vid=n.vid
  WHERE n.status=1 AND spr.organization_nid=%d AND n.type='stormproject' ORDER BY n.title";
  $s = stormproject_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid);
  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[$project->nid] = $project->title;
  }
  $projects = array(0 => t('All')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['budgetitemcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $budgetitemcategory,
    '#options' => $category_list['values'],
  );

  $form['filter']['group2']['budgetitemstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $budgetitemstatus,
    '#options' => $status_list['values'],
  );

  $form['filter']['group2']['budgetitempriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $budgetitempriority,
    '#options' => $priority_list['values'],
  );

  $form['filter']['group6'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group6']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormbudget_list_filter_filter'),
  );

  $form['filter']['group6']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormbudget_list_filter_reset'),
  );

  $form['filter']['group6']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormbudget_list_filter_filter($form, &$form_state) {
  $_SESSION['stormbudget_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormbudget_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormbudget_list_filter']['budgetitemcategory'] = $form_state['values']['budgetitemcategory'];
  $_SESSION['stormbudget_list_filter']['budgetitemstatus'] = $form_state['values']['budgetitemstatus'];
  $_SESSION['stormbudget_list_filter']['budgetitempriority'] = $form_state['values']['budgetitempriority'];
  $_SESSION['stormbudget_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormbudget_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormbudget_list_filter']);
}