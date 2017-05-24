
#include "bd_checker.h"

void BD_Checker::showTrayIcon	()
{
	// Создаём экземпляр класса и задаём его свойства...
	trayIcon = new QSystemTrayIcon(this);
	QIcon trayImg			("BD_Checker.ico");
	trayIcon->setIcon		(trayImg);
	trayIcon->setContextMenu(trayIconMenu);

	// Подключаем обработчик клика по иконке...
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
	// Выводим значок...
	trayIcon -> show();
}
//__________________________________________________________________________________________________

void BD_Checker::trayActionExecute()
{
	//QMessageBox::information(this, "TrayIcon", "Тестовое сообщение. Замените вызов этого сообщения своим кодом.");
	vizual();
}
//__________________________________________________________________________________________________

void BD_Checker::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:
		this -> trayActionExecute();
		break;
	default:
		break;
	}
}
//__________________________________________________________________________________________________

void BD_Checker::setTrayIconActions()
{
	// Setting actions...
	minimizeAction	= new QAction("Свернуть", this);
	restoreAction	= new QAction("Восстановить", this);
	quitAction		= new QAction("Выход", this);

	// Connecting actions to slots...
	connect (minimizeAction,	SIGNAL(triggered()), this, SLOT(hide()));
	connect (restoreAction,		SIGNAL(triggered()), this, SLOT(showNormal()));
	connect (quitAction,		SIGNAL(triggered()), qApp, SLOT(quit()));

	// Setting system tray's icon menu...
	trayIconMenu = new QMenu(this);
	trayIconMenu -> addAction (minimizeAction);
	trayIconMenu -> addAction (restoreAction);
	trayIconMenu -> addAction (quitAction);
}
//__________________________________________________________________________________________________

void BD_Checker::changeEvent(QEvent *event)
{
	QDialog::changeEvent(event);
	if (event -> type() == QEvent::WindowStateChange)
	{
		if (isMinimized())
		{
			this -> hide();
		}
	}
}



//__________________________________________________________________________________________________
int Person::setData(QString _str)
{
	_str.simplified();

	// Birth
	date_birth = QDate::fromString(_str.left(10),"dd.MM.yyyy");

	_str.remove(0, 11);

	int pos1 = _str.indexOf(" ");

	// Surname
	surname  =  _str.mid(1, pos1);

	_str.remove(0, pos1+1);

	pos1	 = _str.indexOf(".");

	// Name
	name	 =  _str.mid(0, pos1);

	_str.remove(0, pos1+1);

	pos1	 = _str.indexOf(".");

	// Father name
	father_name	 =  _str.mid(0, pos1);

	_str.remove(0, pos1+2);

	// Description
	description  = _str.simplified(); 

	return 0;
}
//__________________________________________________________________________________________________

void BD_Checker::save_person()
{
	person_data[settings_form.add_pers_form.prs.date_birth] = settings_form.add_pers_form.prs;

	QFile F(FNAME);
	Person prs;
	QString str = "\n";
	if (F.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		while(!F.atEnd())
		{
			F.readLine();
		}
		QTextStream out(&F);
		str+= settings_form.add_pers_form.prs.date_birth.toString("dd.MM.yyyy") + "\t\t";
		str+= settings_form.add_pers_form.prs.surname		+ " "	;
		str+= settings_form.add_pers_form.prs.name			+ "."	;
		str+= settings_form.add_pers_form.prs.father_name	+ ".\t" ;
		str+= settings_form.add_pers_form.prs.description	;

		out << str;
		F.close();
	}

	vizual();
}




//static int msecsTo(const QTime & at) {
//	const int msecsPerDay = 24 * 60 * 60 * 1000;
//	int msecs = QTime::currentTime().msecsTo(at);
//	if (msecs < 0) msecs += msecsPerDay;
//	return msecs;
//}
//
//// C++11
//
//void runAt(const std::function<void> & job, const QTime & at, Qt::TimerType type = Qt::VeryCoarseTimer) {
//	// Timer ownership prevents timer leak when the thread terminates.
//	auto timer = new QTimer(QAbstractEventDispatcher::instance());
//	timer->start(msecsTo(at), type);
//	QObject::connect(timer, &QTimer::timeout, [=job, &timer]{
//		job();
//		timer->deleteLater();
//	});
//} 