<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 16.02.17
 * Time: 10:48
 */

require_once ("survey_create_boxplot.inc");

function survey_question_evaluation_form()
{

    //Die ID der Frage wird aus der URL gelesen
    $question_id = arg(1);

    //Es werden alle Fragen aus der Datenbank gelesen
    $questions = survey_get_all_questions();

    //DIe Überschrift wird erstellt
    $headline = $questions[$question_id - 1]->title . " - Überprüfung der eigenen Einschätzung";
    $headline = "
        <h1>Frage " . $question_id . "(b)</h1>
        <h2>" . $headline . "</h2><br>";

    $form['headline']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('Headline'),
        '#prefix' => $headline,
    );

    //In diese Variable werden alle Antwortmöglichkeiten der Frage eingespeichert
    $answers = array();

    //TODO Check for refactoring
    //SQL-> Get lables for radio buttons
    $sql = "SELECT * FROM {question_buttons_title} WHERE question_id = " . $questions[$question_id - 1]->question_id . " ORDER BY answer_id";
    $button_title_result = db_query($sql);

    //In diese Variable werden die Antwortmöglichkeiten für jede Frage eingefügt
    $button_title_array = array();

    //Diese Variable zählt die Durchgänge innerhalb der foreach-Schleife
    $iterations = 0;

    //Diese Variable zählt die Antwortmöglichkeiten jeder Antwort-ID
    $response_option_counter = 0;

    //Diese Variable dient zum Vergleich zweier Antwort-IDs
    $answer_id = 0;

    //Diese Variable zählt die Anzahl der Antworten
    $answer_id_counter = 0;

    //Es wird über alle Antwortmöglichkeiten iteriert
    foreach ($button_title_result as $button_title) {

        //Sollte es mindestens der zweite Durchlauf sein:
        if ($iterations > 0) {

            //Sollte die ID der aktuellen Antwort mit der vorigen übereinstimmen:
            if ($button_title->answer_id == $answer_id) {

                //Die Antwortmöglichkeit wird um eins erhöht und eingefügt
                $response_option_counter++;
                $button_title_array[$answer_id_counter][$response_option_counter] = $button_title->title;

            } else {

                //Sollte die ID der aktuellen Antwort nicht mit der vorigen übereinstimmen:
                $response_option_counter = 0;
                $answer_id_counter++;
                $button_title_array[$answer_id_counter][$response_option_counter] = $button_title->title;
                $answer_id = $button_title->answer_id;

            }
        } else {

            //Im ersten Durchlauf wird das Array mit der ersten Antwort-ID und der ersten Antwortmöglichkeit befüllt.
            $button_title_array[$answer_id_counter][$response_option_counter] = $button_title->title;
            $answer_id = $button_title->answer_id;
        }
        $iterations++;
    }

    //Die ID der Frage wird aus der URL gelesen
    $question_id = arg(1);

    //Die ID des aktuellen Benutzers wird aus der URL gelesen
    $user_id = arg(2);

    //Die gegebenen Antworten des Nutzers werden geladen
    $sql = "SELECT * FROM {question_user_answers} WHERE user_pw = '" . $user_id . "' AND question_id = " . $question_id;
    $answers_result = db_query($sql);

    //Die Antworten werden ausgelesen und in ein Array gespeichert
    foreach ($answers_result as $answer) {


        $check = searchForAnswerId($answer->answer_id, $answers);

        if ($check > -1) {
            $answers[$check] = $answer;
        } else {
            array_push($answers, $answer);
        }
    }
    //Es wird über alle Antworten iteriert (graues Feld in der UI)
    for ($i = 0; $i < sizeof($answers); $i++) {

        //Die Fragen-ID wird aus der URL ausgelesen
        $question_id = arg(1);

        //Es werden alle Fragen aus der Datenbank ausgelesen
        $questions = survey_get_all_questions();

        //Die aktuelle Fragen-ID wird ermittelt
        $question_id = $questions[$question_id - 1]->question_id;

        //Die Antwortmöglichkeiten der Frage werden geladen
        $sql = "SELECT * FROM {question_possible_answers} WHERE question_id = " . $question_id;

        $possible_answers_result = db_query($sql);

        //In dieses Array werden die Antwortmöglichkeiten der entsprechenden Frage gespeichert
        $possible_answers_array = array();

        //Es wird über alle Antwortmöglichkeiten der Frage iteriert
        foreach ($possible_answers_result as $possible_answer) {
            array_push($possible_answers_array, $possible_answer);
        }

        $form['fieldset' . $i] = array(
            '#type' => 'fieldset',
        );

        //Sollte die aktuelle Antwortmöglichkeit Radio-Buttons beinhalten:
        if ($possible_answers_array[$i]->isRadioButton) {

            //Der Boxplot zur Antwort wird geladen
            $boxplot_graphic = get_graphic($i, 1, $button_title_array[$i]);

            $form['fieldset' . $i]['my_markup' . $i] = array(
                '#markup' => $boxplot_graphic,
            );

            $form['fieldset' . $i]['dim' . $i] = array(
                '#type' => 'radios',
                '#title' => $possible_answers_array[$i]->description,
                '#options' => $button_title_array[$i],
                '#default_value' => $answers[$i]->answer,
            );
            $form['fieldset' . $i]['comment_dim' . $i] = array(
                '#title' => t('Kommentar'),
                '#resizable' => FALSE,
                '#type' => 'textarea',
                '#default_value' => $answers[$i]->comment,
            );

            //Sollte die aktuelle Antwortmöglichkeit eine freie Antwort beinhalten:
        } else {

            //Der Boxplot zur Antwort wird geladen
            $boxplot_graphic = get_graphic($i, 0, array('Min', 'Max'));

            $form['fieldset' . $i]['my_markup' . $i] = array(
                '#markup' => $boxplot_graphic,
            );
            $form['fieldset' . $i]['quantity' . $i] = array(
                '#type' => 'textfield',
                '#title' => $possible_answers_array[$i]->description,
                '#size' => 2,
                '#maxlength' => 2,
                '#default_value' => $answers[$i]->answer,
            );

            $form['fieldset' . $i]['comment_dim' . $i] = array(
                '#title' => t('Kommentar'),
                '#resizable' => FALSE,
                '#type' => 'textarea',
                '#default_value' => $answers[$i]->comment,
            );
        }

    }

    //Sollte es sich um die erste Frage der Umfrage handeln, wird ein Zurück-Button erstellt
    if ($question_id == 1) {
    } else {
        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('survey_get_back_to_start')
        );
    }

    //Der zweite Button führt zur zweiten Frage
    $button_text = "Weiter zur nächsten Frage";

    //Die ID der nächsten Frage wird ermittelt
    $id = survey_get_questions($question_id);

    //Sollte es die letzte Frage sein, wird die Umfrage beendet
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

