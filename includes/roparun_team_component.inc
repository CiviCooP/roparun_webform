<?php

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_roparun_team() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'unique' => TRUE,
      'private' => FALSE,
      'empty_team_option' => t(' - Geen team - '),
      'team_label' => t('Team'),
      'empty_team_member_option' => t(' - Geen teamlid - '),
      'team_member_label' => t('Teamlid'),
    ),
  );
}


/**
 * Implements _webform_edit_component().
 */
function _webform_edit_roparun_team($component) {
  $form = array();
	$form['roparun']['#type'] = 'fieldset';
	$form['roparun']['#title'] = t('Roparun Team en Teamdeelnemers');
  $form['roparun']['team_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label teamveld'),
    '#default_value' => $component['extra']['team_label'],
    '#parents' => array('extra', 'team_label'),
    '#required' => true,
  );
  $form['roparun']['empty_team_option'] = array(
    '#type' => 'textfield',
    '#title' => t('Optie geen team'),
    '#default_value' => $component['extra']['empty_team_option'],
    '#parents' => array('extra', 'empty_team_option'),
    '#required' => true,
  );
	$form['roparun']['team_member_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label teamlidveld'),
    '#default_value' => $component['extra']['team_member_label'],
    '#parents' => array('extra', 'team_member_label'),
    '#required' => true,
  );
  $form['roparun']['empty_team_member_option'] = array(
    '#type' => 'textfield',
    '#title' => t('Optie geen teamlid'),
    '#default_value' => $component['extra']['empty_team_member_option'],
    '#parents' => array('extra', 'empty_team_member_option'),
    '#required' => true,
  );
	// Hide name, it's never shown
  $form['name']['#type'] = 'value';
  // Hack - name is required by webform but we don't expose it to the user. Instead we'll replace it with the value of form_key.
  $form['name']['#value'] = 'Roparun Team en Teamdeelnemer';
  return $form;
}

function _webform_roparun_team_from_url() {
	if (isset($_GET['team']) && is_numeric($_GET['team'])) {
		return $_GET['team'];
	}
	return null;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_roparun_team($component, $value = NULL, $filter = TRUE) {
	civicrm_initialize();
	$default_team_id = _webform_roparun_team_from_url();
	if (!empty($default_team_id)) {
		$default_team_id .= 's_'.$default_team_id;
	}
	$teams = civicrm_api3('RoparunTeam', 'get', array(
		'options' => array('limit' => 0),
	));
	$team_options = array('' => $component['extra']['empty_team_option']);
	foreach($teams['values'] as $team) {
		$string_team_id = "s_" . $team['id'];
		$team_options[$string_team_id] = $team['teamnr'] .' - '.$team['name'];
	}
	
	
	$element = array(
		'#title' => '',
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
	);
  $element_roparun['_roparun_team'] = array(
    '#type' => 'select',
    '#title' => $filter ? _webform_filter_xss($component['extra']['team_label']) : $component['extra']['team_label'],
    '#options' => $team_options,
    '#default_value' => $default_team_id,
    '#theme_wrappers' => array('webform_element'),
  );
	
	foreach($team_options as $string_team_id => $team_name) {
		$team_id = str_replace("s_", "", $string_team_id);
		if (empty($team_id)) {
			continue;
		}
		
		$team_members = civicrm_api3('RoparunTeam', 'getmembers', array(
			'team_id' => $team_id,
			'only_show_donations_enabled' => 1,
			'options' => array('limit' => 0),
		));
		$team_member_options = array('' => $component['extra']['empty_team_member_option']);
		foreach($team_members['values'] as $team_member) {
			$team_member_options[$team_member['id']] = $team_member['name'] . ' (' . $team_member['city'] . ') - '.$team_member['role'];
		}
		$element_roparun['_roparun_team_member_'.$team_id] = array(
    '#type' => 'select',
    '#title' => $filter ? _webform_filter_xss($component['extra']['team_member_label']) : $component['extra']['team_member_label'],
    '#options' => $team_member_options,
    '#states' => array(
    	'visible' => array(
    		'.webform-component--donatie--team---roparun-team select' => array('value' => $string_team_id),
			)
		),
  );
	}
	
	
	
	//$element_roparun_team_member_rendered = render($element_roparun_team_member);
  
  //$element['#markup'] = $element_roparun_team_rendered . $element_roparun_team_member_rendered;
  //$element['#markup'] =  element($element_roparun);
	
  return $element_roparun;
}
