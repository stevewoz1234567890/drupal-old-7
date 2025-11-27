<?php

/**
* @file
* Default theme implementation to format modal takeover.
*
* Available variables:
*
*
* Page content (in order of occurrence in the default page.tpl.php):
* - $title: The page title.
* - $content (array): An array with value and filtered safe_value.
* - $mobile_image:
* - $image:
* - $cta_link:
*
* @see node.tpl.php
*
* @ingroup themeable
*/
?>
<div id="takeover" aria-hidden="true" class="modal" data-micromodal-close>
  <div tabindex="-1" data-micromodal-close>
    <div role="dialog" aria-modal="true" aria-labelledby="modal-takeover-title" class="modal-container">
        <h2 id="modal-takeover-title" style="height: 1em;"></h2>
        <div class="modal-inner">
            <div id="modal-takeover-content">
                <div>
                  <div class="image mobile_image"><?php print $mobile_image; ?></div>
                  <div class="text">
                    <?php print $content['safe_value']; ?>
                    <div class="cta_link">
                      <a href="<?php print $cta_link['url']; ?>" class="cta_link">
                        <span>
                          <span><?php print $cta_link['title']; ?></span>
                          <svg verion="1.1" aria-label="Right arrow"><path d="M23.987,12a2.411,2.411,0,0,0-.814-1.8L11.994.361a1.44,1.44,0,0,0-1.9,2.162l8.637,7.6a.25.25,0,0,1-.165.437H1.452a1.44,1.44,0,0,0,0,2.88H18.563a.251.251,0,0,1,.165.438l-8.637,7.6a1.44,1.44,0,1,0,1.9,2.161L23.172,13.8A2.409,2.409,0,0,0,23.987,12Z" fill="currentColor"></path></svg>
                        </span>
                      </a>
                    </div>
                  </div>
                  <div class="image">
                    <a href="<?php print $cta_link['url']; ?>">
                      <?php print $image; ?>
                    </a>
                  </div>
                </div>
            </div>
        </div>
        <a class="close ignore" data-micromodal-close>
          <span class="icon icon-close"></span>
        </a>
    </div>
  </div>
</div>
