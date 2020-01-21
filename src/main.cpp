#include "mark.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    marK w;
    w.setFixedSize(qApp->primaryScreen()->availableSize());
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return app.exec();
}

