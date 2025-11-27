<?php

/**
* @file
* Default theme implementation to format eyebrow.
*
* Available variables:
*
*
* Available variables:
* - $title: The page title
* - $content (array): An array with value and filtered safe_value.
* - $bgcolor: style attribute to set background color
*   prominently.
* - $text: style attribute to set text color
*
* @see node.tpl.php
*
* @ingroup themeable
*/
?>

<div id="eyebrow">
  <div class="eyebrow-wrapper"<?php print $bgcolor; ?>>
    <div class="eyebrow-inner"<?php print $text; ?>>
      <?php print $content['safe_value']; ?>
    </div>
    <a class="close"><span class="icon-close"></span></a>
  </div>
</div>
