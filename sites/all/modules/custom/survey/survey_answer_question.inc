<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 10.05.17
 * Time: 18:18
 */

/*
 * Diese Funktion baut ein Formular, in das der Nutzer Antworten zu einer entsprechenden Frage eingeben kann.
 * Der Passcode des Nutzers und die Fragen-ID der Frage werden über die URL übergeben.
 */
function survey_form()
{

    //Die ID der Frage wird aus der URL ausgelesen
    $question_id = arg(1);

    //Der Passcode des Nutzers wird aus der URL ausgelesen
    $user_pass = arg(2);

    //Das Form-Element, welches am Ende zurückgegeben wird, wird erstellt
    $form = "";

    //Die bisherigen antworten des Nutzers auf die Frage werden aus der Datenbank geladen
    $user_answers = survey_get_answers($question_id, $user_pass);

    //Es werden alle verfügbaren Fragen aus der Datenbank geladen und gezählt
    $questions = survey_get_all_questions();
    $number_of_questions = sizeof($questions);

    //Sollte es ein valider Passcode des Nutzers sein und die Frage ebenfalls valide sein
    if (survey_check_valid_user_pw($user_pass) && $number_of_questions >= $question_id) {

        //Der Titel der Frage wird geladen, die Überschrift wird erstellt
        $text = $questions[$question_id - 1]->title;
        $headline = "<h1>Frage " . $question_id . "(a)</h1><h2>" . $text . "</h2><br><br>";

        //Die Antworten der Frage werden aus der Datenbank geladen
        $answers = survey_get_answers_from_question($questions[$question_id - 1]->question_id);

        //Die Bezeichungen der Buttons bzw. der Textfelder werden aus der Datenbank geladen
        $sql = "SELECT * FROM {question_buttons_title} WHERE question_id = " . $questions[$question_id - 1]->question_id . " ORDER BY answer_id";
        $result = db_query($sql);

        //Die Bezeichungen der Buttons werden geladen
        $button_title_array = survey_get_button_titles($result);


        $form['question']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $headline,
        );

        //Es wird über jede Antwortmöglichkeit iteriert
        for ($i = 0; $i < sizeof($answers); $i++) {

            //Sollte es bereits eine Antwort vom Nutzer in der Datenbank geben, wird diese als Standardwert
            //eingetragen
            if (!isset($user_answers[$i]->answer)) {
                $default_value = "";
            } else {
                $default_value = $user_answers[$i]->answer;
            }

            //Sollte es sich bei der Antwortmöglichkeit um Radio-Buttons handeln
            if ($answers[$i]->isRadioButton) {
                $form['dim' . $i] = array(
                    '#type' => 'radios',
                    '#prefix' => '<div id ="visualization' . $i . '">',
                    '#title' => $answers[$i]->description,
                    '#required' => true,
                    '#options' => $button_title_array[$i],
                );

                //Sollte es sich bei der Antwortmöglichkeit um ein Textfeld handeln
            } else {
                $form['quantity' . $i] = array(
                    '#type' => 'textfield',
                    '#title' => $answers[$i]->description,
                    '#size' => 4,
                    '#maxlength' => 4,
                    '#default_value' => $default_value,
                );
            }
        }

        //Sollte es sich nicht um die erste Frage handeln
        if ($question_id != 1) {
            $form['back'] = array(
                '#type' => 'submit',
                '#value' => 'Zurück',
                '#submit' => array('survey_get_back_to_start')
            );
        }

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('survey_get_evaluation')
        );
    }

    return $form;
}

/*
 * Diese Funktion führt den Nutzer zurück ins Hauptmenü
 */
function survey_finish_survey()
{
    return "Danke für Ihre Teilnahme, die Umfrage ist nun beendet!";
}

/*
 * Diese Funktion bringt den Nutzer zurück zur letzten Frage
 */
function survey_get_back_to_start()
{

    $question_id = arg(1);
    $user_pw = arg(2);

    if ($question_id == 1) {
        drupal_goto("");
    } else {
        $question_id--;
        drupal_goto("survey_question_evaluation/" . $question_id . "/" . $user_pw);
    }
}

/*
 * Diese Funktion speichert die Antworten des Nutzers in die Datenbank und leitet ihn weiter zur Anzeige
 * der Boxplots
 */
function survey_get_evaluation($form, &$form_state)
{
    //Die ID der Frage wird aus der URL ausgelesen
    $question = arg(1);

    //Der Passcode des Nutzers wird aus der URL geladen
    $user_passcode = arg(2);

    //Es werden alle Fragen aus der Datenbank geladen
    $questions = survey_get_all_questions();
    $answers = survey_get_answers_from_question($questions[$question - 1]->question_id);

    for ($i = 0; $i < sizeof($answers); $i++) {


        if ($answers[$i]->isRadioButton) {
            $dim1 = $form_state['values']['dim' . $i];
        } else {

            $dim1 = $form_state['values']['quantity' . $i];
        }

        $sql = "SELECT * FROM {question_user_answers} WHERE answer_id = " . $i . " AND question_id = " . $question . " AND user_pw = '" . $user_passcode . "'";
        $result = db_query($sql);

        if ($result->rowCount()) {
            $sql = "UPDATE {question_user_answers} SET answer = " . $dim1 . " WHERE answer_id = " . $i . " AND question_id = " . $question . " AND user_pw = '" . $user_passcode . "' ";
            db_query($sql);
        } else {
            db_insert('question_user_answers')
                ->fields(array(
                    'question_id' => $question,
                    'answer_id' => $i,
                    'answer' => $dim1,
                    'user_pw' => $user_passcode,
                    'comment' => "",
                ))
                ->execute();
        }

    }
    drupal_goto("survey_question_evaluation/" . $question . "/" . $user_passcode);

}