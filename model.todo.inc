<?php

/**
 * Ok now some code:
 *
 * All this code is placed in the file model.todo.inc
 * We don't use the view.todo.inc, but you could place
 * all html in a class or in functions in this file,
 * if you would like to separet html from php.
 *
 */

/**
 * all our code is made up of one class which does almost 
 * anything. 
 */

include_once "mySqlForm.php";

class todo extends db {

    // we know that we will use the id of the todo table when doing insert,
    // update or delete of todo items
    public $id;

    // we also make an array for holding errors on form submission.
    public $errors = array();

    /**
     * we make a constuctor where we figure out what the id of a todo item is
     * our url will look like this:
     *
     * /todo/operation/id
     *
     * where todo is the name of our module.
     * operation will be which operation we will use
     *
     * and id will be the item to do operations on: e.g.
     *
     * /todo/edit/234
     *
     */
    function __construct(){
        // we use the uri class for figuring out what fragment is the id.
        // fragments starts at 0 and is the first part of the uri. Then
        // we know that out fragment of our id is 2
        $uri = URI::getInstance();
        $this->id = $uri->fragment(2);

        // we also add some javascript.
        //template::setJs("http://ajax.googleapis.com/ajax/libs/jquery/1.3/jquery.min.js");
        //template::setJs("http://ajax.googleapis.com/ajax/libs/jqueryui/1.7.2/jquery-ui.js");
    }


    /**
     *
     * method for creating a html form from a database table
     *
     * this function is used to interact with the todo table, which
     * we created previously. We make this method staticly so we can
     * use it without creating any object first.
     *
     * @param method to use with the form
     * @param id  used when we update or delete a todo
     * @param $values, only used when we need to set some deafult values
     *        only needed when updating a todo.
     */
    static function createTodoForm($method, $id = null, $values = array()){
        // we use ourr mySqlForm object for creating forms directly using
        // the database definition of the todo table.
        
        // we create the mysql form from the todo table, and we are using 
        // the fields id, todo, list

        // also values are set or set with an empty array
        $tf = new mySqlForm('todo', array('id', 'todo'), $values);

        // we also add some labels for the html form
        $tf->setLabels(array(
            'id' => 'Id is a hidden field', // id is a hidden field.
            'todo' => lang::translate('Enter description of todo'),
           )
        );

        // we add some captions for the fields.
        if ($method == 'update'){
            $tf->setMethod($method, $id);
            $caption = lang::translate('Edit Todo');
        } else if ($method== 'insert') {
            $tf->setMethod($method);
            $caption = lang::translate('Add Todo');
        } else {
            $tf->setMethod($method);
            $caption = lang::translate('Delete Todo');
        }

        // finally we create the form from the given params
        $tf->createForm('', 'post',  'test', '',
                           true,  $caption);
    }

    // ok we now have a form for doing all operations on the todo list
    // we then make a form for validating user input
    public function validate(){
        // only need to validate if we have a submission

        if (empty($_POST['todo']) OR strlen($_POST['todo']) == 0 ){
            $this->errors[] = lang::translate('No todo');
        }
        //if (strlen($_POST['todo']) == 0 ){
        //    $this->errors[] = lang::translate('No todo');
        //}
    }

    // we will also make a sanitize function for sanitizing user input when
    // user finally has a accepted submission
    public function sanitize(){

        // cos_htmlentities same as htmlentities, but works on arrays
        $_POST = cos_htmlentities($_POST);

    }

    // we now how some methods for validating and submitting todos to our table,
    // now we can write a function for adding a todo.
    public function addTodo(){

        // we add the users user_id to the $_POST array
        $_POST['user_id'] = session::getUserId();

        // prepareToPost examine the $_POST values and cleans out fields like
        // captcha and submit, and then returns a postable array
        $values = self::prepareToPost();

        $res = $this->insert('todo', $values);

        // we return the result (boolean)
        return $res;
    }

    // we now make a function for updating a todo
    public function updateTodo(){

        // same as adding, cleaning out fields which are not used.
        $values = self::prepareToPost();

        // we update the item. Notice exactly the params same as adding, but
        // also a id to specify which todo we are updating
        $res = $this->update('todo', $values, $this->id);
        
        // we return the result (boolean)
        return $res;

    }

    // we now make a function for delting a todo
    public function deleteTodo(){



        // in the delete function we again specify which table we want to
        // delete from. We then specify that we want to delete where
        // id = $this->id (uri's 2 fragment e.g. /todo/delete/243
        $res = $this->delete('todo', 'id', $this->id);

        // we return the result (boolean)
        return $res;
    }

    // we also make a function for checking if user owns the todo he is
    // deleting
    public function checkUser (){

        // we check if the user owns the todo
        $user_id = session::getUserId();
        $search = array('id' => $this->id, 'user_id' => $user_id );

        // this will make a search were id = $this->id AND user_id = $user_id
        $row = $this->selectOne('todo', null, $search);
        if (empty($row)){
            return false;
        } else {
            return true;
        }
        
    }

    // now we make a function for getting all the users todos
    public function getTodos(){
        $user_id = session::getUserId();
        $search = array('user_id' => $user_id);

        // we specify the table, we specify the fields to get (null = all fields
        // and we specify what to search for (user_id)
        //$this->selectAll($table, $fields, $search, $from, $limit, $order_by)
        $rows = $this->selectAll('todo', null, $search, null, null, 'order', true);
        return $rows;
    }
    
    // we now make a function for printing all the todos for a user.
    // if you want to separate html from php, then you can place this function
    // in view.todo.inc instead. We does it simple here.

    public function displayTodos(){
        $todos = $this->getTodos();
        $str = "<div id=\"list\"><div id=\"response\"></div>";
        $str.= "<ul id=\"todo\">\n";
        foreach ($todos as $key => $value){
            $str.= "<li id = \"arrayorder_$value[id]\">$value[todo]";
            $str.= create_link("/todo/delete/$value[id]", lang::translate('Delete')) . "";
            $str.= "</li><div class=\"clear\"></div>";            
        }
        $str.= "</ul></div>\n";
        print $str;
    }

    // we make a function for adding a todo
    public function viewFormAddTodo(){
        template::setInlineJs(_COS_PATH . "/modules/todo/assets/drag.js");
        template::setInlineCss(_COS_PATH . "/modules/todo/assets/style.css");
        template::setTitle(lang::translate('View Todos'));
        if (isset($_POST['submit'])){
            // we validate and sanitize
            $this->validate();
            $this->sanitize();

            // if no errors we add the todo
            if (empty($this->errors)){
                $res = $this->addTodo();

                // we set a message and redirect
                // message will bee shown after redirect
                session::setActionMessage(lang::translate('Todo Added'));
                    $redirect = "Location: /todo/index";
                    header($redirect);

            // if any errors we print them and show the form again
            } else {
                view_form_errors($this->errors);
                self::createTodoForm('insert');
                //create_event_form('insert', null);
            }
        } else {
            // if no form submission we just print the form
            self::createTodoForm('insert');
        }
    }

    public function viewFormDeleteTodo(){
        template::setTitle(lang::translate('Delete Todo'));

        if (isset($_POST['submit'])){
            $this->deleteTodo($this->id);

            // we set a message and redirect
            // message will be shown after redirect
            session::setActionMessage(lang::translate('Todo Deleted'));
            $redirect = "Location: /todo/index";
            header($redirect);

        }
        todo::createTodoForm('delete', $this->id);
    }
}