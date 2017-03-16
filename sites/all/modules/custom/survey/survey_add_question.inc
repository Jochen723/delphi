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

function survey_change_question_form() {

    $question_id = arg(1);
    $id = arg(2);

    $title = "";
    $answers = array();
    $boxes = array();
    for($i=0;$i<$id;$i++) {
        $nr = $i+1;
        array_push($answers, "Dimension ".$nr);
        array_push($boxes, "Radio-Buttons");
    }

    $sql = "SELECT * FROM {question}";
    $result = db_query($sql);
    foreach ($result as $item) {
        $title = $item->title;
    }

    $identifier = 0;
    $sql = "SELECT * FROM {question_possible_answers} ORDER BY answers_id";
    $result = db_query($sql);
    foreach ($result as $item2) {
        $answers[$identifier] = $item2->description;
        if(!$item2->isRadioButton) {
            $boxes[$identifier] = "Textfeld";
        }
        $identifier++;
    }

    $array = array("Radio-Buttons" => "Radio-Buttons", "Textfeld" => "Textfeld");

    $form['question'] = array(
        '#type' => 'textfield',
        '#title' => "Titel der Frage",
        '#default_value' => $title,
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
        '#submit' => array('survey_change_question_change_quantity')
    );

    for ($i = 1; $i <= $id; $i++) {

        $form['test' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => $i . ". Antwortmöglichkeit",
            '#default_value' => $answers[$i-1],
            '#required' => TRUE,

        );

        $form['radios' . $i] = array(
            '#type' => 'radios',
            '#default_value' => $boxes[$i-1],
            '#options' => $array,
        );
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('survey_change_question_save_changes')
    );

    return $form;
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

        drupal_set_message($var1);
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

