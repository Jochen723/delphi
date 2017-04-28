<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 16.02.17
 * Time: 10:48
 */

function survey_question_evaluation_form() {

    $question_id = arg(1);


    $questions = survey_get_all_questions();
    $headline = $questions[$question_id - 1]->title . " - Überprüfung der eigenen Einschätzung";
    $headline = "<h1>Frage ".$question_id."(b)</h1><h2>".$headline."</h2><br>";

    $form['headline']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('Headline'),
        '#prefix' => $headline,
    );

    $answers = array();
    
    $a = array();

    //$radio_labels = array(1=>"1", 2=>"2", 3=>"3", 4=>"4", 5=>"5", 6=>"k.A.");


    $sql = "SELECT * FROM {question_buttons_title} WHERE question_id = ".$questions[$question_id - 1]->question_id . " ORDER BY answer_id";
    $result = db_query($sql);

    $radio_labels = array(1 => "", 2 => "", 3 => "", 4 => "", 5 => "", 6 => "k.A.");

    $bezeichnungsArray = array();

    $durchlaufsID = 0;

    $checkID = 0;

    $answer_id = 0;

    $uebereinstimmungsHelper = 0;
    foreach ($result as $item) {

        if($durchlaufsID > 0) {
            if($item->answer_id == $answer_id) {
                $checkID++;
                $bezeichnungsArray[$uebereinstimmungsHelper][$checkID] = $item->title; //Array[0][0]

            } else {
                $checkID = 0;
                $uebereinstimmungsHelper++;
                $bezeichnungsArray[$uebereinstimmungsHelper][$checkID] = $item->title; //Array[0][0]
                $answer_id = $item->answer_id;

            }
        } else {
            $bezeichnungsArray[$uebereinstimmungsHelper][$checkID] = $item->title; //Array[0][0]
            $answer_id = $item->answer_id;
        }

        $durchlaufsID++;

    }


    $question_id = arg(1);
    $user_id = arg(2);
    $sql = "SELECT * FROM {question_user_answers} WHERE user_pw = '" . $user_id . "' AND question_id = ".$question_id;
    $result = db_query($sql);
    foreach ($result as $item) {
        $check = searchForAnswerId($item->answer_id,$answers);

        if($check >-1) {
            $answers[$check] = $item;
        } else {
            array_push($answers, $item);
        }
    }

    for ($i = 0; $i <sizeof($answers); $i++) {

        $question_id = arg(1);
        $questions = survey_get_all_questions();
        $question_id = $questions[$question_id - 1]->question_id;

        $sql = "SELECT * FROM {question_possible_answers} WHERE question_id = ".$question_id;
        $result = db_query($sql);

        foreach ($result as $item2) {
            array_push($a, $item2);
        }

        $form['fieldset'.$i] = array(
            '#type' => 'fieldset',
        );


        if($a[$i]->isRadioButton) {

            $boxplot_graphic = get_graphic($i, 1);

            $form['fieldset'.$i]['my_markup'.$i] = array(
                '#markup' => $boxplot_graphic,
            );
            $form['fieldset'.$i]['dim'.$i] = array(
                '#type' => 'radios',
                '#title' => $a[$i]->description,
                '#options' => $bezeichnungsArray[$i],
                '#default_value' => $answers[$i]->answer,
            );
            $form['fieldset'.$i]['comment_dim'.$i] = array(
                '#title' => t('Kommentar'),
                '#resizable' => FALSE,
                '#type' => 'textarea',
                '#default_value' => $answers[$i]->comment,
            );


        } else {

            $boxplot_graphic = get_graphic($i, 0);
            $form['fieldset'.$i]['my_markup'.$i] = array(
                '#markup' => $boxplot_graphic,
            );
            $form['fieldset'.$i]['quantity' . $i] = array(
                '#type' => 'textfield',
                '#title' => $a[$i]->description,
                '#size' => 2,
                '#maxlength' => 2,
                '#default_value' => $answers[$i]->answer,
            );

            $form['fieldset'.$i]['comment_dim'.$i] = array(
                '#title' => t('Kommentar'),
                '#resizable' => FALSE,
                '#type' => 'textarea',
                '#default_value' => $answers[$i]->comment,
            );
        }


    }

    if($question_id == 1) {
    } else {
        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('survey_get_back_to_start')
        );
    }

    $button_text = "Weiter zur nächsten Frage";
    $id = survey_get_questions($question_id);


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