/*
 *
 */
function searchForAnswerId($id, $array)
{

    $check = -1;

    foreach ($array as $key => $val) {
        if (!strcmp($val->answer_id, $id)) {
            $check = $key;
        }
    }
    return $check;
}

/*
 * Diese Funktion speichert die eingegebenen Daten des Nutzers in die Datenbank und führt zur nächsten Frage, falls
 * noch eine vorhanden ist. Andernfalls wird die Umfrage beendet
 */
function survey_get_next_question($form, &$form_state)
{

    //Die ID der aktuellen Frage wird aus der URL geladen
    $question = arg(1);

    //Die ID des aktuellen Nutzers wird aus der URL geladen
    $user_id = arg(2);

    //Es werden alle Fragen aus der Datenbank geladen
    $questions = survey_get_all_questions();

    //Es werden alle Antwortmöglichkeiten aus der Datenbank geladen
    $answers = survey_get_answers_from_question($questions[$question - 1]->question_id);

    //Es wird über alle Antwortmöglichkeiten iteriert
    for ($i = 0; $i < sizeof($answers); $i++) {

        //Die Eingaben des Nutzers werden erfasst und in eine Variable gespeichert
        if ($answers[$i]->isRadioButton) {
            $dim1 = $form_state['values']['dim' . $i];
        } else {
            $dim1 = $form_state['values']['quantity' . $i];
        }

        $comment = $form_state['values']['comment_dim' . $i];

        //Die aktuelle Antwort des Nutzers zur Antwort wird aus der Datenbank geladen
        $sql = "SELECT * FROM {question_user_answers} WHERE user_pw = '" . $user_id . "' AND question_id = " . $question . " AND answer_id = " . $i . " ORDER BY question_user_answers_id DESC LIMIT 1";
        $result = db_query($sql);

        foreach ($result as $item) {

            //Sollte bereits eine Antwort vorliegen, wird diese aktualisiert
            if (!strcmp($dim1, $item->answer)) {
                $sql = "UPDATE {question_user_answers} SET comment = '" . $comment . "' WHERE user_pw = '" . $user_id . "' AND question_id = " . $question . " AND answer_id = " . $i;
                db_query($sql);
            } else {
                db_insert('question_user_answers')
                    ->fields(array(
                        'question_id' => $question,
                        'answer_id' => $i,
                        'answer' => $dim1,
                        'user_pw' => $user_id,
                        'comment' => $comment,
                    ))
                    ->execute();
            }
        }
    }

    //Es wird ermittelt, ob es eine weitere Frage gibt. Wenn ja, wird zur nächsten Frage weitergeleitet, andernfalls
    //wird die Umfrage beendet
    $id = survey_get_questions($question);

    if ($id == -1) {
        drupal_goto("finish_survey/" . $user_id);
    } else {
        drupal_goto("survey_question/" . $id . "/" . $user_id);
    }
}

