<?php

/**
 * @file
 * Helper functions for Custom Language Detector module.
 */

/**
 * Sets language.
 *
 * @param $ip
 *   IP address.
 *
 * @see ip_address()
 */
function custom_lang_set_lang($ip) {
  $country_lang_flag = &drupal_static(__FUNCTION__);
  if ($country_lang_flag) {

    return FALSE;
  }

  $url = 'http://ip-api.com/json/';
  $response = drupal_http_request($url . $ip);

  if ($response->code == 200) {
    $data = json_decode($response->data);

    // Get module settings.
    $custom_lang_settings = variable_get('custom_lang_settings', FALSE);
    if ($custom_lang_settings) {
      if (isset($data->countryCode) && isset($custom_lang_settings[$data->countryCode])) {
        // Country settings.
        $country_settings = $custom_lang_settings[$data->countryCode];

        $langs = language_list('enabled');

        $country_lang = FALSE;
        // If more than one language has been attached to the country.
        if (count($country_settings) > 1) {

          $http_lang= '';
          $matches = array();

          // Determine user preferred language by browser variable.
          if (preg_match_all('@(?<=[, ]|^)([a-zA-Z-]+|\*)(?:;q=([0-9.]+))?(?:$|\s*,\s*)@', trim($_SERVER['HTTP_ACCEPT_LANGUAGE']), $matches, PREG_SET_ORDER)) {
            foreach ($matches as $match) {
              $http_lang = drupal_strtolower($match[1]);
              break;

            }
          }

          // Compare http language codes taken from settings with server variable.
          foreach ($country_settings as $lang_code => $http_lang_codes) {
            $http_lang_codes = explode(',', $http_lang_codes);
            $http_lang_codes = array_map('trim', $http_lang_codes);
            if (in_array($http_lang, $http_lang_codes)) {
              $country_lang = $lang_code;
              break;

            }
          }
        }
        elseif (count($country_settings) == 1) {
          $country_lang = key($country_settings);
        }

        if ($country_lang) {
          if (isset($langs[1][$country_lang])) {
            $path = current_path();
            $lang = $langs[1][$country_lang];
            user_cookie_save(array(
              'custom_lang' => TRUE,
            ));
            $country_lang_flag = TRUE;
            drupal_goto($path, array(
              'language' => $lang,
            ));
          }
        }

      }
    }
  }
}
