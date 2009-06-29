<?php
// $Id: storm.theme.inc,v 1.6.4.13 2009/06/20 14:08:50 magnity Exp $

/**
 * @file
 */

function theme_storm_form_group($form) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $row = array();
  foreach (element_children($form) as $key) {
    array_push($row, drupal_render($form[$key]));
  }
  $rows[] = array('data' => $row, 'class' => 'formgroup');
  $output .= theme('table', array(), $rows, array('class' => 'formgroup'));
  return $output;
}

function theme_datetime($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

function theme_dateext($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

function theme_storm_list_report($header, $rows, $title, $footer) {
  $css_all_file = drupal_get_path('module', 'storm') .'/storm_list_report_all.css';
  $css_print_file = drupal_get_path('module', 'storm') .'/storm_list_report_print.css';
  $query_string = '?'. drupal_substr(variable_get('css_js_query_string', '0'), 0, 1);

  $o = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $o .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $o .= '<head>';
  $o .= '<title>'. (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal')) .'</title>';
  $o .= '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $css_all_file . $query_string .'" />';
  $o .= '<link type="text/css" rel="stylesheet" media="print" href="'. base_path() . $css_print_file . $query_string .'" />';
  $o .= '</head>';
  $o .= '<body>';
  $o .= '<table class="storm_report_header"><tr>';
  $o .= '<td class="storm_report_myorganization">';
  $o .= '<div class="site_name">'. variable_get('site_name', '') .'</div>';
  $o .= '<div class="site_slogan">'. variable_get('site_slogan', '') .'</div>';
  $o .= '</td>';  $o .= '<td class="storm_report_header">'. variable_get('storm_report_header', '') .'</td>';
  $o .= '</tr></table>';
  $o .= '<div class="storm_list_report_title">'. $title .'</div>';
  $o .= '<div class="storm_list_report_data">';
  $o .= theme('table', $header, $rows);
  $o .= '</div>';
  $o .= '<div class="storm_list_report_footer">'. $footer .'</div>';
  $o .= '</body>';
  $o .= '</html>';

  return $o;
}

function theme_storm_report($header, $content, $title, $footer, $headtitle='') {
  $css_all_file = drupal_get_path('module', 'storm') .'/storm_report_all.css';
  $css_print_file = drupal_get_path('module', 'storm') .'/storm_report_print.css';
  $query_string = '?'. drupal_substr(variable_get('css_js_query_string', '0'), 0, 1);

  $o = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $o .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $o .= '<head>';
  $o .= '<title>'. ($headtitle ? strip_tags($headtitle) : $title) .'</title>';
  $o .= '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $css_all_file . $query_string .'" />';
  $o .= '<link type="text/css" rel="stylesheet" media="print" href="'. base_path() . $css_print_file . $query_string .'" />';
  $o .= '</head>';
  $o .= '<body>';
  $o .= '<table class="storm_report_header"><tr>';
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $o .= '<td class="storm_report_myorganization">';
  $o .= '<div class="site_name">'. variable_get('site_name', '') .'</div>';
  $o .= '<div class="site_slogan">'. variable_get('site_slogan', '') .'</div>';
  $o .= '</td>';
  $o .= '<td class="storm_report_header">'. variable_get('storm_report_header', '') .'</td>';
  $o .= '</tr></table>';
  $o .= '<div class="storm_report_title">'. $title .'</div>';
  $o .= '<div id="storm_report_content">';
  $o .= $content;
  $o .= '</div>';
  $o .= '<div class="storm_report_footer">'. $footer .'</div>';
  $o .= '</body>';
  $o .= '</html>';

  return $o;
}

function theme_storm_view_item($label, $value) {
  $o = '<div class="label"><span class="label">'. t($label) .' : </span></div><div class="value">'. $value .'</div>';
  return $o;
}

function theme_storm_dashboard() {
  $o = '<div id="stormdashboard">';
  $o .= '<div class="stormdashboard">';
  $o .= '<dl class="stormdashboard">';
  if (module_exists('stormorganization') && user_access('Storm organization: access')) {
    $o .= '<dt class="stormcomponent" id="stormorganizations">'. l(t('Organizations'), 'storm/organizations');
    $i = new stdClass();
    $i->type = 'stormorganization';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormperson') && user_access('Storm person: access')) {
    $o .= '<dt class="stormcomponent" id="stormpeople">'. l(t('People'), 'storm/people');
    $i = new stdClass();
    $i->type = 'stormperson';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormteammember') && user_access('Storm teammember: access')) {
    $o .= '<dt class="stormcomponent" id="stormteammembers">'. l(t('Team members'), 'storm/teammembers');
    $i = new stdClass();
    $i->type = 'stormteammember';
    $o .= storm_icon_add('storm/teammembers/add', $i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormproject') && user_access('Storm project: access')) {
    $o .= '<dt class="stormcomponent" id="stormprojects">'. l(t('Projects'), 'storm/projects');
    $i = new stdClass();
    $i->type = 'stormproject';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormtask') && user_access('Storm task: access')) {
    $o .= '<dt class="stormcomponent" id="stormtasks">'. l(t('Tasks'), 'storm/tasks');
    $i = new stdClass();
    $i->type = 'stormtask';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormticket') && user_access('Storm ticket: access')) {
    $o .= '<dt class="stormcomponent" id="stormtickets">'. l(t('Tickets'), 'storm/tickets');
    $i = new stdClass();
    $i->type = 'stormticket';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormtimetracking') && user_access('Storm timetracking: access')) {
    $o .= '<dt class="stormcomponent" id="stormtimetrackings">'. l(t('Timetrackings'), 'storm/timetrackings');
    $i = new stdClass();
    $i->type = 'stormtimetracking';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  $o .= '</div>';

  $o .= '<div class="stormdashboard">';
  if (module_exists('stormnote') && user_access('Storm note: access')) {
    $o .= '<dt class="stormcomponent" id="stormnotes">'. l(t('Notes'), 'storm/notes');
    $i = new stdClass();
    $i->type = 'stormnote';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }

  if (module_exists('storminvoice') && user_access('Storm invoice: access')) {
    $o .= '<dt class="stormcomponent" id="storminvoices">'. l(t('Invoices'), 'storm/invoices');
    $i = new stdClass();
    $i->type = 'storminvoice';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }

  if (module_exists('stormexpense') && user_access('Storm expense: access')) {
    $o .= '<dt class="stormcomponent" id="stormexpenses">'. l(t('Expenses'), 'storm/expenses');
    $i = new stdClass();
    $i->type = 'stormexpense';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormknowledgebase') && user_access('Storm knowledgebase: access')) {
    $o .= '<dt class="stormcomponent" id="stormknowledgebase">'. l(t('Knowledge base'), 'storm/knowledgebase');
    $i = new stdClass();
    $i->type = 'stormknowledgebase';
    $o .= storm_icon_add_node($i, $_GET);
    $o .= '</dt>';
  }
  if (module_exists('stormattribute') && user_access('Storm attribute: access')) {
    $o .= '<dt class="stormcomponent" id="stormattributes">'. l(t('Attributes'), 'storm/attributes');
    $i = new stdClass();
    $i->type = 'stormattribute';
    $o .= storm_icon_add('storm/attributes/add', $i, $_GET);
    $o .= '</dt>';
  }
  if (user_access('access administration pages')) {
    $o .= '<dt class="stormcomponent" id="stormconfiguration">'. l(t('Configuration'), 'admin/storm') .'</dt>';
  }
  $o .= '</dl>';
  $o .= '</div>';
  $o .= '</div>';
  return $o;
}
