<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 12.07.17
 * Time: 15:47
 */

function survey_change_question_form() {

    $question_id = arg(1);
    $id = arg(2);
    $title = "";

    $sql = "SELECT * FROM {question} WHERE question_id = ".$question_id;
    $result = db_query($sql);
    foreach ($result as $item) {
        $title = $item->title;
    }

    //Textfeld, in das der Titel der Frage eingesetzt wird
    $form['question'] = array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => "Titel der Frage",
        '#default_value' => $title,
        '#attributes' => array(
            'id' => 'question-field',
        ),
    );

    //Textfeld, in das die Anzahl der Antwortmöglichkeiten eingegeben werden soll
    $form['quantity'] = array(
        '#type' => 'textfield',
        '#title' => "Anzahl der Antwortmöglichkeiten",
        '#size' => 2,
        '#maxlength' => 1,
        '#default_value' => $id,

    );

    //Button, der die Änderung der Anzahl von Antwortmöglichkeiten übernimmt
    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Ändern',
        '#submit' => array('survey_change_question_change_quantity')
    );


    $answer_array = array();
    $check_array = array();
    $answer_id_array = array();
    $answer_counter = 0;
    $sql = "SELECT * FROM {question_possible_answers} WHERE question_id = ".$question_id . " ORDER BY question_id";

    $result = db_query($sql);

    foreach ($result as $item) {
        array_push($answer_array, $item->description);
        array_push($check_array, $item->isRadioButton);
        array_push($answer_id_array, $item->answers_id);
        $answer_counter++;
    }

    while (sizeof($answer_array) <6) {
        array_push($answer_array, "");
    }

    //Diese Schleife wird so oft durchgeführt, wie viele Antwortmöglichkeiten es geben soll
    for ($i = 1; $i <= $id; $i++) {
        //Für jede Antwortmöglichkeit wird ein Fieldset zur Übersichtlichkeit angelegt
        $form['content' . $i] = array(
            '#title' => $i. " Antwortmöglichkeit",
            '#type' => 'fieldset',
            '#description' => ""
        );

        //Für jede Antwortmöglichkeit muss eine Bezeichnung festgelegt werden.
        $form['content' . $i]['test' . $i] = array(
            '#type' => 'textfield',
            '#title' => "<b>Bezeichnung der Antwortmöglichkeit:</b>",
            '#default_value' => $answer_array[$i-1],
            '#required' => TRUE,
        );

        $answer_choose_array = array(1 => "Radio-Buttons", 0 => "Textfeld");

        //Für jede Antwortmöglichkeit muss bestimmt werden, ob die Antwort per Radio-Buttons oder per Textfeld
        //gegeben werden soll
        $form['content' . $i]['radios' . $i] = array(
            '#type' => 'radios',
            '#title' => 'Art der Antwortmöglichkeit:',
            '#default_value' => $check_array[$i-1],
            '#options' => $answer_choose_array,
        );


        $button_counter_result = 0;
        $button_title_array = array();


        if($check_array[$i-1]) {
            $sql = "SELECT * FROM {question_buttons_title} WHERE answer_id = ".$answer_id_array[$i-1];

            $button_result = db_query($sql);

            foreach ($button_result as $item_button_result) {
                $button_counter_result= $button_counter_result+1;
                array_push($button_title_array, $item_button_result->title);
            }

            while (sizeof($button_title_array) <6) {
                array_push($button_title_array, "Dimension");
            }
        } else {
            array_push($button_title_array, "Dimension");
            array_push($button_title_array, "Dimension");
            array_push($button_title_array, "Dimension");
            array_push($button_title_array, "Dimension");
            array_push($button_title_array, "Dimension");
            array_push($button_title_array, "Dimension");
            array_push($button_title_array, "Dimension");
            $button_counter_result = 4;
        }





        $button_array = array(4 => "4", 5 => "5", 6 => "6");
        $var = $i;

        //Sollte es sich bei der Antwortmöglichkeit um Radio-Buttons handeln, muss festgelegt werden, viele viele
        //Buttons für die Antwort vorgesehen sind.
        $form['content' . $i]['button_radios' . $i] = array(
            '#type' => 'radios',
            '#title' => "<b>Anzahl der vorhandenen Radio-Boxen</b>",
            '#default_value' => $button_array[$button_counter_result],
            '#options' => $button_array,
            '#states' => array(
                'visible' => array(
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );



        $form['content' . $i]['textfield_first_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der ersten Radio-Box",
            '#default_value' => $button_title_array[0],
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );


        $form['content' . $i]['textfield_second_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der zweiten Radio-Box",
            '#default_value' => $button_title_array[1],
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );

        $form['content' . $i]['textfield_third_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der dritten Radio-Box",
            '#default_value' => $button_title_array[2],
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );

        $form['content' . $i]['textfield_fourth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der vierten Radio-Box",
            '#default_value' => $button_title_array[3],
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );

        $form['content' . $i]['textfield_fiveth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der fünften Radio-Box",
            '#default_value' => $button_title_array[4],
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );

        $form['content' . $i]['textfield_sixth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der sechsten Radio-Box",
            '#default_value' => $button_title_array[5],
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 1),
                ),
            ),

        );
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('survey_change_question_save_changes')
    );



    return $form;

}

