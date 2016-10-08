#-------------------------------------------------
#
# Project created by QtCreator 2013-01-18T21:04:08
#
#-------------------------------------------------

QT       += core gui opengl network xml
CONFIG   -= app_bundle
TARGET = gvsim
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
equals(QT_MAJOR_VERSION, 5): DEFINES += USING_QT5

#DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += INTEGRATION_STEP_MILLIS=10
DEFINES += DEFAULT_INTEGRATION_STEP=0.01
DEFINES += RECORDING_FACTOR=1
DEFINES += VIEWING_STEP=0.02
DEFINES += RAD2DEG_=57.295779


CONFIG += use_mapviewer
#CONFIG += use_3dview
use_mapviewer {
    DEFINES += USE_MAPVIEWER
    message("Using Mapviewer")
    HEADERS  +=     3dworld/map3d.h
    SOURCES +=     3dworld/map3d.cpp
}

use_3dview {
    DEFINES += USE_3DVIEW
    message("Using 3dworld")
    HEADERS  += 3dworld/world3d.h
    SOURCES += 3dworld/world3d.cpp
}

user_3dview|use_mapviewer {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib64
    LIBS += -L/usr/local/lib \
    -lOpenThreads \
    -losgEarth \
    -losgEarthQt \
    -losgEarthUtil \
    -losg \
    -losgQt \
    -losgDB \
    -losgGA \
    -losgViewer
    LIBS += -lX11
}

INCLUDEPATH += src
INCLUDEPATH += ../gvsim-simengine/include

win32 {
    QMAKE_LFLAGS= -enable-auto-import
    RC_FILE = gvsim.rc
    CONFIG += release
    INCLUDEPATH += C:\Developer\SDL-1.2.15\include
    LIBS  += C:\Developer\SDL-1.2.15\bin\SDL.dll
    LIBS += -lwsock32
}
unix {
    QMAKE_CXXFLAGS_DEBUG += -pg
    QMAKE_LFLAGS_DEBUG += -pg
    LIBS += -lSDL -lGLU
    LIBS += -lX11
    LIBS += -L../gvsim-simengine/ -lgvsim-simengine
}

SOURCES += main.cpp\
        mainwindow.cpp \
    settings/settingsjoystick.cpp \
    stickdisplay.cpp \
    commandstatus.cpp \
    keyselector.cpp \
    keyselectordialog.cpp \
    view3dwidget.cpp \
    recordplayworker.cpp \
    inputworker.cpp \
    logworker.cpp \
    mytoolbar.cpp \
    newflightwindow.cpp \
    view3dwindow.cpp \
    settings/settingsflightgear.cpp \
    graphiccharts/chartalphabeta.cpp \
    graphiccharts/chartcompass.cpp \
    graphiccharts/chartliftdrag.cpp \
    about.cpp \
    toolwidgets/simtools.cpp \
    toolwidgets/windtools.cpp \
    exportdata.cpp \
    exportdataworker.cpp \
    planefilereader.cpp \
    logdisplay.cpp \
    controlpanel.cpp \
    graphiccharts/livegraph.cpp \
    toolwidgets/fueltools.cpp \
    graphiccharts/turnbank.cpp \
    settings/settingsmap.cpp \
    centerwidget.cpp \
    graphiccharts/numericaldata.cpp \
    graphiccharts/chartsdata.cpp \
    graphiccharts/graphicaldata.cpp \
    utils/xmlhighlighter.cpp \
    settings/settingswindow.cpp \
    settingsrecordplay.cpp \
    settings/settingssimulation.cpp \
    settings/settingsinterface.cpp \
    myeventfilter.cpp \
    toolwidgets/autopilottools.cpp

HEADERS  += mainwindow.h \
    settings/settingsjoystick.h \
    stickdisplay.h \
    commandstatus.h \
    keyselector.h \
    keyselectordialog.h \
    view3dwidget.h \
    recordplayworker.h \
    inputworker.h \
    logworker.h \
    mytoolbar.h \
    newflightwindow.h \
    view3dwindow.h \
    settings/settingsflightgear.h \
    graphiccharts/chartalphabeta.h \
    graphiccharts/chartcompass.h \
    graphiccharts/chartliftdrag.h \
    about.h \
    toolwidgets/simtools.h \
    toolwidgets/windtools.h \
    exportdata.h \
    exportdataworker.h \
    planefilereader.h \
    logdisplay.h \
    controlpanel.h \
    graphiccharts/livegraph.h \
    toolwidgets/fueltools.h \
    graphiccharts/turnbank.h \
    settings/settingsmap.h \
    centerwidget.h \
    graphiccharts/numericaldata.h \
    graphiccharts/chartsdata.h \
    graphiccharts/graphicaldata.h \
    utils/xmlhighlighter.h \
    utils/net_fdm.h \
    utils/net_ctrls.h \
    settings/settingswindow.h \
    settingsrecordplay.h \
    settings/settingssimulation.h \
    settings/settingsinterface.h \
    myeventfilter.h \
    toolwidgets/autopilottools.h


RESOURCES += \
    resources/icons.qrc \
    resources/images.qrc \
    resources/files.qrc

OTHER_FILES += \
    gvsim.rc

SUBDIRS += \
    src/simengine/simengine.pro

DISTFILES +=
