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

    global $user;
    $uid = $user->uid;

    $text = survey_get_question_content($question);

    $headline = "<h1>Frage ".$question."(b)</h1><h2>".$text."</h2><br><br>";

    $form['test']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $headline,
    );

    $ar = survey_get_answers();


    $testxx = get_graphic();

    $form['my_markup'] = array(
        '#markup' => $testxx,
    );


    $i=1;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $testxx = get_graphic();

    $form['my_markup2'] = array(
        '#markup' => $testxx,
    );

    $i=2;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $testxx = get_graphic();

    $form['my_markup3'] = array(
        '#markup' => $testxx,
    );


    $i=3;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $testxx = get_graphic();

    $form['my_markup4'] = array(
        '#markup' => $testxx,
    );


    $i=4;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );

    $testxx = get_graphic();

    $form['my_markup5'] = array(
        '#markup' => $testxx,
    );


    $i=5;

    $form['dim'.$i] = array(
        '#type' => 'radios',
        '#title' => t('Dimension '.$i),
        '#default_value' => $ar["dim".$i],
        '#options' => $test,
    );


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

function get_graphic() {

    $median = 0;
    $first_quantil = 0;
    $third_quantil = 0;
    $avg = 0;

    $median_absolut = 0;
    $first_quantil_absolut = 0;
    $third_quantil_absolut = 0;
    $avg_absolut = 0;

    $testArrayOverall = array(1,1,1,1,5,5,5,5,5,5,5,5,5,5,5,5);
    $rangeArray = array("1"=>0,"1.5"=>12.5,"2"=>25,"2.5"=>37.5,"3"=>50,"3.5"=>62.5,"4"=>75,"4.5"=>87.5,"5" =>100);


    sort($testArrayOverall);

    if(sizeof($testArrayOverall)>3) {

        $avg = (!empty($testArrayOverall) ? array_sum($testArrayOverall) / count($testArrayOverall) : 0);
        $avg = roundToValidValue($avg);
        $avg_absolut = $avg;
        $avg = $rangeArray["$avg"];



        $temp1 = getQuantil($testArrayOverall);
        if($temp1["check"]) {
            $median = ($testArrayOverall[$temp1["number"]-1]+$testArrayOverall[$temp1["number"]])/2;
            $median_absolut = $median;
            $median = $rangeArray["$median"];

        } else {
            $median = $testArrayOverall[$temp1["number"]];
            $median_absolut = $median;
            $median = $rangeArray["$median"];
        }

        $testArray = array_slice($testArrayOverall,0, $temp1["number"]);

        $temp2 = getQuantil($testArray);
        if($temp2["check"]) {
            $first_quantil = ($testArray[$temp2["number"]-1]+$testArray[$temp2["number"]])/2;
            $first_quantil_absolut = $first_quantil;
            $first_quantil = $rangeArray["$first_quantil"];

        } else {
            $first_quantil = $testArray[$temp2["number"]];
            $first_quantil_absolut = $first_quantil;
            $first_quantil = $rangeArray["$first_quantil"];
        }

        $testArray = array_slice($testArrayOverall,$temp1["number"]+1);

        $temp3 = getQuantil($testArray);

        if($temp3["check"]) {
            $third_quantil = ($testArray[$temp3["number"]-1]+$testArray[$temp3["number"]])/2;
            $third_quantil_absolut = $third_quantil;
            $third_quantil = $rangeArray["$third_quantil"];

        } else {
            $third_quantil = $testArray[$temp3["number"]];
            $third_quantil_absolut = $third_quantil;
            $third_quantil = $rangeArray["$third_quantil"];
        }

        /*
        drupal_set_message("Median: ".$median_absolut." => ".$median."%");
        drupal_set_message("First: ".$first_quantil_absolut." => ".$first_quantil."%");
        drupal_set_message("Third: ".$third_quantil_absolut." => ".$third_quantil."%");
        drupal_set_message("Average: ".$avg_absolut." => ".$avg."%");
*/


        $third_quantil = $third_quantil-$first_quantil;
    }


    $test = '
    <div id="eins" class="container">
        <div class="boxplot">
            <div class="linie box"> </div>
            <div class="whisker box"> </div>
            <div class="interquart box" style="margin-left: '.$first_quantil.'%;width: '.$third_quantil.'%;"> </div>
            <div class="median box" style="margin-left: '.$median.'%;"> </div>
            <div class="mittel box" style="margin-left: '.$avg.'%;"> </div>
        </div>
    </div>







    ';

        return $test;
}

function getQuantil($array) {
    $returnArray = array();
    $temp = sizeof($array)/2;
    if(is_int($temp)) {
        $returnArray["number"] = $temp;
        $returnArray["check"] = true;

    } else {
        $temp = round($temp, 0, PHP_ROUND_HALF_DOWN);
        $returnArray["number"] = $temp;
        $returnArray["check"] = false;
    }

    return $returnArray;
}

function roundToValidValue($value) {
    $tempzahl = explode(".", $value);
    if (isset($tempzahl[1])) {
        $value = round($value, 1);
        $tempzahl = explode(".", $value);
        switch($tempzahl[1]) {
            case $tempzahl[1] >=0 && $tempzahl[1] <3:
                $tempzahl[1] = 0;
                break;
            case $tempzahl[1] >=3 && $tempzahl[1] <7:
                $tempzahl[1] = 5;
                break;
            case $tempzahl[1] >=7:
                $tempzahl[0] = $tempzahl[0]+1;
                $tempzahl[1] = 0;
                break;
            default:
                break;
        }

        if($tempzahl[1] == 0) {
            $zahl = $tempzahl[0];
        } else {
            $zahl = implode(".",$tempzahl);
        }


    } else {
        $zahl = $value;
    }
    return $zahl;
}