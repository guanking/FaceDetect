#include "Main.h"
#include <iostream> 
#include <QtGui/QFileDialog>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

using namespace std;

#define DEBUG_UI

Main::Main(QMainWindow *mainView ):QMainWindow(mainView)
{
	this->ui.setupUi(this);
	this->ui.retranslateUi(this);
}

void Main::on_aStartBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_aStartBtn_clicked"<<endl;
#endif
	if(this->ui.aRealtimeRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"\trealtimeRtb is checked"<<endl;
#endif
	}
	else
	{
#ifdef DEBUG_UI
		cout<<"\tfileRtb is checked"<<endl;
#endif
	}
}


void Main::on_aRealtimeRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_aRealtimeRtb_clicked"<<endl;
#endif
	this->ui.aImgPathText->setEnabled(false);
	this->ui.aChoiceFileBtn->setEnabled(false);
}

void Main::on_aFileRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_aFileRtb_clicked"<<endl;
#endif
	this->ui.aImgPathText->setEnabled(true);
	this->ui.aChoiceFileBtn->setEnabled(true);
}

void Main::on_aChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_aChoiceFileBtn_clicked"<<endl;
#endif
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",  tr("Images (*.png *.xpm *.jpg)"));  
	this->ui.aImgPathText->setText(fileName);
}

Main::~Main()
{
}
