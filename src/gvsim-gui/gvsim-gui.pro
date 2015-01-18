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

DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += INTEGRATION_STEP_MILLIS=10
DEFINES += DEFAULT_INTEGRATION_STEP=0.01
DEFINES += RECORDING_FACTOR=1
DEFINES += VIEWING_STEP=0.02
DEFINES += RAD2DEG_=57.295779


INCLUDEPATH += src

INCLUDEPATH += ../gvsim-simengine/include

win32 {
    #DEFINES += USING_3DMAP
    QMAKE_LFLAGS= -enable-auto-import
    RC_FILE = gvsim.rc
    CONFIG += release
    INCLUDEPATH += C:\Developer\SDL-1.2.15\include
#    INCLUDEPATH += $$quote(C:\Program Files (x86)\marble\include)
#    INCLUDEPATH += $$quote(C:\Program Files (x86)\OSGEARTH\include)
#    INCLUDEPATH += $$quote(C:\Program Files (x86)\OpenSceneGraph\include)
#    LIBS += $$quote(C:/Program Files (x86)\marble\libmarblewidget.dll)
#    LIBS += -L"C:\Program Files (x86)\marble" -l"libmarblewidget"
#    LIBS += -L:$$quote(C:\Program Files (x86)\OSGEARTH\lib)
#    LIBS += -L:$$quote(C:\Program Files (x86)\OpenSceneGraph\lib)
#    LIBS += $$quote(C:/Program Files (x86)\OSGEARTH\bin\libosgEarth.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OSGEARTH\bin\libosgEarthQt.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OSGEARTH\bin\libosgEarthUtil.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OpenSceneGraph\bin\libosg.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OpenSceneGraph\bin\libOpenThreads.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OpenSceneGraph\bin\libosgDB.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OpenSceneGraph\bin\libosgGA.dll)
#    LIBS += $$quote(C:/Program Files (x86)\OpenSceneGraph\bin\libosgViewer.dll)
    LIBS  += C:\Developer\SDL-1.2.15\bin\SDL.dll
    LIBS += -lwsock32
}
unix {
    DEFINES += USING_3DMAP
    #DEFINES += TESTING_GPROF
#    QMAKE_CXXFLAGS_DEBUG += -pg
#    QMAKE_LFLAGS_DEBUG += -pg
    LIBS += -lSDL -lGLU
#    LIBS += -L/usr/local/lib \
#    -lmarblewidget
#    #OSG
#    LIBS += -L/usr/local/lib \
#    -lOpenThreads \
#    -losgEarth \
#    -losgEarthQt \
#    -losgEarthUtil \
#    -losg \
#    -losgQt \
#    -losgDB \
#    -losgGA \
#    -losgViewer
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
    commandstatusdata.h \
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
