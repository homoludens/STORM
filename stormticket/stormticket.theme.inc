<?php
// $Id: stormticket.theme.inc,v 1.5.4.6 2009/05/25 23:09:26 magnity Exp $

/**
 * @file
 */

function theme_stormticket_list($header, $tickets) {

  $rows = array();
  foreach ($tickets as $ticket) {
    $n = new stdClass();
    $n->nid = $ticket->nid;
    $n->uid = $ticket->uid;
    $n->organization_nid = $ticket->organization_nid;
    $n->type = 'stormticket';

    $rows[] = array(
      storm_icon('category_'. $ticket->ticketcategory, $ticket->ticketcategory),
      l($ticket->organization_title, 'node/'. $ticket->organization_nid),
      l($ticket->project_title, 'node/'. $ticket->project_nid),
      l($ticket->title, 'node/'. $ticket->nid) . theme('mark', node_mark($ticket->nid, $ticket->changed)),
      format_date($ticket->created, 'small'),
      storm_icon('status_'. $ticket->ticketstatus, $ticket->ticketstatus),
      storm_icon('priority_'. $ticket->ticketpriority, $ticket->ticketpriority),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormticket_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormticket'] = array(
    '#prefix' => '<div id="stormticket" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Organization'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Project'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields']['task'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Task'), l($node->task_title, 'node/'. $node->task_nid)),
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields']['category'] = array(
    '#prefix' => '<div class="category">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Category'), $node->ticketcategory),
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields']['status'] = array(
    '#prefix' => '<div class="storm_status">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Status'), $node->ticketstatus),
    '#weight' => $w++,
  );

  $node->content['stormticket']['fields']['priority'] = array(
    '#prefix' => '<div class="priority">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Priority'), $node->ticketpriority),
    '#weight' => $w++,
  );
  
  $node->content['stormticket']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Description'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormticket']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
    '#weight' => $w++,
  );
  
  $w = 0;

  return $node;
}
