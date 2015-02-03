QT += core xml
TEMPLATE= lib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
equals(QT_MAJOR_VERSION, 5): DEFINES += USING_QT5

DEFINES += INTEGRATION_STEP_MILLIS=10
DEFINES += DEFAULT_INTEGRATION_STEP=0.01
DEFINES += RECORDING_FACTOR=1
DEFINES += RAD2DEG_=57.295779

INCLUDEPATH += include
HEADERS += \
    include/simutils.h \
    include/atmosphereengine.h \
    include/autopilot.h \
    include/planemodeldata.h \
    include/planemodelengine.h \
    include/planestatusdata.h \
    include/simengine.h \
    include/curve3dhandler.h \
    include/curvehandler.h \
    include/typedefs.h

SOURCES += \
    src/simutils.cpp \
    src/atmosphereengine.cpp \
    src/autopilot.cpp \
    src/planemodelengine.cpp \
    src/simengine.cpp \
    src/curve3dhandler.cpp \
    src/curvehandler.cpp
