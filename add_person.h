#ifndef ADD_PERSON_H
#define ADD_PERSON_H

#include <QDialog>
#include "ui_add_person.h"
#include <QValidator>

//______________________________________________________
struct Person
{
	QString		name, surname, father_name, description;	// ���, ��������
	QDate		date_birth;									// ���� ��������
	int			age;										// �������
	int			setData(QString _str);						// �������������
};
//______________________________________________________

//______________________________________________________
class Add_Person : public QDialog
{
	Q_OBJECT

public:
	 Add_Person(QWidget *parent = 0);
	~Add_Person();

	Person prs;			// ����� ������

private slots:
	void add	();		// �������
	void cancel	();		// ������
signals:
	void send_Person();	// ������ � ����������
private:
	Ui::Add_Person ui;
};
//______________________________________________________

QString get_string_dayOfWeek(int n);	// ������ ������ � ���� ������ �� �������


#endif // ADD_PERSON_H
