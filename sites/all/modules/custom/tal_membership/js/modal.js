(function ($) {
  Drupal.behaviors.takeover = {
    attach: function () {
      var $takeover = $("#takeover");
      var excepted = Drupal.settings.modal?.excepted_paths?.split("\n") ?? [];
      if ($takeover.length && excepted.indexOf(window.location.pathname) < 0) {
        if (typeof Cookies.get("thisamericanlife-takeover") === "undefined") {
          MicroModal.show("takeover", {
            onClose: function (modal) {
              var expires = Drupal.settings.modal?.expires ?? 7;
              Cookies.set("thisamericanlife-takeover", true, {
                expires: parseInt(expires),
              });
            },
            disableScroll: true,
          });
        }
      }
    },
  };
})(jQuery);
