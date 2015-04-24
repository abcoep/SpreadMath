lessThan(QT_MAJOR_VERSION, 5) {
        error(Qt 5.2 or newer is required but version $$[QT_VERSION] was detected.)
}

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 2) {
        error(Qt 5.2 or newer is required but version $$[QT_VERSION] was detected.)
    }

    QT += widgets
    CONFIG += c++11
}

TEMPLATE = app
TARGET = spreadmath
INCLUDEPATH += . gui math

# Input
HEADERS += gui/c2cpp_interface.h \
           gui/cell.h \
           gui/finddialog.h \
           gui/gotocelldialog.h \
           gui/mainwindow.h \
           gui/manualdialog.h \
           gui/sortdialog.h \
           gui/spreadsheet.h \
           math/expo_stack.h \
           math/func.h
FORMS += gui/gotocelldialog.ui gui/manualdialog.ui gui/sortdialog.ui
SOURCES += main.cpp \
           gui/cell.cpp \
           gui/expression.cpp \
           gui/finddialog.cpp \
           gui/gotocelldialog.cpp \
           gui/mainwindow.cpp \
           gui/sortdialog.cpp \
           gui/spreadsheet.cpp \
           math/answer.c \
           math/calc.c \
           math/expo_stack.c \
           math/func.c \
           math/operators.c
RESOURCES += spreadmath.qrc
