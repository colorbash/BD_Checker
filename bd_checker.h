/***************************************************************************
*****************************    BD_CHECKER     ****************************
***************************************************************************/ 
// Author	: Nikita Basharov
// Version	: 0.1
// Date		: 05.2017
// Company	: SPP/PNBO



#ifndef BD_CHECKER_H
#define BD_CHECKER_H

#define FNAME		"Birthday.txt"
#define INI_FNAME	"BD_Checker.ini"

#include <QtGui/QDialog>
#include "ui_bd_checker.h"
#include <QFile>
#include <QDateTime>
#include <QMap>
#include <QDebug>
#include <QTextCodec>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QObject>
#include "toolsclass.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QVariant>
#include <QTableView>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QHeaderView>
#include <QThread>
#include <QObject>
#include "detail_info.h"
#include <QDir>
//#include "qttelegrambot.h"

class MyTimer : public QThread
{
	Q_OBJECT
public:
	 MyTimer(){};
	~MyTimer(){};
	QTime noyif_time;
private slots:
	void count();
signals:
	void remind_bd();
};

//______________________________________________________________
class BD_Checker : public QDialog
{
	Q_OBJECT

public:
	 BD_Checker(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BD_Checker();

	// Settings
	//_____________________________
	ToolsClass				settings_form;	// Форма настроек
	Detail_Info				detail_info;	// Форма информации
	bool					settings_Changed_flag;	// Проверка на изменение ини файла в деструкторе

	
private:
	// Таблица
	//_____________________________
	QStandardItemModel		*model	;		// Модель такблицы
	QStandardItemModel		*search_model;	// Модель Поиска
	QStandardItem 			*item	;		// Ячейка таблицы
	QStringList				headers;		// Заголовки для кнопки ОК
	QMap<QDate, Person >	person_data;	// Персональные данные
	QVector<Person>			pers_per_index;	// Данные для поиска
	//_____________________________

	MyTimer*				timer_thr;		// Таймер для уведомлений

	//_____________________________
	
	QDate					today;			// Дата сегодня
	QString					outStr;			// Строка на форме

	QStringList				ok_list;		// Строки для кнопки ОК
	bool					hasBD;			// Есть ли ДР на период уведомления

	//_____________________________
	// Slots
	//_____________________________
private slots:
	void check				();					// Проверим ДР
	void ok					();					// Нажали ок
	void first_time			();					// При первом запуске
	void vizual				();					// Выведет на форму
	void remind_bd			();					// Настройки
	void save_settings		(int _n);			// Сохранить настройки
	void save_person		();					// Сохраним человека
	void apply_search		(QString _txt);		// Применить поиск
	//_____________________________
	// Tray slots
	//_____________________________
	void showTrayIcon		();
	void trayIconActivated	(QSystemTrayIcon::ActivationReason reason);
	void trayActionExecute	();
	void setTrayIconActions	();
	void changeEvent		(QEvent*);
	void showDetailInfo		(QModelIndex _ind);
	//_____________________________



	//_____________________________
	// Tray
	//_____________________________
private:
	QSystemTrayIcon *trayIcon;
	QMenu			*trayIconMenu;
	QAction			*minimizeAction	;
	QAction			*restoreAction	;
	QAction			*quitAction		;
	//_____________________________


	int  readFile ();					// Прочитаем файл
	void readIni  ();					// Прочитаем ини
	void createIni();					// возмем настройки из ини
	Ui::BD_CheckerClass ui;
};

#endif // BD_CHECKER_H
