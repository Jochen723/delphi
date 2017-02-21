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

    $testxx = get_graphic("data.csv", "myDiv");
    $testxx2 = get_graphic("data.csv", "myDiv2");
    $testxx3 = get_graphic("data.csv", "myDiv3");
    $testxx4 = get_graphic("data.csv", "myDiv4");
    $testxx5 = get_graphic("data.csv", "myDiv5");

    /*

    $form['test']['form_item22'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => '<div id="myDiv"></div>',
    );


    $form['test']['form_item22'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => '<div id="myDiv2"></div>',
    );

    $form['test']['form_item22'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $testxx2. '<div id="myDiv3"></div>',
    );




    /*
    $form['test']['form_item223'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => '<div id="visualization2">'.$testxx2.'</div>',
    );*/

    $form['my_markup'] = array(
        '#markup' => '<div id="myDiv">'.$testxx.'</div>'
    );

    /*

    $form['my_markup2'] = array(
        '#markup' => '<div id="myDiv2"><p>Lorem ipsum</p></div>'.$testxx2.''
    );*/

    $i=1;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $form['my_markup2'] = array(
        '#markup' => '<div id="myDiv2">'.$testxx2.'</div>'
    );

    $i=2;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $form['my_markup3'] = array(
        '#markup' => '<div id="myDiv3">'.$testxx3.'</div>'
    );

    $i=3;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $form['my_markup4'] = array(
        '#markup' => '<div id="myDiv4">'.$testxx4.'</div>'
    );

    $i=4;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $form['my_markup5'] = array(
        '#markup' => '<div id="myDiv5">'.$testxx5.'</div>'
    );

    $i=5;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );


    /*
    for($i = 1;$i<=$id;$i++) {

        $form['dim'.$i] = array(
            '#type' => 'radios',
            '#title' => t('Dimension '.$i),
            '#default_value' => $ar["dim".$i],
            '#options' => $test,

        );
    }
    */

    /*

    $form['my_markup2'] = array(
        '#markup' => '<div id="myDiv2"><p>Lorem ipsum</p></div>'.$testxx2.''
    );

    */

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


    /*

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

*/




$test = '  <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>

        <script>
        var trace1 = {
  x: [1, 1, 1, 1, 1, 4, 8, 9, 10],
  type: \'box\',
  name: \'Set 1\'
};

var data = [trace1];

var layout = {};

Plotly.newPlot(\''.$id.'\', data, layout);
        </script>';

        return $test;
}