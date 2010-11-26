<?php
// $Id$

/**
 * @file
 * Provides admin forms for the site location_chooser
 */

function location_chooser_location_form() {
  // LOCATION
  foreach (array('city', 'province', 'postal_code') as $key) {
    $location_fields[$key] = array('collect' => 1, 'default' => '');
  }
  $location_fields['locpick'] = array('collect' => 0, 'default' => '', 'weight' => 0);

  $location_settings = array('multiple' => array('min' => 0, 'max' => 1, 'add' => 1), 'form' => array('fields' => $location_fields));
  $locations = location_load_locations('location_chooser_default', 'genid');
  $form['default_location'] = location_form($location_settings, $locations);
  $form['default_location'][0]['#title'] = t('Default Location');
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

function location_chooser_location_form_submit(&$form, $form_state) {
  location_save_locations($form_state['values']['default_location'], array('genid' => 'location_chooser_default'));
  drupal_set_message("The default location has been saved");
}

function location_chooser_settings() {
  $nodetypes = $sources = node_get_types('names');
  $form['location_chooser_fieldset'] = array(
    '#type' => 'select',
    '#title' => t('Location form display'),
    '#description' => t('<ul><li>Standard: keeps default location form display</li>
                        <li>Fieldset: displays location form in a fieldset (good for vertical tabs)</li>
                        <li>Hidden: hides location form on node entry forms (does not provide extra security for location form elements)</li></ul>'),
    '#options' => array('Standard', 'Fieldset', 'Hidden'),
    '#default_value' => variable_get('location_chooser_fieldset', 0),
  );
  $targets = variable_get('location_chooser_targets', array());
  $targetcount = count(array_filter($targets));
  $form['targets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Targets') . " ($targetcount)",
    '#description' => t('Node types on which the location chooser form should appear<br/>
 NOTE: behavior may be unreliable if the same node types are targets and sources for location chooser.'),
    '#collapsible' => TRUE,
    '#collapsed' => count(array_filter($targets)),
  );
  $form['targets']['location_chooser_targets'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Targets'),
    '#options' => $nodetypes,
    '#default_value' => $targets,
  );
  $sources = variable_get('location_chooser_node_sources', array());
  $sourcecount = count(array_filter($sources)) + variable_get('location_chooser_user_source', TRUE) + variable_get('location_chooser_default_source', TRUE);
  $form['location_chooser_sources'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sources') . " ($sourcecount)",
    '#description' => t('Types of locations that will populate the location chooser select box<br/>
  <b>NOTE: If you have EVER had "link locations" enabled, and you switch a node type from a target
  to a source, you could mess up all the locations linked to by those nodes.</b>  See the note above.'),
    '#collapsible' => TRUE,
    '#collapsed' => $sourcecount,
  );
  $form['location_chooser_sources']['location_chooser_node_sources'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => $nodetypes,
    '#default_value' => $sources,
  );
  $form['location_chooser_sources']['location_chooser_user_source'] = array(
    '#type' => 'checkbox',
    '#title' => t('User locations'),
    '#default_value' => variable_get('location_chooser_user_source', TRUE),
  );
  $form['location_chooser_sources']['location_chooser_default_source'] = array(
    '#type' => 'checkbox',
    '#title' => t('Site default location'),
    '#default_value' => variable_get('location_chooser_default_source', TRUE),
  );
  $form['location_chooser_reuse_source_locations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep locations linked if source changes'),
    '#default_value' => variable_get('location_chooser_reuse_source_locations', FALSE),
    '#description' => t('If this option is selected, changing a source location will
 change all nodes linked to that source.  For example, on a site which allows users
 to post events at their homes, if a user changes his or her home location, all existing
 events at that location will point to the new location.  Problems may arise if
 you change which node types are sources and targets after content has been created.
 Therefore, it is suggested that you <b>only enable this setting after you are satisfied
 with the setup of the sources and targets above</b>.')
  );
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);
  return system_settings_form($form);
}