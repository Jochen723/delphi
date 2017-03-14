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

    $array = array("Radio-Buttons" => "Radio-Buttons", "Textfeld" => "Textfeld");


    $form['question'] = array(
        '#type' => 'textfield',
        '#title' => "Titel der Frage",
    );


    $form['quantity'] = array(
        '#type' => 'textfield',
        '#title' => "Anzahl der Antwortmöglichkeiten",
        '#size' => 2,
        '#maxlength' => 1,
        '#default_value' => $id,

    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Ändern',
        '#submit' => array('survey_add_question_change_quantity')
    );


    for ($i = 1; $i <= $id; $i++) {

        $form['test' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => $i . ". Antwortmöglichkeit",
            '#default_value' => "Dimension ".$i,
            '#required' => TRUE,

        );
        $form['radios' . $i] = array(
            '#type' => 'radios',
            '#default_value' => $array["Radio-Buttons"],
            '#options' => $array,
        );
    }


    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Weiter',
        '#submit' => array('survey_add_question_save_question')
    );

    return $form;
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
                'question_id' => $nid
            ))
            ->execute();


    }
    drupal_goto("");
}