//Get id for boxplot
function searchForId($id, $array)
{

    $check = -1;

    foreach ($array as $key => $val) {
        if (!strcmp($val->user_pw, $id)) {
            $check = $key;
        }
    }
    return $check;
}

//Create boxplot
function get_graphic($id, $isRadioButton, $links)
{

    survey_testtest($id, $isRadioButton, $links);


    $test = "";

    $question_id = arg(1);
    $user_pw = arg(2);


    $median = 0;
    $first_quantil = 0;
    $third_quantil = 0;
    $avg = 0;

    $median_absolut = 0;
    $first_quantil_absolut = 0;
    $third_quantil_absolut = 0;
    $avg_absolut = 0;


    $testArrayOverall = array();
    $testArrayOverall2 = array();

    $sql = "SELECT * FROM {question_user_answers} WHERE question_id = " . $question_id . " AND answer_id = " . $id;
    $result = db_query($sql);

    foreach ($result as $item4) {
        if (strcmp($user_pw, $item4->user_pw)) {
            $user = $item4->user_pw;
            $check = searchForId($user, $testArrayOverall2);

            if ($check > -1) {
                unset($testArrayOverall2[$check]);
                unset($testArrayOverall[$check]);
            }
            array_push($testArrayOverall2, $item4);
            array_push($testArrayOverall, $item4->answer);
        }
    }

    sort($testArrayOverall);

    for($i=0;$i<sizeof($testArrayOverall);$i++) {
        if($testArrayOverall[$i] >4) {
            unset($testArrayOverall[$i]);
        }
    }

    if($isRadioButton) {
        $min = 1;

        $max = 5;
        $avg = survey_get_average($testArrayOverall, $isRadioButton, 4);
        $median = survey_get_median($testArrayOverall, $isRadioButton, 4);
        $first_quantil = survey_get_first_quantil($testArrayOverall, $isRadioButton,4);
        $third_quantil = survey_get_third_quantil($testArrayOverall, $isRadioButton, 4);



        $third_quantil2 = $third_quantil["procent"] - $first_quantil["procent"];
    } else {
        $max = array_pop($testArrayOverall);
        $avg = survey_get_average($testArrayOverall, $isRadioButton, $max);
        $median = survey_get_median($testArrayOverall, $isRadioButton, $max);
        $first_quantil = survey_get_first_quantil($testArrayOverall, $isRadioButton,$max);
        $third_quantil = survey_get_third_quantil($testArrayOverall, $isRadioButton, $max);

        $min = array_shift($testArrayOverall);

        $max = array_pop($testArrayOverall);

        $third_quantil2 = $third_quantil["procent"] - $first_quantil["procent"];
    }

    print_r($links);

    $links = array(0 =>"unteres Quartil", 1=>"Average", 2=>"Median", 3=>"oberes Quartil");

        $test = array(
            'container' => array(
                '#prefix' => '<div id="eins">',
                '#suffix' => '</div>',
                'boxplot' => array(
                    '#prefix' => '<div class="boxplot">',
                    '#suffix' => '</div>',
                    'boxlinie' => array(
                        '#prefix' => '<div class="box linie">',
                        '#suffix' => '</div>',
                    ),
                    'boxwhisker' => array(
                        '#prefix' => '<div class="box whisker">',
                        '#suffix' => '</div>',
                    ),
                    'boxinterquant' => array(
                        '#prefix' => '<div class="box interquart" style="margin-left: ' . $first_quantil["procent"] . '%;width: ' . $third_quantil2 . '%;">',
                        '#suffix' => '</div>',
                    ),
                    'boxmedian' => array(
                        '#prefix' => '<div class="box median" style="margin-left: ' . $median["procent"] . '%;">',
                        '#suffix' => '</div>',
                    ),
                    'boxmittel' => array(
                        '#prefix' => '<div class="box mittel" style="margin-left: ' . $avg["procent"] . '%;">',
                        '#suffix' => '</div>',
                    ),
                    's_min' => array(
                        '#prefix' => '<span class="schild s_min" style="margin-left: 0%;">',
                        '#suffix' => '</span>',
                        'markup' => array(
                            '#markup' => $min,
                        ),
                    ),
                    's_average' => array(
                        '#prefix' => '<span class="schild min s_average" style="margin-left: ' . $avg["procent"] . '%;">',
                        '#suffix' => '</span>',
                        'markup' => array(
                            '#markup' => $avg["absolut"],
                        ),
                    ),
                    's_median' => array(
                        '#prefix' => '<span class="schild min s_median" style="margin-left: ' . $median["procent"] . '%;"> ',
                        '#suffix' => '</span>',
                        'markup' => array(
                            '#markup' => $median["absolut"],
                        ),
                    ),
                    's_third_quantil' => array(
                        '#prefix' => '<span class="schild min s_third_quantil" style="margin-left: ' . $third_quantil["procent"] . '%;"> ',
                        '#suffix' => '</span>',
                        'markup' => array(
                            '#markup' => $third_quantil["absolut"],
                        ),
                    ),
                    's_first_quantil' => array(
                        '#prefix' => '<span class="schild s_first_quantil" style="margin-left: ' . $first_quantil["procent"] . '%;"> ',
                        '#suffix' => '</span>',
                        'markup' => array(
                            '#markup' => $first_quantil["absolut"],
                        ),
                    ),
                    's_max' => array(
                        '#prefix' => '<span class="schild min s_max" style="margin-left: 100%;"> ',
                        '#suffix' => '</span>',
                        'markup' => array(
                            '#markup' => $max,
                        ),
                    ),
                ),
                'legend' => array(
                    '#theme' => 'item_list',
                    '#items' => $links,
                    '#type' => 'ul',
                    '#prefix' => '<div class="legend">',
                    '#suffix' => '</div>',
                    '#attributes' => array('class' => 'my-list'),
                ),
            ),



        );

    //}


    return render($test);
}

