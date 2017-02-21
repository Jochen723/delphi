<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 16.02.17
 * Time: 10:48
 */

function survey_question_evaluation_form() {

    $question = arg(1);

    $test = array(1=>"", 2=>"", 3=>"", 4=>"", 5=>"", 6=>"");

    $id = 5;
    $form = array();

    $text = survey_get_question_content($question);

    $headline = "<h1>Frage ".$question."(b)</h1><h2>".$text."</h2><br><br>";

    $form['test']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $headline,
    );

    $ar = survey_get_answers();

    $testxx = get_graphic("data.csv2", "visualization");


    $form['test']['form_item22'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $testxx,
    );




    /*
    $form['test']['form_item223'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => '<div id="visualization2">'.$testxx2.'</div>',
    );*/

    for($i = 1;$i<=$id;$i++) {

        $form['dim'.$i] = array(
            '#type' => 'radios',
            '#title' => t('Dimension '.$i),
            '#default_value' => $ar["dim".$i],
            '#options' => $test,

        );
    }

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Zurück',
        '#submit' => array('')
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Weiter',
        '#submit' => array('survey_get_evaluation')
    );

    $button_text = "Weiter zur nächsten Frage";
    $id = survey_get_questions($question);
    if ($id == -1) {
        $button_text = "Umfrage beenden";
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => $button_text,
        '#submit' => array('survey_get_next_question')
    );


    return $form;
}

function survey_get_next_question($form, &$form_state) {

    $dim1 = $form_state['values']['dim1'];
    $dim2 = $form_state['values']['dim2'];
    $dim3 = $form_state['values']['dim3'];
    $dim4 = $form_state['values']['dim4'];
    $dim5 = $form_state['values']['dim5'];

    if(!empty($dim1) && !empty($dim2) && !empty($dim3) && !empty($dim4) && !empty($dim5)) {
        $question = arg(1);
        $user_id = arg(2);

        $sql = "SELECT * FROM {survey_users} WHERE user_pw = '".$user_id."'";
        $result = db_query($sql);

        $user = 0;

        foreach ($result as $item) {
            $user = $item->user_id;
        }

        $counter = 0;
        $sql = "SELECT * FROM {survey_user_answers} WHERE user_id = ".$user. " AND question = ".$question;
        $result = db_query($sql);
        foreach ($result as $item) {
            $counter++;
        }

        if ($counter == 0) {
            $sql = "INSERT INTO {survey_user_answers} (user_id,question,dim1,dim2,dim3,dim4,dim5) VALUES (".$user.",".$question.",".$dim1.",".$dim2.",".$dim3.",".$dim4.",".$dim5.")";
            db_query($sql);
        } else {
            $sql = "UPDATE {survey_user_answers} SET dim1 = ".$dim1.", dim2 = ".$dim2.", dim3 = ".$dim3.", dim4 = ".$dim4.", dim5 = ".$dim5." WHERE user_id = ".$user." AND question = ".$question;
            db_query($sql);
        }

        $question = arg(1);
        $id = survey_get_questions($question);

        if ($id == -1) {
            drupal_goto("finish_survey/".$user_id);
        } else {
            drupal_goto("survey_question/".$id."/".$user_id);
        }
    } else {
        drupal_set_message("Bitte alle Fragen beantworten!", "error");
    }



}

