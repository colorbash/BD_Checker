#include "detail_info.h"

Detail_Info::Detail_Info(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Detail_Info::~Detail_Info()
{

}

void Detail_Info::set_info(QString _str)
{
	ui.info_lb->setText(_str);
}


void Detail_Info::set_photo(QString _pth)
{
	QPixmap img(_pth);
	
	if (!img.size().isEmpty())
	{
		int x = img.width ();
		int	y = img.height();

		while(x*y > 640*300)
		{
			x /= 1.1;
			y /= 1.1;
		}

		img = img.scaled(x, y,Qt::IgnoreAspectRatio,Qt::FastTransformation);
		ui.photo_lb->setPixmap(img);
	}
	else
	{
		ui.photo_lb->setText("Нет Фото.");
	}
}