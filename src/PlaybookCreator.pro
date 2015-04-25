#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T00:09:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlaybookCreator
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += /usr/local/include/botan-1.10/
LIBS += -lboost_serialization \
    -lbotan-1.10

SOURCES += main.cpp\
    dialogs/mainDialog.cpp \
    models/pbcPlay.cpp \
    models/pbcPlayer.cpp \
    models/pbcRoute.cpp \
    models/pbcPath.cpp \
    gui/pbcPlayerView.cpp \
    util/pbcPositionTranslator.cpp \
    dialogs/pbcCustomRouteDialog.cpp \
    gui/pbcCustomRouteView.cpp \
    gui/pbcPlayView.cpp \
    gui/pbcGridIronView.cpp \
    models/pbcPlaybook.cpp \
    models/pbcDataModel.cpp \
    models/pbcColor.cpp \
    util/pbcStorage.cpp \
    dialogs/pbcExportPdfDialog.cpp

HEADERS  += pbcVersion.h \
    dialogs/mainDialog.h \
    util/pbcDeclarations.h \
    models/pbcPlay.h \
    models/pbcPlayer.h \
    models/pbcRoute.h \
    models/pbcPath.h \
    util/pbcConfig.h \
    models/pbcCategory.h \
    models/pbcFormation.h \
    util/pbcSingleton.h \
    gui/pbcPlayerView.h \
    util/pbcPositionTranslator.h \
    dialogs/pbcCustomRouteDialog.h \
    gui/pbcCustomRouteView.h \
    gui/pbcPlayView.h \
    gui/pbcGridIronView.h \
    models/pbcPlaybook.h \
    models/pbcDataModel.h \
    models/pbcColor.h \
    util/pbcStorage.h \
    util/pbcExceptions.h \
    dialogs/pbcExportPdfDialog.h

FORMS    += dialogs/mainDialog.ui \
    dialogs/pbcCustomRouteDialog.ui \
    dialogs/pbcExportPdfDialog.ui
