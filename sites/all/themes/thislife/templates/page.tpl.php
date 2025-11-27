<?php

/**
* @file
* Default theme implementation to display a single Drupal page.
*
* The doctype, html, head and body tags are not in this template. Instead they
* can be found in the html.tpl.php template in this directory.
*
* Available variables:
*
* General utility variables:
* - $base_path: The base URL path of the Drupal installation. At the very
*   least, this will always default to /.
* - $directory: The directory the template is located in, e.g. modules/system
*   or themes/bartik.
* - $is_front: TRUE if the current page is the front page.
* - $logged_in: TRUE if the user is registered and signed in.
* - $is_admin: TRUE if the user has permission to access administration pages.
*
* Site identity:
* - $front_page: The URL of the front page. Use this instead of $base_path,
*   when linking to the front page. This includes the language domain or
*   prefix.
* - $logo: The path to the logo image, as defined in theme configuration.
* - $site_name: The name of the site, empty when display has been disabled
*   in theme settings.
* - $site_slogan: The slogan of the site, empty when display has been disabled
*   in theme settings.
*
* Navigation:
* - $main_menu (array): An array containing the Main menu links for the
*   site, if they have been configured.
* - $secondary_menu (array): An array containing the Secondary menu links for
*   the site, if they have been configured.
* - $breadcrumb: The breadcrumb trail for the current page.
*
* Page content (in order of occurrence in the default page.tpl.php):
* - $title_prefix (array): An array containing additional output populated by
*   modules, intended to be displayed in front of the main title tag that
*   appears in the template.
* - $title: The page title, for use in the actual HTML content.
* - $title_suffix (array): An array containing additional output populated by
*   modules, intended to be displayed after the main title tag that appears in
*   the template.
* - $messages: HTML for status and error messages. Should be displayed
*   prominently.
* - $tabs (array): Tabs linking to any sub-pages beneath the current page
*   (e.g., the view and edit tabs when displaying a node).
* - $action_links (array): Actions local to the page, such as 'Add menu' on the
*   menu administration interface.
* - $feed_icons: A string of all feed icons for the current page.
* - $node: The node object, if there is an automatically-loaded node
*   associated with the page, and the node ID is the second argument
*   in the page's path (e.g. node/12345 and node/12345/revisions, but not
*   comment/reply/12345).
*
* Regions:
* - $page['help']: Dynamic help text, mostly for admin pages.
* - $page['highlighted']: Items for the highlighted content region.
* - $page['content']: The main content of the current page.
* - $page['sidebar_first']: Items for the first sidebar.
* - $page['sidebar_second']: Items for the second sidebar.
* - $page['header']: Items for the header region.
* - $page['footer']: Items for the footer region.
*
* @see template_preprocess()
* @see template_preprocess_page()
* @see template_process()
* @see html.tpl.php
*
* @ingroup themeable
*/
?>
<?php if (empty($app)): ?><?php print $eyebrow; print $modal; ?>
<header id="site-header" role="banner"<?php print $title_attributes; ?>>
  <div class="scrim"></div>
  <div id="player">
    <div id="jp_container_1" class="jp-audio">
      <div class="jp-type-single">

        <div class="jp-progress">
          <div class="jp-seek-bar">
            <div class="jp-play-bar"></div>
          </div>
        </div> <!-- /jp-progress -->

        <div class="jp-gui jp-interface">
          <ul class="jp-controls">
            <li><a class="jp-previous" title="Previous"><span class="icon-skip-back"></span></a></li>
            <li><a class="jp-rewind" title="Skip back"><span class="icon-rewind"></span></a></li>
            <li class="play"><a class="jp-play" title="Play" tabindex="1"><span class="icon-play"></span></a></li>
            <li class="pause"><a class="jp-pause" title="Pause" tabindex="1"><span class="icon-pause"></span></a></li>
            <li><a class="jp-forward" title="Skip ahead"><span class="icon-forward"></span></a></li>
            <li><a class="jp-next" title="Next"><span class="icon-skip-forward"></span></a></li>
          </ul>
        </div> <!-- /jp-interface -->

        <div id="fplayer" data-audio-only="true" data-audio="true"></div>

        <div class="time">
          <div class="current"><span class="jp-current-time">00:00</span></div>
          <div class="remaining"><span class="jp-remaining">00:00</span></div>
        </div>

        <ul class="actions">
          <li class="transcript"><a class="player-transcript ignore"><span class="icon icon-closed_caption"></span><span class="label">Transcript</span></a></li>
          <?php /*
          <li class="cut-this"><a href="https://shortcut.thisamericanlife.org/" class="cut"><span class="icon icon-cut"></span><span class="label">Share a clip</span></a></li>
          */ ?>
          <li class="share"><a class="player-share ignore" href="javascript:void(0)" data-micromodal-trigger="player-share-modal"><span class="icon icon-share"></span><span class="label">Share</span></a></li>
        </ul>

        <aside id="player-info">
          <header>
            <div class="image"></div>
            <div class="episode"></div>
            <div class="title"></div>
          </header>
          <div class="content">
            <div class="contributor"></div>
            <div class="body"></div>
          </div>
        </aside>

      </div> <!-- /jp-type-single -->

    </div> <!-- /jp-audio -->
    <a class="close"><span class="icon-arrow-down"></span></a>
    <div id="jplayer" class="jp-jplayer"></div>
  </div> <!-- /player -->
  <div class="container clearfix">
    <div class="site-name">
      <a href="<?php print $front_page ?>">
        <span class="flag-wrapper">
          <span class="icon icon-flag"></span>
          <?php /*
          <span class="devil">
            <span class="devil-inner">
              <span class="square"></span>
              <span class="bar-1"></span>
              <span class="bar-2"></span>
              <span class="bar-3"></span>
              <span class="triangle-1"></span>
              <span class="triangle-2"></span>
            </span>
          </span>
          */ ?>
        </span>
        <span class="icon icon-wordmark"></span><span class="element-invisible"><?php print $site_name ?></span></a>
    </div>
    <a id="burger" href="#main-menu"><span class="top"></span><span class="middle"></span><span class="bottom"></span></a>
    <nav id="main-menu">
      <?php if (!empty($menu)): ?>
        <?php print render($menu) ?>
      <?php endif; ?>
    </nav>
  </div>
