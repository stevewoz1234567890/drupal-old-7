<?php

/**
 * TAL Page Settings form.
 *
 * @param array $form
 * @param array $form_state
 * @return void
 */
function tal_admin_page_settings_form($form, &$form_state) {
    $form['tabs'] = [
        '#type' => 'vertical_tabs',
        '#tree' => TRUE,
    ];
    
    $form['top_menu'] = [
        '#type' => 'fieldset',
        '#title' => t('Top Menu'),
        '#group' => 'tabs',
        '#weight' => 0,
    ];
    $form['top_menu']['menu_button_info'] = array(
        '#markup' => '<h2>Top Nav Action Button</h2>This is a link that is visible as a button in the main site menu only on desktop.',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#title' => t('Top Nav Action Button'),
    );
    $form['top_menu']['menu_button_label'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_menu_button_label', 'Merch'),
        '#group' => 'menu_button_group',
        '#title' => t('Button label'),
    );
    $form['top_menu']['menu_button_link'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_menu_button_link', 'https://store.thisamericanlife.org'),
        '#group' => 'menu_button_group',
        '#title' => t('Button link'),
    );

    $form['footer_menu'] = [
        '#type' => 'fieldset',
        '#title' => t('Footer Menu'),
        '#group' => 'tabs',
        '#weight' => 1,
    ];
    $form['footer_menu']['footer_link_info'] = array(
        '#markup' => '<h2>Footer Link</h2>This is a link that appears as the last in the footer menu.',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#title' => t('Footer Link'),
    );
    $form['footer_menu']['footer_link_label'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_footer_link_label', ''),
        '#group' => 'footer_link_group',
        '#title' => t('Link label'),
    );
    $form['footer_menu']['footer_link'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_footer_link', ''),
        '#group' => 'footer_link_group',
        '#title' => t('Link url'),
    );

    $form['mobile_menu'] = [
        '#type' => 'fieldset',
        '#title' => t('Mobile Menu'),
        '#group' => 'tabs',
        '#weight' => 2,
    ];
    $form['mobile_menu']['menu_mobile_info'] = array(
        '#markup' => '<h2>Mobile Menu Top Link</h2>This link will be first in the mobile menu.  It will only render if label and url are set.',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#title' => t('Menu Action Button'),
    );
    $form['mobile_menu']['menu_mobile_label'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_menu_mobile_label', ''),
        '#group' => 'menu_button_group',
        '#title' => t('Link label'),
    );
    $form['mobile_menu']['menu_mobile_link'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_menu_mobile_link', ''),
        '#group' => 'menu_button_group',
        '#title' => t('Link url'),
    );
    $form['mobile_menu']['menu_mobile_color'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('tal_menu_mobile_color', ''),
        '#group' => 'menu_button_group',
        '#title' => t('Link color'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Configuration'),
    );
    return $form;
}

/**
 * Submit Page settings form.
 *
 * @param array $form
 * @param array $form_state
 * @return void
 */
function tal_admin_page_settings_form_submit(&$form, &$form_state) {
    $keys = [
      'menu_button_label',
      'menu_button_link',
      'menu_mobile_label',
      'footer_link_label',
      'footer_link',
      'menu_mobile_link',
      'menu_mobile_color',
    ];
    tal_admin_process_form_values($keys, $form_state['values']);
}

define('MEGA_OPS', [
    'audioFileProcessing',
    'episodeCreate',
    'episodeInitiateUpload',
    'episodeUpdate',
    'episodeUpdateMidroll',
    'episodeDelete',
]);
define('MEGA_RESULTS', [
    'error',
    'success',
]);
/**
 * TAL Email Notifications Settings form.
 *
 * @param array $form
 * @param array $form_state
 * @return void
 */
function tal_admin_email_settings_form($form, &$form_state) {
    $form['preamble'] = [
        '#markup' => '<h1>Email Notification Settings</h1>' .
        '<p>Seperate all email addresses with commas. Like so:</p>' .
        '<ul><li>example@realwebsite.com,fakename@thislife.org</li>' .
        '<li>E. Sample&lt;example@realwebsite.com&gt;,Imma Fake&lt;fakename@thislife.org&gt;</li></ul>',
    ];
    
    $form['tabs'] = [
        '#type' => 'vertical_tabs',
        '#tree' => TRUE,
    ];
    foreach (MEGA_RESULTS as $i => $result) {
        $form_key = $result."_notify";
        $form[$form_key] = [
            '#type' => 'fieldset',
            '#title' => 'Megaphone API ' . t(ucfirst($result)),
            '#group' => 'tabs',
            '#weight' => $i,
        ];
        foreach (MEGA_OPS as $op) {
            $op_key = "megaphone_client_mailto_$result" . "_" . strtolower($op);
            $title =  ucfirst(preg_replace('/([A-Z])/', ' $1', $op)) . " ". ucfirst($result);
            $default = variable_get($op_key);
            $form[$form_key][$op_key] = array(
                '#type' => 'textfield',
                '#default_value' => $default,
                '#title' => t($title),
            );
        }
    }

    $form['nyt_notify'] = [
        '#type' => 'fieldset',
        '#title' => 'NYT Weekly Update Email',
        '#group' => 'tabs',
        '#weight' => $i++,
    ];
    $form['nyt_notify']['nyt_email_update_mailto'] = array(
        '#type' => 'textfield',
        '#default_value' => variable_get('nyt_email_update_mailto'),
        '#title' => t('Email address(es)'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Configuration'),
    );
    return $form;
}

/**
 * Submit Page settings form.
 *
 * @param array $form
 * @param array $form_state
 * @return void
 */
function tal_admin_email_settings_form_submit(&$form, &$form_state) {
    $keys = ['nyt_email_update_mailto'];
    foreach (MEGA_RESULTS as $result) {
        foreach (MEGA_OPS as $op) {
            $keys[] = "megaphone_client_mailto_$result" . "_" . strtolower($op);
        }
    }
    tal_admin_process_form_values($keys, $form_state['values']);
}
/**
 * Save form values to drupal db variables.
 *
 * @param array $keys
 * @param array $values
 * @return void
 */
function tal_admin_process_form_values( $keys = [], $values = []) {
    foreach ($keys as $key) {
        if (isset($values[$key]) && !empty($values[$key])) {
            variable_set($key, $values[$key]);
        } else {
            variable_del($key);
        }
    }
}