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

/* {{{ zend_class_entry */
zend_class_entry *augeas_ce_Augeas;
/* }}} */

/* {{{ ZEND_BEGIN_ARG_INFO */
static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas__construct, 0)
    ZEND_ARG_INFO(0, root)
    ZEND_ARG_INFO(0, loadpath)
    ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas_get, 0)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas_match, 0)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas_set, 0)
    ZEND_ARG_INFO(0, path)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas_save, 0)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas_rm, 0)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO(arginfo_Augeas_insert, 0)
    ZEND_ARG_INFO(0, path)
    ZEND_ARG_INFO(0, label)
    ZEND_ARG_INFO(0, order)
ZEND_END_ARG_INFO();
/* }}} */

/* {{{ augeas_methods */
static zend_function_entry augeas_methods[] = {
    PHP_ME(Augeas, __construct, arginfo_Augeas__construct, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, __destruct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, get, arginfo_Augeas_get, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, set, arginfo_Augeas_set, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, match, arginfo_Augeas_match, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, rm, arginfo_Augeas_rm, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, save, arginfo_Augeas_save, ZEND_ACC_PUBLIC)
	PHP_ME(Augeas, insert, arginfo_Augeas_insert, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ augeas_module_entry
 */
zend_module_entry augeas_module_entry = {
	STANDARD_MODULE_HEADER,
	"augeas",
	NULL,
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

/* {{{ _php_augeas_read_resource() */
static php_augeas * _php_augeas_read_resource(zval *object)
{
	zval *zaug;
    php_augeas *return_value;
	zaug = zend_read_property(Z_OBJCE_P(object), object, "handle", strlen("handle"), 0 TSRMLS_DC);

	if (Z_TYPE_P(zaug) != IS_RESOURCE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "could not read augeas resource");
		return NULL;
	}

	return_value = (php_augeas *) zend_fetch_resource(&zaug TSRMLS_CC, -1, PHP_AUGEAS_RESOURCE_NAME, NULL, 1, le_augeas);

	return return_value;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(augeas)
{
    le_augeas = zend_register_list_destructors_ex(_php_augeas_dtor, NULL, PHP_AUGEAS_RESOURCE_NAME, module_number); 

	zend_class_entry ce;

    /* Register Augeas class */
    INIT_CLASS_ENTRY(ce, "Augeas", augeas_methods);
    augeas_ce_Augeas = zend_register_internal_class(&ce TSRMLS_CC);
    augeas_ce_Augeas->ce_flags |= ZEND_ACC_FINAL_CLASS;

	/* Register Augeas class constants */
	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_NONE", sizeof("AUGEAS_NONE")-1, AUG_NONE TSRMLS_DC);
	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_SAVE_BACKUP", sizeof("AUGEAS_SAVE_BACKUP")-1, AUG_SAVE_BACKUP TSRMLS_DC);
	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_SAVE_NEWFILE", sizeof("AUGEAS_SAVE_NEWFILE")-1, AUG_SAVE_NEWFILE TSRMLS_DC);
	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_TYPE_CHECK", sizeof("AUGEAS_TYPE_CHECK")-1, AUG_TYPE_CHECK TSRMLS_DC);
	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_NO_STDINC", sizeof("AUGEAS_NO_STDINC")-1, AUG_NO_STDINC TSRMLS_DC);

	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_INSERT_BEFORE", sizeof("AUGEAS_INSERT_BEFORE")-1, 0 TSRMLS_DC);
	zend_declare_class_constant_long(augeas_ce_Augeas, "AUGEAS_INSERT_AFTER", sizeof("AUGEAS_INSERT_AFTER")-1, 1 TSRMLS_DC);
 
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

/* {{{ proto void Augeas::__construct([string $root[, string $loadpath[, int $flags]]]) */
PHP_METHOD(Augeas, __construct)
{
  char *root = "/";
  char *loadpath = "";
  int root_len, loadpath_len;
  long flags = AUG_NONE;
  php_augeas *aug;
  zval *resource;
  zval *this;


  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!s!l", &root, &root_len, &loadpath, &loadpath_len, &flags) == FAILURE) {
    RETURN_FALSE;
  }

  if (php_check_open_basedir(root TSRMLS_DC) != 0) {
    RETURN_FALSE;
  }

  aug = emalloc(sizeof(php_augeas));
  aug->augeas = aug_init(root, loadpath, flags);

  int resource_id = ZEND_REGISTER_RESOURCE(resource, aug, le_augeas);

  this = getThis();

  if (this == NULL) {
	php_error_docref(NULL TSRMLS_CC, E_WARNING, "could not initialize augeas resource");
  }

  add_property_resource(this, "handle", resource_id);

}
/* }}} */

/* {{{ proto string Augeas::get(string $path) */
PHP_METHOD(Augeas, get)
{
    char *path, *value;
    char **matches;
    int path_len, retval;
    php_augeas *aug;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (path_len < 1) {
        RETURN_FALSE;
    }   

    aug = _php_augeas_read_resource(getThis());

	if (!aug) RETURN_NULL();

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

/* {{{ proto boolean Augeas::set(resource $augeas, string $path, string $value);
       Sets the value of $path to $value */
PHP_METHOD(Augeas, set)
{
    php_augeas *aug;
    int path_len, value_len, retval;
    char *path, *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &path, &path_len, &value, &value_len)) {
        RETURN_FALSE;
    }

    aug = _php_augeas_read_resource(getThis());

	if (!aug) RETURN_NULL();

    retval = aug_set(aug->augeas, path, value);

    if (retval == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }

}
/* }}} */

/* {{{  proto array Augeas::match(resource $augeas, string $path);
        Returns an array with all the matches */
PHP_METHOD(Augeas, match)
{
    php_augeas *aug;
    int i;
    char *path;
    char *value;
    char **matches;

    int path_len, retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }

	aug = _php_augeas_read_resource(getThis());

	if (!aug) RETURN_NULL();

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

/* {{{ proto boolean Augeas::rm(resource $augeas, string $path);
       Removes a node and all it's children */
PHP_METHOD(Augeas, rm)
{   
    php_augeas *aug;
    int path_len, value_len, retval;
    char *path, *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len, &value, &value_len)) {
        RETURN_FALSE;
    }

	aug = _php_augeas_read_resource(getThis());

	if (!aug) RETURN_NULL();

    /* aug_rm returns the number of removed entries */
    retval = aug_rm(aug->augeas, path);

    RETURN_LONG(retval);
}
/* }}} */

