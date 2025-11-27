/*global Drupal, jQuery, Mustache, window, console, AndroidInterface, jplayer_swfPath, ga */

window.addEventListener("load", function () {
  if (typeof window.cookieconsent !== "undefined") {
    window.cookieconsent.initialise({
      palette: {
        popup: {
          background: "#ffffff",
          text: "#191919",
        },
        button: {
          background: "#ffffff",
          text: "#191919",
        },
      },
      theme: "edgeless",
      content: {
        message:
          'We use cookies and other tracking technologies to enhance your browsing experience. If you continue to use our site, you agree to the use of such cookies. For more info, see our <a href="https://www.thisamericanlife.org/page/privacy-policy">privacy policy</a>.',
        dismiss: "",
        link: false,
        href: "https://www.thisamericanlife.org/page/privacy-policy",
      },
    });
  }
});

var isMobile = {
  Android: function () {
    if (navigator.userAgent.match(/Android/i)) {
      if (typeof AndroidInterface !== "undefined") {
        return true;
      }
    }
    return false;
  },
  iOS: function () {
    if (navigator.userAgent.match(/iPhone|iPad|iPod/i)) {
      if (
        typeof window.webkit !== "undefined" &&
        typeof window.webkit.messageHandlers !== "undefined"
      ) {
        return true;
      }
    }
    return false;
  },
};

// Android
if (isMobile.Android()) {
  document.body.classList.add("android");
  window.addEventListener("talPlay", function (e) {
    AndroidInterface.talPlay(JSON.stringify(e.detail));
  });
  window.addEventListener("talGoto", function (e) {
    AndroidInterface.talGoto(JSON.stringify(e.detail));
  });
}

// iOS
if (/(Mac|iPhone|iPod|iPad)/i.test(navigator.platform)) {
  document.body.classList.add("ios");
}
if (isMobile.iOS()) {
  window.addEventListener("talPlay", function (e) {
    window.webkit.messageHandlers.talPlay.postMessage(JSON.stringify(e.detail));
  });
  window.addEventListener("talGoto", function (e) {
    var detail = e.detail,
      path = detail.url;
    if (path[0] === "/") {
      detail.url = "https://" + window.location.hostname + path;
    }
    window.webkit.messageHandlers.talGoto.postMessage(JSON.stringify(detail));
  });
}

