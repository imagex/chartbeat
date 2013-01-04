<?php
/**
 * @file
 * The admin file containing the menu callback function.
 */

/**
 * Menu callback for the String Overrides module to display its administration
 */
function chartbeat_admin_settings() {
  $form['chartbeat_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#description' => t('The user ID associated with your <a href="@chartbeat">
      Chartbeat</a> account. You can locate it within step two of adding a new
      website to track at ChartBeat.',
      array('@chartbeat' => 'http://chartbeat.com')),
    '#default_value' => variable_get('chartbeat_uid', ''),
  );
  $form['chartbeat_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#description' => t('The domain to be reporting from on your account.'),
    '#default_value' => variable_get('chartbeat_domain', ''),
  );
  $form['chartbeat_options_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['chartbeat_options_advanced']['chartbeat_use_canonical'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Canonical'),
    '#description' => t('If your site defines &lt;link rel="canonical"
      .../&gt;, you can set check this value to to make Chartbeat use the
      canonical path instead of the actual URL.'),
    '#default_value' => variable_get('chartbeat_use_canonical', '1'),
  );
  $form['chartbeat_options_advanced']['chartbeat_cookies'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Cookies'),
    '#description' => t('Customers who are subject to the EU e-Privacy Directive
      can set this variable to prevent Chartbeat from using cookies. NOTE: By
      using Chartbeat without cookies, you will be unable to see if a user is
      new or returning.'),
    '#default_value' => variable_get('chartbeat_cookies', '1'),
  );
  $form['chartbeat_options_live'] = array(
    '#type' => 'fieldset',
    '#title' => t('Live Stats'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['chartbeat_options_live']['chartbeat_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Chartbeat API Key'),
    '#description' => t('To enable live stats, you need to add an API key. if
      you don\'t have one, go <a href="@api">here</a> and click the "Add new
       API" button in the right corner of the table.',
       array('@api' => 'http://chartbeat.com/apikeys/')),
    '#default_value' => variable_get('chartbeat_api_key', ''),
  );
  $form['chartbeat_publishing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chartbeat Publishing Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These options are only available to Chartbeat Publishing accounts. For more
      information, visit <a href=http://chartbeat.com/publishing/>Chartbeat Publishing</a>.'),
  );
  $form['chartbeat_publishing']['chartbeat_sections_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable sections'),
    '#description' => t('Enables section tracking. See <a href="@groups">here</a>
      for more information.',
      array('@groups' => 'http://chartbeat.com/docs/configuration_variables/#groups')),
    '#default_value' => variable_get('chartbeat_sections_enable', '0'),
  );
  if (module_exists('taxonomy')) {
    $form['chartbeat_publishing']['chartbeat_sections'] = array(
      '#type' => 'checkboxes',
      '#options' => _vocab_array_format(taxonomy_get_vocabularies()),
      '#title' => t('Current Vocabularies'),
      '#disabled' => variable_get('chartbeat_sections_enable', FALSE) ? FALSE : TRUE,
      '#description' => t('Select which taxonomy terms will be tracked as sections.'),
    );
  }

  return system_settings_form($form);
}

/**
 * Page callback for the dashboard iframe embed page.
 */
function chartbeat_dashboard_page() {
  $url = variable_get('chartbeat_domain', '');
  $key = variable_get('chartbeat_api_key', '');
  if (!empty($url) && !empty($key)) {
    $content = "<iframe src=\"http://chartbeat.com/dashboard/?url=$url&k=$key&slim=1\" height=700 width=1000></iframe>";
  }
  else {
    $content = 'Notice: You must save the domain and API key on the settings page to see the dashboard.';
  }
  $variables = array(
    '#type' => 'markup',
    '#markup' => $content,
  );

  return render($variables);
}

/**
 * Helper function to format the list of vocabularies properly
 *
 * @param array $array
 *   array of vocabularies
 *
 * @return array
 *   reformatted array of vocabulary names, keyed by VID
 */
function _vocab_array_format($array = NULL) {
  $vocab_array = array();
  foreach ($array as $vocab) {
    $vocab_array[$vocab->vid] = $vocab->name;
  }
  return $vocab_array;
}
