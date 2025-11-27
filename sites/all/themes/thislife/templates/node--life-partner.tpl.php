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

<!--    START above the fold    -->
<section id="be-ours" class="bg-royal-blue py-8 md:px-8 relative overflow-y-hidden">
    <div class="container mx-auto z-10 relative">
        <div class="pt-8">
            <div class="lg:flex flex-row-reverse gap-32 py-4 lg:py-10 items-center justify-center">
                <div class="mb-8 md:mb-0 basis-96 shrink-0 flex items-center justify-center">
                    <img class="rounded-lg lg:w-[400px] w-1/2" style="aspect-ratio: 1 / 1" src="<?php echo file_create_url($logo['uri']); ?>">
                </div>
                <div class="flex flex-col space-around">
                    <div class="relative">
                        <h1 class="text-3xl md:text-5xl font-bold text-gray-200" data-preview="landing_page[headline]">
                            <?php echo $header; ?></h1>
                        <div class="py-4 text-lg">
                            <div class="text-gray-400" data-preview="landing_page[summary]">
                                <?php echo $pitch['safe_value']; ?>
                            </div>
                        </div>
                    </div>
                    <div class="flex gap-4 items-center mb-8 text-gray-200">
                        <?php echo $pitch_sub['safe_value']; ?>
                    </div>
                </div>
            </div>
            <div class="flex flex-col lg:pt-16 show-month" id="subscribe">
                <?php foreach ($levels as $id => $level): ?>
                <div id="<?php print $id; ?>" class="flex flex-wrap justify-center items-stretch">
                    <?php foreach ($level as $plan): ?>
                    <div class="flex grow-0 w-full p-4 md:w-1/2 md:basis-1/2">
                        <?php print $plan; ?>
                    </div>
                    <?php endforeach; ?>
                </div>
                <?php endforeach; ?>
            </div>
        </div>
    </div>
</section>
<!--    END above the fold    -->

<!--    START gift a subscription    -->
<section id="gift" class="bg-black py-8 px-4 md:py-8 relative overflow-y-hidden">
    <div class="container m-auto bg-black" id="gift-a-subscription">
        <div class="text-center text-white pb-8 pt-4">
            <h2 class="text-4xl mb-2 font-bold"><?php echo $break_header; ?></h2>
            <div class="pb-4 text-gray-400"><?php echo $break_text; ?></div>
            <div class="pt-6">
                <a class="btn whitespace-nowrap border border-gray-100 text-gray-100 hover:border-gray-200 hover:text-gray-200 stroke-gray-100"
                    data-controller="button-component" href="<?php echo $button_link['url']; ?>">
                    <span class="flex gap-2 items-center">
                        <?php if (isset($button_logo)) {
                            print theme_image($button_logo);
                        } ?>
                        <span class="flex-grow"><?php echo $button_link['title']; ?></span>
                    </span>
                </a>
            </div>
        </div>
    </div>
</section>
<!--    END gift a subscription    -->

<!--    START faq    -->
<section id="faq" class="bg-gray-100 py-8 px-4 md:py-8 relative overflow-y-hidden">
    <div class="container m-auto py-16 px-4 md:px-0">
        <div class="my-6 md:flex gap-32">
            <div class="md:w-1/3 flex flex-col justify-between">
                <h2 class="text-4xl mt-2 font-bold"><?php echo $faq_header; ?></h2>
                <div class="hidden md:block">
                    <div class="font-bold mb-2"><?php echo $cta; ?></div>
                    <a class="btn whitespace-nowrap btn-accent" data-controller="button-component" href="<?php echo $cta_link['url']; ?>">
                        <span class="flex gap-2 items-center">
                            <span class="flex-grow"><?php echo $cta_link['title']; ?></span>
                            <svg verion="1.1" role="img" width="48" height="48" viewBox="0 0 48 48" aria-label="Right arrow" class="h-5 w-5 text-right mt-2">
                                <path d="M23.987,12a2.411,2.411,0,0,0-.814-1.8L11.994.361a1.44,1.44,0,0,0-1.9,2.162l8.637,7.6a.25.25,0,0,1-.165.437H1.452a1.44,1.44,0,0,0,0,2.88H18.563a.251.251,0,0,1,.165.438l-8.637,7.6a1.44,1.44,0,1,0,1.9,2.161L23.172,13.8A2.409,2.409,0,0,0,23.987,12Z" fill="currentColor"></path>
                            </svg>
                        </span>
                    </a>
                </div>
            </div>
            <div class="md:w-2/3">
                <div class="space-y-4 divide-y-2 divide-gray-100 " data-controller="accordion-component"
                    data-active-styles="" data-expand-first="true" data-inactive-styles="">

                    <?php foreach ($questions as $question): ?>
                    <question class="flex flex-col py-3" data-accordion-container="true">
                        <div class="flex cursor-pointer">
                            <div class="flex-1 font-bold">
                                <?php print $question->field_title[LANGUAGE_NONE][0]['safe_value']; ?>
                            </div>
                            <div class="flex-0">
                                <svg verion="1.1" role="img" width="16" height="16" viewBox="0 0 16 16"
                                    aria-label="chevron_right" class="fill-gray-700 w-4 h-4"
                                    data-accordion-component-target="arrow">
                                    <g transform="matrix(0.6666666666666666,0,0,0.6666666666666666,0,0)">
                                        <path class="st0" d="M19.5,12c0,0.7-0.3,1.3-0.8,1.7L7.5,23.6c-0.8,0.7-2,0.6-2.6-0.2c-0.6-0.8-0.6-1.9,0.2-2.6l9.8-8.6c0.1-0.1,0.1-0.2,0-0.4c0,0,0,0,0,0L5.1,3.2C4.3,2.5,4.3,1.3,5,0.6c0.7-0.7,1.8-0.8,2.6-0.2l11.2,9.8C19.2,10.7,19.5,11.3,19.5,12z"></path>
                                    </g>
                                </svg>
                            </div>
                        </div>
                        <div class="answer py-2 hidden">
                            <?php print $question->field_body[LANGUAGE_NONE][0]['safe_value']; ?>
                        </div>
                    </question>
                    <?php endforeach; ?>

                </div>
            </div>
        </div>

    </div>
</section>
<!--    END faq    -->
