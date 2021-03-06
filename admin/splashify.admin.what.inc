<?php

/**
 * @file
 * The admin "What" tab.
 *
 * Admin functionality that determines what should show up as the splash
 * page.
 */

/**
 * "What" settings tab.
 */
function splashify_admin_what_form($form, &$form_state) {
  $config = config('splashify.settings');
  $form = array();
  $form['#config'] = 'splashify.settings';

  $form['description'] = array(
    '#markup' => '<p>' . t('What should show up as the splash page?') . '</p>',
  );

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  $form['desktop']['splashify_what_desktop_mode'] = array(
    '#type' => 'select',
    '#title' => t('Splash Mode'),
    '#options' => array(
      'random' => t('Pick random path or URL from the list'),
      'sequence' => t('Pick next path or URL from the list'),
      'template' => t('Display entered text in the site template'),
      'fullscreen' => t('Display entered text/HTML full screen'),
    ),
    '#default_value' => $config->get('splashify_what_desktop_mode'),
    '#description' => t('Determines how the content field below will be used with the splash page.'),
    '#ajax' => array(
      'callback' => 'splashify_ajax_what_desktop_mode_callback',
      'wrapper' => 'what-mode-desktop-value',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  // Set a variable that is either defined by the selection from the ajax
  // dropdown menu, or a previously saved value.
  if (isset($form_state['values']['splashify_what_desktop_mode'])) {
    $what_desktop_mode_set = $form_state['values']['splashify_what_desktop_mode'];
  }
  else {
    $what_desktop_mode_set = $config->get('splashify_what_desktop_mode');
  }

  $splashify_what_desktop_content = $config->get('splashify_what_desktop_content');

  $form['desktop']['mode_value']['begin'] = array('#markup' => '<div id="what-mode-desktop-value">');

  // If they specified the template or fullscreen option, we want to hide the
  // window size text field.
  if ($what_desktop_mode_set == 'template' || $what_desktop_mode_set == 'fullscreen') {
    // Display a format field.
    $form['desktop']['mode_value']['splashify_what_desktop_content'] = array(
      '#type' => 'text_format',
      '#title' => t('Content'),
      '#default_value' => (is_array($splashify_what_desktop_content)) ? $splashify_what_desktop_content['value'] : '',
      '#format' => (is_array($splashify_what_desktop_content)) ? $splashify_what_desktop_content['format'] : filter_default_format(),
      '#description' => t('Enter the HTML you want to use on the page.'),
      '#editor_uploads' => TRUE,
    );

    // If the template mode is selected, allow them to set a page title value.
    if ($what_desktop_mode_set == 'template') {
      $form['desktop']['mode_value']['splashify_what_desktop_pagetitle'] = array(
        '#type' => 'textfield',
        '#title' => t('Page Title'),
        '#default_value' => $config->get('splashify_what_desktop_pagetitle'),
      );
    }
  }
  else {
    // If this value is an array, the mode was changed and we want to empty
    // out the value.
    if (is_array($splashify_what_desktop_content)) {
      $splashify_what_desktop_content = '';
    }

    // Display a textarea field.
    $form['desktop']['mode_value']['splashify_what_desktop_content'] = array(
      '#type' => 'textarea',
      '#title' => t('Paths or Url Values'),
      '#default_value' => $splashify_what_desktop_content,
      '#description' => t('Enter the site paths or URLs (one on each line) to use. You can use either the Backdrop source path, or the alias. <br />To open a non-Backdrop path, use an absolute URL, i.e. http://example.com/splash.html<br />To output the full view mode for a node, use this url: _splashify_ajax/[nid] ....[nid] being the id of the node you want to display.'),
    );
  }

  $form['desktop']['mode_value']['end'] = array('#markup' => '</div>');

  /*
   * Mobile settings.
   */


  // If the mobile splash is enabled, display the mobile options.
  if ($config->get('splashify_when_mobile') == 1) {
    $form['mobile'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mobile Settings'),
    );
    $form['mobile']['splashify_what_mobile_mode'] = array(
      '#type' => 'select',
      '#title' => t('Splash Mode'),
      '#options' => array(
        'random' => t('Pick random path or URL from the list'),
        'sequence' => t('Pick next path or URL from the list'),
        'template' => t('Display entered text in the site template'),
        'fullscreen' => t('Display entered text/HTML full screen'),
      ),
      '#default_value' => $config->get('splashify_what_mobile_mode'),
      '#description' => t('Determines how the content field below will be used with the splash page.'),
      '#ajax' => array(
        'callback' => 'splashify_ajax_what_mobile_mode_callback',
        'wrapper' => 'what-mode-mobile-value',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    // Set a variable that is either defined by the selection from the ajax
    // dropdown menu, or a previously saved value.
    if (isset($form_state['values']['splashify_what_mobile_mode'])) {
      $what_mobile_mode_set = $form_state['values']['splashify_what_mobile_mode'];
    }
    else {
      $what_mobile_mode_set = $config->get('splashify_what_mobile_mode');
    }

    $splashify_what_mobile_content = $config->get('splashify_what_mobile_content');

    $form['mobile']['mode_value']['begin'] = array('#markup' => '<div id="what-mode-mobile-value">');

    // If they specified the template or fullscreen option, we want to hide the
    // window size text field.
    if ($what_mobile_mode_set == 'template' || $what_mobile_mode_set == 'fullscreen') {
      $form['mobile']['mode_value']['splashify_what_mobile_content'] = array(
        '#type' => 'text_format',
        '#title' => t('Content'),
        '#default_value' => (is_array($splashify_what_mobile_content)) ? $splashify_what_mobile_content['value'] : '',
        '#format' => (is_array($splashify_what_mobile_content)) ? $splashify_what_mobile_content['format'] : '',
        '#description' => t('Text to show or paths/URLs (one on each line) to use.  To open a non-Backdrop path, use an absolute URL, i.e. http://example.com/splash.html'),
      );

      // If the template mode is selected, allow them to set a page title value.
      if ($what_mobile_mode_set == 'template') {
        $form['mobile']['mode_value']['splashify_what_mobile_pagetitle'] = array(
          '#type' => 'textfield',
          '#title' => t('Page Title'),
          '#default_value' => $config->get('splashify_what_mobile_pagetitle'),
        );
      }
    }
    else {
      // If this value is an array, the mode was changed and we want to empty
      // out the value.
      if (is_array($splashify_what_desktop_content)) {
        $splashify_what_desktop_content = '';
      }

      // Display a textarea field.
      $form['mobile']['mode_value']['splashify_what_mobile_content'] = array(
        '#type' => 'textarea',
        '#title' => t('Paths or Url Values'),
        '#default_value' => $splashify_what_mobile_content,
        '#description' => t('Enter the site paths or URLs (one on each line) to use. You can use either the backdrop source path, or the alias. <br />To open a non-Backdrop path, use an absolute URL, i.e. http://example.com/splash.html'),
        '#editor_uploads' => TRUE,
      );
    }

    $form['mobile']['mode_value']['end'] = array('#markup' => '</div>');

  }
  else {
    $form['splashify_what_mobile_mode'] = array(
      '#markup' => '<p>' . t('In order to specify mobile options, you need to enable the "When: Enable Unique Mobile Splash" option.') . '</p>',
    );
  }

  $form['#validate'][] = 'splashify_admin_what_form_validate';
  $form['#submit'][] = 'splashify_admin_what_form_submit';
  return system_settings_form($form);
}

/**
 * Implements form validation handler.
 */
function splashify_admin_what_form_validate($form, &$form_state) {
  // If they entered paths, make sure the path values are valid.
  $desktop_mode = $form_state['values']['splashify_what_desktop_mode'];
  if ($desktop_mode == 'random' || $desktop_mode == 'sequence') {
    splashify_what_paths_check('splashify_what_desktop_content', $form_state['values']['splashify_what_desktop_content']);
  }

  if (isset($form_state['values']['splashify_what_mobile_mode'])) {
    $mobile_mode = $form_state['values']['splashify_what_mobile_mode'];
    if ($mobile_mode == 'random' || $mobile_mode == 'sequence') {
      splashify_what_paths_check('splashify_what_mobile_content', $form_state['values']['splashify_what_mobile_content']);
    }
  }
}

/**
 * Implements form submit handler.
 */
function splashify_admin_what_form_submit($form, &$form_state) {
  // Clear out the title value if we aren't using the template option.
  if ($form_state['values']['splashify_what_desktop_mode'] != 'template') {
    config_set('splashify.settings', 'splashify_what_desktop_pagetitle', '');
  }
  // Save each file as permanent, preventing it from being deleted.
  // File usages are not currently removed for CKEditor files.
  // See https://github.com/backdrop/backdrop-issues/issues/2137.
  if (isset($form_state['values']['splashify_what_desktop_content']['value'])) {
    $fids = filter_parse_file_fids($form_state['values']['splashify_what_desktop_content']['value']);
    $files = file_load_multiple($fids);
    foreach ($files as $fid => $file) {
      if ($file && $file->status !== FILE_STATUS_PERMANENT) {
        // This makes the file "self-referencing", so it will never be deleted.
        file_usage_add($file, 'file', 'file', $file->fid);
      }
    }
  }
  if (isset($form_state['values']['splashify_what_mobile_content']['value'])) {
    $fids = filter_parse_file_fids($form_state['values']['splashify_what_mobile_content']['value']);
    $files = file_load_multiple($fids);
    foreach ($files as $fid => $file) {
      if ($file && $file->status !== FILE_STATUS_PERMANENT) {
        // This makes the file "self-referencing", so it will never be deleted.
        file_usage_add($file, 'file', 'file', $file->fid);
      }
    }
  }
}

/**
 * Ajax callback for the desktop mode dropdown.
 *
 * The $form array element that is returned is the updated field that should
 * be displayed.
 */
function splashify_ajax_what_desktop_mode_callback($form, &$form_state) {
  return $form['desktop']['mode_value'];
}

/**
 * Ajax callback for the mobile mode dropdown.
 *
 * The $form array element that is returned is the updated field that should
 * be displayed.
 */
function splashify_ajax_what_mobile_mode_callback($form, &$form_state) {
  return $form['mobile']['mode_value'];
}

/**
 * Validate the what:paths field.
 *
 * We put this in a function so it can handle both the desktop and mobile
 * settings. This assumes that each path in $paths is separated by a new line
 * character. We also assume $paths cannot be blank.
 */
function splashify_what_paths_check($field, $paths) {
  if (empty($paths)) {
    form_set_error($field, t('You must enter at least one path.'));
    return;
  }

  // Make sure each path is valid.
  $what_paths = preg_split('/[\n\r]+/', $paths);
  $errors = array();
  foreach ($what_paths as $path) {
    // If this is a valid Backdrop path.
    if (backdrop_valid_path($path)) {
      continue;
    }

    // If this path is an alias, we know this is a valid path.
    if (backdrop_lookup_path('source', $path)) {
      continue;
    }

    // If this path is a source url, we know this is a valid path.
    if (backdrop_lookup_path('alias', $path)) {
      continue;
    }

    // Now check if this is a url value.
    if (substr($path, 0, 7) == 'http://') {
      continue;
    }

    // This path is not an alias or the source url.
    $errors[] .= t('The path "@path" is not valid.', array('@path' => $path));
  }

  // Since there could be multiple errors for this one field, we want to
  // break each error into a separate line.
  if (count($errors) > 0) {
    form_set_error($field, implode('<br />', $errors));
  }
}
