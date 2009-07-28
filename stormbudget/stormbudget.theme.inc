<?php
// $Id: stormbudget.theme.inc,v 1.3.4.13 2009/06/21 21:08:45 magnity Exp $

/**
 * @file
 */

function theme_stormbudget_budgetitems($form) {
  if (count($form['budgetitems'])>0) {
    drupal_add_tabledrag('budgetitems', 'match', 'parent', 'budgetitem-parent-nid', 'budgetitem-parent-nid', 'budgetitem-nid', TRUE);
    drupal_add_tabledrag('budgetitems', 'order', 'sibling', 'budgetitem-weight'); 
  }

  $header = array();
  $row = array();

  $header = $form['header']['#value'];
  unset($form['header']);

  foreach (element_children($form['budgetitems']) as $key) {

    $form['budgetitems'][$key]['budgetitem_parent-nid_'. $key]['#attributes']['class'] = 'budgetitem-parent-nid';
    $form['budgetitems'][$key]['budgetitem_nid_'. $key]['#attributes']['class'] = 'budgetitem-nid';
    $form['budgetitems'][$key]['budgetitem_weight_'. $key]['#attributes']['class'] = 'budgetitem-weight';
//     $durationunit = $form['budgetitems'][$key]['budgetitem_durationunit_'. $key]['#value'];

    $data = array(
      theme('indentation', $form['budgetitems'][$key]['budgetitem_depth_'. $key]['#value']) . drupal_render($form['budgetitems'][$key]['budgetitem_description_'. $key]),
      drupal_render($form['budgetitems'][$key]['budgetitem_category_'. $key]),
      drupal_render($form['budgetitems'][$key]['budgetitem_status_'. $key]),
      drupal_render($form['budgetitems'][$key]['budgetitem_priority_'. $key]),
//       drupal_render($form['budgetitems'][$key]['budgetitem_price_'. $key]),

      array(
        'data' => sprintf('%.2f', drupal_render($form['budgetitems'][$key]['budgetitem_price_'. $key])), 
        'style' => 'text-align: right'),
      array(
        'data' => sprintf('%.2f', drupal_render($form['budgetitems'][$key]['budgetitem_spent_'. $key])), 
        'style' => 'text-align: right'),

//       array(
//         'data' => sprintf('%.2f', drupal_render($form['budgetitems'][$key]['budgetitem_duration_'. $key])) .' '. substr($durationunit, 0, 1), 
//         'style' => 'text-align: right'),
//       drupal_render($form['budgetitems'][$key]['budgetitem_parent-nid_'. $key]) . drupal_render($form['budgetitems'][$key]['budgetitem_nid_'. $key]),
//       drupal_render($form['budgetitems'][$key]['budgetitem_weight_'. $key]),
      array(
        'data' => drupal_render($form['budgetitems'][$key]['budgetitem_operations_'. $key]),
        'class' => 'storm_list_operations',
      ),
    );
    $row['data'] = $data;
    $row['class'] = empty($row['class']) ? 'draggable' : $row['class'] .' draggable';
    $rows[] = $row;
  }
  $o = theme('table', $header, $rows, array('id' => 'budgetitems'));
  return $o;
}

function theme_stormbudget_list($header, $budgetitems, $totals) {
  $rows = array();
  foreach ($budgetitems as $budgetitem) {
    $n = new stdClass();
    $n->nid = $budgetitem->nid;
    $n->uid = $budgetitem->uid;
    $n->organization_nid = $budgetitem->organization_nid;
    $n->type = 'stormbudget';
    $node_person = node_load($budgetitem->person_nid);


    $rows[] = array(
      l($budgetitem->organization_title, 'node/'. $budgetitem->organization_nid),
      l($budgetitem->project_title, 'node/'. $budgetitem->project_nid),
      l($budgetitem->title, 'node/'. $budgetitem->nid) . theme('mark', node_mark($budgetitem->nid, $budgetitem->changed)),
      storm_icon('category_'. $budgetitem->budgetcategory, $budgetitem->budgetcategory),
      storm_icon('status_'. $budgetitem->budgetstatus, $budgetitem->budgetstatus),
      storm_icon('priority_'. $budgetitem->budgetpriority, $budgetitem->budgetpriority),
//       $budgetitem->price,
      sprintf('%.2f', $budgetitem->price), 
//       l($node_person->fullname, 'node/'. $budgetitem->person_nid),
//       l($budgetitem->person_organization_nid, 'node/'. $budgetitem->person_organization_nid),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);

  $header = array(
    array(
      'data' => t('Total price'),
      'style' => 'text-align: center;',
    ),
  );

  $rows = array();
  $rows[] = array(
    array(
      'data' => sprintf('%.2f', $totals->t_price),
      'style' => 'text-align: right;',
    ),
  );
  $o .= theme('table', $header, $rows);

  return $o;
}

function theme_stormbudget_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormbudgetitem'] = array(
    '#prefix' => '<div id="stormbudgetitem" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormbudgetitem']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormbudgetitem']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Organization'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormbudgetitem']['fields']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Project'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormbudgetitem']['fields']['stepno'] = array(
    '#prefix' => '<div class="stepno">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Step no.'), $node->stepno),
    '#weight' => $w++,
  );

  $node->content['stormbudgetitem']['fields']['price'] = array(
    '#prefix' => '<div class="stepno">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Budget'), $node->price),
    '#weight' => $w++,
  );


  $node->content['stormbudgetitem']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Description'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormbudgetitem']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
    '#weight' => $w++,
  );

  $w = 0;
  $i = new stdClass();

  if (module_exists('stormexpense') && user_access('Storm expense: access')) {
    $i->type = 'stormexpense';
    $node->content['stormbudgetitem']['links']['expense']  = array(
      '#prefix' => '<dt id="stormnotes" class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => l(t('Expenses'), 'storm/expenses', array('query' => array('organization_nid' => $node->organization_nid,'project_nid' => $node->project_nid, 'budget_item' => $node->nid))).storm_icon_add_node($i, $_GET),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormnote') && user_access('Storm note: access')) {
    $i->type = 'stormnote';
    $node->content['stormbudgetitem']['links']['notes']  = array(
      '#prefix' => '<dt id="stormnotes" class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => l(t('Notes'), 'storm/notes/', array('query' => array('budgetitem_nid' => $node->nid))).storm_icon_add_node($i, $_GET),
      '#weight' => $w++,
    );
  }
  
  if (module_exists('stormticket') && user_access('Storm ticket: access')) {
    $i->type = 'stormticket';
    $node->content['stormbudgetitem']['links']['tickets'] = array(
      '#prefix' => '<dt id="stormtickets" class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => l(t('Tickets'), 'storm/tickets/', array('query' => array('budgetitem_nid' => $node->nid))).storm_icon_add_node($i, $_GET),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormtimetracking') && user_access('Storm timetracking: access')) {
    $i->type = 'stormtimetracking';
    $node->content['stormbudgetitem']['links']['timetrackings'] = array(
      '#prefix' => '<dt id="stormtimetrackings" class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => l(t('Timetrackings'), 'storm/timetrackings/', array('query' => array('budgetitem_nid' => $node->nid))).storm_icon_add_node($i, $_GET),
      '#weight' => $w++,
    );
  }

  return $node;
}
