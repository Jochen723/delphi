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

    $tempString = "";
    $tempString .= "<table><th>Nr.</th><th>Frage</th><th>Aktion</th>";

    $sql = "SELECT * FROM {question} ORDER BY question_id";
    $result = db_query($sql);

    $id = 1;

    foreach ($result as $item) {
        $tempString .= "<tr><td>".$id."</td><td>$item->title</td><td><a href=''>bearbeiten</a></td></tr>";
        $id++;
    }


    $tempString .= "</table>";

    return $tempString;

}