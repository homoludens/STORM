<?php
// $Id: stormexpense.admin.inc,v 1.2.4.7 2009/02/15 09:03:32 robertogerola Exp $

/**
 * @file
 */

function stormexpense_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);
// print_r($_GET);
  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormexpense_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
//     unset($_SESSION['stormexpense_list_filter']['project_nid']);
//     unset($_SESSION['stormexpense_list_filter']['budget_item']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormexpense_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormexpense_list_filter']['organization_nid'] = $p->organization_nid;

//     unset($_SESSION['stormexpense_list_filter']['budget_item']);
  }


  if (array_key_exists('budget_item', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['budget_item'] != $_GET['budget_item']) {
      $_SESSION['stormexpense_list_filter']['budget_item'] = $_GET['budget_item'];
    }
    $ti = node_load($_GET['budget_item']);
    $_SESSION['stormexpense_list_filter']['organization_nid'] = $ti->organization_nid;
    $_SESSION['stormexpense_list_filter']['project_nid'] = $ti->project_nid;
    $_SESSION['stormexpense_list_filter']['budget_item'] = $ti->nid;
  }

  $o = drupal_get_form('stormexpense_list_filter');

  $i = new stdClass();
  $i->type = 'stormexpense';

  $report_attrs = array(
    'onclick' => "storm_popup(this, 'expense_list_form_report', 'Expenses', 300, 150, 'stormexpense_list_form_report', 'l'); return false;",
  );

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'sex.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sex.project_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Date'),
      'field' => 'sex.expensedate',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Total'),
      'style' => 'text-align: right;'
    ),
    array(
      'data' => storm_icon_l('report', '', t('Reports'), '', array(), $report_attrs) .'&nbsp;'. storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.*,  sex.* FROM {node} AS n INNER JOIN {stormexpense} AS sex ON n.vid=sex.vid  INNER JOIN {stormbudget} AS sbg ON sex.budget_item=sbg.nid WHERE n.status=1 AND n.type='stormexpense' ";

  $s_totals = "SELECT SUM(amount) AS t_amount, SUM(tax) AS t_tax, SUM(total) AS t_total FROM
  {node} AS n INNER JOIN {stormexpense} AS sex ON n.vid=sex.vid INNER JOIN {stormbudget} AS sbg ON sex.budget_item=sbg.nid WHERE n.status=1 AND n.type='stormexpense' ";

  $where = array();
  $args = array();

  if ($_SESSION['stormexpense_list_filter']['organization_nid']) {
    $where[] = 'sex.organization_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormexpense_list_filter']['project_nid']) {
    $where[] = 'sex.project_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['project_nid'];
  }
  if ($_SESSION['stormexpense_list_filter']['budget_item']) {
    $where[] = 'sex.budget_item=%d OR sbg.parent_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['budget_item'];
    $args[] = $_SESSION['stormexpense_list_filter']['budget_item'];
  }

  if ($_SESSION['stormexpense_list_filter']['datefrom']) {
    $datefrom = $_SESSION['stormexpense_list_filter']['datefrom'];
    $datefrom['hour'] = 0;
    $datefrom['minute'] = 0;
    $where[] = 'sex.expensedate>=%d';
    $args[] = _storm_datetime_to_gmtimestamp($datefrom);
  }
  if ($_SESSION['stormexpense_list_filter']['dateto']) {
    $dateto = $_SESSION['stormexpense_list_filter']['dateto'];
    $dateto['hour'] = 23;
    $dateto['minute'] = 59;
    $where[] = 'sex.expensedate<=%d';
    $args[] = _storm_datetime_to_gmtimestamp($dateto);
  }
  if ($_SESSION['stormexpense_list_filter']['title']) {
    $where[] = "LOWER(n.title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormexpense_list_filter']['title'];
  }
  if ($_SESSION['stormexpense_list_filter']['provider_title']) {
    $where[] = "LOWER(sex.provider_title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormexpense_list_filter']['provider_title'];
  }

  $itemsperpage = $_SESSION['stormexpense_list_filter']['itemsperpage'];

  if ($_SESSION['stormexpense_list_filter']['user']) {
    $expenseuser = user_load(array('name' => $_SESSION['stormexpense_list_filter']['user']));
    if (!$expenseuser->uid) $expenseuser->uid =  -1;
    $where[] = 'n.uid=%d';
    $args[] = $expenseuser->uid;
  }

  $s = stormexpense_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $_SESSION['stormexpense_list_filter']['sql'] = $s . $tablesort;
  $_SESSION['stormexpense_list_filter']['args'] = $args;

  $expenses = array();
  while ($item = db_fetch_object($r)) {
    $expenses[] = $item;
  }

  $s_totals = stormexpense_access_sql($s_totals, $where);
  $s_totals = db_rewrite_sql($s_totals);
  $r = db_query($s_totals, $args);
  $totals = db_fetch_object($r);

  $o .= theme('stormexpense_list', $header, $expenses, $totals);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  $o .= drupal_get_form('stormexpense_list_form_report');
  print theme('page', $o);
}

