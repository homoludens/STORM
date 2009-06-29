<?php
// $Id: stormattribute_handler_filter_attributes_domain.inc,v 1.1.2.2 2009/06/24 21:25:26 magnity Exp $

/**
 * Filter by vocabulary id
 */

class stormattribute_handler_filter_attributes_domain extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = stormattribute_attributes_bydomain($this->definition['domain']);
    }
  }
}
