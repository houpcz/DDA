#include <QApplication>

#include "DDAWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DDAWidget widget;
	widget.show();

    return app.exec();
}