/* {{{  proto boolean Augeas::insert(resource $augeas, string $path, string $label, int $order);
        Inserts a new sibling of path expression $path with label $label before or after $path, depending on $order. $path must match exactly one node in the tree. */
PHP_METHOD(Augeas, insert)
{    
    php_augeas *aug;
    char *path, *label;
    int retval, path_len, label_len, order;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssl", &path, &path_len,
        &label, &label_len, &order)) {
        RETURN_FALSE;
    }

	aug = _php_augeas_read_resource(getThis());
	if (!aug) RETURN_NULL();

    retval = aug_insert(aug->augeas, path, label, order);

    if (retval == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{  proto boolean Augeas::save(resource $augeas);
        Saves the parts of the tree that have been changed into their respective files. */
PHP_METHOD(Augeas, save)
{
    php_augeas *aug;
    zval *zaug;
    int retval;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, NULL)) {
        RETURN_FALSE;
    }

	aug = _php_augeas_read_resource(getThis());
	if (!aug) RETURN_NULL();

    retval = aug_save(aug->augeas);

    if (retval == 0) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }

}
/* }}} */

/* {{{ proto void Augeas::__destruct();
       Class destructor */
PHP_METHOD(Augeas, __destruct)
{
    php_augeas *aug;
    zval *zaug;

	aug = _php_augeas_read_resource(getThis());
	if (!aug) return;

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
