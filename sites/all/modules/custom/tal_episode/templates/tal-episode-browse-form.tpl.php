<?php print render($form['keyword']) ?>

<?php print render($form['type']) ?>

<?php print render($form['tag']) ?>

<?php print render($form['contributor']) ?>

<?php print render($form['year']) ?>

<a href="#browse-options" class="options ignore"><span class="icon-filter"></span></a>
<div class="filters">

<section class="screen" id="browse-options">
  <header>
    <h3><?php print t('Filter by') ?></h3>
    <a class="close ignore"><span class="icon-close"></span></a>
  </header>
  <?php print render($form['options']) ?>
  <div class="form-buttons clearfix">
    <?php print render($form['buttons']) ?>
  </div>
</section>

<section class="screen modal" id="browse-type">
  <header>
    <h3><?php print t('Content Type') ?></h3>
    <div class="description">(<?php print t('Choose 1') ?>)</div>
    <a class="back ignore"><span class="icon-arrow-left"></span></a>
    <a class="close ignore"><span class="icon-close"></span></a>
  </header>
  <div class="inner">
    <div class="meta">
      <div class="description"><?php print t('You may only select 1 content type') ?></div>
    </div>
    <?php print render($form['type_list']) ?>
  </div>
</section>

<section class="screen modal" id="browse-tag">
  <header>
    <h3><?php print t('Tag') ?></h3>
    <div class="description">(<?php print t('Choose 1') ?>)</div>
    <a class="back ignore"><span class="icon-arrow-left"></span></a>
    <a class="close ignore"><span class="icon-close"></span></a>
  </header>
  <div class="inner">
    <div class="meta">
      <div class="description"><?php print t('You may only select 1 tag') ?></div>
      <?php print render($form['tag_meta']) ?>
    </div>
    <?php print render($form['tag_list']) ?>
  </div>
</section>

<section class="screen modal" id="browse-contributor">
  <header>
    <h3><?php print t('Contributor') ?></h3>
    <div class="description">(<?php print t('Choose 1') ?>)</div>
    <a class="back ignore"><span class="icon-arrow-left"></span></a>
    <a class="close ignore"><span class="icon-close"></span></a>
  </header>
  <div class="inner">
    <div class="meta">
      <div class="description"><?php print t('You may only select 1 contributor') ?></div>
      <?php print render($form['contributor_meta']) ?>
    </div>
    <?php print render($form['contributor_list']) ?>
  </div>
</section>

<section class="screen modal" id="browse-year">
  <header>
    <h3><?php print t('Year') ?></h3>
    <div class="description">(<?php print t('Choose 1') ?>)</div>
    <a class="back ignore"><span class="icon-arrow-left"></span></a>
    <a class="close ignore"><span class="icon-close"></span></a>
  </header>
  <div class="inner">
    <div class="meta">
      <div class="description"><?php print t('You may only select 1 year') ?></div>
    </div>
    <?php print render($form['year_list']) ?>
  </div>
</section>
</div>

<?php print drupal_render_children($form); ?>
