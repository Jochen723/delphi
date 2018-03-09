<?php
/**
 * Created by PhpStorm.
 * User: julien
 * Date: 09.03.18
 * Time: 15:06
 */

namespace Drupal\delphi_evaluation\Helper;


class Delphi_Evaluation_Helper
{

    /**
     * Gets the survey data from the database
     * @return string
     * @throws \Exception
     */
    public function delphi_evaluation_get_survey_data() {
        $sql = "SELECT user_pw, title, type, q.question_id, weight, answer_id, answer, comment, is_last_answer FROM {question} q
        LEFT JOIN {question_user_answers} a ON q.question_id = a.question_id;";

        $result = db_query($sql);

        $result = $result->fetchAll();
        $header = array('user_pw',
            'title',
            'item',
            'answer',
            'comment',
            'is_last_answer',
        );

        foreach ($result as $row) {
            $rows[] = array(
                $row->user_pw,
                $row->title,
                $this->delphi_evaluation_get_item($row->question_id, $row->answer_id + 1),
                $row->answer,
                $row->comment,
                $row->is_last_answer,

            );
        }
        //return a HTML Table
        return theme('table', array('header' => $header, 'rows' => $rows));
    }

    /**
     * Gets the item from the database based on question_id and and weight
     * weight is the answer_id+1 in the parameter call
     * @param $question_id
     * @param $weight
     * @return \DatabaseStatementInterface
     */
    public function delphi_evaluation_get_item($question_id, $weight) {
        $sql = "SELECT description, question_id, weight FROM {question_possible_answers} item
                WHERE item.question_id = :question_id AND item.weight = :weight";

        $result = db_query($sql, array('question_id' => $question_id, 'weight' => $weight));
        $result = $result->fetchField(0);

        return $result;
    }
}