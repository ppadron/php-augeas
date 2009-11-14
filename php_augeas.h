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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_AUGEAS_H
#define PHP_AUGEAS_H

extern zend_module_entry augeas_module_entry;
#define phpext_augeas_ptr &augeas_module_entry

#ifdef PHP_WIN32
#define PHP_AUGEAS_API __declspec(dllexport)
#else
#define PHP_AUGEAS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


typedef struct _php_augeas {
    augeas *augeas;
} php_augeas;

PHP_MINIT_FUNCTION(augeas);
PHP_MSHUTDOWN_FUNCTION(augeas);
PHP_RINIT_FUNCTION(augeas);
PHP_RSHUTDOWN_FUNCTION(augeas);
PHP_MINFO_FUNCTION(augeas);

PHP_FUNCTION(augeas_init);
PHP_FUNCTION(augeas_match);
PHP_FUNCTION(augeas_get);
PHP_FUNCTION(augeas_set);
PHP_FUNCTION(augeas_save);
PHP_FUNCTION(augeas_rm);
PHP_FUNCTION(augeas_insert);
PHP_FUNCTION(augeas_close);

#define PHP_AUGEAS_RESOURCE_NAME "augeas"

#ifdef ZTS
#define AUGEAS_G(v) TSRMG(augeas_globals_id, zend_augeas_globals *, v)
#else
#define AUGEAS_G(v) (augeas_globals.v)
#endif

#endif	/* PHP_AUGEAS_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