</header>
<?php endif; ?>
  <div id="top">
    <?php print render($page['top']); ?>
  </div> <!-- /#top -->
<div id="content">
  <div id="sidebar" class="column sidebar">
    <?php print render($page['sidebar_first']); ?>
  </div> <!-- /#sidebar-first -->
  <main id="main">
    <?php if (empty($app) && !empty($show_title)): ?>
      <h1 id="page-title"><span><?php print $title ?></span></h1>
    <?php endif; ?>
    <?php print render($page['content']); ?>
  </main>
</div>
<!--#content-->

<?php print render($page['bottom']); ?>

<?php if (empty($app)): ?>
  <footer id="footer">
    <div class="footer-inner clearfix">
      <div class="site-name">
        <a href="<?php print $front_page ?>"><span class="icon icon-flag"></span><span class="icon icon-wordmark"></span><span class="icon icon-wbez"></span><span class="element-invisible"><?php print $site_name ?></span></a>
        <p><em>This American Life</em> is produced in collaboration with WBEZ Chicago and delivered to stations by PRX The Public Radio Exchange.</p>
      </div>
      <nav id="footer-menu">
        <?php if (!empty($footer_menu)): ?>
          <?php print render($footer_menu) ?>
        <?php endif; ?>
      </nav>
      <ul class="links">
        <li class="social facebook"><a href="https://www.facebook.com/thislife"><span class="icon-facebook"></span></a></li>
        <li class="social twitter"><a href="https://x.com/thisamerlife"><span class="icon-twitter"></span></a></li>
        <?php if ($instagram = variable_get('tal_instagram', false)): ?>
          <li class="social instagram"><a href="<?php print $instagram ?>"><span class="icon-instagram"></span></a></li>
        <?php endif; ?>
      </ul>
      <div class="copyright">
        &copy; 1995 - <?php print date('Y') ?> This American Life
        <a href="<?php print url('page/privacy-policy') ?>">Privacy Policy</a> | <a href="<?php print url('page/terms-of-use') ?>">Terms of Use</a>
      </div>
    </div>
  </footer> <!-- /#footer -->

  <div id="share-modal" class="modal" aria-hidden="true" data-micromodal-close>
    <div tabindex="-1" data-micromodal-close>
      <div class="modal-container" role="dialog" aria-modal="true" aria-labelledby="share-title">
        <h2 id="share-title"><?php print t('Share') ?></h2>
        <div class="modal-inner">
          <ul class="share-social">
            <li class="facebook">
              <a href="<?php print tal_share_url('facebook') ?>" class="shareout ignore facebook">
                <span class="icon icon-facebook"></span>
                <span class="label"><?php print t('Facebook') ?></span>
              </a>
            </li>
            <li class="twitter">
              <a href="<?php print tal_share_url('twitter') ?>" class="shareout ignore twitter">
                <span class="icon icon-twitter"></span>
                <span class="label"><?php print t('X') ?></span>
              </a>
            </li>
            <li class="mail">
              <a href="<?php print tal_share_url('mail') ?>" class="shareout ignore mail">
                <span class="icon icon-mail"></span>
                <span class="label"><?php print t('Email') ?></span>
              </a>
            </li>
          </ul>
        </div>
        <a class="close ignore" data-micromodal-close>
          <span class="icon icon-close"></span>
        </a>
      </div>
      <a class="close ignore" data-micromodal-close>
        <span class="icon icon-close"></span>
      </a>
    </div>
  </div>

  <div id="subscribe-modal" class="modal" aria-hidden="true" data-micromodal-close>
    <div tabindex="-1" data-micromodal-close>
      <div class="modal-container" role="dialog" aria-modal="true" aria-labelledby="subscribe-title">
        <h2 id="subscribe-title"><?php print t('Subscribe') ?></h2>
        <div class="modal-inner">
          <ul class="share-social">
            <li class="spotify">
              <a href="https://open.spotify.com/show/41zWZdWCpVQrKj7ykQnXRc?si=0a07e7f79db64f54" class="shareout ignore spotify">
                <span class="icon icon-spotify"></span>
                <span class="label"><?php print t('on Spotify') ?></span>
              </a>
            </li>
            <li class="apple">
              <a href="https://podcasts.apple.com/us/podcast/this-american-life/id201671138?itscg=30200&itsct=podcast_box&ls=1&mttnsubad=201671138&at=1001lJHP&ct=thisamericanlife.org+episode+Subscribe" class="shareout ignore apple">
                <span class="icon icon-podcast"></span>
                <span class="label"><?php print t('in Apple Podcasts') ?></span>
              </a>
            </li>
          </ul>
        </div>
        <a class="close ignore" aria-label="close" data-micromodal-close>
          <span class="icon icon-close"></span>
        </a>
      </div>
      <a class="close ignore" aria-label="close" data-micromodal-close>
        <span class="icon icon-close"></span>
      </a>
    </div>
  </div>

  <div id="player-share-modal" class="modal" aria-hidden="true" data-micromodal-close>
    <div tabindex="-1" data-micromodal-close>
      <div class="modal-container" role="dialog" aria-modal="true" aria-labelledby="player-share-title">
        <h2 id="player-share-title"><?php print t('Share') ?></h2>
        <div class="modal-inner">
          <ul class="share-social">
            <li class="facebook">
              <a href="<?php print tal_share_url('facebook') ?>" class="shareout ignore facebook">
                <span class="icon icon-facebook"></span>
                <span class="label"><?php print t('Facebook') ?></span>
              </a>
            </li>
            <li class="twitter">
              <a href="<?php print tal_share_url('twitter') ?>" class="shareout ignore twitter">
                <span class="icon icon-twitter"></span>
                <span class="label"><?php print t('X') ?></span>
              </a>
            </li>
            <li class="mail">
              <a href="<?php print tal_share_url('mail') ?>" class="shareout ignore mail">
                <span class="icon icon-mail"></span>
                <span class="label"><?php print t('Email') ?></span>
              </a>
            </li>
          </ul>
        </div>
        <a class="close ignore" data-micromodal-close>
          <span class="icon icon-close"></span>
        </a>
      </div>
      <a class="close ignore" data-micromodal-close>
        <span class="icon icon-close"></span>
      </a>
    </div>
  </div>

  <script type="text/javascript">
    var jplayer_swfPath = '<?php print base_path().$directory ?>/bower_components/jPlayer/dist/jplayer/';
  </script>
<?php endif; ?>