function searchForAnswerId($id, $array) {

    $check = -1;

    foreach ($array as $key => $val) {
        if (!strcmp($val->answer_id,$id)) {
            $check = $key;
        }
    }
    return $check;
}

function survey_get_next_question($form, &$form_state) {


    $question = arg(1);
    $user_id = arg(2);



    $questions = survey_get_all_questions();

    $answers = survey_get_answers_from_question($questions[$question - 1]->question_id);

    for ($i = 0; $i < sizeof($answers); $i++) {

        if ($answers[$i]->isRadioButton) {
            $dim1 = $form_state['values']['dim' . $i];
        } else {

            $dim1 = $form_state['values']['quantity' . $i];

        }

        $comment = $form_state['values']['comment_dim' . $i];

        $sql = "SELECT * FROM {question_user_answers} WHERE user_pw = '" . $user_id . "' AND question_id = ".$question. " AND answer_id = ".$i. " ORDER BY question_user_answers_id DESC LIMIT 1";
        $result = db_query($sql);
        foreach ($result as $item) {
            if(!strcmp($dim1, $item->answer)) {
                $sql = "UPDATE {question_user_answers} SET comment = '".$comment."' WHERE user_pw = '" . $user_id . "' AND question_id = ".$question. " AND answer_id = ".$i;
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




    $id = survey_get_questions($question);

    if ($id == -1) {
        drupal_goto("finish_survey/".$user_id);
    } else {
        drupal_goto("survey_question/".$id."/".$user_id);
    }
}

function searchForId($id, $array) {

    $check = -1;

    foreach ($array as $key => $val) {
        if (!strcmp($val->user_pw,$id)) {
            $check = $key;
        }
    }
    return $check;
}

function get_graphic($id, $isRadioButton) {

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

    $sql = "SELECT * FROM {question_user_answers} WHERE question_id = ".$question_id." AND answer_id = ".$id;
    $result = db_query($sql);

    foreach ($result as $item4) {
        if(strcmp($user_pw, $item4->user_pw)) {
            $user = $item4->user_pw;
            $check = searchForId($user, $testArrayOverall2);

            if($check >-1) {
                unset($testArrayOverall2[$check]);
                unset($testArrayOverall[$check]);
            }
            array_push($testArrayOverall2, $item4);
            array_push($testArrayOverall, $item4->answer+1);
        }
    }



    $rangeArray = array("1"=>0,"1.5"=>12.5,"2"=>25,"2.5"=>37.5,"3"=>50,"3.5"=>62.5,"4"=>75,"4.5"=>87.5,"5" =>100);

    sort($testArrayOverall);

    if(sizeof($testArrayOverall)>3) {

        $avg = (!empty($testArrayOverall) ? array_sum($testArrayOverall) / count($testArrayOverall) : 0);
+        $avg = roundToValidValue($avg);
+        $avg_absolut = $avg;
        if($isRadioButton) {
            $avg = $rangeArray["$avg"];

        }


        $temp1 = getQuantil($testArrayOverall);
        if($temp1["check"]) {
            $median = ($testArrayOverall[$temp1["number"]-1]+$testArrayOverall[$temp1["number"]])/2;
            $median_absolut = $median;
            if($isRadioButton) {
                $median = $rangeArray["$median"];
            }


        } else {
            $median = $testArrayOverall[$temp1["number"]];
            $median_absolut = $median;
            if($isRadioButton) {
                $median = $rangeArray["$median"];
            }

        }

        $testArray = array_slice($testArrayOverall,0, $temp1["number"]);

        $temp2 = getQuantil($testArray);
        if($temp2["check"]) {
            $first_quantil = ($testArray[$temp2["number"]-1]+$testArray[$temp2["number"]])/2;
            $first_quantil_absolut = $first_quantil;
            if($isRadioButton) {
                $first_quantil = $rangeArray["$first_quantil"];
            }


        } else {
            $first_quantil = $testArray[$temp2["number"]];
            $first_quantil_absolut = $first_quantil;
            if($isRadioButton) {
                $first_quantil = $rangeArray["$first_quantil"];
            }

        }

        $testArray = array_slice($testArrayOverall,$temp1["number"]+1);

        $temp3 = getQuantil($testArray);

        if($temp3["check"]) {
            $third_quantil = ($testArray[$temp3["number"]-1]+$testArray[$temp3["number"]])/2;
            $third_quantil_absolut = $third_quantil;
            if($isRadioButton) {
                $third_quantil = $rangeArray["$third_quantil"];
            }


        } else {
            $third_quantil = $testArray[$temp3["number"]];
            $third_quantil_absolut = $third_quantil;
            if($isRadioButton) {
                $third_quantil = $rangeArray["$third_quantil"];
            }

        }




        $third_quantil2 = $third_quantil-$first_quantil;




    if($isRadioButton) {
        $min = 1;
        $max = 5;

    } else {

        $min = $testArrayOverall[0];
        $max = $testArrayOverall[sizeof($testArrayOverall)-1];

        $median = round($median/$max*100);
        $third_quantil = round($third_quantil/$max*100);

        $first_quantil = round($first_quantil/$max*100);

        $third_quantil2 = $third_quantil-$first_quantil;

        $avg = round($avg/$max*100);
    }

	  $links = array('Min = ' . $min, 'Max = ' . $max,); //TODO Max min werte

	  //Boxplot
	  $test = array(
		'container' => array(
		  '#prefix' => '<div id="eins">',
		  '#suffix' => '</div>',
		  'markup' => array(
			'#markup' => 'This is inside the container div',
		  ),
		  'boxplot' => array(
			'#prefix' => '<div class="boxplot">',
			'#suffix' => '</div>',
			'boxlinie' => array(
			  '#prefix' => '<div class="box linie">',
			  '#suffix' =>'</div>',
			),
			'boxwhisker' => array(
			  '#prefix' => '<div class="box whisker">',
			  '#suffix' =>'</div>',
			),
			'boxinterquant' => array(
			  '#prefix' => '<div class="box interquart" style="margin-left: '. $first_quantil. '%;width: '.$third_quantil2.'%;">',
			  '#suffix' =>'</div>',
			),
			'boxmedian' => array(
			  '#prefix' => '<div class="box median" style="margin-left: '.$median.'%;">',
			  '#suffix' =>'</div>',
			),
			'boxmittel' => array(
			  '#prefix' => '<div class="box mittel" style="margin-left: '.$avg.'%;">',
			  '#suffix' =>'</div>',
			),
			's_min' => array(
			  '#prefix' => '<span class="schild s_min" style="margin-left: 0%;">',
			  '#suffix' =>'</span>',
			  'markup' => array(
				'#markup' => $min,
			  ),
			),
			's_average' => array(
			  '#prefix' => '<span class="schild min s_average" style="margin-left: '.$avg.'%;">',
			  '#suffix' =>'</span>',
			  'markup' => array(
				'#markup' => $avg_absolut,
			  ),
			),
			's_median' => array(
			  '#prefix' => '<span class="schild min s_median" style="margin-left: '.$median.'%;"> ',
			  '#suffix' =>'</span>',
			  'markup' => array(
				'#markup' => $median_absolut,
			  ),
			),
			's_third_quantil' => array(
			  '#prefix' => '<span class="schild min s_third_quantil" style="margin-left: '.$third_quantil.'%;"> ',
			  '#suffix' =>'</span>',
			  'markup' => array(
				'#markup' => $third_quantil_absolut,
			  ),
			),
			's_first_quantil' => array(
			  '#prefix' => '<span class="schild s_first_quantil" style="margin-left: '.$first_quantil.'%;"> ',
			  '#suffix' =>'</span>',
			  'markup' => array(
				'#markup' => $first_quantil_absolut,
			  ),
			),
			's_max' => array(
			  '#prefix' => '<span class="schild min s_max" style="margin-left: 100%;"> ',
			  '#suffix' =>'</span>',
			  'markup' => array(
				'#markup' => $max,
			  ),
			),
			'legend' => array(
			  '#theme' => 'item_list',
			  '#items' => $links,
			  '#type' => 'ul',
			  '#attributes' => array('class' => 'my-list'),
			),
		  ),
		),
	  );

    }



        return render($test);
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

            if($tempzahl[0] != 0) {
                $zahl = implode(".",$tempzahl);
            } else {
                $zahl = 1;
            }

        }


    } else {
        $zahl = $value;
    }
    return $zahl;
}
