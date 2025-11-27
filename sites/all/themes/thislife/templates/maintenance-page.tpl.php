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

   <link rel="apple-touch-icon" sizes="180x180" href="<?php print base_path() . path_to_theme() ?>/favicons/apple-touch-icon.png?v=2">
   <link rel="icon" type="image/png" sizes="32x32" href="<?php print base_path() . path_to_theme() ?>/favicons/favicon-32x32.png">
   <link rel="icon" type="image/png" sizes="16x16" href="<?php print base_path() . path_to_theme() ?>/favicons/favicon-16x16.png">
   <link rel="manifest" href="<?php print base_path() . path_to_theme() ?>/favicons/manifest.json">
   <link rel="mask-icon" href="<?php print base_path() . path_to_theme() ?>/favicons/safari-pinned-tab.svg" color="#02135B">
   <link rel="shortcut icon" href="<?php print base_path() . path_to_theme() ?>/favicons/favicon.ico">
   <meta name="msapplication-config" content="<?php print base_path() . path_to_theme() ?>/favicons/browserconfig.xml">
   <meta name="theme-color" content="#02135B">
   <!--[if IE]><link rel="shortcut icon" href="<?php print base_path() . path_to_theme() ?>/favicons/favicon.ico?v=9BaMN9bn64"/><![endif]-->


   <?php print $head; ?>
   <title>Be right back</title>
   <script src="https://use.typekit.net/zhh3wfx.js"></script>
   <script>try{Typekit.load({ async: false });}catch(e){}</script>
   <?php print $styles; ?>
   <!--[if lte IE 8]>
   <script type="text/javascript" src="<?php print base_path() . path_to_theme() ?>/js/min/html5shiv.min.js"></script>
   <script type="text/javascript" src="<?php print base_path() . path_to_theme() ?>/js/min/respond.min.js"></script>
   <![endif]-->
 </head>
 <body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <?php print $page_top; ?>

  <header id="site-header" role="banner"<?php print $title_attributes; ?>>
    <div class="container clearfix">
      <div class="site-name">
        <a href="<?php print $front_page ?>"><span class="icon icon-flag"></span><span class="icon icon-wordmark"></span><span class="element-invisible"><?php print $site_name ?></span></a>
      </div>
    </div>
  </header>
  <div id="content">
    <main id="main">
      <p>We're doing a little housekeeping. Be right back.</p>
    </main>
  </div>
  <!--#content-->
  <!--
  <?php print $messages ?>
  -->
  <footer id="footer">
    <div class="footer-inner clearfix">
      <div class="site-name">
        <a href="<?php print $front_page ?>"><span class="icon icon-flag"></span><span class="icon icon-wordmark"></span><span class="icon icon-wbez"></span><span class="element-invisible"><?php print $site_name ?></span></a>
        <p><em>This American Life</em> is produced in collaboration with WBEZ Chicago and delivered to stations by PRX The Public Radio Exchange.</p>
      </div>
      <ul class="links">
        <li class="social facebook"><a href="https://www.facebook.com/thislife"><span class="icon-facebook"></span></a></li>
        <li class="social twitter"><a href="https://x.com/thisamerlife"><span class="icon-twitter"></span></a></li>
      </ul>
      <div class="copyright">
        &copy; 1995 - <?php print date('Y') ?> This American Life
      </div>
    </div>
  </footer> <!-- /#footer -->


  <?php print $page_bottom; ?>
  <?php print $scripts; ?>
</body>
</html>
