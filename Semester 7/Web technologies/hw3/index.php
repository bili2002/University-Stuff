<?php
function validateForm($data) {
    $errors = [];
    if (!isset($data['name'])) {
        $errors['name'] = "Полето за име на учебния предмет е задължително.";
    } elseif (mb_strlen($data['name']) < 2) {
        $errors['name'] = "Името на учебния предмет трябва да бъде поне 2 символа.";
    } elseif (mb_strlen($data['name']) > 150) {
        $errors['name'] = "Името на учебния предмет трябва да бъде до 150 символа.";
    }
    
    if (!isset($data['teacher'])) {
        $errors['teacher'] = "Полето за име на преподавателя е задължително.";
    } elseif (mb_strlen($data['teacher']) < 3) {
        $errors['teacher'] = "Името на преподавателя трябва да бъде поне 3 символа.";
    } elseif (mb_strlen($data['teacher']) > 200) {
        $errors['teacher'] = "Името на преподавателя трябва да бъде до 200 символа.";
    }

    if (!isset($data['description'])) {
        $errors['description'] = "Полето за описание е задължително.";
    } elseif (mb_strlen($data['description']) < 10) {
        $errors['description'] = "Описанието трябва да бъде с дължина поне 10 символа.";
    }

    if (!isset($data['group'])) {
        $errors['group'] = "Полето за група е задължително.";
    } elseif (!in_array($data['group'], ["М", "ПМ", "ОКН", "ЯКН"])) {
        $errors['group'] = "Невалидна група, изберете една от М, ПМ, ОКН и ЯКН.";
    }

    if (!isset($data['credits'])) {
        $errors['credits'] = "Полето за кредити е задължително.";
    } elseif (!is_int($data['credits'])) {
        $errors['credits'] = "Кредитите трябва да бъдат цяло число.";
    } elseif ($data['credits'] <= 0) {
        $errors['credits'] = "Кредитите трябва да бъдат положително число.";
    }
    return $errors;
}

$data = json_decode(file_get_contents('php://input'), true);
$response = ["success" => true, "errors" => []];
$response['errors'] = validateForm($data);

if (!empty($response['errors'])) {
    $response['success'] = false;
}

header('Content-Type: application/json; charset=utf-8');
echo json_encode($response, JSON_UNESCAPED_UNICODE | JSON_PRETTY_PRINT);
