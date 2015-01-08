#include "mainwindow.h"

#ifndef WIN32
#include <X11/Xlib.h>
#endif

int main(int argc, char *argv[])
{

    #ifndef WIN32
        XInitThreads();
    #endif
    QApplication a(argc, argv);
    a.setApplicationName("GVSim");
    MainWindow w;
    w.showMaximized();
    w.setAttribute(Qt::WA_QuitOnClose);
    return a.exec();
}
