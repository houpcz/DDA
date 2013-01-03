#include <QApplication>
#include <QGlobal.h>
#include <QTime>

#include "DDAWidget.h"
#include "QApp.h"

int main(int argc, char *argv[])
{
    QApp app(argc, argv);

    DDAWidget widget;
	widget.show();

    return app.exec();
}