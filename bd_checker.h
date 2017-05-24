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
	ToolsClass				settings_form;	// ����� ��������
	Detail_Info				detail_info;	// ����� ����������
	bool					settings_Changed_flag;	// �������� �� ��������� ��� ����� � �����������

	
private:
	// �������
	//_____________________________
	QStandardItemModel		*model	;		// ������ ��������
	QStandardItemModel		*search_model;	// ������ ������
	QStandardItem 			*item	;		// ������ �������
	QStringList				headers;		// ��������� ��� ������ ��
	QMap<QDate, Person >	person_data;	// ������������ ������
	QVector<Person>			pers_per_index;	// ������ ��� ������
	//_____________________________

	MyTimer*				timer_thr;		// ������ ��� �����������

	//_____________________________
	
	QDate					today;			// ���� �������
	QString					outStr;			// ������ �� �����

	QStringList				ok_list;		// ������ ��� ������ ��
	bool					hasBD;			// ���� �� �� �� ������ �����������

	//_____________________________
	// Slots
	//_____________________________
private slots:
	void check				();					// �������� ��
	void ok					();					// ������ ��
	void first_time			();					// ��� ������ �������
	void vizual				();					// ������� �� �����
	void remind_bd			();					// ���������
	void save_settings		(int _n);			// ��������� ���������
	void save_person		();					// �������� ��������
	void apply_search		(QString _txt);		// ��������� �����
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


	int  readFile ();					// ��������� ����
	void readIni  ();					// ��������� ���
	void createIni();					// ������ ��������� �� ���
	Ui::BD_CheckerClass ui;
};

#endif // BD_CHECKER_H