(function ($) {
  $.fn.matchHeight._maintainScroll = true;

  $(document).ready(function () {
    $("body").addClass("loaded");
  });

  Drupal.behaviors.thislife = {
    attach: function () {},
  };

  Drupal.behaviors.thislifePlayer = {
    attach: function () {
      var $body = $("body"),
        $header = $("#site-header"),
        $burger = $("#burger"),
        $nav = $("#main-menu"),
        $saved_archive = $("#saved-archive"),
        open_menu = false,
        isMobile = function () {
          try {
            document.createEvent("TouchEvent");
            return true;
          } catch (e) {
            return false;
          }
        };

      $nav.on("click", function (e) {
        e.stopPropagation();
      });

      $burger.on("click", function (e) {
        e.preventDefault();
        e.stopPropagation();
        if (open_menu) {
          open_menu = false;
          $body.removeClass("open-menu");
        } else {
          open_menu = true;
          $body.addClass("open-menu");
        }
      });

      if (isMobile()) {
        $body.swipe({
          swipeRight: function () {
            if (open_menu) {
              open_menu = false;
              $body.removeClass("open-menu");
            }
          },
          threshold: 20,
        });
      }

      $header.on("click", ".scrim", function (e) {
        if (open_menu) {
          open_menu = false;
          $body.removeClass("open-menu");
          e.preventDefault();
          e.stopPropagation();
        }
      });

      var $main = $("#main"),
        $sidebar = $("#sidebar"),
        $top = $("#top"),
        $jp = $("#jplayer"),
        $player = $("#player"),
        $progress = $player.find(".jp-progress"),
        $info = $player.find("#player-info"),
        $share_modal = $("#share-modal"),
        $subscribe_modal = $("#subscribe-modal"),
        $player_share_modal = $("#player-share-modal"),
        last_position = 0,
        playing = false,
        ready = false,
        current_episode = false,
        current_act = 0,
        last_path = window.location.pathname + window.location.search,
        $window = $(window),
        scrollTop = 0,
        windowHeight = 0,
        scrolled = false,
        adOffsets = [],
        viewSize = function () {
          var width = $window.width(),
            newview =
              width >= 1024 ? "desktop" : width >= 768 ? "tablet" : "mobile";
          var classes = $body.attr("class").split(" ");
          $.each(classes, function (i, c) {
            if (c.indexOf("view-") === 0) {
              $body.removeClass(c);
            }
          });
          $body.addClass("view-" + newview);
          return newview;
        },
        view = viewSize(),
        updatePage = function () {
          scrollTop = $window.scrollTop();
          animateElements();
          window.requestAnimationFrame(updatePage);
        },
        animateElements = function () {
          if (!scrolled && scrollTop > 0) {
            scrolled = true;
            $body.addClass("scrolled");
          } else if (scrolled && scrollTop <= 0) {
            scrolled = false;
            $body.removeClass("scrolled");
          }
        },
        setAudioSource = function (act) {
          if (ready && current_episode) {
            const data = $jp.data("jPlayer");
            if (act.number == 0 && data.status.src !== current_episode.audio) {
              $jp.jPlayer("setMedia", { mp3: current_episode.audio });
            }
            if (act.number > 0 && data.status.src !== current_episode.archive) {
              $jp.jPlayer("setMedia", { mp3: current_episode.archive });
            }
          }
        },
        playAct = function (act) {
          var data = current_episode,
            current_timestamp = 0;
          $("a.play").removeClass("playing");
          $info.find(".title").html("").hide();
          if (typeof act !== "undefined") {
            current_act = act;
          } else {
            current_act = 0;
          }
          if (typeof current_episode.acts !== "undefined") {
            $.each(current_episode.acts, function (i, act) {
              if (act.number === current_act) {
                setAudioSource(act);
                current_timestamp = act.timestamp ? act.timestamp : 0;
                $info
                  .find(".title")
                  .html(act.name)
                  .css("display", "inline-block");
              }
            });
          }

          $jp.jPlayer("play", current_timestamp);
          $(
            "a.play-" +
              current_episode.episode +
              ", a.play-" +
              current_episode.episode +
              "-" +
              current_act
          ).addClass("playing");
        },
        playEpisode = function (act) {
          var data = current_episode,
            current_timestamp = 0;
          $("a.play").removeClass("playing");
          $info.find(".episode").html(data.title);
          $info.find(".image").html($("<img/>").attr("src", data.thumbnail));
          $info.find(".title").html("").hide();
          $progress.find(".jump").remove();
          if (typeof act !== "undefined") {
            current_act = act;
          } else {
            current_act = 0;
          }
          if (typeof current_episode.acts !== "undefined") {
            $.each(current_episode.acts, function (i, act) {
              if (act.number === current_act) {
                current_timestamp = act.timestamp;
                $info
                  .find(".title")
                  .html(act.name)
                  .css("display", "inline-block");
              }
            });
          }

          //  Use archive for act timestamps
          if (
            current_timestamp > 0 &&
            current_episode.archive != null &&
            current_episode.archive.length > 0 &&
            current_episode.audio != current_episode.archive
          ) {
            source = data.archive;
          } else {
            source = data.audio;
          }

          $jp
            .jPlayer("setMedia", {
              mp3: source,
            })
            .jPlayer("play", current_timestamp);
          $(
            "a.play-" +
              current_episode.episode +
              ", a.play-" +
              current_episode.episode +
              "-" +
              current_act
          ).addClass("playing");
          if (typeof ga !== "undefined") {
            ga(
              "send",
              "event",
              "Play source",
              current_episode.episode,
              last_path
            );
          }
        },
        initArchive = function () {
          var $form = $("#tal-episode-browse-form");
          if ($form.length) {
            var $top = $("#top"),
              $body = $("body"),
              $modals = $form.find(".modal"),
              $options = $form.find("#browse-options"),
              $selects = $form.find(".form-wrapper-select"),
              $type_sort = $("#type-sort"),
              $filter_wrapper = $form.find(".filters"),
              dirty = false,
              active = true;
            $top.removeClass("disabled");

            $type_sort.on("click", function (e) {
              if (view !== "desktop") {
                if (!$type_sort.hasClass("open")) {
                  e.preventDefault();
                  e.stopPropagation();
                  $type_sort.addClass("open");
                }
              }
            });

            $body.on("click", function (e) {
              if ($type_sort.hasClass("open")) {
                $type_sort.removeClass("open");
                e.preventDefault();
              }
            });

            $form.find("a.options").on("click", function (e) {
              e.preventDefault();
              $top.addClass("open-filters");
            });

            $options.on("click", "a.close", function (e) {
              e.preventDefault();
              $top.removeClass("open-filters");
            });

            $options.on("click", "a.modal", function (e) {
              e.preventDefault();
              var target = $(this).attr("href"),
                $target = $(target);
              $top.addClass("open-modals");
              $target.show();
            });

            $options.find("a.modal").each(function () {
              var $option = $(this),
                element = $(this).data("element"),
                $element = $form.find("." + element).find("select");
              $option.on("click", function (e) {
                if ($option.hasClass("selected")) {
                  e.preventDefault();
                  e.stopPropagation();
                  $option
                    .removeClass("selected")
                    .find(".selection .value")
                    .text("");
                  $element.val(0);
                  if (!dirty) {
                    dirty = true;
                    $form.addClass("dirty");
                    $form.submit();
                  }
                  $top.removeClass("open-filters open-modals");
                }
              });
            });

            $modals.each(function () {
              var $modal = $(this),
                id = $modal.attr("id"),
                $option = $options.find("." + id),
                $select_wrapper = $selects.filter("." + id),
                $alpha_nav = $modal.find(".alpha-nav"),
                $alpha = $modal.find(".alpha");
              $modal.on("click", "header a", function (e) {
                e.preventDefault();
                $modal.hide();
                $top.removeClass("open-modals");
              });
              $alpha_nav.on("click", "a", function (e) {
                var href = $(this).attr("href"),
                  $anchor = $(href);
                e.preventDefault();
                e.stopPropagation();
                $modal.find(".inner > .item-list").animate(
                  {
                    scrollTop: $anchor.position().top,
                  },
                  250
                );
              });
              $modal.on("click", ".inner a", function (e) {
                e.preventDefault();
                var value = $(this).data("value"),
                  key = $(this).data("key"),
                  element = $(this).data("element"),
                  $element = $form.find("." + element),
                  $select = $element.find("select");
                $select.val(key);
                $option
                  .addClass("selected")
                  .find(".selection .value")
                  .text(value);
                $select_wrapper
                  .addClass("selected")
                  .find(".selection .value")
                  .text(value);
                dirty = true;
                $form.addClass("dirty");
                if (view === "desktop") {
                  $form.submit();
                } else {
                  $modal.hide();
                  $top.removeClass("open-modals");
                }
              });
            });

            $selects.each(function () {
              var $select = $(this);
              $select.on("click", "label", function (e) {
                e.preventDefault();
                if ($select.hasClass("selected")) {
                  e.stopPropagation();
                  $select.find("select").val(0);
                  $form.submit();
                } else {
                  var target = $select.data("target"),
                    $target = $("#" + target);
                  $top.addClass("open-modals");
                  $target.show();
                }
              });
            });

            $form.on("submit", function (e) {
              e.preventDefault();
              if (active) {
                active = false;
                $top
                  .removeClass("open-filters open-modals")
                  .addClass("disabled");
                $modals.hide();
                $.ajax({
                  url: $(this).attr("action"),
                  type: $(this).attr("method"),
                  dataType: "json",
                  data: $(this).serialize(),
                  success: function (data) {
                    $top.removeClass("open-modals");
                    gotoPath(data.redirect);
                  },
                });
              }
            });

            /*
						$modals.on('click', function(e) {
							e.stopPropagation();
						});
						$options.on('click', function(e) {
							e.stopPropagation();
						});

						$filter_wrapper.on('click', function(e) {
							$top.removeClass('open-modals open-filters');
							$modals.hide();
							$options.hide();
						});
						*/
          }
        },
        resizePage = function () {
          scrollTop = $window.scrollTop();
          windowHeight = $window.height();
          view = viewSize();
        },
        initLinks = function () {
          $(".contextual-links-wrapper").find("a").addClass("ignore");

          // Modal - TODO: move all archive filters to MicroModal.js and cleanup initArchive
          MicroModal.init();

          // External Links
          $("a.shareout")
            .once("links")
            .addClass("external")
            .on("click", function (e) {
              var event = new CustomEvent("talGoto", {
                detail: {
                  type: "external",
                  url: $(this).prop("href"),
                  ctrlKey: e.ctrlKey,
                  metaKey: e.metaKey,
                },
              });

              window.dispatchEvent(event);
              if ($jp.length) {
                e.preventDefault();
              }
            });

          // External Links
          $(
            'a[href]:not([href*="' +
              window.location.hostname +
              '"]):not([href^="#"]):not([href^="/"]):not([href^="javascript:"]):not([href^="mailto:"]):not(.cut)'
          )
            .once("links")
            .addClass("external")
            .on("click", function (e) {
              var event = new CustomEvent("talGoto", {
                detail: {
                  type: "external",
                  url: $(this).prop("href"),
                  ctrlKey: e.ctrlKey,
                  metaKey: e.metaKey,
                },
              });

              window.dispatchEvent(event);
              if ($jp.length) {
                e.preventDefault();
              }
            });

          // External Links
          $("a.external")
            .once("links")
            .on("click", function (e) {
              var event = new CustomEvent("talGoto", {
                detail: {
                  type: "external",
                  url: $(this).prop("href"),
                  ctrlKey: e.ctrlKey,
                  metaKey: e.metaKey,
                },
              });

              window.dispatchEvent(event);
              if ($jp.length) {
                e.preventDefault();
              }
            });

          //  Links
          $('a[href*="' + window.location.hostname + '"],[href^="/"]')
            .not(".goto, .play, .ignore")
            .once("links")
            .addClass("internal")
            .on("click", function (e) {
              var event = new CustomEvent("talGoto", {
                detail: {
                  type: "internal",
                  url: $(this).prop("href"),
                  ctrlKey: e.ctrlKey,
                  metaKey: e.metaKey,
                },
              });
              window.dispatchEvent(event);
              if ($jp.length) {
                e.preventDefault();
              }
            });

          $(".node-episode")
            .once("links")
            .each(function () {
              var detail = {
                type: $(this).data("type"),
                id: $(this).data("id"),
                episode: $(this).data("episode"),
              };
              $(this).on("click.special", "a.goto-episode", function (e) {
                detail.url = $(this).prop("href");
                detail.ctrlKey = e.ctrlKey;
                detail.metaKey = e.metaKey;
                var event = new CustomEvent("talGoto", {
                  detail: detail,
                });
                window.dispatchEvent(event);
                if ($jp.length) {
                  e.preventDefault();
                }
              });
            });
          $(".node-transcript")
            .once("links")
            .each(function () {
              var $node = $(this);
              $(this).on("click.special", "a.goto-episode", function (e) {
                var detail = {
                  type: "episode",
                  id: $node.data("episode-id"),
                  episode: $node.data("episode"),
                  url: $(this).prop("href"),
                };
                detail.ctrlKey = e.ctrlKey;
                detail.metaKey = e.metaKey;
                var event = new CustomEvent("talGoto", {
                  detail: detail,
                });
                window.dispatchEvent(event);
                if ($jp.length) {
                  e.preventDefault();
                }
              });
            });
          $(".node-pick")
            .once("links")
            .each(function () {
              var detail = {
                type: $(this).data("type"),
                id: $(this).data("id"),
              };
              $(this).on("click.special", "a.goto-collection", function (e) {
                detail.url = $(this).prop("href");
                detail.ctrlKey = e.ctrlKey;
                detail.metaKey = e.metaKey;
                var event = new CustomEvent("talGoto", {
                  detail: detail,
                });
                window.dispatchEvent(event);
                if ($jp.length) {
                  e.preventDefault();
                }
              });
              $(this).on(
                "click.special",
                "a.goto-act, a.goto-episode",
                function (e) {
                  var $act = $(this),
                    detail = {
                      type: $act.data("type"),
                      id: $act.data("id"),
                      episode: $act.data("episode"),
                      url: $(this).prop("href"),
                    };
                  detail.ctrlKey = e.ctrlKey;
                  detail.metaKey = e.metaKey;
                  var event = new CustomEvent("talGoto", {
                    detail: detail,
                  });
                  window.dispatchEvent(event);
                  if ($jp.length) {
                    e.preventDefault();
                  }
                }
              );
            });
          $(".node-act")
            .once("links")
            .each(function () {
              var $act = $(this);
              $(this).on("click.special", "a.goto-act", function (e) {
                var detail = {
                  type: $act.data("type"),
                  id: $act.data("id"),
                  episode: $act.data("episode"),
                  url: $(this).prop("href"),
                };
                detail.url = $(this).prop("href");
                detail.ctrlKey = e.ctrlKey;
                detail.metaKey = e.metaKey;
                var event = new CustomEvent("talGoto", {
                  detail: detail,
                });
                window.dispatchEvent(event);
                if ($jp.length) {
                  e.preventDefault();
                }
              });
              $(this).on("click.special", "a.goto-episode", function (e) {
                var detail = {
                  type: "episode",
                  id: $act.data("episode-id"),
                  episode: $act.data("episode"),
                  url: $(this).prop("href"),
                };
                detail.ctrlKey = e.ctrlKey;
                detail.metaKey = e.metaKey;
                var event = new CustomEvent("talGoto", {
                  detail: detail,
                });
                window.dispatchEvent(event);
                if ($jp.length) {
                  e.preventDefault();
                }
              });
            });
          $(".node-collection")
            .once("links")
            .each(function () {
              var detail = {
                type: $(this).data("type"),
                id: $(this).data("id"),
              };
              $(this).on("click.special", "a.goto-collection", function (e) {
                detail.url = $(this).prop("href");
                detail.ctrlKey = e.ctrlKey;
                detail.metaKey = e.metaKey;
                var event = new CustomEvent("talGoto", {
                  detail: detail,
                });
                window.dispatchEvent(event);
                if ($jp.length) {
                  e.preventDefault();
                }
              });
            });

          var $actions = $("ul.actions");
          $actions
            .find("a.download")
            .off("click")
            .on("click.download", function (e) {
              e.preventDefault();
            });

          // Play
          $("a.play")
            .off("click")
            .on("click.play", function (e) {
              e.preventDefault();
              if ($(this).hasClass("playing")) {
                $jp.jPlayer("pause");
              } else {
                if (
                  !$body.hasClass("player") &&
                  $body.hasClass("node-type-homepage")
                ) {
                  var event = new CustomEvent("talGoto", {
                    detail: {
                      type: "internal",
                      url: $(this).prop("href"),
                    },
                  });
                  window.dispatchEvent(event);
                }

                var event = new CustomEvent("talPlay", {
                  detail: {
                    type: $(this).data("type"),
                    id: $(this).data("id"),
                    episode: $(this).data("episode"),
                    act: $(this).data("act"),
                  },
                });
                window.dispatchEvent(event);

                if ($(this).hasClass("play-transcript")) {
                  $body.addClass("has-closed-caption");
                  if (typeof ga !== "undefined") {
                    ga(
                      "send",
                      "event",
                      "Play transcript",
                      current_episode.episode,
                      last_path
                    );
                  }
                }
              }
            });

          $("a.js-closed-caption")
            .off("click")
            .on("click.closed-caption", function (e) {
              if ($body.hasClass("has-closed-caption")) {
                $body.removeClass("has-closed-caption");
              } else {
                $body.addClass("has-closed-caption");
                if (typeof ga !== "undefined") {
                  ga(
                    "send",
                    "event",
                    "CC button",
                    current_episode.episode,
                    last_path
                  );
                }
              }
            });
        },
        initPage = function () {
          initLinks();
          initArchive();
          Drupal.behaviors.contextualLinks.attach();
          Drupal.behaviors.thislifeHeight.attach();
          Drupal.behaviors.thislifeAbout.attach();
          Drupal.behaviors.thislifeExtras.attach();
          if (typeof current_episode !== "undefined") {
            if (playing) {
              $(
                "a.play-" +
                  current_episode.episode +
                  ", a.play-" +
                  current_episode.episode +
                  "-" +
                  current_act
              ).addClass("playing");
            } else {
              $("a.play").removeClass("playing");
            }
          }
        },
        gotoPath = function (url, data, replace) {
          var split = url.split("#"),
            path = split[0],
            fragment = split[1],
            scrollTo = 0;
          $body.removeClass("has-closed-caption");

          if (path[0] === "/") {
            if (last_path === path) {
              $top.removeClass("disabled");
              if (fragment && $("#" + fragment).length) {
                var offset = $("#" + fragment).offset().top;
                $("html, body").animate(
                  {
                    scrollTop: offset,
                  },
                  0
                );
              } else {
                $("html, body").animate(
                  {
                    scrollTop: 0,
                  },
                  0
                );
              }
            } else {
              if (
                typeof replace === "undefined" &&
                last_path.substring(0, 8) === "/archive"
              ) {
                // If archive, save current state for when we need to go back
                $saved_archive.html($main.html()).data({
                  path: last_path,
                  scroll: scrollTop,
                });
              }
              var goto_path = Drupal.settings.basePath + "goto" + path;
              if (path === "/") {
                goto_path += "?type=json";
              }
              $.ajax({
                url: goto_path,
                cache: true,
                data: data,
                dataType: "json",
                error: function () {
                  window.open(path, "_self");
                },
                success: function (data) {
                  last_path = path;
                  if (window.history.pushState) {
                    if (replace) {
                      window.history.replaceState(
                        {
                          path: path,
                        },
                        "",
                        path
                      );
                    } else {
                      window.history.pushState(
                        {
                          path: path,
                        },
                        "",
                        path
                      );
                    }
                  }
                  $top.empty();
                  $sidebar.empty();
                  if (
                    typeof replace !== "undefined" &&
                    replace &&
                    url.substring(0, 8) === "/archive" &&
                    $saved_archive.html() &&
                    $saved_archive.data("path") === url
                  ) {
                    // We're going back to the archive. Try to load from saved state.
                    if ($saved_archive.data("scroll")) {
                      scrollTo = $saved_archive.data("scroll");
                    }
                    $main.html($saved_archive.html());
                    $main
                      .find(".links-processed")
                      .removeClass("links-processed");
                    $saved_archive.empty().removeData("scroll path");
                  } else {
                    $main.html(data.main);
                  }
                  if (typeof data.sidebar !== "undefined") {
                    $sidebar.html(data.sidebar);
                  }
                  if (typeof data.top !== "undefined") {
                    $top.html(data.top);
                  }
                  if (typeof data.color !== "undefined") {
                    $header.css("background-color", data.color);
                  } else {
                    $header.css("background-color", "");
                  }
                  $nav.find("li.active-trail").removeClass("active-trail");
                  $nav.find("a.active").removeClass("active");
                  open_menu = false;
                  $body.removeClass("open-menu player-open");

                  if (typeof data.facebook !== "undefined") {
                    $share_modal.find("a.facebook").attr("href", data.facebook);
                  }
                  if (typeof data.twitter !== "undefined") {
                    $share_modal.find("a.twitter").attr("href", data.twitter);
                  }
                  if (typeof data.mail !== "undefined") {
                    $share_modal.find("a.mail").attr("href", data.mail);
                  }
                  if (
                    fragment &&
                    fragment === "keyword" &&
                    $("#edit-keyword").length
                  ) {
                    $("#edit-keyword").focus();
                  } else if (fragment && $("#" + fragment).length) {
                    var offset = $("#" + fragment).offset().top;
                    $("html, body").animate(
                      {
                        scrollTop: offset,
                      },
                      0
                    );
                  } else {
                    $("html, body").animate(
                      {
                        scrollTop: scrollTo,
                      },
                      0
                    );
                  }

                  var classes = $("body").attr("class").split(" ");
                  $.each(classes, function (i, c) {
                    if (c.indexOf("page-") === 0) {
                      $("body").removeClass(c);
                    }
                    if (c.indexOf("node-") === 0) {
                      $("body").removeClass(c);
                    }
                  });
                  if (typeof data.node_type !== "undefined") {
                    $("body").addClass("node-type-" + data.node_type);
                  }
                  if (typeof data.section !== "undefined") {
                    $("body").addClass("page-" + data.section);
                    if (!isNaN(data.section)) {
                      $("body").addClass("page-episode-number-" + data.section);
                    }
                    var section = data.section;
                    if (section === "listen") {
                      section = "how-to-listen";
                    }
                    $nav.find("li." + section).addClass("active-trail");
                  }

                  $("title").text(data.title);
                  initPage();
                  //Drupal.attachBehaviors($main);

                  if (typeof ga !== "undefined") {
                    ga("send", "pageview", {
                      page: path,
                      title: data.title,
                    });
                  }
                },
              }); /* end ajax */
            }
          }
        };

      initLinks();
      initArchive();
      $window
        .on("load", function () {
          resizePage();
          $body.addClass("is-loaded");
        })
        .on("throttledresize", function () {
          resizePage();
        });
      resizePage();
      window.requestAnimationFrame(updatePage);

      if ($jp.length) {
        var fingerprint,
          heartbeat = function () {
            if (typeof fingerprint !== "undefined") {
              var data = {
                fingerprint: fingerprint,
                episode: current_episode.episode,
                position: last_position,
                source: "thisamericanlife",
              };
              $.ajax({
                url: "https://hb.serialpodcast.org/",
                data: data,
              });
            }
          };
        new Fingerprint2().get(function (result) {
          fingerprint = result;
        });

        $jp.jPlayer({
          ready: function () {
            ready = true;
          },
          timeupdate: function (event) {
            var position = parseInt(event.jPlayer.status.currentTime),
              new_total = Math.floor(event.jPlayer.status.duration),
              new_act = 0;
            if (typeof current_episode.acts !== "undefined") {
              $.each(current_episode.acts, function (i, act) {
                if (position >= act.timestamp) {
                  new_act = act.number;
                }
              });
            }
            if (new_act !== current_act) {
              $("a.play-act").removeClass("playing");
              if (typeof current_episode.acts !== "undefined") {
                $.each(current_episode.acts, function (i, act) {
                  if (act.number === new_act) {
                    current_act = act.number;
                    $info
                      .find(".title")
                      .html(act.name)
                      .css("display", "inline-block");
                    if (typeof act.byline !== "undefined") {
                      $info.find(".contributor").html(act.byline);
                    } else {
                      $info.find(".contributor").empty();
                    }
                    if (typeof act.summary !== "undefined") {
                      $info.find(".body").html(act.summary);
                    } else {
                      $info.find(".body").empty();
                    }
                    initLinks();
                  }
                });
              }
              $(
                "a.play-" +
                  current_episode.episode +
                  ", a.play-" +
                  current_episode.episode +
                  "-" +
                  current_act
              ).addClass("playing");
            }
            if (new_total > 60 && position % 1 === 0) {
              if (position > last_position + 14) {
                last_position = position;
                //heartbeat();
              }
            }

            var status = event.jPlayer.status;
            $(".jp-remaining").text(
              $.jPlayer.convertTime(status.duration - status.currentTime)
            );

            /* Start captions */
            if ($body.hasClass("node-type-transcript")) {
              var $node = $(".node-transcript.view-full");
              if ($node.data("episode") == current_episode.episode) {
                var $content = $node.find(".content");
                ($paragraphs = $content.find("p").removeClass("is-current")),
                  (last_paragraph = 0);
                $paragraphs.each(function (i) {
                  var $paragraph = $(this),
                    timestamp = $paragraph.data("timestamp");

                  $.each(adOffsets, function (i) {
                    if (timestamp > adOffsets[i].offset) {
                      timestamp += adOffsets[i].length;
                    }
                  });

                  if (position >= timestamp) {
                    last_paragraph = i;
                  } else {
                    return false;
                  }
                });
                $paragraphs;
                var $current_paragraph = $paragraphs
                  .eq(last_paragraph)
                  .addClass("is-current");

                if ($body.hasClass("has-closed-caption")) {
                  var offset =
                    $current_paragraph.offset().top -
                    parseInt($body.css("padding-top"), 10) -
                    parseInt($node.css("padding-top"), 10) -
                    80;
                  $("html, body").animate(
                    {
                      scrollTop: offset,
                    },
                    250
                  );
                }
              }
            }
            /* End captions */
          },
          play: function () {
            playing = true;
            $("body").addClass("player playing");
            $(
              "a.play-" +
                current_episode.episode +
                ", a.play-" +
                current_episode.episode +
                "-" +
                current_act
            ).addClass("playing");
          },
          pause: function () {
            playing = false;
            $("body").removeClass("playing");
            $("a.play").removeClass("playing");
          },
          loadeddata: function (event) {
            var duration = event.jPlayer.status.duration;
            if (typeof current_episode.acts !== "undefined") {
              $(".jp-progress a.jump").remove();
              $.each(current_episode.acts, function (i, act) {
                if (act.number > 0) {
                  var $jump = $("<a/>")
                    .addClass("jump")
                    .css({
                      left: (act.timestamp / duration) * 100 + "%",
                    })
                    .on("click", function () {
                      $jp.jPlayer("play", act.timestamp);
                    });
                  $progress.append($jump);
                }
              });
            }
          },
          ended: function () {
            playing = false;
            $("body").removeClass("playing");
            last_position = 0;
          },
          swfPath: jplayer_swfPath,
          wmode: "window",
        });

        if ($body.hasClass("node-type-embed")) {
          var $target = $("#playlist-data");
          if ($target.length) {
            var data = JSON.parse($target.html());
            $body.on("click", ".embed__play", function (e) {
              e.preventDefault();
              $jp
                .jPlayer("setMedia", {
                  mp3: data.audio,
                })
                .jPlayer("play");
            });
          }
        }

        $player.on("click", "a.player-transcript", function (e) {
          e.preventDefault();
          if (
            typeof current_episode !== false &&
            typeof current_episode.transcript !== "undefined"
          ) {
            if ($body.hasClass("has-closed-caption")) {
              $body.removeClass("has-closed-caption");
            } else {
              gotoPath(current_episode.transcript);
              $body.addClass("has-closed-caption");
              if (typeof ga !== "undefined") {
                ga(
                  "send",
                  "event",
                  "CC button",
                  current_episode.episode,
                  last_path
                );
              }
            }
          }
        });

        $(window).on("talGoto", function (e) {
          var detail = e.originalEvent.detail;
          if (
            (typeof detail.ctrlKey !== "undefined" && detail.ctrlKey) ||
            (typeof detail.metaKey !== "undefined" && detail.metaKey)
          ) {
            window.open(detail.url, "_blank");
          } else if ($body.hasClass("page-broadcast")) {
            window.open(detail.url, "_blank");
          } else if ($body.hasClass("page-status-404-not-found")) {
            window.open(detail.url, "_self");
          } else if ($body.hasClass("page-heartbeat")) {
            window.open(detail.url, "_self");
          } else if (detail.type === "external") {
            window.open(detail.url, "_blank");
          } else {
            var path = detail.url;
            if (path[0] !== "/") {
              path = path.replace(
                "http://" +
                  window.location.hostname +
                  (location.port ? ":" + location.port : ""),
                ""
              );
              path = path.replace(
                "https://" +
                  window.location.hostname +
                  (location.port ? ":" + location.port : ""),
                ""
              );
            }
            if (path.match(/^\/(user|admin|system|heartbeat)/i)) {
              // Admin path, open regularly
              window.open(detail.url, "_self");
            } else if (path[0] === "/") {
              gotoPath(path);
            }
          }
        });

        $(window).bind("popstate", function () {
          var path = window.location.pathname + window.location.search;
          gotoPath(path, {}, true);
        });

        $(window).on("talPlay", function (e) {
          var detail = e.originalEvent.detail;
          if (typeof detail.type !== "undefined") {
            $("body").addClass("player");
            if (detail.type === "episode") {
              if (typeof detail.episode !== "undefined") {
                if (
                  typeof current_episode === false ||
                  current_episode.episode !== detail.episode
                ) {
                  var $target = $("#playlist-data");
                  if ($("#playlist-data-" + detail.episode).length) {
                    $target = $("#playlist-data-" + detail.episode);
                  }
                  if ($target.length) {
                    var data = JSON.parse($target.html());
                    current_episode = data;
                    if (typeof data.facebook !== "undefined") {
                      $player_share_modal
                        .find("a.facebook")
                        .attr("href", data.facebook);
                    }
                    if (typeof data.twitter !== "undefined") {
                      $player_share_modal
                        .find("a.twitter")
                        .attr("href", data.twitter);
                    }
                    if (typeof data.mail !== "undefined") {
                      $player_share_modal
                        .find("a.mail")
                        .attr("href", data.mail);
                    }
                    playEpisode();
                  }
                } else {
                  $jp.jPlayer("play");
                }
              }
            } else if (detail.type === "act") {
              if (typeof detail.episode !== "undefined") {
                if (
                  typeof current_episode === false ||
                  current_episode.episode !== detail.episode
                ) {
                  var $target = $("#playlist-data");
                  if ($("#playlist-data-" + detail.episode).length) {
                    $target = $("#playlist-data-" + detail.episode);
                  }
                  if ($target.length) {
                    var data = JSON.parse($target.html());
                    current_episode = data;
                    if (typeof data.facebook !== "undefined") {
                      $player_share_modal
                        .find("a.facebook")
                        .attr("href", data.facebook);
                    }
                    if (typeof data.twitter !== "undefined") {
                      $player_share_modal
                        .find("a.twitter")
                        .attr("href", data.twitter);
                    }
                    if (typeof data.mail !== "undefined") {
                      $player_share_modal
                        .find("a.mail")
                        .attr("href", data.mail);
                    }
                    playEpisode(detail.act);
                  }
                } else if (detail.act !== current_act) {
                  playAct(detail.act);
                } else {
                  $jp.jPlayer("play");
                }
              }
            } else if (detail.type === "extra") {
              if ($("#playlist-data").length) {
                var data = JSON.parse($("#playlist-data").html());
                current_episode = data;
                if (typeof data.facebook !== "undefined") {
                  $player_share_modal
                    .find("a.facebook")
                    .attr("href", data.facebook);
                }
                if (typeof data.twitter !== "undefined") {
                  $player_share_modal
                    .find("a.twitter")
                    .attr("href", data.twitter);
                }
                if (typeof data.mail !== "undefined") {
                  $player_share_modal.find("a.mail").attr("href", data.mail);
                }
                playEpisode();
              }
            }
          }
        });

        $player.on("click", "a, .jp-progress", function (e) {
          e.stopPropagation();
        });

        $player.on("click", "a.close", function () {
          $body.removeClass("player-open");
        });

        $player.find("a.jp-rewind").click(function (e) {
          e.preventDefault();
          if (ready) {
            var new_time =
              Math.floor($jp.data("jPlayer").status.currentTime) - 10;
            if (new_time < 0) {
              new_time = 0;
            }
            $jp.jPlayer("play", new_time);
          }
        });

        $player.find("a.jp-forward").click(function (e) {
          e.preventDefault();
          if (ready) {
            var new_time =
              Math.floor($jp.data("jPlayer").status.currentTime) + 30;
            if (new_time < $jp.data("jPlayer").status.duration) {
              $jp.jPlayer("play", new_time);
            }
          }
        });

        $player.find("a.jp-previous").click(function (e) {
          e.preventDefault();
          if (ready && current_episode) {
            if (typeof current_episode.acts !== "undefined") {
              var time = Math.floor($jp.data("jPlayer").status.currentTime),
                acts = current_episode.acts.slice().reverse();
              $.each(acts, function (i, act) {
                if (
                  (act.number <= current_act && act.timestamp < time) ||
                  (act.number == 0 && time > 0)
                ) {
                  setAudioSource(act);
                  $jp.jPlayer("play", act.timestamp);
                  return false;
                }
              });
            }
          }
        });

        $player.find("a.jp-next").click(function (e) {
          e.preventDefault();
          if (ready && current_episode) {
            if (typeof current_episode.acts !== "undefined") {
              var time = Math.floor($jp.data("jPlayer").status.currentTime);
              $.each(current_episode.acts, function (i, act) {
                if (act.number > current_act && act.timestamp > time) {
                  setAudioSource(act);
                  $jp.jPlayer("play", act.timestamp);
                  return false;
                }
              });
            }
          }
        });

        $player.on("click", function () {
          if (view === "mobile") {
            if ($body.hasClass("player-open")) {
            } else {
              $body.addClass("player-open");
            }
          }
        });
      }

      $main.on("click", "a.pager", function (e) {
        e.preventDefault();

        var $a = $(this),
          path = $(this).attr("href");
        $.ajax({
          url: path,
          cache: true,
          dataType: "json",
          success: function (data) {
            var $pager = $a.parents("ul.pager").parent(".item-list");
            $pager.replaceWith(data.html);
            if (window.history.pushState) {
              window.history.replaceState(
                {
                  path: path,
                },
                "",
                data.path
              );
            }
            Drupal.behaviors.thislifeHeight.attach();
            initLinks();
            if (typeof ga !== "undefined") {
              ga("send", "pageview", {
                page: path,
                title: data.title,
              });
            }
          },
        });
      });
    },
  };

  Drupal.behaviors.thislifeAbout = {
    attach: function () {
      var $menu = $("#block-tal-about-menu");
      if ($menu.length) {
        $menu.on("click", function (e) {
          e.preventDefault();
          $menu.toggleClass("open");
        });
        $menu.on("click", ".content", function (e) {
          e.stopPropagation();
        });
        $menu.on("click", "a", function () {
          $menu.removeClass("open");
        });
      }
      if ($("body").hasClass("page-about-announcements")) {
        var $nodes = $("#main").find(".node-announcement").slice(0, 2);
        $nodes.matchHeight();
      }
    },
  };

  Drupal.behaviors.thislifeExtras = {
    attach: function () {
      if ($("body").hasClass("node-type-gallery")) {
        var $node = $("#main").find(".node-gallery"),
          $gallery_slideshow = $node.find("#gallery-slideshow"),
          $gallery_slides = $gallery_slideshow.find(".gallery-slide"),
          $gallery_images = $node.find("figure.file-image"),
          gallery_slide_template = $node.find("#gallery-slide-template").html(),
          current_slide = 0,
          total_slides = $gallery_images.length,
          $window = $(window),
          window_width = $window.width(),
          desktop_width = 1024,
          caption_offset = 0,
          reset_caption_offset = function () {
            $gallery_slides
              .removeClass("offset")
              .find(".meta > .inner")
              .removeAttr("style");
            caption_offset = $gallery_slides
              .eq(current_slide)
              .find(".meta > .inner")
              .position();
            $gallery_slides
              .addClass("offset")
              .find(".meta > .inner")
              .css("top", caption_offset.top);
          };

        $(window)
          .on("load", function () {
            window_width = $window.width();
          })
          .on("throttledresize", function () {
            var new_width = $window.width();
            if (new_width < desktop_width) {
              $gallery_slideshow.hide();
              $gallery_slides.hide();
            }
            window_width = new_width;
            reset_caption_offset();
          });

        $gallery_images.each(function (i) {
          var $gallery_image = $(this),
            slide_data = {
              color: $node.data("background"),
              img: $gallery_image.find("img").attr("src"),
              width: $gallery_image.find("img").attr("width"),
              height: $gallery_image.find("img").attr("height"),
              current: i + 1,
              total: total_slides,
              caption: $gallery_image.find("figcaption").html(),
            },
            slide = Mustache.to_html(gallery_slide_template, slide_data);
          $gallery_slideshow.append(slide);
          $gallery_image.on("click", function (e) {
            if (window_width >= desktop_width) {
              e.preventDefault();
              current_slide = i;
              $gallery_slides.eq(i).show();
              $gallery_slideshow.show();
              reset_caption_offset();
            }
          });
        });

        $gallery_slides = $gallery_slideshow.find(".gallery-slide");

        $gallery_slideshow.on("click", "button.close", function (e) {
          e.preventDefault();
          $gallery_slideshow.hide();
          $gallery_slides.hide();
        });

        $gallery_slideshow.on("click", "button.prev", function (e) {
          e.preventDefault();
          var new_slide = current_slide - 1;
          if (new_slide < 0) {
            new_slide = total_slides - 1;
          }
          $gallery_slides.hide();
          $gallery_slides.eq(new_slide).show();
          current_slide = new_slide;
        });

        $gallery_slideshow.on("click", "button.next", function (e) {
          e.preventDefault();
          var new_slide = current_slide + 1;
          if (new_slide >= total_slides) {
            new_slide = 0;
          }
          $gallery_slides.hide();
          $gallery_slides.eq(new_slide).show();
          current_slide = new_slide;
        });

        $(document).keydown(function (e) {
          switch (e.keyCode) {
            // Left
            case 37:
              var new_slide = current_slide - 1;
              if (new_slide < 0) {
                new_slide = total_slides - 1;
              }
              $gallery_slides.hide();
              $gallery_slides.eq(new_slide).show();
              current_slide = new_slide;
              break;
            // Right
            case 39:
              var new_slide = current_slide + 1;
              if (new_slide >= total_slides) {
                new_slide = 0;
              }
              $gallery_slides.hide();
              $gallery_slides.eq(new_slide).show();
              current_slide = new_slide;
              break;
            // Escape
            case 27:
              $gallery_slideshow.hide();
              $gallery_slides.hide();
              break;
          }
        });
      }
    },
  };

  Drupal.behaviors.thislifeHeight = {
    attach: function () {
      if ($("body").hasClass("page-archive")) {
        $("#main").find(".node.view-teaser").matchHeight();
      }

      if ($("body").hasClass("node-type-video-collection")) {
        $("#main").find(".node-video.view-collection .content").matchHeight();
      }

      if ($("body").hasClass("node-type-homepage")) {
        var $node = $("#main").find(".node-homepage"),
          $featured = $node.find(".featured");
        if ($featured.length) {
          $featured.find(".node.view-featured .inner").matchHeight();
        }
      }
    },
  };

  /* This is the end */
})(jQuery);
