# Code Conventions #

* Use 4 spaces instead of one tab (you should be able to configure your editor
    to do this)
* 80 characters per line maximum (this can be exceeded if there's a reason)
* Header files should use guard definitions in the form of
    `GUARD_<header_name>_h`
* Typenames should use Pascal Case
* Other names should use lower-case letters separated by underscores
* Private and protected variable names should be post-fixed with an underscore
* Class members should be declared in the order of: public, protected, private
* With the exception of classes, structs, and functions, opening braces should
    be placed on the same line
* Opening braces for classes, structs, and functions should be placed on a new
    line, except if the closing parenthesis for the function's parameter list
    is on it's own line, then the opening brace should be on the same line as
    the closing parenthesis
* API calls where one or more arguments passed require explanation should have
    each passed argument on its own line with a comment following the argument
