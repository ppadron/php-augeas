INSTALL
==========================================================================

Install instructions (you’ll need php5 dev package):

$ pecl install augeas

or from the git repo:

$ git clone git://github.com/ppadron/php-augeas.git
$ cd php-augeas
$ phpize
$ ./configure
$ make
$ make install

Create a file called augeas.ini in your PHP conf.d directory containing:

extension=augeas.so


API REFERENCE
==========================================================================

void    Augeas::__construct([string $root[, string $loadpath[, int $flags]]])
string  Augeas::get(string $path)
array   Augeas::match(string $path);
boolean Augeas::set(string $path, string $value);
boolean Augeas::rm($augeas, string $path);
boolean Augeas::insert(string $path, string $label, int $order);
boolean Augeas::mv(string $source, string $destination);
boolean Augeas::save();

Constants used as $flags in Augeas::__construct()

Augeas::AUGEAS_NONE = 0
Augeas::AUGEAS_SAVE_BACKUP = 1
Augeas::AUGEAS_SAVE_NEWFILE = 2
Augeas::AUGEAS_TYPE_CHECK = 4
Augeas::AUGEAS_NO_STDINC = 8

Constants used as $order in Augeas::insert()

Augeas::AUGEAS_INSERT_BEFORE = 0
Augeas::AUGEAS_INSERT_AFTER = 1
