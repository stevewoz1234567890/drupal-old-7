<?php

/**
 * @file
 * tal_episode.features.wysiwyg.inc
 */

/**
 * Implements hook_wysiwyg_default_profiles().
 */
function tal_episode_wysiwyg_default_profiles() {
  $profiles = array();

  // Exported profile: full_html.
  $profiles['full_html'] = array(
    'format' => 'full_html',
    'editor' => 'ckeditor',
    'settings' => array(
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Underline' => 1,
          'Strike' => 1,
          'JustifyLeft' => 1,
          'JustifyCenter' => 1,
          'JustifyRight' => 1,
          'JustifyBlock' => 1,
          'BulletedList' => 1,
          'NumberedList' => 1,
          'Link' => 1,
          'Unlink' => 1,
          'Source' => 1,
          'HorizontalRule' => 1,
          'Paste' => 1,
          'PasteText' => 1,
          'PasteFromWord' => 1,
          'Format' => 1,
        ),
        'drupal' => array(
          'media' => 1,
        ),
      ),
      'theme' => '',
      'language' => 'en',
      'toolbarLocation' => 'top',
      'resize_enabled' => 1,
      'default_toolbar_grouping' => 0,
      'simple_source_formatting' => 1,
      'acf_mode' => 0,
      'acf_allowed_content' => '',
      'css_setting' => 'theme',
      'css_theme' => 'thislife',
      'css_path' => '',
      'stylesSet' => '',
      'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
      'forcePasteAsPlainText' => 0,
      'pasteFromWordPromptCleanup' => 1,
      'pasteFromWordRemoveFontStyles' => 1,
    ),
    'preferences' => array(
      'default' => 1,
      'user_choose' => 0,
      'show_toggle' => 1,
      'add_to_summaries' => 1,
      'version' => '4.6.2.20af917',
    ),
    'name' => 'formatfull_html',
  );

  // Exported profile: simple.
  $profiles['simple'] = array(
    'format' => 'simple',
    'editor' => 'ckeditor',
    'settings' => array(
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Link' => 1,
          'Unlink' => 1,
        ),
      ),
      'theme' => '',
      'language' => 'en',
      'toolbarLocation' => 'top',
      'resize_enabled' => 1,
      'default_toolbar_grouping' => 0,
      'simple_source_formatting' => 0,
      'acf_mode' => 0,
      'acf_allowed_content' => '',
      'css_setting' => 'theme',
      'css_theme' => 'seven',
      'css_path' => '',
      'stylesSet' => '',
      'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
      'forcePasteAsPlainText' => 0,
      'pasteFromWordPromptCleanup' => 0,
      'pasteFromWordRemoveFontStyles' => 0,
    ),
    'preferences' => array(
      'add_to_summaries' => 1,
      'default' => 1,
      'show_toggle' => 1,
      'user_choose' => 0,
      'version' => '4.6.2.20af917',
    ),
    'name' => 'formatsimple',
  );

  return $profiles;
}
