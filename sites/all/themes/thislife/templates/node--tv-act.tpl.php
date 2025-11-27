<?php

/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<article class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>
  <?php if ($page): ?>
    <header<?php print $title_attributes; ?>>
      <div class="container clearfix">
        <?php if (!empty($episode_url)): ?>
          <a href="<?php print $episode_url ?>" class="full-episode">
            <div class="label"><span class="icon icon-arrow-left"></span> <?php print t('Full episode') ?></div>
            <?php print render($content['field_episode_image']) ?>
          </a>
        <?php endif; ?>
        <div class="meta">
          <?php print render($content['field_episode']) ?>
          <?php print render($content['field_act_label']) ?>
        </div>
        <div class="episode-title">
          <h1><?php print $title; ?></h1>
        </div>
        <ul class="actions">
          <?php if (!empty($content['play'])): ?>
            <li><?php print render($content['play']) ?></li>
          <?php endif; ?>
          <?php if (!empty($content['shortcut'])): ?>
            <li><?php print render($content['shortcut']) ?></li>
          <?php endif; ?>
          <?php if (empty($app)): ?>
            <li class="social">
              <a href="<?php print tal_share_url('facebook', $node) ?>" class="shareout ignore facebook">
                <span class="icon icon-facebook"></span>
              </a>
            </li>
            <li class="social">
              <a href="<?php print tal_share_url('twitter', $node) ?>" class="shareout ignore twitter">
                <span class="icon icon-twitter"></span>
              </a>
            </li>
            <li class="share"><a href="#" class="share"><span class="icon icon-share"></span></a></li>
          <?php endif; ?>
        </ul>
      </div>
    </header>
    <div class="content"<?php print $content_attributes; ?>>
      <?php print render($content['field_image']) ?>
      <?php print render($content['field_contributor']) ?>
      <?php print render($content['body']) ?>
      <?php print render($content['field_song']) ?>
      <?php print render($content['field_extras']) ?>
    </div>
  <?php elseif ($view_mode == 'episode'): ?>
    <header<?php print $title_attributes; ?>>
      <ul class="actions">
        <?php if (!empty($content['play'])): ?>
          <li><?php print render($content['play']) ?></li>
        <?php endif; ?>
        <?php if (empty($app)): ?>
          <li class="social">
            <a href="<?php print tal_share_url('facebook', $node) ?>" class="shareout ignore facebook">
              <span class="icon icon-facebook"></span>
            </a>
          </li>
          <li class="social">
            <a href="<?php print tal_share_url('twitter', $node) ?>" class="shareout ignore twitter">
              <span class="icon icon-twitter"></span>
            </a>
          </li>
        <?php endif; ?>
      </ul>
      <?php print render($content['field_act_label']) ?>
    </header>
    <div class="content"<?php print $content_attributes; ?>>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" class="goto goto-act"><?php print $title; ?></a></h2>
      <?php print render($content['field_contributor']) ?>
      <?php print render($content['body']) ?>
      <?php print render($content['field_song']) ?>
      <?php print render($content['field_extras']) ?>
    </div>
  <?php elseif ($view_mode == 'related'): ?>
    <header class="clearfix">
      <?php print render($content['image']) ?>
      <div class="container">
        <?php print render($content['episode_number']) ?>
        <?php print render($content['radio_air_date']) ?>
        <h3<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" class="goto goto-act"><?php print $episode_title; ?></a></h3>
        <?php print render($content['image']) ?>
        <h3<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" class="goto goto-act"><?php print $title; ?></a></h3>
      </div>
    </header>

  <?php elseif ($view_mode == 'collection'): ?>
    <header class="clearfix">
      <?php print render($content['episode_number']) ?>
      <?php print render($content['radio_air_date']) ?>
      <h3<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" class="goto goto-act"><?php print $episode_title; ?></a></h3>
      <?php print render($content['image']) ?>
      <h3<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" class="goto goto-act"><?php print $title; ?></a></h3>
    </header>


  <?php elseif ($view_mode == 'archive'): ?>
    <header>
        <?php print render($content['episode_number']) ?>
        <?php print render($content['radio_air_date']) ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>" class="goto goto-act"><?php print $title; ?></a></h2>
    </header>
    <?php print render($content['body']) ?>

  <?php endif ?>

</article>
