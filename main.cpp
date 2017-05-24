#include "bd_checker.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon("BD_Checker.ico"));
	a.setQuitOnLastWindowClosed(false);
	BD_Checker w;
	if (w.settings_form.sayHallo_flag)
	{
		w.show();
	}
	return a.exec();
}
