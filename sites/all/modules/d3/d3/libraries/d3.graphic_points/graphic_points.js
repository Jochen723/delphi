/**
 * @file
 * Javascript for [library name].
 */

(function($) {

  /**
   * Adds library to the global d3 object.
   *
   * @param select
   * @param settings
   *   Array of values passed to d3_draw.
   *   id: required. This will be needed to attach your
   *       visualization to the DOM.
   */
  Drupal.d3.graphic_points = function (select, settings) {
    // Your custom JS.

    w = 800,
    h = 400,

    var svg = d3.select('#' + div).append("svg")
      .attr("width", w)
      .attr("height", h)
      .append("g");

      svg.append('text')
        .attr("dy", ".35em")
        .attr('dx', 15)
        .attr("text-anchor", "end")
        .text(function(d, i) { return "HALLO"; })
        .ellipsis(60);

  }

})(jQuery);
