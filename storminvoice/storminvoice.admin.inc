<?php
// $Id: storminvoice.admin.inc,v 1.2.4.13 2009/06/22 16:22:16 magnity Exp $

/**
 * @file
 */

function storminvoice_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['storminvoice_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['storminvoice_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['storminvoice_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['storminvoice_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['storminvoice_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['storminvoice_list_filter']['organization_nid'] = $p->organization_nid;
  }

  $o = drupal_get_form('storminvoice_list_filter');

  $i = new stdClass();
  $i->type = 'storminvoice';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Number'),
      'field' => 'sin.number',
      'sort' => 'asc',
      'nowrap' => 'nowrap',
    ),
    array(
      'data' => t('Organization'),
      'field' => 'sin.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sin.project_title',
    ),
    array(
      'data' => t('Description'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Req. date'),
      'field' => 'sin.requestdate',
    ),
    array(
      'data' => t('Total'),
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.title, sin.* FROM {node} AS n INNER JOIN {storminvoice} AS sin ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice' ";

  $s_totals_topay = "SELECT SUM(amount) amount, SUM(tax) tax, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice' AND sin.paymentdate=0";

  $s_totals_paid = "SELECT SUM(amount) amount, SUM(tax) tax, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice' AND sin.paymentdate<>0";

  $s_totals = "SELECT SUM(amount) amount, SUM(tax) tax, SUM(total) total FROM {storminvoice} sin
  INNER JOIN {node} n ON n.nid=sin.nid WHERE n.status=1 AND n.type='storminvoice'";

  $where = array();
  $args = array();

  if ($_SESSION['storminvoice_list_filter']['organization_nid']) {
    $where[] = 'sin.organization_nid=%d';
    $args[] = $_SESSION['storminvoice_list_filter']['organization_nid'];
  }
  if ($_SESSION['storminvoice_list_filter']['project_nid']) {
    $where[] = 'sin.project_nid=%d';
    $args[] = $_SESSION['storminvoice_list_filter']['project_nid'];
  }
  if ($_SESSION['storminvoice_list_filter']['reqdatefrom']) {
    $reqdatefrom = $_SESSION['storminvoice_list_filter']['reqdatefrom'];
    $where[] = 'sin.requestdate>=%d';
    $args[] = _storm_date_to_gmtimestamp($reqdatefrom);
  }
  if ($_SESSION['storminvoice_list_filter']['reqdateto']) {
    $reqdateto = $_SESSION['storminvoice_list_filter']['reqdateto'];
    $where[] = 'sin.requestdate<=%d';
    $args[] = _storm_date_to_gmtimestamp($reqdateto);
  }

  $status = $_SESSION['storminvoice_list_filter']['status'];
  if ($status && $status !='-') {
    if ($status=='to pay') {
      $where[] = "sin.paymentdate=0";
    }
    if ($status=='paid') {
      $where[] = "sin.paymentdate<>0";
    }
    if ($status=='overdue') {
      $where[] = "sin.paymentdate=0 AND sin.duedate<". time();
    }
    $where[] = 'sin.requestdate<=%d';
    $args[] = _storm_date_to_gmtimestamp($reqdateto);
  }

  $itemsperpage = $_SESSION['storminvoice_list_filter']['itemsperpage'];
  if (!$itemsperpage) $itemsperpage = 10;

  $tablesort = tablesort_sql($header);

  if ($tablesort == " ORDER BY  sin.number ASC") {
    $tablesort = ' ORDER BY LPAD(sin.number, 10, "0") ASC';
  }
  else if ($tablesort == " ORDER BY  sin.number DESC") {
    $tablesort = ' ORDER BY LPAD(sin.number, 10, "0") DESC';
  }

  $s = storminvoice_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);
  $invoices = array();
  while ($invoice = db_fetch_object($r)) {
    $invoices[] = $invoice;
  }

  $s_totals_topay = storminvoice_access_sql($s_totals_topay, $where);
  $s_totals_topay = db_rewrite_sql($s_totals_topay);
  $r = db_query($s_totals_topay, $args);
  if ($r) $totals_topay = db_fetch_object($r);

  $s_totals_paid = storminvoice_access_sql($s_totals_paid, $where);
  $s_totals_paid = db_rewrite_sql($s_totals_paid);
  $r = db_query($s_totals_paid, $args);
  if ($r) $totals_paid = db_fetch_object($r);

  $s_totals = storminvoice_access_sql($s_totals, $where);
  $s_totals = db_rewrite_sql($s_totals);
  $r = db_query($s_totals, $args);
  if ($r) $totals = db_fetch_object($r);

  $o .= theme('storminvoice_list', $header, $invoices, $itemsperpage, $totals_topay, $totals_paid, $totals);
  print theme('page', $o);
}

function storminvoice_list_filter() {
  $organization_nid = $_SESSION['storminvoice_list_filter']['organization_nid'];
  $project_nid = $_SESSION['storminvoice_list_filter']['project_nid'];
  $status = $_SESSION['storminvoice_list_filter']['status'];
  $itemsperpage = $_SESSION['storminvoice_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['storminvoice_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $reqdatefrom = $_SESSION['storminvoice_list_filter']['reqdatefrom'];
  $today = time();
  if (!$reqdatefrom) {
    $reqdatefrom = array('day' => 1,
                         'month' => 1,
                         'year' => format_date($today, 'custom', 'Y'),
                         'hour' => 0,
                         'minute' => 0,
                        );
    $_SESSION['storminvoice_list_filter']['reqdatefrom'] = $reqdatefrom;
  }

  $reqdateto = $_SESSION['storminvoice_list_filter']['reqdateto'];
  if (!$reqdateto) {
    $reqdateto = array('day' => 31,
                       'month' => 12,
                       'year' => format_date($today, 'custom', 'Y'),
                       'hour' => 23,
                       'minute' => 59,
    );
    $_SESSION['storminvoice_list_filter']['reqdateto'] = $reqdateto;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Search'),
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
  while ($project = db_fetch_array($r)) {
    $projects[$project['nid']] = $project['title'];
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
    '#weight' => -19,
  );

  $form['filter']['group2']['reqdatefrom'] = array(
    '#type' => 'dateext',
    '#title' => t('Req. date from'),
    '#default_value' => $reqdatefrom,
  );

  $form['filter']['group2']['reqdateto'] = array(
    '#type' => 'dateext',
    '#title' => t('Req. date to'),
    '#default_value' => $reqdateto,
  );

  $form['filter']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array('-' => t('all'), 'to pay' => t('to pay'), 'paid' => t('paid'), 'overdue' => t('overdue')),
    '#default_value' => $status,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('storminvoice_list_filter_filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('storminvoice_list_filter_reset'),
  );

  $form['filter']['group3']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function storminvoice_list_filter_filter($form, &$form_state) {
  $_SESSION['storminvoice_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['storminvoice_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['storminvoice_list_filter']['reqdatefrom'] = $form_state['values']['reqdatefrom'];
  $_SESSION['storminvoice_list_filter']['reqdateto'] = $form_state['values']['reqdateto'];
  $_SESSION['storminvoice_list_filter']['status'] = $form_state['values']['status'];
  $_SESSION['storminvoice_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function storminvoice_list_filter_reset($form, &$form_state) {
  unset($_SESSION['storminvoice_list_filter']);
}

function _storminvoice_project_invoices_js($organization_nid, $project_nid) {
  $invoices = array();

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {storminvoice} AS sin
  ON n.vid=sin.vid WHERE n.status=1 AND n.type='storminvoice' AND sin.organization_nid=%d AND sin.project_nid=%d
  ORDER BY n.title";

  $s = storminvoice_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid, $project_nid);
  while ($item = db_fetch_object($r)) {
    $nid = $item->nid;
    $invoices[$nid] = $item->title;
  }
  print drupal_to_js($invoices);
  exit();
}

function storminvoice_report($node, $report, $language) {
  print theme('storminvoice_report', $node, $report, $language);
}

function storminvoice_items($invoice) {
  $node_invoice = node_load($invoice->nid);

  $i = new stdClass();
  $i->type = 'storminvoiceitem';

  $params = $_GET;
  $params['organization_nid'] = $invoice->organization_nid;
  $params['project_nid'] = $invoice->project_nid;
  $params['invoice_nid'] = $invoice->nid;

  $header = array(
    array(
      'data' => t('Item'),
    ),
    array(
      'data' => t('Amount'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => t('Tax'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => t('Total'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => t('Weight'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.*, sit.* FROM {node} AS n INNER JOIN {storminvoiceitem} sit ON n.vid=sit.vid WHERE n.status=1 AND n.type='storminvoiceitem' ORDER BY sit.weight";
  $where[] = 'sit.invoice_nid='. $invoice->nid;

  $s = storminvoice_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = db_query($s . $tablesort);

  $items = array();
  while ($item = db_fetch_object($r)) {
    $items[] = $item;
  }

  $o = drupal_get_form('storminvoice_items_form', $header, $items, $invoice);
  return $o;
}

function storminvoice_items_form($form_id, $header, $invoiceitems, $invoice) {
  $form = array();

  $form['invoiceitems']['#theme'] = 'storminvoice_items';

  $form['invoiceitems']['header'] = array (
    '#value' => $header,
  );

  foreach ($invoiceitems as $invoiceitem) {
    $n = new stdClass();
    $n->nid = $invoiceitem->nid;
    $n->uid = $invoiceitem->uid;
    $n->type = 'storminvoiceitem';

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_title_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->title,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_amount_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->amount,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_tax_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->tax,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_total_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->total,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_weight_'. $invoiceitem->nid] = array (
      '#type' => 'weight',
      '#default_value' => $invoiceitem->weight,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_operations_'. $invoiceitem->nid] = array (
      '#value' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('storminvoice_items_submit'),
    '#value' => t('Save item ordering'),
  );

  return $form;
}

// function storminvoice_send_email($node, $language){
//   _storminvoice_report_file_pdf( $node, $language);
//   drupal_set_message(variable_get('storminvoice_email_pdf_file_path', ''));
// //   print variable_get('storminvoice_email_pdf_file_path', '');
// }


/**
 * Page
 */
function storminvoice_send_page($node, $language) {
  drupal_set_html_head('<meta name="robots" content="noindex, nofollow" />');
  _storminvoice_report_file_pdf( $node, $language);
  $pdffile=variable_get('storminvoice_email_pdf_file_path', '');
  drupal_set_message($pdffile);

  $path =  'node/'. $node->nid;

/*

  if (is_numeric($nid)) {
    // we have a node
    $node = node_load(array('nid' => $nid));
    if (!node_access('view', $node)) {
      // Access is denied
      return drupal_access_denied();
    }
    $form_state['values']['path'] = 'node/'. $node->nid;
  }
  else {
    $args = explode('/', $form_state['values']['path']);
    if ($args[0] == 'admin') {
      return drupal_access_denied();
    }
    $node = new stdClass();
    $node->title = $form_state['values']['path'];
  }
  $cid = '';
  if (!empty($_GET['cid'])) {
    $cid = '?cid='.$_GET['cid'];
  }
  $form_state['values']['path'] .= $cid;*/
  //print "$form_state['values']['path'] - $nid - $node->title"; exit;
  return drupal_get_form('storminvoice_send_form', $path, $node->title);
}

/**
 * Form
 */
function storminvoice_send_form(&$form_state, $path = NULL, $title = NULL, $nodeapi = FALSE) {
  global $base_url, $user;

  $form = array();
  $cid = array();

    $emailtype = 'email';
//     if (preg_match("/\?cid=/i", $path) == 1) {
//       $paths = explode('?cid=', $path);
//       $cid = array('fragment' => 'comment-'.$paths[1]);
//       $path = $paths[0];
//     }
// 
//   if ($nodeapi == TRUE) {
//     $form['message'] = array(
//       '#type' => 'fieldset',
//       '#title' => t(variable_get('forward_link_title', 'Email this !type'), array('!type' => 'page')),
//       '#description' => '',
//       '#collapsed' => TRUE,
//       '#collapsible' => TRUE,
//     );
//   }
  $form['message']['instructions'] = array(
    '#type' => 'item',
    '#value' => t(variable_get('forward_instructions', '<p>Thank you for your interest in spreading the word on !site.</p><p>NOTE: We only request your email address so that the person you are recommending the page to knows that you wanted them to see it, and that it is not junk mail. We do not capture any email address.</p>'), array('!site' => variable_get('site_name', 'drupal'))),
  );
  $form['message']['yemail'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Email'),
    '#size' => 58,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['message']['yname'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Name'),
    '#size' => 58,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['message']['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Send To'),
    //'#default_value' => str_replace(', ', "\n", $recipients),
    '#cols' => 50,
    '#rows' => 5,
    '#description' => t('Enter multiple addresses on separate lines or separate them with commas.'),
    '#required' => TRUE,
  );

  $form['message']['page'] = array(
      '#type' => 'item',
      '#title' => t('You are going to email the following'),
      '#value' => l($title, $path),
    );

  $form['message']['subject'] = array(
    '#type' => 'item',
    '#title' => t('Message Subject'),
    '#value' => t(variable_get('forward_'. $emailtype .'subject', '!name has sent you a message from !site'), array('!name' => t('(Your Name)'), '!site' => variable_get('site_name', 'drupal'))),
    '#description' => '',
  );
  $form['message']['body'] = array(
    '#type' => 'item',
    '#title' => t('Message Body'),
    '#value' => t(variable_get('forward_'. $emailtype .'message', '!name thought you would like to see the !site web site.'), array('!name' => t('(Your Name)'), '!site' => variable_get('site_name', 'drupal'))),
    '#description' => '',
  );
  if (variable_get('forward_allow_message', TRUE)) {
    $form['message']['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Your Personal Message'),
      '#default_value' => '',
      '#cols' => 50,
      '#rows' => 10,
      '#description' => '',
      '#required' => TRUE,
    );
  }
  $form['message']['path'] = array(
    '#type' => 'hidden',
    '#value' => $path,
  );
//   $form['message']['path_cid'] = array(
//     '#type' => 'hidden',
//     '#value' => (!empty($cid['fragment'])) ? '#'.$cid['fragment'] : '',
//   );
  $form['message']['forward_footer'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('forward_footer', ''),
  );
  $form['message']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Message'),
  );

  if ($user->uid != 0) {
    $form['message']['yemail']['#default_value'] = $user->mail;
    $form['message']['yemail']['#disabled'] = TRUE;
    $form['message']['yemail']['#value'] = $user->mail;
    $form['message']['yname']['#default_value'] = $user->name;
  }

  return $form;
}



function storminvoice_items_submit($form, &$form_state) {
  $invoiceitems = array();
  foreach($form_state['values'] as $key=>$value) {
    $ar = explode('_', $key);
    if($ar[0]=='invoiceitem') {
      if($ar[1]=='weight') $invoiceitems[$ar[2]]['weight'] = $value;
    }
  }

  $s = "UPDATE {storminvoiceitem} SET weight=%d WHERE nid=%d";  
  foreach($invoiceitems as $nid=>$values) {
    db_query($s, $values['weight'], $nid);
  }

  drupal_set_message(t('Invoice items saved'));
}

//  generate pdf file for sending almost the same as 
//  theme_storminvoice_report_pdf
function _storminvoice_report_file_pdf($node, $language) {
  $tcpdf_dir = variable_get('storminvoice_tcpdf_location', 'sites/all/libraries/tcpdf');

  // Performs simple check for existance of tcpdf library . If it doesn't exist,  revert to node display with message about tcpdf library.
  if(!file_exists($tcpdf_dir .'/tcpdf.php')) {
    drupal_set_message(t('The tcpdf library has not been installed. See the Storm module README.txt for more details.'));
    drupal_goto('node/'. $node->nid);
  }
  
  require_once($tcpdf_dir .'/config/lang/eng.php');
  require_once($tcpdf_dir .'/tcpdf.php');
  $languages = explode(',', $language);
  $language = $languages[0];
  $language1 = '';
  if (array_key_exists(1, $languages)) {
    $language1 = $languages[1];
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  else if ($node->duedate < time()) {
    $status = 'overdue';
  }

  $countries = stormattribute_attributes_bydomain('Country');
  $countries = $countries['values'];
  $currencies = stormattribute_attributes_bydomain('Currency');
  $currencies = $currencies['values'];
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $mycurrency = $currencies[$myorg->currency];

  $organization = node_load($node->organization_nid);
  $project = node_load($node->project_nid);

  $o = '';
  $title = t('Invoice', array(), $language);
  $complete_title = $title .' '. $myorg->title .' : '. $organization->title .' - '. $node->number;

  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true); 
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("Storm");
  $pdf->SetTitle($complete_title);
  $pdf->SetSubject($title);
  $pdf->SetKeywords($title, $myorg->title, $organization->title, $node->number);
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);
  $margins = $pdf->getMargins();
  $pageWidth = $pdf->getPageWidth() - $margins['left'] - $margins['right'];
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);
  $pdf->AddPage();
  $pdf->setDrawColor(204, 204, 204);
  $pdf->setFillColor(220, 220, 220);

  $pdf->SetFont("times", "B", 14);
  $headerleft = variable_get('site_name', '') .'<br />'. variable_get('site_slogan', '');
  $pdf->writeHTMLCell($pageWidth *.5, 0, $pdf->getX(), $pdf->getY(), $headerleft, 0 ,0 ,0, false, 'L');
  $pdf->SetFont("times", "N", 10);
  $pdf->writeHTMLCell($pageWidth *.5, 0, $pdf->getX(), $pdf->getY(), variable_get('storm_report_header', ''), 0 ,1 ,0, false, 'R');
  $pdf->SetFont("times", "B", 14);
  $o = $title;
  if ($language1) $o .= "\n". t('Invoice', array(), $language1);
  $pdf->MultiCell(0, 0, $o, 0 ,'C', 0, 1,$pdf->getX(), $pdf->getY() + 10);

  $y = $pdf->getY() + 10;

  $pdf->SetFont("times", "B", 10);
  $o = t('Bill to', array(), $language);
  if ($language1) $o .= "\n". t('Bill to', array(), $language1);
  $pdf->MultiCell($pageWidth *.4, 0, $o, 'B', 'L', 0, 1, PDF_MARGIN_LEFT, $y);
  $o =  $organization->fullname ."\n";
  $o .= $organization->address ."\n";
  $o .= $organization->city ."\n";
  if ($organization->zip) $o .= $organization->zip .' ';
  if ($organization->provstate) $o .= $organization->provstate .' ';
  $o .= $countries[$organization->country] ."\n";;
  if ($organization->taxid) {
    $o .= t('Tax ID', array(), $language);
    if ($language1) $o .= ' / '. t('Tax ID', array(), $language1);
    $o .= ' : '. $organization->taxid;
  }
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($pageWidth *.4, 0, $o,0 , 'L' ,0, 1, PDF_MARGIN_LEFT);
  $destY = $pdf->getY();

  $w = ($pageWidth *.5) / 4;

  $pdf->SetFont("times", "B", 10);

  $o = t('Invoice#', array(), $language);
  if ($language1) $o .= "\n". t('Invoice#', array(), $language1);
  $pdf->MultiCell($w-2, 0, $o, 1, 'L', 1, 0, PDF_MARGIN_LEFT + $pageWidth *.5, $y);

  $o = t('Currency', array(), $language);
  if ($language1) $o .= "\n". t('Currency', array(), $language1);
  $pdf->MultiCell($w-3, 0, $o, 1, 'L', 1, 0);
  $o = t('Date', array(), $language);
  if ($language1) $o .= "\n". t('Date', array(), $language1);
  $pdf->MultiCell($w+5, 0, $o, 1, 'L', 1, 0);
  $o = t('Reference', array(), $language);
  if ($language1) $o .= "\n". t('Reference', array(), $language1);
  $pdf->MultiCell($w, 0, $o, 1, 'L', 1, 1);

  $pdf->SetFont("times", "N", 10);
  $h = $pdf->getY();
  $pdf->MultiCell($w, 0, $node->reference?$node->reference:'-' , 1, 'L', 0, 1, PDF_MARGIN_LEFT + $pageWidth*.5 + $w*3);
  $h = $pdf->getY() - $h;
  $pdf->MultiCell($w-2, $h, $node->number, 1, 'L', 0, 0, PDF_MARGIN_LEFT + $pageWidth *.5, $pdf->getY() - $h);
  $pdf->MultiCell($w-3, $h, t($mycurrency, array(), $language), 1, 'L', 0, 0);
  $pdf->MultiCell($w+5, $h, format_date($node->requestdate, 'custom', 'Y-m-d'), 1, 'L', 0, 1);

  $pdf->SetFont("times", "B", 10);

  $o = t('Due total', array(), $language);
  if ($language1) $o .= "\n". t('Due total', array(), $language1);
  $pdf->MultiCell($w*2 - 5, 0, $o, 1, 'L', 1, 0, PDF_MARGIN_LEFT + $pageWidth *.5);

  $o = t('Due date', array(), $language);
  if ($language1) $o .= "\n". t('Due date', array(), $language1);
  $pdf->MultiCell($w + 5, 0, $o, 1, 'L', 1, 0);

  $o = t('Terms', array(), $language);
  if ($language1) $o .= "\n". t('Terms', array(), $language1);
  $pdf->MultiCell($w, 0, $o, 1, 'L', 1, 1);

  $pdf->SetFont("times", "B", 10);
  $o = t($mycurrency, array(), $language) .' '. sprintf('%.2f', $node->total);
  if ($organization->currency != $myorg->currency) {
    $o .= "\n". $organization->currency .' '. sprintf('%.2f', $node->totalcustomercurr);
  }
  $pdf->MultiCell($w*2 - 5, 12, $o, 1, 'C', 0, 0, PDF_MARGIN_LEFT + $pageWidth *.5);
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($w + 5, 12, format_date($node->duedate, 'custom', 'Y-m-d'), 1, 'L', 0, 0);
  $pdf->MultiCell($w, 12, t(variable_get('storminvoice_payment_terms', ''), array(), $language), 1, 'L', 0, 1);

  $y = $pdf->getY();
  if($destY > $y) $y = $destY;
  $pdf->setY($y+10);

  $pdf->setX(PDF_MARGIN_LEFT);
  $pdf->SetFont("times", "B", 10);

  $o = t('Description', array(), $language);
  if ($language1) $o .= "\n". t('Description', array(), $language1);
  $pdf->MultiCell($pageWidth * .4, 0, $o, 1, 'L', 1, 0);

  $o = t('Amount', array(), $language);
  if ($language1) $o .= "\n". t('Amount', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  $o = t('Tax %', array(), $language);
  if ($language1) $o .= "\n". t('Tax %', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  $o = t('Tax', array(), $language);
  if ($language1) $o .= "\n". t('Tax', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  $o = t('Total', array(), $language);
  if ($language1) $o .= "\n". t('Total', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 1);

  $pdf->SetFont("times", "N", 10);
  $items = storminvoiceitem_getitems($node->nid);
  $rows = array();
  $pdf->setFillColor(245, 245, 245);
  $c = 0;
  foreach ($items as $i) {
    if ($c==2) $c=0;
    $y = $pdf->getY();
    $h = $pdf->getY();
    $pdf->MultiCell($pageWidth * .4, 0, $i->title, 1, 'L', $c, 1);
    $h = $pdf->getY() - $h;
    $pdf->setY($y);
    $pdf->setX(PDF_MARGIN_LEFT + $pageWidth * .4);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->amount), 1, 0, 'R', $c);
    $pdf->Cell($pageWidth * .15, $h, $i->taxpercent, 1, 0, 'R', $c);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->tax), 1, 0, 'R', $c);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->total), 1, 1, 'R', $c);
    $c++;
  }

  if ($node->taxexempt && $language1=='it') {
    $o = t('Tax exempt art. 7', array(), $language);
    if ($language1) $o .= "\n". t('Tax exempt art. 7', array(), $language1);
    $pdf->MultiCell($pageWidth, 0, $o,0 , 'L' ,0, 1, $pdf->getX(), $pdf->getY() + 5);
  }

  $y = $pdf->getY() + 10;
  $pdf->setY($y);
  $pdf->SetFont("times", "B", 10);
  $pdf->Cell($pageWidth, 5, t('Payment', array(), $language), 'B', 0, 'L');
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($pageWidth, 0, t(variable_get('storminvoice_payment_modes', ''), array(), $language),0 , 'L' ,0, 1, PDF_MARGIN_LEFT, $pdf->getY(), true, 0, true);

  if ($status=='paid') {
    $y = $pdf->getY() + 10;
    $pdf->setY($y);
    $pdf->SetFont("times", "B", 14);
    $pdf->Cell(0, 12, t('Paid in full', array(), $language), 0 ,1,'C');
  }

  $filename = strtolower($title . '_' . $myorg->title . '_' . $organization->title . '_' . str_replace('/', '-', $node->number)) .'.pdf';


    //Close and save PDF document as file ready for sending email
    $dir = file_directory_path().'/'.variable_get('storminvoice_email_pdf_location', 'storminvoice').'/';
    $pdf->Output($dir.$filename, "F");
    variable_set('storminvoice_email_pdf_file_path', $dir.$filename);

}