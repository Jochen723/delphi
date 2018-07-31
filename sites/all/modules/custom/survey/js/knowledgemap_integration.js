(function ($) {
  Drupal.behaviors.exampleModule = {
    attach: function (context, settings) {

      $('area').each(function(index) {
        $(this).qtip({
          content: {
            text: $(this).attr('title') + '<BR/>' + $(this).attr('data-description'),
            show: {
              delay: 1
            }
          },
          my: 'left',
          position: {
            target: 'mouse',
            adjust: {
              x: 30, y: 0
            }
          },

          style: {
            tip: false
          }
        });

      });
    }
  };
}(jQuery));
