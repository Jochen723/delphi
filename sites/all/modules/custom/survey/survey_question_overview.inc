<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 14.03.17
 * Time: 13:36
 */

function survey_question_overview_form() {

    $headline = survey_create_question_overview();

    $form['test']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $headline,
    );


    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Weiter',
        '#submit' => array('survey_get_evaluation')
    );

    return $form;
}

function survey_create_question_overview() {

    global $base_url;


    $tempString = "";
    $tempString .= "<table><th>Nr.</th><th>Frage</th><th>Aktion</th>";

    $sql = "SELECT * FROM {question} ORDER BY question_id";
    $result = db_query($sql);

    $id = 1;



    foreach ($result as $item) {

        $sql = "SELECT * FROM {question_possible_answers} WHERE question_id = ".$item->question_id;
        drupal_set_message($sql);
        $result = db_query($sql);

        $quan = $result->rowCount();

        $link = $base_url . "/?q=delete_question/".$item->question_id;
        $link2 = $base_url . "/?q=change_question/".$item->question_id."/".$quan;


        $tempString .= "<tr><td>".$id."</td><td>$item->title</td><td><a href='".$link2."'>bearbeiten</a><br><a href='".$link."'>löschen</a></td></tr>";
        $id++;
    }


    $tempString .= "</table>";

    return $tempString;

}

function survey_delete_question() {

    $question_id = arg(1);

    $sql = "DELETE FROM {question} WHERE question_id = ".$question_id;
    db_query($sql);

    $sql = "DELETE FROM {question_possible_answers} WHERE question_id = ".$question_id;
    db_query($sql);

    $sql = "DELETE FROM {question_user_answers} WHERE question_id = ".$question_id;
    db_query($sql);


    drupal_set_message("Frage wurde gelöscht");
    drupal_goto("question_overview");
}