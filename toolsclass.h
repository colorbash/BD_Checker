#ifndef TOOLSCLASS_H
#define TOOLSCLASS_H

#include <QDialog>
#include "ui_toolsclass.h"
#include <QMessageBox>
#include "add_person.h"

// Типы уведомлений - каждый день, раз в неделю, никогда
#define NOTIF_EVERYDAY	0
#define NOTIF_EVERYWEEK	1
#define NOTIF_NEVER		2

//_______________________________________________________
class ToolsClass : public QDialog
{
	Q_OBJECT

public:
	 ToolsClass(QWidget *parent = 0);
	~ToolsClass();
	//___________________________________
	Add_Person				add_pers_form;	// Форма добавления человека
	int						day_warning;	// За сколько предупреждать
	int						type_notif;		// тип
	int						day_notif;		// День недели в который оповещать
	QTime					time_notif;		// Время предупреждения
	bool					sayHallo_flag;	// Приветствовать?
	//___________________________________

private slots:
	//___________________________________
	void closeApp		();					// Выход	
	void ok				();					// Применить настройки
	void cancel			();					// Нажали отмена
	void show_credits	();					// Показать создателей
	void set_notifi_stat(int n);			// Изменился тип уведомлений
	void addPerson		();					// Добавить человека
	void set_form_params();					// Установить все параметры на форме
	//___________________________________

signals:
	void apply_settings	(int);				// Сообщаем в главный класс
private:
	Ui::ToolsClass ui;
};
//________________________________________________________
#endif // TOOLSCLASS_H
