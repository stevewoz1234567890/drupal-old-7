<?php

/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 *
 * @ingroup themeable
 */
?><!DOCTYPE html>
 <!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
 <!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8"> <![endif]-->
 <!--[if IE 8]>         <html class="no-js lt-ie9"> <![endif]-->
 <!--[if gt IE 8]><!--> <html class="no-js"> <!--<![endif]-->
 <head>
   <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
   <meta name="viewport" content="width=device-width, height=device-height, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />

   <link rel="apple-touch-icon" sizes="180x180" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/apple-touch-icon.png?v=2">
   <link rel="icon" type="image/png" sizes="32x32" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/favicon-32x32.png">
   <link rel="icon" type="image/png" sizes="16x16" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/favicon-16x16.png">
   <link rel="manifest" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/manifest.json">
   <link rel="mask-icon" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/safari-pinned-tab.svg" color="#02135B">
   <link rel="shortcut icon" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/favicon.ico">
   <meta name="msapplication-config" content="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/browserconfig.xml">
   <meta name="theme-color" content="#02135B">
   <!--[if IE]><link rel="shortcut icon" href="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/favicons/favicon.ico?v=9BaMN9bn64"/><![endif]-->


   <?php $head; ?>
   <title><?php print $head_title; ?></title>
   <script src="https://use.typekit.net/zhh3wfx.js"></script>
   <script>try{Typekit.load({ async: false });}catch(e){}</script>
   <?php print $styles; ?>
   <!--[if lte IE 8]>
   <script type="text/javascript" src="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/js/min/html5shiv.min.js"></script>
   <script type="text/javascript" src="<?php print base_path() . drupal_get_path('theme', 'thislife') ?>/js/min/respond.min.js"></script>
   <![endif]-->
   <!-- Google Tag Manager -->
    <script>(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
    new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
    j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
    'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
    })(window,document,'script','dataLayer','GTM-5D6MWRV');</script>
    <!-- End Google Tag Manager -->
 </head>
 <body class="<?php print $classes; ?>" <?php print $attributes;?>>
   <!-- Google Tag Manager (noscript) -->
    <noscript><iframe src="https://www.googletagmanager.com/ns.html?id=GTM-5D6MWRV"
    height="0" width="0" style="display:none;visibility:hidden"></iframe></noscript>
    <!-- End Google Tag Manager (noscript) -->
   <div id="skip-link">
     <a href="#content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
   </div>
   <!--[if lt IE 7]>
   <p class="chromeframe">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> or <a href="http://www.google.com/chromeframe/?redirect=true">activate Google Chrome Frame</a> to improve your experience.</p>
   <![endif]-->
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  <?php print $scripts; ?>
  <script type="text/html" id="saved-archive"></script>
</body>
</html>
