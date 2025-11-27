<figure class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($link): ?>
    <a href="<?php print url('node/'.$node->nid) ?>" class="thumbnail goto goto-<?php print $node->type ?>">
      <video autoplay loop muted playsinline>
        <source type="video/mp4" src="<?php print $video ?>">
      </video>
    </a>
  <?php else: ?>
    <video autoplay loop muted playsinline>
      <source type="video/mp4" src="<?php print $video ?>">
    </video>
  <?php endif; ?>
  <?php if (!empty($caption)): ?>
    <figcaption>
      <?php print render($caption) ?>
    </figcaption>
  <?php endif; ?>
</figure>
