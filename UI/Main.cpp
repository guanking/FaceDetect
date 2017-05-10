#include <iostream> 
#include <QtGui/QFileDialog>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include "Main.h"

using namespace std;
using namespace cv;

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
	Adaboost ada;
	Mat img;
	if(this->ui.aRealtimeRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"\trealtimeRtb is checked"<<endl;
#endif
		VideoCapture cap(0);
		if(!cap.isOpened())
		{
			QMessageBox::information(this, tr("Error"), tr("VideoCapture is not opened !"));  
			return;
		}
		while(this->ui.aRealtimeRtb->isChecked())
		{
#ifdef DEBUG_UI
			cout<<"aRealtimeRtb is checked : "<<this->ui.aRealtimeRtb->isChecked()<<endl;
#endif//DEBUG_UI
			cap>>img;
			ada.setImage(img);
			ada.detect();
			this->mat2Label(img,this->ui.aSrcImgLabel);
			this->mat2Label(ada.getDrawnImg(),this->ui.aDstImgLabel);
			waitKey(100);
		}
		cap.release();
	}
	else
	{
#ifdef DEBUG_UI
		cout<<"\tfileRtb is checked"<<endl;
		cout<<"ImgPath = "<<this->ui.aImgPathText->text().data()<<endl;
#endif
		QString qImgPath = this->ui.aImgPathText->text();
		string imgPath = qImgPath.toStdString();
		if(imgPath.empty())
		{
			QMessageBox::information(this, tr("Error"), tr("Please input image path !"));  
			return;
		}
		img = imread(imgPath.c_str());
		ada.setImage(img);
		ada.detect();
#ifdef DEBUG_UI
		imshow("image",img);
#endif//DEBUG_UI
		this->mat2Label(img,this->ui.aSrcImgLabel);
		this->mat2Label(ada.getDrawnImg(),this->ui.aDstImgLabel);
		this->mat2Label(ada.getDrawnImg(),this->ui.aDstImgLabel);
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
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/guanjiecao/study/opencv-3.2.0/samples/data",  tr("Images (*.png *.xpm *.jpg)"));  
	this->ui.aImgPathText->setText(fileName);
}

void Main::mat2Label(Mat image,QLabel* label)
{
	cvtColor(image,image,CV_BGR2RGB);
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows, image.cols*image.channels(),  QImage::Format_RGB888);  
	label->clear();
	label->setPixmap(QPixmap::fromImage(img));
}

Main::~Main()
{
}
