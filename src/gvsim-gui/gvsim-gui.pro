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
}

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/settings/settingsjoystick.cpp \
    src/stickdisplay.cpp \
    src/commandstatus.cpp \
    src/keyselector.cpp \
    src/keyselectordialog.cpp \
    src/view3dwidget.cpp \
    src/recordplayworker.cpp \
    src/inputworker.cpp \
    src/logworker.cpp \
    src/mytoolbar.cpp \
    src/newflightwindow.cpp \
    src/view3dwindow.cpp \
    src/settings/settingsflightgear.cpp \
    src/graphiccharts/chartalphabeta.cpp \
    src/graphiccharts/chartcompass.cpp \
    src/graphiccharts/chartliftdrag.cpp \
    src/about.cpp \
    src/toolwidgets/simtools.cpp \
    src/toolwidgets/windtools.cpp \
    src/exportdata.cpp \
    src/exportdataworker.cpp \
    src/curvehandler.cpp \
    src/planefilereader.cpp \
    src/curve3dhandler.cpp \
    src/logdisplay.cpp \
    src/controlpanel.cpp \
    src/graphiccharts/livegraph.cpp \
    src/toolwidgets/fueltools.cpp \
    src/graphiccharts/turnbank.cpp \
    src/simengine/simengine.cpp \
    src/simengine/atmosphereengine.cpp \
    src/simengine/simutils.cpp \
    src/simengine/planemodelengine.cpp \
    src/simengine/autopilot.cpp \
    src/settings/settingsmap.cpp \
    src/centerwidget.cpp \
    src/graphiccharts/numericaldata.cpp \
    src/graphiccharts/chartsdata.cpp \
    src/graphiccharts/graphicaldata.cpp \
    src/utils/xmlhighlighter.cpp \
    src/settings/settingswindow.cpp \
    src/settingsrecordplay.cpp \
    src/settings/settingssimulation.cpp \
    src/settings/settingsinterface.cpp \
    src/myeventfilter.cpp \
    src/toolwidgets/autopilottools.cpp

HEADERS  += src/mainwindow.h \
    src/settings/settingsjoystick.h \
    src/stickdisplay.h \
    src/typedefs.h \
    src/commandstatus.h \
    src/keyselector.h \
    src/keyselectordialog.h \
    src/commandstatusdata.h \
    src/view3dwidget.h \
    src/recordplayworker.h \
    src/inputworker.h \
    src/logworker.h \
    src/mytoolbar.h \
    src/newflightwindow.h \
    src/view3dwindow.h \
    src/settings/settingsflightgear.h \
    src/graphiccharts/chartalphabeta.h \
    src/graphiccharts/chartcompass.h \
    src/graphiccharts/chartliftdrag.h \
    src/about.h \
    src/toolwidgets/simtools.h \
    src/toolwidgets/windtools.h \
    src/exportdata.h \
    src/exportdataworker.h \
    src/curvehandler.h \
    src/planefilereader.h \
    src/curve3dhandler.h \
    src/logdisplay.h \
    src/controlpanel.h \
    src/graphiccharts/livegraph.h \
    src/toolwidgets/fueltools.h \
    src/graphiccharts/turnbank.h \
    src/simengine/simengine.h \
    src/simengine/atmosphereengine.h \
    src/simengine/simutils.h \
    src/simengine/planemodelengine.h \
    src/simengine/autopilot.h \
    src/simengine/planestatusdata.h \
    src/simengine/planemodeldata.h \
    src/settings/settingsmap.h \
    src/centerwidget.h \
    src/graphiccharts/numericaldata.h \
    src/graphiccharts/chartsdata.h \
    src/graphiccharts/graphicaldata.h \
    src/utils/xmlhighlighter.h \
    src/utils/net_fdm.h \
    src/utils/net_ctrls.h \
    src/settings/settingswindow.h \
    src/settingsrecordplay.h \
    src/settings/settingssimulation.h \
    src/settings/settingsinterface.h \
    src/myeventfilter.h \
    src/toolwidgets/autopilottools.h

RESOURCES += \
    resources/icons.qrc \
    resources/images.qrc \
    resources/files.qrc

OTHER_FILES += \
    gvsim.rc
