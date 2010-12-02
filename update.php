<?php

$array	= $_POST['arrayorder'];
if ($_POST['update'] == "update"){
    $db = new todo();
    $count = 1;
    foreach ($array as $key => $val) {
        $values = array('order' => $key);
        $db->update('todo', $values, $val);
        $count ++;
    }
    echo lang::translate('Order saved');
}

die;