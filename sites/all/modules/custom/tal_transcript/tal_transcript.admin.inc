<?php

function tal_transcript_capitalization_admin($form, &$form_state) {

  $form['tal_transcript_capitalization'] = array(
    '#type' => 'textarea',
    '#title' => t('Names'),
    '#rows' => 40,
    '#description' => t('Please enter one name per line with a pipe separating the search and replacement format. Example: Lin-manuel|Lin-Manuel'),
    '#default_value' => variable_get('tal_transcript_capitalization', ''),
  );

  return system_settings_form($form);
}
