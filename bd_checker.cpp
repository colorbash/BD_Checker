#include "bd_checker.h"

BD_Checker::BD_Checker(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{

	// Russian languege
	//_________________________________________________
	srand(QDateTime::currentDateTime().time().msec());
	setlocale(LC_NUMERIC,"C");
	QTextCodec::setCodecForTr		(QTextCodec::codecForName("CP1251"));	
	QTextCodec::setCodecForCStrings	(QTextCodec::codecForName("CP1251"));
	//_________________________________________________

	// Запустим таймер
	//_________________________________________________
	QThread *thread = new QThread;
	timer_thr = new MyTimer;
	timer_thr->moveToThread(thread);
	connect(thread, SIGNAL(started()), timer_thr, SLOT(count()));
	thread->start();
	//_________________________________________________

	ui.setupUi(this);
	
	// Tray
	this -> setTrayIconActions	();
	this -> showTrayIcon		();
	
	readIni();	// Прочтем ини

	// Connect buttons
	//_________________________________________________
	connect(ui.pushButton				, SIGNAL(clicked())					, this			, SLOT(ok				()));
	connect(ui.toolButton				, SIGNAL(clicked())					, &settings_form, SLOT(set_form_params()));
	connect(&settings_form				, SIGNAL(apply_settings(int))		, this			, SLOT(save_settings	(int)));
	connect(&settings_form.add_pers_form, SIGNAL(send_Person())				, this			, SLOT(save_person	()));
	connect(ui.search_le				, SIGNAL(textChanged(QString))		, this			, SLOT(apply_search	(QString)));
	connect(ui.tableView				, SIGNAL(doubleClicked(QModelIndex)), this			, SLOT(showDetailInfo (QModelIndex)));
	connect(timer_thr					, SIGNAL(remind_bd())				, this			, SLOT(remind_bd		()));
	//_________________________________________________

	ok_list << "Крутяк!"		<< "Пацаны вообще котята!"	<< "ОК"		<< "Вот спасиба!" 
			<< "Скоро пожрем!"	<< "Агонь!"					<< "=-)"	<< "Ку-ку Ёпта!"
			<< "Карамба!"		<< "Свистать всех наверх!";

	// Table
	//_________________________________________________
	headers << "Осталось\nдней" << "День\nнедели" << "У кого" << "Возраст" << "Дата\n(дд/мм)";// << "Примечание";
	model		= new QStandardItemModel(0,headers.size(),this);
	const QStringList c_headers = headers;
	model		->setHorizontalHeaderLabels(c_headers);
	ui.tableView->setSortingEnabled(false);
	//_________________________________________________

	first_time	();
	settings_Changed_flag = false;
}

// Проверка
//__________________________________________________________________________________________________
void BD_Checker::check   ()
{
	pers_per_index.clear();
	QVector<int>	days_before_vect;
	QVector<QDate>	next_b_date_vect;
	today = QDateTime::currentDateTime().date();
	outStr	= "Скоро день рождения у:\n";
	int		days_before, b_size = 0;
	QDate day_in_year_of_birth;	// этот день в году в котором родился человек
	QDate next_b_date;			// ДР в этом году

	QMap<QDate, Person>::iterator iter = person_data.begin();
	// View all pers
	//________________________________________________________
	while(iter != person_data.end())
	{
		day_in_year_of_birth.setDate(iter->date_birth.year(), today.month(), today.day());
		if (iter->date_birth < day_in_year_of_birth)
		{
			next_b_date.setDate(today.year()+1, iter.key().month(), iter.key().day());
		}
		else
		{
			next_b_date.setDate(today.year()  , iter.key().month(), iter.key().day());
		}

		days_before = today.daysTo(next_b_date);
		// Remember them
		//____________________________________________________
		if ( days_before <= settings_form.day_warning && days_before >= 0)
		{
			days_before_vect << days_before;
			pers_per_index   << iter.value();
			next_b_date_vect << next_b_date;
			++b_size;
		}
		//____________________________________________________
		++iter;
	}

	// Есть др?
	if (b_size == 0)
	{
		hasBD = false;
		return;
	}
	else
	{
		hasBD  = true;
	}

	// Sort by days before BD
	//____________________________________________________
	bool	cond = 0;
	int		tmp;
	QDate	tmp_dt;
	Person	tmp_pers;
	while (!cond)
	{
		cond = true;
		for (int j=1;j<days_before_vect.size();j++)
		{
			if (days_before_vect[j]<days_before_vect[j-1])
			{
				tmp = days_before_vect[j];
				days_before_vect[j] = days_before_vect[j-1];
				days_before_vect[j-1] = tmp;

				tmp_pers = pers_per_index[j];
				pers_per_index[j] = pers_per_index[j-1];
				pers_per_index[j-1] = tmp_pers;

				tmp_dt = next_b_date_vect[j];
				next_b_date_vect[j] = next_b_date_vect[j-1];
				next_b_date_vect[j-1] = tmp_dt;
				
				cond = 0;
			}
		}
	}
	//____________________________________________________

	// Show on table
	//____________________________________________________
	for (int i=0;i<days_before_vect.size();++i)
	{
		pers_per_index[i].age = (today.year() - pers_per_index[i].date_birth.year());	
		if (days_before_vect[i])
		{
			item = new QStandardItem(QString::number(days_before_vect[i]));
		}
		else
		{
			item = new QStandardItem("СЕГОДНЯ!");
		}
		model->setItem(i,0,item);

		item = new QStandardItem(get_string_dayOfWeek(next_b_date_vect[i].dayOfWeek()));
		model->setItem(i,1,item);

		item = new QStandardItem(pers_per_index[i].surname + " " + pers_per_index[i].name + " " + pers_per_index[i].father_name);
		model->setItem(i,2,item);

		item = new QStandardItem(QString::number(pers_per_index[i].age));
		model->setItem(i,3,item);

		item = new QStandardItem(pers_per_index[i].date_birth.toString("dd.MM"));
		model->setItem(i,4,item);
	}
	//____________________________________________________

	
}
//__________________________________________________________________________________________________

// Первый раз
//__________________________________________________________________________________________________
void BD_Checker::first_time	()
{
	if(readFile	())
		outStr =	 "Привет! я Cуперпрога, создана для того,\nчтобы ты не забыл когда ДР у твоих коллег. \nНо чтобы я работала правильно, положи файлик с ДР в мою папочку.\nДо новых встреч, юзверь! ";
	else
	{
		check();
		if (hasBD)
		{
			outStr = "Привет! я Cуперпрога, создана для того,\nчтобы ты не забыл когда ДР у твоих коллег. :)\nКстати, скоро ДР у: ";
		}
		else
		{
			outStr = "Привет! я Cуперпрога, создана для того,\nчтобы ты не забыл когда ДР у твоих коллег.\nДР на ближайшее время отсутствуют. :(";
		}
	}

	
	ui.tableView	->setModel(model						);
	ui.tableView	->resizeRowsToContents		();
	ui.tableView	->resizeColumnsToContents	();
	ui.text_lb		->setText (outStr						);
	ui.pushButton	->setText ("Спасибо, тебе, Cуперпрога!"	);
}

// Отображение
//__________________________________________________________________________________________________
void BD_Checker::vizual()
{
	check();
	if (hasBD)
	{
		int n = abs(((rand()/((double)RAND_MAX))-0.5)*(ok_list.size() - 1));
		
		ui.text_lb		->setText	(outStr			);
		ui.tableView	->setModel	(model			);
		ui.tableView	->resizeRowsToContents		();
		ui.tableView	->resizeColumnsToContents	();
		ui.pushButton	->setText	(ok_list[n]		);
	}
	show();
}
//__________________________________________________________________________________________________

// Из файла
//__________________________________________________________________________________________________
int BD_Checker::readFile()
{
	QFile F(FNAME);
	Person prs;
	if (F.open(QIODevice::ReadOnly))
	{
		QString temp_str = F.readLine();
		QDate bDate;
		int d,m,y;
		while(!F.atEnd())
		{
			if (temp_str != "")
			{
				if (temp_str.at(0) != '\t')
				{
					// Целостность данных
					if(!prs.setData(temp_str))
					{
						bDate = QDate::fromString(temp_str.left(10),"dd.MM.yyyy");
						person_data	[bDate] = prs;
					}
				}
			}
			temp_str = F.readLine();
		}
		return 0;
	}
	else
		return 1;
}
//__________________________________________________________________________________________________

// Press ok
//__________________________________________________________________________________________________
void BD_Checker::ok		 ()
{
	if(this->isVisible())
	{
		if (person_data.size())
		{
			hide();
		}
		else
		{
			QApplication::quit();
			reject();
		}
	}
}
//__________________________________________________________________________________________________

//__________________________________________________________________________________________________

//__________________________________________________________________________________________________

void BD_Checker::save_settings	(int _n)
{
	settings_form.day_warning	= _n;
	timer_thr->noyif_time		= settings_form.time_notif;
	settings_Changed_flag		= true;
	//qDebug()<<settings_form.time_notif.toString();
	vizual();
}
//__________________________________________________________________________________________________

void BD_Checker::readIni  ()
{
	QFile F(INI_FNAME);
	if (F.exists())
	{
		//QSettings ini_File(QSettings::IniFormat, QSettings::UserScope, INI_FNAME);
		QSettings* ini_File = new QSettings(INI_FNAME, QSettings::IniFormat);
		ini_File->sync();
		ini_File->beginGroup("SETTINGS");
		settings_form.sayHallo_flag	= ini_File->value("SETTINGS\sayHallo"	).toBool();
		settings_form.day_warning	= ini_File->value("SETTINGS\day_warning").toInt();
		settings_form.type_notif	= ini_File->value("SETTINGS\type_notif"	).toInt();
		settings_form.day_notif		= ini_File->value("SETTINGS\day_notif"	).toInt();
		int m						= ini_File->value("SETTINGS\minutes"	).toInt();
		int h						= ini_File->value("SETTINGS\hour"		).toInt();
		ini_File->endGroup();
		settings_form.time_notif	= QTime(h,m,0,0);
	}
	else
	{
		// Init condition
		hasBD			= false;
		settings_form.day_warning	= 3;
		settings_form.type_notif	= NOTIF_EVERYDAY;
		settings_form.day_notif		= 1;
		settings_form.time_notif	= QTime(0,0,0,0);
		settings_form.sayHallo_flag	= true;
	}
	
	
}
//_________________________________________________
void BD_Checker::createIni()
{
	if (settings_Changed_flag)
	{
		QSettings* ini_File = new QSettings(QDir::currentPath() + "/" + INI_FNAME, QSettings::IniFormat);
		ini_File->beginGroup("SETTINGS");
		ini_File->setValue("SETTINGS\sayHallo", 	settings_form.sayHallo_flag	);
		ini_File->setValue("SETTINGS\day_warning", 	settings_form.day_warning	);
		ini_File->setValue("SETTINGS\type_notif", 	settings_form.type_notif	);
		ini_File->setValue("SETTINGS\day_notif", 	settings_form.day_notif		);
		ini_File->setValue("SETTINGS\minutes", 		settings_form.time_notif.minute	());
		ini_File->setValue("SETTINGS\hour", 		settings_form.time_notif.hour	());
		ini_File->endGroup();
		ini_File->sync();
	}
}
//_________________________________________________

// Поиск
//_________________________________________________
void BD_Checker::apply_search(QString _txt)
{
	pers_per_index.clear();

	if (_txt == "")
	{
		vizual();
		return;
	}

	QDate			day_in_year_of_birth, next_b_date;
	QVector<QDate>	next_b_date_vect;
	QVector<int>	days_before_vect;
	int				days_before, b_size		= 0;
	search_model	= new QStandardItemModel(0,headers.size(),this);
	const QStringList c_headers				= headers;
	search_model->setHorizontalHeaderLabels(c_headers);
	QMap<QDate, Person>::iterator iter = person_data.begin();
	while(iter != person_data.end())
	{

		day_in_year_of_birth.setDate(iter->date_birth.year(), today.month(), today.day());
		if (iter->date_birth < day_in_year_of_birth)
		{
			next_b_date.setDate(today.year()+1, iter.key().month(), iter.key().day());
		}
		else
		{
			next_b_date.setDate(today.year()  , iter.key().month(), iter.key().day());
		}

		days_before = today.daysTo(next_b_date);
		
		

		if (iter->surname.contains(_txt))
		{
			days_before_vect << days_before;
			pers_per_index   << iter.value();
			next_b_date_vect << next_b_date;
			++b_size;
		}
		++iter;
	}

	// Sort by days before BD
	//____________________________________________________
	bool	cond = 0;
	int		tmp;
	QDate	tmp_dt;
	Person	tmp_pers;
	while (!cond)
	{
		cond = true;
		for (int j=1;j<days_before_vect.size();j++)
		{
			if (days_before_vect[j]<days_before_vect[j-1])
			{
				tmp = days_before_vect[j];
				days_before_vect[j] = days_before_vect[j-1];
				days_before_vect[j-1] = tmp;

				tmp_pers = pers_per_index[j];
				pers_per_index[j] = pers_per_index[j-1];
				pers_per_index[j-1] = tmp_pers;

				tmp_dt = next_b_date_vect[j];
				next_b_date_vect[j] = next_b_date_vect[j-1];
				next_b_date_vect[j-1] = tmp_dt;

				cond = 0;
			}
		}
	}
	//____________________________________________________

	// Show on table
	//____________________________________________________
	for (int i=0;i<days_before_vect.size();++i)
	{
		pers_per_index[i].age = (today.year() - pers_per_index[i].date_birth.year());	
		if (days_before_vect[i])
		{
			item = new QStandardItem(QString::number(days_before_vect[i]));
		}
		else
		{
			item = new QStandardItem("СЕГОДНЯ!");
		}
		search_model->setItem(i,0,item);

		item = new QStandardItem(get_string_dayOfWeek(next_b_date_vect[i].dayOfWeek()));
		search_model->setItem(i,1,item);

		item = new QStandardItem(pers_per_index[i].surname + " " + pers_per_index[i].name + " " + pers_per_index[i].father_name);
		search_model->setItem(i,2,item);

		item = new QStandardItem(QString::number(pers_per_index[i].age));
		search_model->setItem(i,3,item);

		item = new QStandardItem(pers_per_index[i].date_birth.toString("dd.MM"));
		search_model->setItem(i,4,item);
	}
	//____________________________________________________



	ui.text_lb		->setText	(outStr			);
	ui.tableView	->setModel	(search_model	);
	ui.tableView	->resizeRowsToContents		();
	ui.tableView	->resizeColumnsToContents	();
}
//_________________________________________________


BD_Checker::~BD_Checker()
{
	createIni();

	delete(trayIcon		)	;
	delete(trayIconMenu	)	;
	delete(minimizeAction)	;
	delete(restoreAction)	;
	delete(quitAction	)	;

}

//_________________________________________________
void BD_Checker::showDetailInfo(QModelIndex _ind)
{

	QString str =	pers_per_index.at(_ind.row()).surname	+ " ";
	QString pth =	pers_per_index.at(_ind.row()).surname	+ 
					pers_per_index.at(_ind.row()).name		+
					pers_per_index.at(_ind.row()).father_name;

	while(pth.contains(" "))
	{
		pth = pth.remove(pth.indexOf(" "),1);
	}

	pth  = "photos/" + pth + ".jpg";
	str += pers_per_index.at(_ind.row()).name			+" ";
	str += pers_per_index.at(_ind.row()).father_name	+"\n";
	str += pers_per_index.at(_ind.row()).description;

	detail_info.set_photo(pth);
	detail_info.set_info (str);
	detail_info.exec();
}
//_________________________________________________

void MyTimer::count()
{
	QTime cur_t;
	while(true)
	{
		sleep(60);
		cur_t = QDateTime::currentDateTime().time();
		qDebug()<<cur_t.toString();
		if (cur_t.hour() == noyif_time.hour() && cur_t.minute() == noyif_time.minute())
		{
			emit remind_bd();	
		}
	}
}

//_________________________________________________
void BD_Checker::remind_bd			()
{
	switch(settings_form.type_notif)
	{
	case NOTIF_EVERYDAY:
		{
			check();
			if (hasBD)
			{
				vizual();
			}
			break;
		}
	case NOTIF_EVERYWEEK:
		{
			int curr_d = QDateTime::currentDateTime().date().dayOfWeek();
			if (curr_d == settings_form.day_notif)
			{
				check();
				if (hasBD)
				{
					vizual();
				}
			}
			break;
		}
	case NOTIF_NEVER:
		{
			return;
			break;
		}

	}
	
}//_________________________________________________