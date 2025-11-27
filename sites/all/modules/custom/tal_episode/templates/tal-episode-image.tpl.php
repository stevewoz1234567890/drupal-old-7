<figure class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($link): ?>
    <a href="<?php print url('node/'.$node->nid) ?>" class="thumbnail goto goto-<?php print $node->type ?>">
      <?php print render($image) ?>
    </a>
  <?php else: ?>
    <?php print render($image) ?>
  <?php endif; ?>
  <?php if (!empty($caption)): ?>
    <figcaption>
      <?php print render($caption) ?>
    </figcaption>
  <?php endif; ?>
</figure>

<?php if (!empty($mobile_image_url)): ?>
  <style type="text/css" media="all">
    .node.node-episode figure.episode-image.shifter .image {
      background-image: url(<?php print $mobile_image_url ?>);
    }
    @media only screen and (min-width: 768px) {
      .node.node-episode figure.episode-image.shifter .image {
        background-image: url(<?php print $image_url ?>);
        <?php if (!empty($image_padding)): ?>
        padding-top: <?php print $image_padding ?>% !important;
        <?php endif; ?>
      }
    }
  </style>
<?php endif; ?>
