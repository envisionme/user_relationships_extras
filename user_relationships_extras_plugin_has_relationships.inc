<?php
class user_relationships_extras_plugin_has_relationships extends views_plugin_access {
  function init(&$view, &$display) {
    $this->view = &$view;
    $this->display = &$display;
    $this->options = array();

    if (is_object($display->handler)) {
    // Note: The below is read only.
      $this->options = $display->handler->get_option('access');
    }
  }

  function access($account) {
      return views_has_relationship(array_filter($this->options['relationships']), $account);
    }

    function get_access_callback() {
      return array('views_has_relationship', array(array_filter($this->options['relationships'])));
    }

    function summary_title() {
      $count = count($this->options['relationships']);
      if ($count < 1) {
        return t('No relationship(s) selected');
      }
      else if ($count > 1) {
        return t('Multiple relationships');
      }
      else {
        $value = user_relationships_types_load();
        $relationsip_name = array();
        foreach($value as $rname){
          $relationsip_name[$rname->rtid] = $rname->name;
        }
        $rtids = $relationsip_name;
        $rid = reset($this->options['relationships']);
        // Display the relationship that was selected
        return $rtids[$rid];
      }
    }

    function option_defaults(&$options) {
      $options['relationships'] = array();
    }

    function options_form(&$form, &$form_state) {
      // Get all the relationships from the system.
      $value = user_relationships_types_load();
      $relationsip_name = array();
      foreach($value as $rname){
       $relationsip_name[$rname->rtid] = $rname->name;
      }

      $form['relationships'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Relationship'),
        '#default_value' => $this->options['relationships'],
        '#options' => $relationsip_name,
        '#description' => t('Only checked relationships will have access to this view'),
      );
    }

    function options_validate(&$form, &$form_state) {
      if (!array_filter($form_state['values']['access_options']['relationships'])) {
        form_error($form['relationships'], t('You must select at least one relationship if type is "by relationships"'));
      }
    }

    function options_submit(&$form, &$form_state) {
      // I hate checkboxes.
      $form_state['values']['access_options']['relationships'] = array_filter($form_state['values']['access_options']['relationships']);
    }
}