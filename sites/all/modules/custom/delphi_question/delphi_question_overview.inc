<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 24.07.17
 * Time: 11:39
 */

/*
 * Diese Funktion baut eine Seite, die in einer Tabelle alle Fragen enthält
 */
function delphi_question_question_overview_form()
{
    //Die Tabelle wird geladen
    $table_data = delphi_question_create_question_overview();

    $form['test']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $table_data,
    );

    return $form;
}

/*
 * Diese Funktion lädt alle Fragen aus der Datenbank und zeigt sie in einer HTML-Tabelle
 */
function delphi_question_create_question_overview()
{

    global $base_url;

    //die Variable, die am Ende zurückgegeben wird
    $temp_string = "";

    //Kopf der Tabelle wird erstellt
    $temp_string .= "
        <table>
            <th>Nr.</th>
            <th>Frage</th>
            <th>Aktion</th>";

    //Fragen werden geladen
    $question_result = db_query("SELECT * FROM {question} ORDER BY question_id");

    $id = 1;

    foreach ($question_result as $question) {

        //Antworten der Fragen werden geladen
        $answer_result = db_query("SELECT * FROM {question_possible_answers} WHERE question_id = :question_id", array(
            ':question_id' => $question->question_id
        ));

        //Die Antworten werden gezählt
        $quan = $answer_result->rowCount();

        //Links zum Ändern und Löschen der Frage werden erstellt
        $link = $base_url . "/?q=delete_question/" . $question->question_id;
        $link2 = $base_url . "/?q=change_question/" . $question->question_id . "/" . $quan;

        //Für die Frage wird eine neue Zeile erstellt
        $temp_string .= "
            <tr>
                <td>" . $id . "</td>
                <td>$question->title</td>
                <td><a href='" . $link2 . "'>bearbeiten</a><br><a href='" . $link . "'>löschen</a></td>
            </tr>";
        $id++;
    }

    //Tabelle wird geschlossen
    $temp_string .= "</table>";

    return $temp_string;

}

/*
 * Diese Funktion löscht eine Frage aus der Datenbank
 */
function delphi_question_delete_question()
{
    //ID der Frage wird ermittelt
    $question_id = arg(1);

    //Frage wird gelöscht
    db_query("DELETE FROM {question} WHERE question_id = :question_id", array(
        ':question_id' => $question_id
    ));

    //Antwortmöglichkeiten der Frage werden gelöscht
    db_query("DELETE FROM {question_possible_answers} WHERE question_id = :question_id", array(
        ':question_id' => $question_id
    ));

    //Antworten werden aus der Beantwortungstabelle entfernt
    db_query("DELETE FROM {question_user_answers} WHERE question_id = :question_id", array(
        ':question_id' => $question_id
    ));

    drupal_set_message("Frage wurde gelöscht");
    drupal_goto("question_overview");
}
