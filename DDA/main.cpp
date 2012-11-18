#include <QApplication>
#include <QGlobal.h>
#include <QTime>

#include "DDAWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DDAWidget widget;
	QTime time = QTime::currentTime();
	srand((uint)time.msec());
	qsrand((uint)time.msec());
	widget.show();

    return app.exec();
}