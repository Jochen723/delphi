<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 09.05.17
 * Time: 08:03
 */

function survey_testtest($id, $isRadioButton, $links)
{

    $question_id = arg(1);
    $user_pw = arg(2);


    $testArrayOverall = array();
    $testArrayOverall2 = array();

    $sql = "SELECT * FROM {question_user_answers} WHERE question_id = " . $question_id . " AND answer_id = " . $id;
    $result = db_query($sql);

    foreach ($result as $item4) {
        if (strcmp($user_pw, $item4->user_pw)) {
            $user = $item4->user_pw;
            $check = searchForId($user, $testArrayOverall2);

            if ($check > -1) {
                unset($testArrayOverall2[$check]);
                unset($testArrayOverall[$check]);
            }
            array_push($testArrayOverall2, $item4);
            array_push($testArrayOverall, $item4->answer);
        }
    }
}

function survey_get_average($testArrayOverall, $isRadioButton, $max)
{

    $avg_array = array();

    $avg = (!empty($testArrayOverall) ? array_sum($testArrayOverall) / count($testArrayOverall) : 0);
    $avg = round($avg, 1);


    if ($isRadioButton) {
        $avg_array["absolut"] = $avg + 1;

        $avg_array["procent"] = round($avg / $max * 100, 1);
    } else {
        $avg_array["absolut"] = $avg;


        $avg_array["procent"] = round($avg / $max * 100, 1);
    }


    return $avg_array;
}

function survey_get_median($testArrayOverall, $isRadioButton, $max)
{

    $temp = array();
    $n = sizeof($testArrayOverall);

    foreach ($testArrayOverall as $item) {
        array_push($temp, $item);
    }

    if ($isRadioButton) {
        if (sizeof($temp) % 2 == 0) {
            $median = 0.5 * ($temp[$n / 2 - 1] + $temp[($n / 2)]);
        } else {
            $median = $temp[($n) / 2];
        }

        $median_array["absolut"] = $median + 1;
        $median_array["procent"] = round($median / $max * 100, 1);
    } else {
        if (sizeof($temp) % 2 == 0) {
            $median = 0.5 * ($temp[$n / 2 - 1] + $temp[($n / 2)]);
        } else {
            $median = $temp[($n) / 2];
        }

        $median_array["absolut"] = $median;
        $median_array["procent"] = round($median / $max * 100, 1);
    }


    return $median_array;
}

function survey_get_first_quantil($testArrayOverall, $isRadioButton, $max)
{
    $temp = array();
    $n = sizeof($testArrayOverall);

    foreach ($testArrayOverall as $item) {
        array_push($temp, $item);
    }

    $np = $n * 0.25;

    if($isRadioButton) {
        if ($np == round($np)) {

            $first_quantil = 0.5 * ($temp[$np] + $temp[$np + 1]);

        } else {
            $np = ceil($np);
            $np = intval($np);
            $first_quantil = $temp[$np - 1];
        }

        $first_quantil_array["absolut"] = $first_quantil + 1;
        $first_quantil_array["procent"] = round($first_quantil / $max * 100, 1);
    } else {
        if ($np == round($np)) {

            $first_quantil = 0.5 * ($temp[$np] + $temp[$np + 1]);

        } else {
            $np = ceil($np);
            $np = intval($np);
            $first_quantil = $temp[$np - 1];
        }
        $first_quantil_array["absolut"] = $first_quantil;
        $first_quantil_array["procent"] = round($first_quantil / $max * 100, 1);
    }

    return $first_quantil_array;
}

function survey_get_third_quantil($testArrayOverall, $isRadioButton, $max)
{

    $temp = array();
    $n = sizeof($testArrayOverall);

    foreach ($testArrayOverall as $item) {
        array_push($temp, $item);
    }

    $np = $n * 0.75;

    if($isRadioButton) {
        if ($np == round($np)) {

            $third_quantil = 0.5 * ($temp[$np] + $temp[$np + 1]);

        } else {
            $np = ceil($np);
            $np = intval($np);
            $third_quantil = $temp[$np - 1];
        }

        $third_quantil_array["absolut"] = $third_quantil + 1;
        $third_quantil_array["procent"] = round($third_quantil / $max * 100, 1);
    } else {
        if ($np == round($np)) {

            $third_quantil = 0.5 * ($temp[$np] + $temp[$np + 1]);

        } else {
            $np = ceil($np);
            $np = intval($np);
            $third_quantil = $temp[$np - 1];
        }
        $third_quantil_array["absolut"] = $third_quantil;
        $third_quantil_array["procent"] = round($third_quantil / $max * 100, 1);
    }

    return $third_quantil_array;
}
