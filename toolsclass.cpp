#include "toolsclass.h"

ToolsClass::ToolsClass(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.ok_pb			, SIGNAL(clicked			())		, this, SLOT(ok				()));
	connect(ui.cancel_pb		, SIGNAL(clicked			())		, this, SLOT(cancel			()));
	connect(ui.close_pb			, SIGNAL(clicked			())		, this, SLOT(closeApp		()));
	connect(ui.credits_pb		, SIGNAL(clicked			())		, this, SLOT(show_credits	()));
	connect(ui.motifications_cb	, SIGNAL(currentIndexChanged(int))	, this, SLOT(set_notifi_stat(int)));
	connect(ui.add_person_pb	, SIGNAL(clicked			())		, this, SLOT(addPerson		()));
	connect(&add_pers_form		, SIGNAL(send_Person		())		, this, SLOT(cancel			()));

	
}

//_____________________________________

void ToolsClass::set_form_params()
{
	ui.day_of_week_cb	->setCurrentIndex(day_notif );
	ui.motifications_cb	->setCurrentIndex(type_notif);
	ui.timeEdit			->setTime		 (time_notif);
	ui.checkBox			->setChecked	 (sayHallo_flag);
	ui.days_warn_le		->setText		 (QString::number(day_warning));
	set_notifi_stat(type_notif);
	this->exec();
}

//_____________________________________


ToolsClass::~ToolsClass()
{

}

//_____________________________________

// Exit
//_____________________________________
void ToolsClass::closeApp()
{
	QApplication::quit();
}

//_____________________________________

// Apply settings
//_____________________________________
void ToolsClass::ok		()
{ 
	time_notif		= ui.timeEdit			->time();
	type_notif		= ui.motifications_cb	->currentIndex();
	day_notif		= ui.day_of_week_cb		->currentIndex();
	sayHallo_flag	= ui.checkBox			->isChecked();
	day_warning		= ui.days_warn_le		->text().toInt() + 1;
	emit apply_settings(ui.days_warn_le->text().toInt());
	accept();
}

//_____________________________________

// Cancel
//_____________________________________
void ToolsClass::cancel	()
{
	reject();
}

//_____________________________________
void ToolsClass::show_credits	()
{
	QMessageBox::warning(this, "Создатели",
		"Эту программу создал Никита Башаров, сотрудник СПП Филиала ПНБО и по совместительству студент МАИ 5-го курса, в 2017 году.\nПо поводу багов пишите на colorbash3000@gmail.com\nЯндекс Деньги: 410015245438532");
}

//_____________________________________

void ToolsClass::set_notifi_stat(int n)
{
	type_notif = n;
	switch (type_notif)
	{

	case NOTIF_EVERYDAY:
		{
			ui.day_of_week_cb	->setVisible(false);
			ui.day_of_week_lb	->setVisible(false);
			ui.timeEdit			->setVisible(true);	
			ui.time_lb			->setVisible(true);	
			break;
		}
	case NOTIF_EVERYWEEK:
		{
			ui.day_of_week_cb	->setVisible(true );
			ui.day_of_week_lb	->setVisible(true);
			ui.timeEdit			->setVisible(true );
			ui.time_lb			->setVisible(true);	
			break;
		}
	case NOTIF_NEVER:
		{
			ui.day_of_week_cb	->setVisible(false);
			ui.day_of_week_lb	->setVisible(false);
			ui.timeEdit			->setVisible(false);
			ui.time_lb			->setVisible(false);	
			break;
		}
	}
}
//_____________________________________

void ToolsClass::addPerson		()
{
	add_pers_form.exec();
}

//_____________________________________

