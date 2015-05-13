# SpreadMath

Copyright © 2015 Amit Shekhar Bongir

SpreadMath is a simple Spreadsheet application. Along with text manipulation, it supports various mathematical operators and trigonometric functions, which could be operated upon the universal number system of complex numbers. Only '.sm' file formats supported

Along with basic editing options it consists of functionalities for sorting selected cells, finding certain text, going to a particular cell, etc.

Use QMake to build the project from source with either of the following Qt versions :

1) Qt4 - 4.8 :

    cd src && qmake-qt4 spreadmath.pro && make

2) Qt5 - 5.2 or higher :

    cd src && qmake spreadmath.pro && make

Then run the application with :

    cd .. && ./spreadmath
