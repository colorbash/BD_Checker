#ifndef DETAIL_INFO_H
#define DETAIL_INFO_H

#include <QDialog>
#include "ui_detail_info.h"
//______________________________________________________
class Detail_Info : public QDialog
{
	Q_OBJECT

public:
	 Detail_Info(QWidget *parent = 0);
	~Detail_Info();

	void set_info (QString _str);	// ��������� �������
	void set_photo(QString _pth);	// ��������� ����
private:
	Ui::Detail_Info ui;
};
//______________________________________________________

#endif // DETAIL_INFO_H
