# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/pen
# Target is a library:  

INSTALLS += target
target.path = /lib/

HEADERS += tuppenwidget.h tuppenthicknesswidget.h
SOURCES += tuppenwidget.cpp tuppenthicknesswidget.cpp

CONFIG += dll warn_on
TEMPLATE = lib
TARGET = pen

FRAMEWORK_DIR = "../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../components_config.pri)
