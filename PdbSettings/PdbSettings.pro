#-------------------------------------------------
#
# Project created by QtCreator 2012-06-01T10:25:11
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += xml

CONFIG 	 += plugin
CONFIG 	 += crypto

LIBS	 += -lqca

TARGET = PdbSettings
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colorgraficelements.cpp \
    nodesandattachmentsgraficeelements.cpp \
    abstractgraficeelements.cpp \
    dbgraficelements.cpp \
    loggetelements.cpp \
    securityelements.cpp \
    dbcreator.cpp \
    dbstructuredescriptor.cpp \
    ../CommonInclude/pdb/DBSettings.cpp \
    ../CommonInclude/pdb/cryptosupport.cpp

HEADERS  += mainwindow.h \
    dbgraficelements.h \
    colorgraficelements.h \
    ../../../../../home/alex/MyProjects/MyQtSamples/CommonInclude/pdb/pdb_style.h \
    nodesandattachmentsgraficeelements.h \
    abstractgraficeelements.h \
    SectionNames.h \
    loggetelements.h \
    securityelements.h \
    dbcreator.h \
    dbstructuredescriptor.h \
    ../CommonInclude/pdb/DBSettings.h \
    ../CommonInclude/pdb/DBSettingsStream.h \
    ../CommonInclude/pdb/cryptosupport.h \
    ../CommonInclude/pdb/VariantPtr.h

FORMS    += mainwindow.ui
