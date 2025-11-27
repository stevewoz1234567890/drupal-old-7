<?php

/**
* @file
* Default theme implementation to format RSS extra item.
*
* Available variables:
*
*
* Page content (in order of occurrence in the default page.tpl.php):
* - $pubDate: .
* - $description (array): An array with value and filtered safe_value.
* - $enclosure: link to audio file set by .
* - $image: url to share image.
* - $tags (array): Actions local to the page, such as 'Add menu' on the
*   menu administration interface.
* - $feed_icons: A string of all feed icons for the current page.
* - $node: The node object, if there is an automatically-loaded node
*   associated with the page, and the node ID is the second argument
*   in the page's path (e.g. node/12345 and node/12345/revisions, but not
*   comment/reply/12345).
*
* @see node.tpl.php
*
* @ingroup themeable
*/
?>
<item>
  <title><?php print $node->title; ?></title>
  <description><?php print htmlentities( trim( $description['safe_value'] ) ); ?></description>
  <pubDate><?php print $pubDate; ?></pubDate>
  <guid isPermaLink="false"><?php print $node->nid; ?> at https://www.thisamericanlife.org</guid>
  <enclosure url="<?php print $enclosure; ?>" type="audio/mpeg" />
  <itunes:image href="<?php print $image; ?>" />
  <media:thumbnail url="<?php print $youtube_image; ?>" height="720" width="1280" />
<?php foreach ($tags as $tag => $value): ?>
  <<?php print $tag; ?>><?php print $value; ?></<?php print $tag; ?>>
<?php endforeach; ?>
</item>