function survey_change_question_save_changes($form, &$form_state) {

    $title = $form_state['values']['question'];

    $question_id = arg(1);
    $quantity_id = arg(2);

    db_update('question')
    ->fields(array(
        'title' => $title,
    ))
        ->condition('question_id', $question_id, '=')
        ->execute();


    $sql = "DELETE FROM {question_possible_answers} WHERE question_id = ".$question_id;
    db_query($sql);

    $sql = "DELETE FROM {question_buttons_title} WHERE question_id = ".$question_id;
    db_query($sql);






    for($i = 1;$i<=$quantity_id;$i++) {

        $description = $form_state['values']['test'.$i];
        $var2 = $form_state['values']['radios'.$i];
        $check = 1;

        if(strcmp($var2, 1)) {
            $check = 0;
        }


        $answer_id = db_insert('question_possible_answers')
            ->fields(array(
                'description' => $description,
                'isRadioButton' => $check,
                'question_id' => $question_id
            ))
            ->execute();


        if($check) {

            $var = $form_state['values']['button_radios'.$i];

            $radio_name_1 = $form_state['values']['textfield_first_button'.$i];
            $radio_name_2 = $form_state['values']['textfield_second_button'.$i];
            $radio_name_3 = $form_state['values']['textfield_third_button'.$i];
            $radio_name_4 = $form_state['values']['textfield_fourth_button'.$i];
            $radio_name_5 = $form_state['values']['textfield_fiveth_button'.$i];
            $radio_name_6 = $form_state['values']['textfield_sixth_button'.$i];

            $radio_array = array();
            array_push($radio_array, $radio_name_1);
            array_push($radio_array, $radio_name_2);
            array_push($radio_array, $radio_name_3);
            array_push($radio_array, $radio_name_4);
            array_push($radio_array, $radio_name_5);
            array_push($radio_array, $radio_name_6);



            for($j = 0;$j<=$var-1;$j++) {


                $title = $radio_array[$j];

                db_insert('question_buttons_title')
                    ->fields(array(
                        'question_id' => $question_id,
                        'answer_id' => $answer_id,
                        'button_id' => $j,
                        'title' => $title,
                    ))
                    ->execute();


            }
        }

    }
    //drupal_goto("");





    drupal_goto("question_overview");

}


function survey_change_question_change_quantity($form, &$form_state) {
    $question_id = arg(1);
    $quantity_id = arg(2);
    $quantity = $form_state['values']['quantity'];


    if ($quantity_id == $quantity) {
        //drupal_goto("change_question/".$question_id."/".$quantity_id);
    } else if($quantity > $quantity_id) {
        $diff = $quantity-$quantity_id;
        for ($i=0;$i<$diff;$i++) {

            $answer_id = db_insert('question_possible_answers')
                ->fields(array(
                    'description' => "Neue Antwortmöglichkeit",
                    'isRadioButton' => 1,
                    'question_id' => $question_id
                ))
                ->execute();

            for ($j=0;$j<4;$j++) {
                db_insert('question_buttons_title')
                    ->fields(array(
                        'question_id' => $question_id,
                        'answer_id' => $answer_id,
                        'button_id' => $j,
                        'title' => "Neue Button-Bezeichnung",
                    ))
                    ->execute();
            }
        }
        drupal_goto("change_question/".$question_id."/".$quantity);
    } else if($quantity < $quantity_id) {
        $diff = $quantity_id - $quantity;

        for ($i=0;$i<$diff;$i++) {

            $sql = "SELECT * FROM {question_possible_answers} WHERE question_id = ".$question_id . " ORDER BY answers_id DESC LIMIT 1";
            $result = db_query($sql);


            foreach ($result as $item) {
                $sql2 = "DELETE FROM {question_buttons_title} WHERE answer_id = ".$item->answers_id;
                db_query($sql2);
            }

            $sql = "DELETE FROM {question_possible_answers} WHERE answers_id = ".$item->answers_id;
            db_query($sql);
        }
        drupal_goto("change_question/".$question_id."/".$quantity);

    } else {

    }



    //
    //drupal_goto("add_questions/".$quantity);
}