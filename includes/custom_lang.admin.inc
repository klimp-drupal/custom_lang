<?php
function custom_lang_settings_form($form, &$form_state) { $form = array();

$form['text'] = array(
  '#markup' => t('Check country codes at') . ' ' . l('ip-api.com', 'http://ip-api.com'),
);

$form['container'] = array(
  '#tree' => TRUE,
  '#theme' => 'custom_lang_tabular_form',
  '#title' => '',
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

