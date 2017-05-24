#ifndef ADD_PERSON_H
#define ADD_PERSON_H

#include <QDialog>
#include "ui_add_person.h"
#include <QValidator>

//______________________________________________________
struct Person
{
	QString		name, surname, father_name, description;	// ФИО, Описание
	QDate		date_birth;									// Дата рождения
	int			age;										// Возраст
	int			setData(QString _str);						// Инициализация
};
//______________________________________________________

//______________________________________________________
class Add_Person : public QDialog
{
	Q_OBJECT

public:
	 Add_Person(QWidget *parent = 0);
	~Add_Person();

	Person prs;			// Новая запись

private slots:
	void add	();		// Дбавить
	void cancel	();		// Отмена
signals:
	void send_Person();	// Сигнал о добавлении
private:
	Ui::Add_Person ui;
};
//______________________________________________________

QString get_string_dayOfWeek(int n);	// Вернет строку с днем недели по индексу


#endif // ADD_PERSON_H
