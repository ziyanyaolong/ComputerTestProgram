#include <QtWidgets/qapplication.h>
#include <qdebug.h>
#include <qtextcodec.h>

#include "core/GUI/GUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI w;
    w.show();
    return a.exec();
}
