#include "add_person.h"

Add_Person::Add_Person(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.add_pb			, SIGNAL(clicked()), this, SLOT(add				()));
	connect(ui.cancel_pb		, SIGNAL(clicked()), this, SLOT(cancel			()));

}

Add_Person::~Add_Person()
{

}

void Add_Person::cancel	()
{
	reject();
}

void Add_Person::add	()
{
	
	prs.name		= ui.name_le		->text();
	prs.surname		= ui.surname_le		->text();
	prs.father_name = ui.father_name_le	->text();
	prs.description = ui.description_le	->toPlainText();
	prs.date_birth  = ui.dateEdit		->date();
	prs.age			= QDateTime::currentDateTime().date().year() - prs.date_birth.year();

	emit send_Person();
	reject();
}


QString get_string_dayOfWeek(int n)
{
	switch(n)
	{
	case 1:
		return "��";
	case 2:
		return "��";
	case 3:
		return "��";
	case 4:
		return "��";
	case 5:
		return "��";
	case 6:
		return "��";
	case 7:
		return "��";
	default:
		return "������";
	}
}
