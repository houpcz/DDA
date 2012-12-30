#pragma once
#include <QApplication>
class QApp :
	public QApplication
{
public:
	virtual bool QApp::notify(QObject * receiver, QEvent * event);
	QApp(int & argc, char ** argv);
	~QApp(void);
};

