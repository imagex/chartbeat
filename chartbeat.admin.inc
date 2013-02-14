<?php
/**
 * @file
 * The admin file containing the menu callback function.
 */

/**
 * Menu callback to display the Chartbeat administration options.
 */
function chartbeat_admin_settings() {
  $form['chartbeat_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#description' => t('The user ID associated with your <a href="@chartbeat">
      Chartbeat</a> account.', array('@chartbeat' => 'http://chartbeat.com')),
    '#default_value' => variable_get('chartbeat_uid', ''),
  );

  $domain = variable_get('chartbeat_domain', _chartbeat_get_default_domain());
  if (empty($domain)) {
    $domain = _chartbeat_get_default_domain();
  }

  $form['chartbeat_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#description' => t('The domain to be reporting from on your account.'),
    '#default_value' => $domain,
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
    '#description' => t('To enable live stats on the content admin screen, you
      need to add an API key. if you don\'t have one, go <a href="@api">here</a>
      and click the "Add new API" button in the right corner of the table.',
       array('@api' => 'http://chartbeat.com/apikeys/')),
    '#default_value' => variable_get('chartbeat_api_key', ''),
  );
  $form['chartbeat_publishing_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chartbeat Publishing Options'),
    '#description' => t('These options are available only for Chartbeat Publishing accounts. For more information, visit <a href="@url">Chartbeat Publishing</a>.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $sections_enabled = variable_get('chartbeat_sections_enable', FALSE);
  $form['chartbeat_publishing_options']['chartbeat_sections_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable sections'),
    '#description' => !module_exists('taxonomy') ? t('The taxonomy module must be enabled to use sections. After enabling the taxonomy module, you will be able to use Chartbeat section tracking.') : (t('Enable section tracking. See <a href="@groups">here</a> for more information.', array(
      '@groups' => 'http://chartbeat.com/docs/configuration_variables/#groups',
    ))),
    '#default_value' => module_exists('taxonomy') ? $sections_enabled : FALSE,
    '#disabled' => module_exists('taxonomy') ? FALSE : TRUE,
  );

  if (module_exists('taxonomy')) {
    $form['chartbeat_publishing_options']['chartbeat_sections_enable']['#ahah'] = array(
      'path' => 'admin/settings/chartbeat/settings/publishing/sections_callback',
      'wrapper' => 'chartbeat-sections-taxonomy-vocab-wrapper',
      'method' => 'replace',
      'effect' => 'none',
    );

    $form['chartbeat_publishing_options']['chartbeat_sections_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy Vocabularies'),
      '#description' => !module_exists('taxonomy') ? t('The taxonomy module must be enabled to use sections.') : (!$sections_enabled ? t('You must enable sections support to select taxonomy vocabularies.') : t('Select which taxonomy vocabularies will be tracked as sections.')),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="chartbeat-sections-taxonomy-vocab-wrapper">',
      '#suffix' => '</div>',
    );

    if ($sections_enabled || !empty($form_state['values']['chartbeat_sections_enable'])) {
      $form['chartbeat_publishing_options']['chartbeat_sections_wrapper']['chartbeat_sections'] = array(
        '#type' => 'checkboxes',
        '#options' => _chartbeat_taxonomy_array_format(taxonomy_get_vocabularies()),
        '#default_value' => variable_get('chartbeat_sections', array()),
      );
    }
  }

  $form['chartbeat_publishing_options']['chartbeat_authors_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable authors'),
    '#description' => t('Enable author tracking. See <a href="@groups">here</a> for more information.', array(
      '@groups' => 'http://chartbeat.com/docs/configuration_variables/#groups',
    )),
    '#default_value' => variable_get('chartbeat_authors_enabled', '0'),
  );

  return system_settings_form($form);
}

/**
 * Form callback for the dashboard iframe embed page.
 */
function chartbeat_dashboard_form() {
  $url = variable_get('chartbeat_domain', '');
  $key = variable_get('chartbeat_api_key', '');
  if (!empty($url) && !empty($key)) {
    $form['dashboard_iframe'] = array(
      '#markup' => "<iframe src=\"http://chartbeat.com/dashboard/?url=$url&k=$key&slim=1\" height=700 width=1000></iframe>",
    );
  }
  else {
    $form['dashboard_iframe'] = array(
      '#markup' => 'Notice: You must save the domain and API key on the settings page to see the dashboard.',
    );
  }

  return $form;
}

/**
 * Page callback for the dashboard iframe embed page.
 */
function chartbeat_dashboard_page() {
  $url = variable_get('chartbeat_domain', '');
  $key = variable_get('chartbeat_api_key', '');
  if (!empty($url) && !empty($key)) {
    $content = "<iframe src=\"http://chartbeat.com/dashboard/?url=$url&k=$key&slim=1\" height=\"700\" width=\"100%\"></iframe>";
  }
  else {
    $content = 'Notice: You must save the domain and API key on the settings page to see the dashboard.';
  }

  return $content;
}

/**
 * Page callback for the dashboard iframe embed page.
 */
function chartbeat_publishing_dashboard_page() {
  $url = variable_get('chartbeat_domain', '');
  $key = variable_get('chartbeat_api_key', '');
  if (!empty($url) && !empty($key)) {
    $content = "<iframe src=\"http://chartbeat.com/publishing/dashboard/?url=$url&k=$key&slim=1\" height=\"700\" width=\"100%\"></iframe>";
  }
  else {
    $content = 'Notice: You must save the domain and API key on the settings page to see the dashboard.';
  }

  return $content;
}

/**
 * Callback method for AHAH enabled "Enable sections".
 *
 * AHAH callback method for handling the loading or unloading of taxonomy
 * vocabularies in a checkbox list. Outputs json using `drupal_json` and exits.
 */
function chartbeat_admin_settings_publishing_sections_callback() {
  if (!_chartbeat_is_ajax_request()) {
    return drupal_goto('admin/settings/chartbeat/settings');
  }

  if (!module_exists('taxonomy')) {
    drupal_json(array('status' => FALSE));
    exit();
  }

  $form = _chartbeat_ahah_init_helper();
  $chartbeat_sections_wrapper = $form['chartbeat_publishing_options']['chartbeat_sections_wrapper'];
  unset($chartbeat_sections_wrapper['#prefix'], $chartbeat_sections_wrapper['#suffix']);

  /* Simply gets and clears the messages - we don't want status messages rendering */
  $messages = drupal_get_messages('status', TRUE);
  $output = drupal_render($chartbeat_sections_wrapper);

  drupal_json(array('status' => TRUE, 'data' => $output));
  exit();
}

/**
 * Formats an array of vocabularies into a usable checkboxes #options value.
 *
 * @param array $taxonomies
 *   The array of taxonomy vocabularies to format.
 *
 * @return array
 *   The formatted array of vocabularies.
 */
function _chartbeat_taxonomy_array_format($taxonomies = array()) {
  $taxonomy_array = array();
  foreach ($taxonomies as $taxonomy) {
    $taxonomy_array[$taxonomy->vid] = $taxonomy->name;
  }

  return $taxonomy_array;
}