function getQuantil($array)
{
    $returnArray = array();
    $temp = sizeof($array) / 2;
    if (is_int($temp)) {
        $returnArray["number"] = $temp;
        $returnArray["check"] = true;

    } else {
        $temp = round($temp, 0, PHP_ROUND_HALF_DOWN);
        $returnArray["number"] = $temp;
        $returnArray["check"] = false;
    }

    return $returnArray;
}

function roundToValidValue($value)
{

    $tempzahl = explode(".", $value);

    if (isset($tempzahl[1])) {

        $value = round($value, 1);
        $tempzahl = explode(".", $value);


        switch ($tempzahl[1]) {
            case $tempzahl[1] >= 0 && $tempzahl[1] < 3:
                $tempzahl[1] = 0;
                break;
            case $tempzahl[1] >= 3 && $tempzahl[1] < 7:
                $tempzahl[1] = 5;
                break;
            case $tempzahl[1] >= 7:
                $tempzahl[0] = $tempzahl[0] + 1;
                $tempzahl[1] = 0;
                break;
            default:
                break;
        }

        if ($tempzahl[1] == 0) {
            $zahl = $tempzahl[0];
        } else {

            if ($tempzahl[0] != 0) {
                $zahl = implode(".", $tempzahl);
            } else {
                $zahl = 1;
            }
        }

    } else {
        $zahl = $value;
    }
    return $zahl;
}
