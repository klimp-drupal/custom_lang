<?php

/**
 * @file
 * Administrative callbacks for Custom Language Detector module.
 */

/**
 * Form constructor for custom_lang_settings_form.
 *
 * @see custom_lang_settings_form_submit()
 *
 * @ingroup forms
 */
function custom_lang_settings_form($form, &$form_state) {
  $form = array();

  $form['text'] = array(
    '#markup' => t('Check country codes at') . ' ' . l('ip-api.com', 'http://ip-api.com'),
  );

  $form['container'] = array(
    '#tree' => TRUE,
    '#theme' => 'custom_lang_tabular_form',
    'header' => array(
      '#type' => 'value',
      '#value' => array(t('Language'), t('Prefix'), t('Country code'), t('HTTP language code')),
    ),
  );

  $langs = language_list();
  foreach ($langs as $lang) {
    $form['container']['data'][$lang->language]['name'] = array(
      '#markup' => t($lang->name),
    );
    $form['container']['data'][$lang->language]['prefix'] = array(
      '#markup' => t($lang->prefix),
    );
    $form['container']['data'][$lang->language]['country_code'] = array(
      '#type' => 'textfield',
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#default_value' => variable_get('custom_lang_' . $lang->language . '_country_code'),
    );
    $form['container']['data'][$lang->language]['http_lang_code'] = array(
      '#type' => 'textfield',
      '#attributes' => array('autocomplete' => 'off'),
      '#default_value' => variable_get('custom_lang_' . $lang->language . '_http_lang_code'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler for custom_lang_settings_form_form().
 */
function custom_lang_settings_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['container']['data'])) {

    $country_langs = array();
    foreach ($form_state['values']['container']['data'] as $lang => $item) {
      variable_set('custom_lang_' . $lang . '_country_code', $item['country_code']);
      variable_set('custom_lang_' . $lang . '_http_lang_code', $item['http_lang_code']);
      $country_langs[$item['country_code']][$lang] = $item['http_lang_code'];
    }
    variable_set('custom_lang_settings', $country_langs);
  }
}

/**
 * Form constructor for custom_lang_ip_test_form.
 *
 * @see custom_lang_ip_test_form_submit()
 *
 * @ingroup forms
 */
function custom_lang_ip_test_form($form, &$form_state) {
  $form['ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP address'),
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Test'),
  );

  return $form;
}

/**
 * Form submission handler for custom_lang_ip_test_form_form().
 */
function custom_lang_ip_test_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['ip'])) {
    $ip = $form_state['values']['ip'];
    custom_lang_set_lang($ip);
  }
}

/**
 * Theme a tabular form.
 *
 */
function theme_custom_lang_tabular_form($vars) {
  $form = $vars['form'];
  $rows = array();

  foreach (element_children($form['data']) as $key) {
    foreach (element_children($form['data'][$key]) as $name) {
      $rows[$key][] = drupal_render($form['data'][$key][$name]);
    }
  }

  $params = array(
    'header' => $form['header']['#value'],
    'rows' => $rows,
    'caption' => $form['#title'],
  );
  if (!empty($form['#title'])) {
    $params['caption'] = $form['#title'];
  }

  return theme('table', $params);
}
