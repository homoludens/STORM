// $Id: stormbudget.js,v 1.1.4.1 2009/04/13 11:35:03 magnity Exp $

function stormbudget_project_budgetitems(_project_select, _budgetitem_select_id, _with_all_option, _all_text) {
  var budgetitem_select = $("#" + _budgetitem_select_id).get(0);
  storm_empty_select(budgetitem_select);
  var project_nid = _project_select.value;
  if (!project_nid) project_nid=0;

  $.ajax({
    type: "GET",
    async: true,
    url: Drupal.settings.storm.project_budget_url + Drupal.encodeURIComponent(project_nid),
    dataType: "string",
    success: function (data) {
      var items = Drupal.parseJson(data);
      storm_fill_select(budget_select, items, _with_all_option, _all_text);
    }
  });
};

function stormbudget_organization_project_budgetitems(_organization_select, _project_select_id, _budget_select_id, _with_all_option, _all_text) {
  stormproject_organization_projects(_organization_select, _project_select_id, _with_all_option, _all_text);
  var project_select = $("#" + _project_select_id).get(0);
  stormbudget_project_budgetitems((project_select, _budgetitem_select_id, _with_all_option, _all_text);
};

