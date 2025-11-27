(function ($) {
  Drupal.behaviors.eyebrow = {
    attach: function () {
      var $eyebrow = $("#eyebrow");
      if ($eyebrow.length) {
        if (typeof Cookies.get("thisamericanlife-eyebrow") === "undefined") {
          var $body = $("body");
          var expires = Drupal.settings.eyebrow?.expires ?? 7;
          $body.addClass("open-eyebrow");
          $eyebrow.on("click", "a.close", function (e) {
            e.preventDefault();
            $body.removeClass("open-eyebrow");
            Cookies.set("thisamericanlife-eyebrow", true, {
              expires: parseInt(expires),
            });
          });
        }
      }
    },
  };
})(jQuery);
