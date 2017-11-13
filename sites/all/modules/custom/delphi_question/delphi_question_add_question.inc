<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 24.07.17
 * Time: 11:30
 */

function delphi_question_add_question_form()
{

    //Die Anzahl der Antwortmöglichkeiten wird geladen
    $answer_quantity_id = arg(1);

    //Der Form wird eine ID übergeben
    $form['#id'] = 'question-form';

    //Daten für das Array, welches die Radio-Buttons benutzen
    $answer_choose_array = array("Radio-Buttons" => "Radio-Buttons", "Textfeld" => "Textfeld");

    $button_array = array(4 => "4", 5 => "5", 6 => "6");


    //Textfeld, in das der Titel der Frage eingesetzt wird
    $form['question'] = array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => "Titel der Frage",
        '#attributes' => array(
            'id' => 'question-field',
        ),
        '#maxlength' => 1000, // DB field type is varchar(1023)
    );

    //Textfeld, in das die Anzahl der Antwortmöglichkeiten eingegeben werden soll
    $form['quantity'] = array(
        '#type' => 'textfield',
        '#title' => "Anzahl der Antwortmöglichkeiten",
        '#size' => 2,
        '#maxlength' => 2,
        '#default_value' => $answer_quantity_id,
        '#element_validate' => array('element_validate_integer_positive'),

    );

    //Button, der die Änderung der Anzahl von Antwortmöglichkeiten übernimmt
    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Ändern',
        '#limit_validation_errors' => array(array('quantity')),
        '#submit' => array('delphi_question_add_question_change_quantity')
    );


    //Diese Schleife wird so oft durchgeführt, wie viele Antwortmöglichkeiten es geben soll
    for ($i = 1; $i <= $answer_quantity_id; $i++) {

        //Für jede Antwortmöglichkeit wird ein Fieldset zur Übersichtlichkeit angelegt
        $form['content' . $i] = array(
            '#title' => $i . ". Antwortmöglichkeit",
            '#type' => 'fieldset',
            '#description' => ""
        );

        //Für jede Antwortmöglichkeit muss eine Bezeichnung festgelegt werden.
        $form['content' . $i]['test' . $i] = array(
            '#type' => 'textfield',
            '#title' => "<b>Bezeichnung der Antwortmöglichkeit:</b>",
            '#default_value' => "",
            '#required' => TRUE,
            '#maxlength' => 254,
            '#attributes' => array(
              'class' => array('item-title'),
            ),

        );

        //Für jede Antwortmöglichkeit muss bestimmt werden, ob die Antwort per Radio-Buttons oder per Textfeld
        //gegeben werden soll
        $form['content' . $i]['radios' . $i] = array(
            '#type' => 'radios',
            '#title' => 'Art der Antwortmöglichkeit:',
            '#default_value' => $answer_choose_array["Radio-Buttons"],
            '#options' => $answer_choose_array,
        );

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
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );

        //Textfeld für den ersten Button
        $form['content' . $i]['textfield_first_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der ersten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios' . $var . '"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );

        //Textfeld für den zweiten Button
        $form['content' . $i]['textfield_second_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der zweiten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios' . $var . '"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );

        //Textfeld für den dritten Button
        $form['content' . $i]['textfield_third_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der dritten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios' . $var . '"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );

        //Textfeld für den vierten Button
        $form['content' . $i]['textfield_fourth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der vierten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios' . $var . '"]' => array(
                        array('value' => t('4')),
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );

        //Textfeld für den fünften Button
        $form['content' . $i]['textfield_fiveth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der fünften Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios' . $var . '"]' => array(
                        array('value' => t('5')),
                        array('value' => t('6')),
                    ),
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );

        //Textfeld für den sechsten Button
        $form['content' . $i]['textfield_sixth_button' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => "Bezeichnung der sechsten Radio-Box",
            '#default_value' => "Dimension " . $var,
            '#required' => TRUE,
            '#states' => array(
                'visible' => array(
                    ':input[name="button_radios' . $var . '"]' => array(
                        array('value' => t('6')),
                    ),
                    ':input[name="radios' . $var . '"]' => array('value' => 'Radio-Buttons'),
                ),
            ),
        );
    }

    //Submit-Button
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('delphi_question_add_question_save_question')
    );

    return $form;
}

//Diese Funktion ändert die Anzahl von Antwortmöglichkeiten in der Adresszeile und lädt das Formular zum Anlegen einer
//Frage neu.
function delphi_question_add_question_change_quantity($form, &$form_state)
{
    $quantity = $form_state['values']['quantity'];
    drupal_goto("add_questions/" . $quantity);
}

//Diese Funktion speichert die eingebenen Daten für die Frage in die DB ab.
function delphi_question_add_question_save_question($form, &$form_state)
{
    $question_title = $form_state['values']['question'];
    $number_of_answers = arg(1);

    //Der Titel der Frage wird in die DB gespeichert, um eine Fragen-ID zu erhalten
    $nid = db_insert('question')
        ->fields(array(
            'title' => $question_title,
        ))
        ->execute();

    //Es wird über alle Antworten iteriert
    for ($i = 1; $i <= $number_of_answers; $i++) {

        $description = $form_state['values']['test' . $i];

        //Der Typ der Antwort ermittelt. Entweder Radio-Button oder Textfeld
        $type = $form_state['values']['radios' . $i];
        $type_check = 1;

        if (strcmp($type, "Radio-Buttons")) {
            $type_check = 0;
        }

        //Die Antwortmöglichkeit wird in die DB gespeichert
        $answer_id = db_insert('question_possible_answers')
            ->fields(array(
                'description' => $description,
                'isRadioButton' => $type_check,
                'question_id' => $nid
            ))
            ->execute();

        //Sollte es sich bei der Antwortmöglichkeit um Radio-Buttons handeln
        if ($type_check) {

            //Es wird ausgelesen, ob 4,5 oder 6 Radio-Buttons zur Antwort gehören
            $number_of_radio_buttons = $form_state['values']['button_radios' . $i];

            $radio_name_1 = $form_state['values']['textfield_first_button' . $i];
            $radio_name_2 = $form_state['values']['textfield_second_button' . $i];
            $radio_name_3 = $form_state['values']['textfield_third_button' . $i];
            $radio_name_4 = $form_state['values']['textfield_fourth_button' . $i];
            $radio_name_5 = $form_state['values']['textfield_fiveth_button' . $i];
            $radio_name_6 = $form_state['values']['textfield_sixth_button' . $i];

            $radio_array = array();
            array_push($radio_array, $radio_name_1);
            array_push($radio_array, $radio_name_2);
            array_push($radio_array, $radio_name_3);
            array_push($radio_array, $radio_name_4);
            array_push($radio_array, $radio_name_5);
            array_push($radio_array, $radio_name_6);

            //Es wird über alle nötigen Radio-Buttons iteriert
            for ($j = 0; $j <= $number_of_radio_buttons - 1; $j++) {

                $question_title = $radio_array[$j];

                //Der Titel jedes Radio-Buttons wird abgespeichert
                db_insert('question_buttons_title')
                    ->fields(array(
                        'question_id' => $nid,
                        'answer_id' => $answer_id,
                        'button_id' => $j,
                        'title' => $question_title,
                    ))
                    ->execute();
            }
        }
    }
    drupal_goto("");
}