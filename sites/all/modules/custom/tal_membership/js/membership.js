(function ($) {
  Drupal.behaviors.faq = {
    attach: function () {
      //  toggle method
      var toggleQ = function (el) {
        $(el).parent().find("div.answer").toggleClass("hidden");
        $(el).find("svg").toggleClass("rotate-90");
      };
      //  listen for clicks
      $("question").on("click", ".cursor-pointer", function () {
        toggleQ(this);
      });
      //  open first q
      var firstQ = $("question:first-child .cursor-pointer");
      toggleQ(firstQ);
    },
  };
})(jQuery);
