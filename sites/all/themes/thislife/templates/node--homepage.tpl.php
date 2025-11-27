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

  <?php print render($content['this_week']) ?>

  <?php print render($content['field_banner']) ?>

  <?php if (!empty($content['promo'])) { print $content['promo']; } ?>

  <?php if (!empty($content['field_featured'])): ?>
    <section class="featured clearfix">
      <h2><span><?php print t('Featured') ?></span></h2>
      <?php print render($content['field_featured']) ?>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['recently_aired'])): ?>
    <section class="recently-aired">
      <h2><span><?php print t('Recently Aired') ?></span></h2>
      <?php print l(t('View archive'), 'archive', array('attributes' => array('class' => array('view-all')))); ?>
      <div class="nodes">
        <?php print render($content['recently_aired']) ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['field_collections'])): ?>
    <section class="recommended">
      <h2><span><?php print t('Recommended') ?></span></h2>
      <?php print render($content['field_notes']) ?>
      <?php print render($content['field_collections']) ?>
    </section>
  <?php endif; ?>

  <div class="bottom">
    <div class="inner clearfix">
      <div class="section newsletter">
        <h3>Sign up for our newsletter for weekly news and updates.</h3>
        <div id="mc_embed_signup">
        <form action="//thisamericanlife.us2.list-manage.com/subscribe" method="post" id="mc-embedded-subscribe-form" name="mc-embedded-subscribe-form" class="validate" target="_blank" novalidate>
        <input type="hidden" name="u" value="231d7e24815c65f94bf421633">
        <input type="hidden" name="id" value="09eaca450d">
        <div id="mc_embed_signup_scroll">
        <div class="mc-field-group">
        <label for="mce-EMAIL" class="element-invisible">Subscribe to our newsletter</label>
        <input type="email" value="" name="EMAIL" class="required email" id="mce-EMAIL" placeholder="Email address">
        </div>
        <div style="position: absolute; left: -5000px;" aria-hidden="true"><input type="text" name="b_231d7e24815c65f94bf421633_09eaca450d" tabindex="-1" value=""></div>
        <div class="clear"><input type="submit" value="Subscribe" name="subscribe" id="mc-embedded-subscribe" class="button form-submit"></div>
        </div>
        </form>
        </div>
      </div>
      <div class="section store">
        <a href="https://store.thisamericanlife.org/" class="image"><img src="<?php print base_path().$directory ?>/img/tote.jpg"></a>
        <h3><?php print t('Visit our store for shirts, posters, tote bags, and more.') ?></h3>
        <a href="https://store.thisamericanlife.org/" class="shop-now">Shop now</a>
      </div>
    </div>
  </div>

</article>
