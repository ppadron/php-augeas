/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Pedro Padron <ppadron@php.net>                               |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "augeas.h"
#include "php_augeas.h"

static int le_augeas;

/* {{{ augeas_functions[]
 */
function_entry augeas_functions[] = {
    PHP_FE(augeas_init,   NULL)
    PHP_FE(augeas_get,    NULL)
    PHP_FE(augeas_match,  NULL)
    PHP_FE(augeas_set,    NULL)
    PHP_FE(augeas_save,   NULL)
    PHP_FE(augeas_rm,     NULL)
    PHP_FE(augeas_insert, NULL)
    PHP_FE(augeas_close,  NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ augeas_module_entry
 */
zend_module_entry augeas_module_entry = {
	STANDARD_MODULE_HEADER,
	"augeas",
	augeas_functions,
	PHP_MINIT(augeas),
	PHP_MSHUTDOWN(augeas),
	NULL,
	NULL,
	PHP_MINFO(augeas),
	"0.1",
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_AUGEAS
ZEND_GET_MODULE(augeas)
#endif

/* {{{ _php_augeas_dtor */
static void _php_augeas_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    php_augeas *aug = (php_augeas *) rsrc->ptr;

    if (aug) {
        efree(aug);
    }

}
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(augeas)
{
    le_augeas = zend_register_list_destructors_ex(_php_augeas_dtor, NULL, PHP_AUGEAS_RESOURCE_NAME, module_number); 

    REGISTER_LONG_CONSTANT("AUGEAS_NONE", AUG_NONE, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AUGEAS_SAVE_BACKUP", AUG_SAVE_BACKUP, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AUGEAS_SAVE_NEWFILE", AUG_SAVE_NEWFILE, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AUGEAS_TYPE_CHECK", AUG_TYPE_CHECK, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AUGEAS_NO_STDINC", AUG_NO_STDINC, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AUGEAS_INSERT_BEFORE", 0, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AUGEAS_INSERT_AFTER", 1, CONST_CS|CONST_PERSISTENT);
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(augeas)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(augeas)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "augeas support", "enabled");
	php_info_print_table_end();

}
/* }}} */


/* {{{ proto string augeas_init([string $root, [string $loadpath, [int $flags]]);
       Initilizes an augeas resource  */
PHP_FUNCTION(augeas_init)
{
  char *root = "/";
  char *loadpath = "";
  int root_len, loadpath_len;
  long flags = AUG_NONE;
  php_augeas *aug;


  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!s!l", &root, &root_len, &loadpath, &loadpath_len, &flags) == FAILURE) {
    RETURN_FALSE;
  }

  aug = emalloc(sizeof(php_augeas));
  aug->augeas = aug_init(root, loadpath, flags);

  ZEND_REGISTER_RESOURCE(return_value, aug, le_augeas);  

}


/* {{{  proto string augeas_get(resource $augeas, string $path);
        Returns the node value. Will throw E_WARNING if path is invalid. */
PHP_FUNCTION(augeas_get)
{
    php_augeas *aug;
    char *path, *value;
    char **matches;
    int path_len, retval;
    zval *zaug;
   
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zaug, &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (path_len < 1) {
        RETURN_FALSE;
    }   

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    retval = aug_get(aug->augeas, path, &value);

    switch (retval) {

        /* No match */
        case 0:
            RETURN_NULL();
            break;

        /* Exactly one match */
        case 1:
            /* If the specified path is a tree, NULL is returned */
            if (value == NULL) {
                RETURN_NULL();
            }
            RETURN_STRING(value, 1);
            break;

        default:
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "specified path is invalid");
            RETURN_NULL();
            break;
    }
    
}
/* }}} */


/* {{{  proto array augeas_match(resource $augeas, string $path);
        Returns an array with all the matches */
PHP_FUNCTION(augeas_match)
{
    php_augeas *aug;
    zval *zaug;
    int i;
    char *path;
    char *value;
    char **matches;

    int path_len, retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zaug, &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    retval = aug_match(aug->augeas, path, &matches);

    array_init(return_value);

    if (retval == 0) {
        RETURN_NULL();
    }

    array_init(return_value);

    for (i=0; i<retval; i++) {
        add_next_index_string(return_value, matches[i], 1);
    }

}
/* }}} */


/* {{{ proto array augeas_rm(resource $augeas, string $path);
       Removes a node and all it's children */
PHP_FUNCTION(augeas_rm)
{   
    php_augeas *aug;
    zval *zaug;
    int path_len, value_len, retval;
    char *path, *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zaug, &path, &path_len, &value, &value_len)) {
        RETURN_FALSE;
    }

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    /* aug_rm returns the number of removed entries */
    retval = aug_rm(aug->augeas, path);

    RETURN_LONG(retval);
}
/* }}} */


/* {{{ boolean array augeas_set(resource $augeas, string $path, string $value);
       Sets the value of $path to $value */
PHP_FUNCTION(augeas_set)
{
    php_augeas *aug;
    zval *zaug;
    int path_len, value_len, retval;
    char *path, *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss", &zaug, &path, &path_len, &value, &value_len)) {
        RETURN_FALSE;
    }

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    retval = aug_set(aug->augeas, path, value);

    if (retval == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }

}
/* }}} */


/* {{{  proto boolean augeas_insert(resource $augeas, string $path, string $label, int $before);
        Inserts a new sibling of path expression $path with label $label before or after $path, depending on $before. $path must match exactly one node in the tree. */
PHP_FUNCTION(augeas_insert)
{    
    php_augeas *aug;
    zval *zaug;
    char *path, *label;
    int retval, path_len, label_len, before;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rssl", &zaug, &path,
        &path_len, &label, &label_len, &before)) {
        RETURN_FALSE;
    }

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    retval = aug_insert(aug->augeas, path, label, before);

    if (retval == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}


/* {{{  proto boolean augeas_save(resource $augeas);
        Saves the parts of the tree that have been changed into their respective files. */
PHP_FUNCTION(augeas_save)
{
    php_augeas *aug;
    zval *zaug;
    int retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zaug)) {
        RETURN_FALSE;
    }

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    retval = aug_save(aug->augeas);

    if (retval == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }

}
/* }}} */


/* {{{ proto boolean augeas_close(resource $augeas);
       Closes an Augeas resource */
PHP_FUNCTION(augeas_close)
{
    php_augeas *aug;
    zval *zaug;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zaug)) {
        RETURN_FALSE;
    }

    aug = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

    aug_close(aug->augeas);

}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
