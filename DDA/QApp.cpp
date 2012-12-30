#include "QApp.h"
#include <QDebug>

QApp::QApp(int & argc, char ** argv) : QApplication(argc, argv)
{
}


QApp::~QApp(void)
{ 
}

bool QApp::notify(QObject * receiver, QEvent * event) 
{
     try 
     {
		return QApplication::notify(receiver, event);
     } 
     catch(char * error) 
     {
      qDebug() << "Exception thrown:" << error;
     }
	 return false;
}
