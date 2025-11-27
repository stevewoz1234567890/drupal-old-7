<figure class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($image )?>
  <?php if (!empty($caption)): ?>
    <figcaption>
      <?php print render($caption) ?>
    </figcaption>
  <?php endif; ?>
</figure>