function get_graphic($data, $id) {

     //Get data
        //dsm("Hello");
        //$result = db_query("SELECT * FROM {node}");
        //dsm($result);
        //dsm(drupal_json_encode($result));
        //$record = $result->fetchAll();

        //array_walk($record, function(&$value, $key){$value .= "huhuhuhu";});
        //dsm($record);
        //dsm($record);
        //$record = drupal_json_encode($record);
        //foreach ($result as $r){
        //    dsm($r->field_anfangsdatum_value);
        //}


        $chart = array(
            'id' => $id,
            'type' => 'gantt',
            'data' => 'http://localhost/delphi/'.$data,
        );



        $d3_chart = d3_draw($chart);
        return $d3_chart;







        /*
        $test = '<style>

    body {
      font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
    }

    .box {
      font: 10px sans-serif;
    }

    .box line,
    .box rect,
    .box circle {
      fill: #fff;
      stroke: #000;
      stroke-width: 1.5px;
    }

    .box .center {
      stroke-dasharray: 3,3;
    }

    .box .outlier {
      fill: none;
      stroke: #ccc;
    }

    </style>
    <body>
    <script src="http://d3js.org/d3.v3.min.js"></script>
    <script>

    (function() {

    // Inspired by http://informationandvisualization.de/blog/box-plot
    d3.box = function() {
     var width = 1,
         height = 1,
         duration = 0,
         domain = null,
         value = Number,
         whiskers = boxWhiskers,
         quartiles = boxQuartiles,
         tickFormat = null;

     // For each small multiple…
     function box(g) {
       g.each(function(d, i) {
         d = d.map(value).sort(d3.ascending);
         var g = d3.select(this),
             n = d.length,
             min = d[0],
             max = d[n - 1];

         // Compute quartiles. Must return exactly 3 elements.
         var quartileData = d.quartiles = quartiles(d);

         // Compute whiskers. Must return exactly 2 elements, or null.
         var whiskerIndices = whiskers && whiskers.call(this, d, i),
             whiskerData = whiskerIndices && whiskerIndices.map(function(i) { return d[i]; });

         // Compute outliers. If no whiskers are specified, all data are "outliers".
         // We compute the outliers as indices, so that we can join across transitions!
         var outlierIndices = whiskerIndices
             ? d3.range(0, whiskerIndices[0]).concat(d3.range(whiskerIndices[1] + 1, n))
             : d3.range(n);

         // Compute the new x-scale.
         var x1 = d3.scale.linear()
             .domain(domain && domain.call(this, d, i) || [min, max])
             .range([height, 0]);

         // Retrieve the old x-scale, if this is an update.
         var x0 = this.__chart__ || d3.scale.linear()
             .domain([0, Infinity])
             .range(x1.range());

         // Stash the new scale.
         this.__chart__ = x1;

         // Note: the box, median, and box tick elements are fixed in number,
         // so we only have to handle enter and update. In contrast, the outliers
         // and other elements are variable, so we need to exit them! Variable
         // elements also fade in and out.

         // Update center line: the vertical line spanning the whiskers.
         var center = g.selectAll("line.center")
             .data(whiskerData ? [whiskerData] : []);

         center.enter().insert("line", "rect")
             .attr("class", "center")
             .attr("x1", width / 2)
             .attr("y1", function(d) { return x0(d[0]); })
             .attr("x2", width / 2)
             .attr("y2", function(d) { return x0(d[1]); })
             .style("opacity", 1e-6)
           .transition()
             .duration(duration)
             .style("opacity", 1)
             .attr("y1", function(d) { return x1(d[0]); })
             .attr("y2", function(d) { return x1(d[1]); });

         center.transition()
             .duration(duration)
             .style("opacity", 1)
             .attr("y1", function(d) { return x1(d[0]); })
             .attr("y2", function(d) { return x1(d[1]); });

         center.exit().transition()
             .duration(duration)
             .style("opacity", 1e-6)
             .attr("y1", function(d) { return x1(d[0]); })
             .attr("y2", function(d) { return x1(d[1]); })
             .remove();

         // Update innerquartile box.
         var box = g.selectAll("rect.box")
             .data([quartileData]);

         box.enter().append("rect")
             .attr("class", "box")
             .attr("x", 0)
             .attr("y", function(d) { return x0(d[2]); })
             .attr("width", width)
             .attr("height", function(d) { return x0(d[0]) - x0(d[2]); })
           .transition()
             .duration(duration)
             .attr("y", function(d) { return x1(d[2]); })
             .attr("height", function(d) { return x1(d[0]) - x1(d[2]); });

         box.transition()
             .duration(duration)
             .attr("y", function(d) { return x1(d[2]); })
             .attr("height", function(d) { return x1(d[0]) - x1(d[2]); });

         // Update median line.
         var medianLine = g.selectAll("line.median")
             .data([quartileData[1]]);

         medianLine.enter().append("line")
             .attr("class", "median")
             .attr("x1", 0)
             .attr("y1", x0)
             .attr("x2", width)
             .attr("y2", x0)
           .transition()
             .duration(duration)
             .attr("y1", x1)
             .attr("y2", x1);

         medianLine.transition()
             .duration(duration)
             .attr("y1", x1)
             .attr("y2", x1);

         // Update whiskers.
         var whisker = g.selectAll("line.whisker")
             .data(whiskerData || []);

         whisker.enter().insert("line", "circle, text")
             .attr("class", "whisker")
             .attr("x1", 0)
             .attr("y1", x0)
             .attr("x2", width)
             .attr("y2", x0)
             .style("opacity", 1e-6)
           .transition()
             .duration(duration)
             .attr("y1", x1)
             .attr("y2", x1)
             .style("opacity", 1);

         whisker.transition()
             .duration(duration)
             .attr("y1", x1)
             .attr("y2", x1)
             .style("opacity", 1);

         whisker.exit().transition()
             .duration(duration)
             .attr("y1", x1)
             .attr("y2", x1)
             .style("opacity", 1e-6)
             .remove();

         // Update outliers.
         var outlier = g.selectAll("circle.outlier")
             .data(outlierIndices, Number);

         outlier.enter().insert("circle", "text")
             .attr("class", "outlier")
             .attr("r", 5)
             .attr("cx", width / 2)
             .attr("cy", function(i) { return x0(d[i]); })
             .style("opacity", 1e-6)
           .transition()
             .duration(duration)
             .attr("cy", function(i) { return x1(d[i]); })
             .style("opacity", 1);

         outlier.transition()
             .duration(duration)
             .attr("cy", function(i) { return x1(d[i]); })
             .style("opacity", 1);

         outlier.exit().transition()
             .duration(duration)
             .attr("cy", function(i) { return x1(d[i]); })
             .style("opacity", 1e-6)
             .remove();

         // Compute the tick format.
         var format = tickFormat || x1.tickFormat(8);

         // Update box ticks.
         var boxTick = g.selectAll("text.box")
             .data(quartileData);

         boxTick.enter().append("text")
             .attr("class", "box")
             .attr("dy", ".3em")
             .attr("dx", function(d, i) { return i & 1 ? 6 : -6 })
             .attr("x", function(d, i) { return i & 1 ? width : 0 })
             .attr("y", x0)
             .attr("text-anchor", function(d, i) { return i & 1 ? "start" : "end"; })
             .text(format)
           .transition()
             .duration(duration)
             .attr("y", x1);

         boxTick.transition()
             .duration(duration)
             .text(format)
             .attr("y", x1);

         // Update whisker ticks. These are handled separately from the box
         // ticks because they may or may not exist, and we want don\'t want
         // to join box ticks pre-transition with whisker ticks post-.
         var whiskerTick = g.selectAll("text.whisker")
             .data(whiskerData || []);

         whiskerTick.enter().append("text")
             .attr("class", "whisker")
             .attr("dy", ".3em")
             .attr("dx", 6)
             .attr("x", width)
             .attr("y", x0)
             .text(format)
             .style("opacity", 1e-6)
           .transition()
             .duration(duration)
             .attr("y", x1)
             .style("opacity", 1);

         whiskerTick.transition()
             .duration(duration)
             .text(format)
             .attr("y", x1)
             .style("opacity", 1);

         whiskerTick.exit().transition()
             .duration(duration)
             .attr("y", x1)
             .style("opacity", 1e-6)
             .remove();
       });
       d3.timer.flush();
     }

     box.width = function(x) {
       if (!arguments.length) return width;
       width = x;
       return box;
     };

     box.height = function(x) {
       if (!arguments.length) return height;
       height = x;
       return box;
     };

     box.tickFormat = function(x) {
       if (!arguments.length) return tickFormat;
       tickFormat = x;
       return box;
     };

     box.duration = function(x) {
       if (!arguments.length) return duration;
       duration = x;
       return box;
     };

     box.domain = function(x) {
       if (!arguments.length) return domain;
       domain = x == null ? x : d3.functor(x);
       return box;
     };

     box.value = function(x) {
       if (!arguments.length) return value;
       value = x;
       return box;
     };

     box.whiskers = function(x) {
       if (!arguments.length) return whiskers;
       whiskers = x;
       return box;
     };

     box.quartiles = function(x) {
       if (!arguments.length) return quartiles;
       quartiles = x;
       return box;
     };

     return box;
    };

    function boxWhiskers(d) {
     return [0, d.length - 1];
    }

    function boxQuartiles(d) {
     return [
       d3.quantile(d, .25),
       d3.quantile(d, .5),
       d3.quantile(d, .75)
     ];
    }

    })();




    </script>
    <script>

    var margin = {top: 10, right: 50, bottom: 20, left: 50},
        width = 120 - margin.left - margin.right,
        height = 170 - margin.top - margin.bottom;

    var min = Infinity,
        max = -Infinity;

    var chart = d3.box()
        .whiskers(iqr(1.5))
        .width(width)
        .height(height);

    d3.csv("http://localhost/delphi/'.$csv2.'", function(error, csv) {
      var data = [];

      csv.forEach(function(x) {
        var e = Math.floor(x.Expt - 1),
            r = Math.floor(x.Run - 1),
            s = Math.floor(x.Speed),
            d = data[e];
        if (!d) d = data[e] = [s];
        else d.push(s);
        if (s > max) max = s;
        if (s < min) min = s;
      });

      chart.domain([min, max]);

      var svg = d3.select("#'.$id.'").selectAll("svg")
          .data(data)
        .enter().append("svg")
          .attr("class", "box")
          .attr("width", width + margin.left + margin.right)
          .attr("height", height + margin.bottom + margin.top)
        .append("g")
          .attr("transform", "translate(" + margin.left + "," + margin.top + ")")

          .call(chart);

    });


    // Returns a function to compute the interquartile range.
    function iqr(k) {
      return function(d, i) {
        var q1 = d.quartiles[0],
            q3 = d.quartiles[2],
            iqr = (q3 - q1) * k,
            i = -1,
            j = d.length;
        while (d[++i] < q1 - iqr);
        while (d[--j] > q3 + iqr);
        return [i, j];
      };
    }

    </script>';

        return $test;*/
}