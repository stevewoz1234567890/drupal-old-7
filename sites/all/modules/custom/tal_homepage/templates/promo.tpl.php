<?php

/**
* @file
* Default theme implementation to format homepage promo.
*
* Available variables:
*
*
* Available variables:
* - $header: Section header.
* - $content (array): An array with value and filtered safe_value.
* - $cta
* - $mobile_image:
* - $image:
* - $image_link
* - $bgcolor: style attribute to set background color
*   prominently.
* - $text: style attribute to set text color
*
* @see node.tpl.php
*
* @ingroup themeable
*/
?>
<section class="promo"<?php print $bgcolor; ?>>
  <div>
    <div class="image mobile_image">
      <?php if (isset($image_link)): ?>
      <a href="<?php print $image_link['url'];?>"<?php foreach ($image_link['attributes'] as $k => $v) { print " $k='$v'"; } ?>><?php endif; ?>
      <?php print $mobile_image; ?>
      <?php if (isset($image_link)): ?></a><?php endif; ?>
    </div>
    <div class="text"<?php print $text; ?>>
      <h2><?php print $header; ?></h2>
      <div class="content"><?php print $content; ?></div>
      <div class="cta">
        <?php print $cta; ?>
      </div>
    </div>
    <div class="image">
      <?php if (isset($image_link)): ?>
      <a href="<?php print $image_link['original_url'];?>"<?php foreach ($image_link['attributes'] as $k => $v) { print " $k='$v'"; } ?>><?php endif; ?>
      <?php print $image; ?>
      <?php if (isset($image_link)): ?></a><?php endif; ?>
    </div>
  </div>
</section>
