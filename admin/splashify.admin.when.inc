<?php

/**
 * @file
 * The admin "When" tab.
 *
 * Admin functionality that determines when the splash page should show up
 * on the website.
 */

/**
 * "When" settings tab.
 */
function splashify_admin_when_form($form, &$form_state) {
  $config = config('splashify.settings');
  $roles = user_roles(TRUE);
  $form_roles = array();

  foreach ($roles as $k => $v) {
    $form_roles[$k] = $v;
  }

  $form = array();
  $form['#config'] = 'splashify.settings';

  $form['description'] = array(
    '#markup' => '<p>' . t('When should the splash page show up? This is also where the main mobile option is defined.') . '</p>',
  );

  $form['splashify_when_anonymous'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show to Anonymous users'),
    '#default_value' => $config->get('splashify_when_anonymous'),
  );

  // Add control to disable referrer check
  // @see splashify_init.js
  $form['disable_referrer_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable referrer check'),
    '#default_value' => $config->get('disable_referrer_check'),
    '#description' => t('Show splash page even when page was loaded from an internal page.<br /><strong>WARNING:</strong> Be very careful with this setting when you use the Always option and have the splash page displayed on every page! This would prevent you from accessing the admin.'),
  );

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  $form['desktop']['splashify_when_desktop_frequency'] = array(
    '#type' => 'select',
    '#title' => t('How often should visitors see the splash page?'),
    '#default_value' => $config->get('splashify_when_desktop_frequency'),
    '#options' => array(
      'never' => t('Never (off)'),
      'always' => t('Always'),
      'once' => t('Once'),
      'daily' => t('Daily'),
      'weekly' => t('Weekly'),
    ),
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display for roles'),
    '#states' => array(
      'visible' => array(
        ':input[name="splashify_when_anonymous"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['roles']['splashify_when_roles'] = array(
    '#type' => 'checkbox',
    '#title' => 'Restrict to selected roles',
    '#default_value' => $config->get('splashify_when_roles'),
  );

  $form['roles']['splashify_when_roles_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Which roles should this apply to'),
    '#default_value' => $config->get('splashify_when_roles_options'),
    '#options' => $form_roles,
    '#description' => t('Select the roles for which it will apply'),
  );

  $form['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
  );

  if (module_exists('mobile_detect')) {
    include_once (backdrop_get_path('module', 'mobile_detect') . '/library/Mobile_Detect.php');
    $form['mobile']['splashify_when_mobile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Unique Mobile Splash'),
      '#default_value' => $config->get('splashify_when_mobile'),
      '#disabled' => FALSE,
      '#ajax' => array(
        'callback' => 'splashify_ajax_when_mobile_callback',
        'wrapper' => 'when-mobile-settings',
        'method' => 'replace',
        'effect' => 'slide',
      ),
    );

    // Set a variable that is either defined by the ajax checkbox, or a
    // previously saved value.
    if (isset($form_state['values']['splashify_when_mobile'])) {
      $when_mobile_set = $form_state['values']['splashify_when_mobile'];
    }
    else {
      $when_mobile_set = $config->get('splashify_when_mobile');
    }

    $form['mobile']['options']['begin'] = array('#markup' => '<div id="when-mobile-settings">');

    // If the mobile splash is enabled, display the mobile options.
    if ($when_mobile_set == 1) {
      $form['mobile']['options']['splashify_when_mobile_frequency'] = array(
        '#type' => 'select',
        '#title' => t('How often should mobile visitors see the mobile splash page?'),
        '#default_value' => $config->get('splashify_when_mobile_frequency'),
        '#options' => array(
          'never' => t('Never (off)'),
          'always' => t('Always'),
          'once' => t('Once'),
          'daily' => t('Daily'),
          'weekly' => t('Weekly'),
        ),
      );

      $form['mobile']['options']['splashify_when_mobile_test'] = array(
        '#type' => 'checkbox',
        '#title' => t('Test Mobile'),
        '#default_value' => $config->get('splashify_when_mobile_test'),
        '#description' => t('Turn this option on to force the mobile settings to take affect so you can test from your desktop browser.'),
      );
    }

    $form['mobile']['options']['end'] = array('#markup' => '</div>');
  }
  else {
    $form['mobile']['description'] = array('#markup' => t('In order to enable the mobile splash, you need to have the Mobile Detect module installed and enabled.'));
  }

  $form['#submit'][] = 'splashify_admin_when_form_submit';
  return system_settings_form($form);
}

/**
 * Implements form submit handler.
 */
function splashify_admin_when_form_submit($form, &$form_state) {
  if (isset($form_state['values']['splashify_when_mobile'])) {
    if ($form_state['values']['splashify_when_mobile'] != 1) {
      // Unset these values, if the mobile option is disabled.
      config_set('splashify.settings', 'splashify_when_mobile_frequency', 'never');
      config_set('splashify.settings', 'splashify_when_mobile_test', 0);
    }
  }

  if (isset($form_state['values']['splashify_when_anonymous'])) {
    if ($form_state['values']['splashify_when_anonymous'] == 1) {
      $form_state['values']['splashify_when_roles'] = FALSE;
    }
  }

  if ($form_state['values']['splashify_when_roles'] !== 1) {
    config_set('splashify.settings', 'splashify_when_roles_options', 0);
  }
}

/**
 * Ajax callback for the "enable mobile" checkbox.
 */
function splashify_ajax_when_mobile_callback($form, &$form_state) {
  return $form['mobile']['options'];
}