function stormexpense_list_filter() {
  $organization_nid = $_SESSION['stormexpense_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormexpense_list_filter']['project_nid'];
  $budget_item_nid = $_SESSION['stormexpense_list_filter']['budget_item'];
  $itemsperpage = $_SESSION['stormexpense_list_filter']['itemsperpage'];
  $title = $_SESSION['stormexpense_list_filter']['title'];
  $provider_title = $_SESSION['stormexpense_list_filter']['provider_title'];

  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormexpense_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $datefrom = $_SESSION['stormexpense_list_filter']['datefrom'];
  if (!$datefrom) {
    $datefrom = array('day' => 1,
                      'month' => format_date(time(), 'custom', 'n'),
                      'year' => format_date(time(), 'custom', 'Y'),
                      'hour' => 0,
                      'minute' => 0,
                      );
    $_SESSION['stormexpense_list_filter']['datefrom'] = $datefrom;
  }

  $expenseuser = $_SESSION['stormexpense_list_filter']['user'];

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => ('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#theme' => 'stormexpense_list_filter',
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON
  sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
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
//     '#attributes' => array('onchange' => "stormticket_organization_project_task_tickets(this, 'edit-project-nid', 'edit-task-nid', 'edit-ticket-nid', true, 'All')"),
    '#attributes' => array('onchange' => "stormbudget_organization_project_budgetitems(this, 'edit-project-nid', 'edit-budget-item', true, '-')"),
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
//     '#attributes' => array('onchange' => "stormticket_project_task_tickets(this, 'edit-organization-nid', 'edit-task-nid', 'edit-ticket-nid', true, 'All')"),
    '#attributes' => array('onchange' => "stormbudget_project_budgetitems(this, 'edit-budget-item', true, '-')"),
//     '#attributes' => array('onchange' => "stormbudget_project_budgetitems(this, 'edit-parent-nid', true, '-')"),
  );

  $tree = _stormbudget_get_tree($project_nid);
  $budget_tree = _stormbudget_plain_tree($tree);
  $budget_tree = array(0 => t('All')) + $budget_tree;
  $form['filter']['group1']['budget_item'] = array(
    '#type' => 'select',
    '#title' => t('Budget tree'),
    '#default_value' => $budget_item_nid,
    '#options' => $budget_tree,
    '#process' => array('storm_dependent_select_process'),
//     '#attributes' => array('onchange' => "stormticket_task_tickets(this, 'edit-organization-nid', 'edit-project-nid', 'edit-ticket-nid', true, 'All')"),
  );


  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -19,
  );

  $form['filter']['group2']['datefrom'] = array(
    '#type' => 'dateext',
    '#title' => t('Date from'),
    '#date_format' =>  'Y-m-d',
    '#default_value' => $datefrom,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#size' => 40,
    '#default_value' => $title,
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group4']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormexpense_list_filter_filter'),
  );

  $form['filter']['group4']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormexpense_list_filter_reset'),
  );

  $form['filter']['group4']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormexpense_list_filter_filter($form, &$form_state) {
  $_SESSION['stormexpense_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormexpense_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormexpense_list_filter']['budget_item'] = $form_state['values']['budget_item'];
  $_SESSION['stormexpense_list_filter']['datefrom'] = $form_state['values']['datefrom'];
  $_SESSION['stormexpense_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
  $_SESSION['stormexpense_list_filter']['user'] = $form_state['values']['user'];
  $_SESSION['stormexpense_list_filter']['title'] = $form_state['values']['title'];
  $_SESSION['stormexpense_list_filter']['provider_title'] = $form_state['values']['provider_title'];
}

function stormexpense_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormexpense_list_filter']);
}

function stormexpense_list_form_report() {
  $form = array();
  $form['#prefix'] = '<div id="stormexpense_list_form_report" class="storm_list_form_report">';
  $form['report'] = array(
    '#type' => 'select',
    '#title' => t('Reports'),
    '#options' => theme('stormexpense_list_form_report_reports'),
  );

  if (module_exists('locale')) {
    $language_list = language_list();
    $languages = array();
    foreach ($language_list as $key => $lang) {
      $languages[$key] = $lang->name;
    }
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $languages,
    );
  }
  else {
    $form['language'] = array(
      '#type' => 'hidden',
      '#default_value' => 'en',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Report'),
  );
  $form['#suffix'] = '</div>';
  return $form;
}

function stormexpense_list_form_report_submit($form, &$form_state) {
  $report = $form_state['values']['report'];
  $language = $form_state['values']['language'];
  drupal_goto('storm/expenses/report/'. $report .'/'. $language);
}

function stormexpense_list_report($report, $language) {
  $timetrackings = array();

  $sql = $_SESSION['stormexpense_list_filter']['sql'];
  $args = $_SESSION['stormexpense_list_filter']['args'];
  $r = db_query($sql, $args);
  while ($e = db_fetch_object($r)) {
    $expense = node_load($e->nid);
    $expenses[] = $expense;
  }

  print theme('stormexpense_list_report', $report, $language, $expenses);
}

function stormexpense_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $s = "SELECT provider_title FROM {stormexpense} sex WHERE LOWER(provider_title) LIKE LOWER('%s%%')";
    $s = stormexpense_access_sql($s);
    $s = db_rewrite_sql($s);
    $result = db_query_range($s, $string, 0, 10);
    while ($pro = db_fetch_object($result)) {
      $matches[$pro->provider_title] = check_plain($pro->provider_title);
    }
  }

  drupal_json($matches);
}

