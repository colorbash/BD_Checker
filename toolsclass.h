#ifndef TOOLSCLASS_H
#define TOOLSCLASS_H

#include <QDialog>
#include "ui_toolsclass.h"
#include <QMessageBox>
#include "add_person.h"

// ���� ����������� - ������ ����, ��� � ������, �������
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
	Add_Person				add_pers_form;	// ����� ���������� ��������
	int						day_warning;	// �� ������� �������������
	int						type_notif;		// ���
	int						day_notif;		// ���� ������ � ������� ���������
	QTime					time_notif;		// ����� ��������������
	bool					sayHallo_flag;	// ��������������?
	//___________________________________

private slots:
	//___________________________________
	void closeApp		();					// �����	
	void ok				();					// ��������� ���������
	void cancel			();					// ������ ������
	void show_credits	();					// �������� ����������
	void set_notifi_stat(int n);			// ��������� ��� �����������
	void addPerson		();					// �������� ��������
	void set_form_params();					// ���������� ��� ��������� �� �����
	//___________________________________

signals:
	void apply_settings	(int);				// �������� � ������� �����
private:
	Ui::ToolsClass ui;
};
//________________________________________________________
#endif // TOOLSCLASS_H
