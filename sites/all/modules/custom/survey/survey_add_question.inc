<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 14.03.17
 * Time: 11:29
 */

function survey_add_question_form()
{

    $id = arg(1);

    $form['#id'] = 'question-form';


    //Textfeld, in das der Titel der Frage eingesetzt wird
    $form['question'] = array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => "Titel der Frage",
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
        '#submit' => array('survey_add_question_change_quantity')
    );


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
            '#default_value' => "",
            '#required' => TRUE,

        );

        $answer_choose_array = array("Radio-Buttons" => "Radio-Buttons", "Textfeld" => "Textfeld");

        //Für jede Antwortmöglichkeit muss bestimmt werden, ob die Antwort per Radio-Buttons oder per Textfeld
        //gegeben werden soll
        $form['content' . $i]['radios' . $i] = array(
            '#type' => 'radios',
            '#title' => 'Art der Antwortmöglichkeit:',
            '#default_value' => $answer_choose_array["Radio-Buttons"],
            '#options' => $answer_choose_array,
        );


        $button_array = array(4 => "4", 5 => "5", 6 => "6");
        $var = $i;

        //Sollte es sich bei der Antwortmöglichkeit um Radio-Buttons handeln, muss festgelegt werden, viele viele
        //Buttons für die Antwort vorgesehen sind.
        $form['content' . $i]['button_radios' . $i] = array(
            '#type' => 'radios',
            '#title' => "<b>Anzahl der vorhandenen Radio-Boxen</b>",
            '#default_value' => $button_array[4],
            '#options' => $button_array,
            '#states' => array(
                'visible' => array(
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );


        $form['content' . $i]['textfield_first_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der ersten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );

        $form['content' . $i]['textfield_second_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der zweiten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );

        $form['content' . $i]['textfield_third_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der dritten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );

        $form['content' . $i]['textfield_fourth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der vierten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );

        $form['content' . $i]['textfield_fiveth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der fünften Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );

        $form['content' . $i]['textfield_sixth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der sechsten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios'.$var.'"]' => array(
                        array('value' => t('6')),
                    ),
                    ':input[name="radios'.$var.'"]' => array('value' => 'Radio-Buttons'),
                ),
            ),

        );
    }



    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('survey_add_question_save_question')
    );

    return $form;
}

function survey_autocheckboxes_callback($form, $form_state) {
    return $form['checkboxes_fieldset'];
}

function survey_add_question_change_quantity($form, &$form_state) {
    $quantity = $form_state['values']['quantity'];
    drupal_goto("add_questions/".$quantity);
}


function survey_add_question_save_question($form, &$form_state) {


    $title = $form_state['values']['question'];

    $id = arg(1);


    $nid = db_insert('question')
        ->fields(array(
            'title' => $title,
        ))
        ->execute();



        for($i = 1;$i<=$id;$i++) {

            $description = $form_state['values']['test'.$i];
            $var2 = $form_state['values']['radios'.$i];
            $check = 1;

            if(strcmp($var2, "Radio-Buttons")) {
                $check = 0;
            }


            $answer_id = db_insert('question_possible_answers')
                ->fields(array(
                    'description' => $description,
                    'isRadioButton' => $check,
                    'question_id' => $nid
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
                            'question_id' => $nid,
                            'answer_id' => $answer_id,
                            'button_id' => $j,
                            'title' => $title,
                        ))
                        ->execute();

                }
            } else {
                drupal_set_message("HEHEHE");

            }

        }
        drupal_goto("");

}



function survey_change_question_change_quantity($form, &$form_state) {

    $question_id = arg(1);

    $quantity = $form_state['values']['quantity'];

    drupal_goto("change_question/".$question_id."/".$quantity);

}

function survey_change_question_save_changes($form, &$form_state) {

    $title = $form_state['values']['question'];
    $id = arg(2);
    $question_id = arg(1);


    $sql = "UPDATE {question} SET title = '".$title."' WHERE question_id = ".$question_id;
    db_query($sql);

    $sql = "DELETE FROM {question_possible_answers} WHERE question_id = ".$question_id;
    db_query($sql);


    for($i = 1;$i<=$id;$i++) {

        $var1 = $form_state['values']['test'.$i];
        $var2 = $form_state['values']['radios'.$i];

        $check = 1;

        if(strcmp($var2, "Radio-Buttons")) {
            $check = 0;
        }



        db_insert('question_possible_answers')
            ->fields(array(
                'description' => $var1,
                'isRadioButton' => $check,
                'question_id' => $question_id
            ))
            ->execute();

    }
    drupal_goto("question_overview");

}

