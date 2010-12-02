<?php

$todo = new todo();
if (!$todo->checkUser()){
    moduleLoader::$status['403'] = 1;
    return;
} else {
    $todo->viewFormDeleteTodo();
}


