<?php

/**
 * install file for todo
 * we add the following package doc code if we want good docs
 *
 * @package     todo
 */


/**
 * All modules which print to the screen has run_level set to 0 in module
 * registry. Any other modules shold be set to a run level.
 * If you module needs to run a special levels you need to set:
 * $_INSTALL['RUN_LEVEL'] = "1,7";
 * This will make the module run at level 1 (beginning) and 7 (last level)
 */

/**
 * We set a version, so we can control if there are any upgrades etc.
 */
$_INSTALL['VERSION'] = 1.11;

/**
 * We also create a menu item to be added to the main menu
 */
$_INSTALL['MAIN_MENU_ITEM'] = array (
    'title' => 'Todo', // Title of the menu item
    'url' => '/todo/index', // url of the item
    'module_name' => 'todo', // beloning to which module
    'parent' => 0, // does it have a parent item
    'weight' => 2, // weight
    'auth' => 'user', // Who can se this menu item
); 
