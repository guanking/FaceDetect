#include  <iostream>
#include <exception>
#include <sstream>
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
	this->ps = NULL;
	this->SVMKernelTypes.insert(pair<string,int>("LINEAR",SVM::LINEAR));
	this->SVMKernelTypes.insert(pair<string,int>("POLY",SVM::POLY));
	this->SVMKernelTypes.insert(pair<string,int>("RBF",SVM::RBF));
	this->SVMKernelTypes.insert(pair<string,int>("SIGMOID",SVM::SIGMOID));
	this->SVMKernelTypes.insert(pair<string,int>("CHI2",SVM::CHI2));
	this->SVMKernelTypes.insert(pair<string,int>("INTER",SVM::INTER));
	for(map<string,int>::iterator ele = this->SVMKernelTypes.begin(); ele != this->SVMKernelTypes.end();ele++)
	{
		this->ui.ySVMCmpBox->addItem(QString::fromStdString(ele->first));
	}
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
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "./img",  tr("Images (*.png *.xpm *.jpg)"));  
	this->ui.aImgPathText->setText(fileName);
	Mat img = imread(fileName.toStdString().c_str());
	if(img.empty())
	{
		QMessageBox::information(this, tr("Error"), tr("The image you have choiced is in a wrong format !"));
		return;
	}
	else
	{
		this->mat2Label(img,this->ui.aSrcImgLabel);
	}
}

void Main::on_pStartTrain_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_pStartTrain_clicked"<<endl;
#endif
	string facePath = this->ui.pTrainFaceImgPathText->text().toStdString();
	string notfacePath = this->ui.pTrainNotfaceImgPathText->text().toStdString();
	int faceBegin = this->ui.pTrainFaceBeginBox->value();
	int faceEnd = this->ui.pTrainFaceEndBox->value();
	int notfaceBegin = this->ui.pTrainNotfaceBeginBox->value();
	int notfaceEnd = this->ui.pTrainNotfaceEndBox->value();
	int pcaPercent = this->ui.pCptRateBox->value();	
	string kernelType = this->ui.ySVMCmpBox->currentText().toStdString();

	if(facePath.find_last_of('/') != facePath.length() - 1)
	{
		facePath.append("/");
	}
	if(notfacePath.find_last_of('/') != notfacePath.length())
	{
		notfacePath.append("/");
	}
	string str = "Face Path = ";
	str.append(facePath);
	str += "; from ";
	this->ui.yMsgText->setText(QString::fromStdString(str));
	stringstream sStream;
	sStream<<faceBegin;
	sStream>>str;
	str += "to";
	this->ui.yMsgText->append(QString::fromStdString(str));
	sStream<<faceEnd;
	sStream>>str;
	str.append("\nnotfacePath = ");
	str.append(notfacePath);
	str.append("; from ");
	this->ui.yMsgText->append(QString::fromStdString(str));
	sStream<<notfaceBegin;
	sStream>>str;
	str.append("to");
	this->ui.yMsgText->append(QString::fromStdString(str));
	sStream<<notfaceEnd;
	sStream>>str;
	str.append("\nPCA component is ");
	this->ui.yMsgText->append(QString::fromStdString(str));
	sStream<<pcaPercent/100.0;
	sStream>>str;
	str.append("%\nSVM kernel type is ");
	str.append(kernelType);
	this->ui.yMsgText->append(QString::fromStdString(str));
#ifdef DEBUG_UI
	cout<<"\tfacePath = "<<facePath<<" from "<<faceBegin<<" to "<<faceEnd<<endl;
	cout<<"\tnotfacePath = "<<notfacePath<<" from "<<notfaceBegin<<" to "<<notfaceEnd<<endl;
	cout<<"\tPCA component percent is "<<pcaPercent<<"%"<<endl;
	cout<<"\tSVM kernel type is "<<kernelType<<endl;
#endif//DEBUG_UI
	if(this->ps != NULL)
		delete this->ps;
	this->ps = new PCASVM(facePath,faceBegin,faceEnd,notfacePath,notfaceBegin,notfaceEnd);
	if(this->ps == NULL)
	{
		QMessageBox::information(this, tr("Error"), tr("Malloc space failed!"));  
		return;
	}
	this->ps->setCptPercent(pcaPercent/100.0);
	map<string,int>::iterator ele = this->SVMKernelTypes.find(kernelType);
	if(ele == this->SVMKernelTypes.end())
	{
		QMessageBox::information(this, QString::fromStdString(kernelType), tr("Unknow Kernel type !")); 
		return;
	}
	this->ps->setSVMKernelType(ele->second);
	this->ui.yMsgText->append(tr("training......"));
	try
	{
		this->ps->init();
	}
	catch(exception& e)
	{
		QMessageBox::information(this, tr("Error"), QString::fromStdString(string(e.what()))); 
	}
	this->ui.yMsgText->append(tr("training finished !"));
}

void Main::on_pStartTest_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_pStartTest_clicked"<<endl;
#endif
	string facePath = this->ui.pTestFaceImgPathText->text().toStdString();
	string notfacePath = this->ui.pTestNotfaceImgPathText->text().toStdString();
	int faceBegin = this->ui.pTestFaceBeginBox->value();
	int faceEnd = this->ui.pTestFaceEndBox->value();
	int notfaceBegin = this->ui.pTestNotfaceBeginBox->value();
	int notfaceEnd = this->ui.pTestNotfaceEndBox->value();

	if(facePath.find_last_of('/') != facePath.length() - 1)
	{
		facePath.append("/");
	}
	if(notfacePath.find_last_of('/') != notfacePath.length())
	{
		notfacePath.append("/");
	}
#ifdef DEBUG_UI
	cout<<"\tfacePath = "<<facePath<<" from "<<faceBegin<<" to "<<faceEnd<<endl;
	cout<<"\tnotfacePath = "<<notfacePath<<" from "<<notfaceBegin<<" to "<<notfaceEnd<<endl;
#endif//DEBUG_UI
	stringstream sStream;
	string str;
	QString qstr = tr("facePath = ");
	qstr.append(facePath.c_str());
	qstr.append(";from ");
	qstr.append(str.c_str());
	sStream<<faceBegin;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append(" to ");
	sStream<<faceEnd;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nnotfacePath = ");
	qstr.append(notfacePath.c_str());
	qstr.append(";from ");
	sStream<<notfaceBegin;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append(" to ");
	sStream<<notfaceEnd;
	sStream>>str;
	qstr.append(str.c_str());
	this->ui.yMsgText->append(qstr);
	if(this->ps == NULL)
	{
		QMessageBox::information(this, tr("Error"), tr("Please train before test !")); 
		return;
	}
	Mat tImg;
	char fileName[126];
	int truePos = 0,falsePos = 0;
	int trueNeg = 0,falseNeg = 0;
	for(int i = faceBegin;i <= faceEnd;i++)
	{
		sprintf(fileName,"%s%d.png",facePath.c_str(),i);
		tImg = imread(fileName,IMREAD_GRAYSCALE);
		if(this->ps->isFace(tImg))
		{
			truePos++;
		}
		else
		{
			falsePos++;
		}
	}
	for(int i = notfaceBegin;i <= notfaceEnd;i++)
	{
		sprintf(fileName,"%s%d.png",notfacePath.c_str(),i);
		tImg = imread(fileName,IMREAD_GRAYSCALE);
		if(this->ps->isFace(tImg))
		{
			falseNeg++;
		}
		else
		{
			trueNeg++;
		}
	}
	qstr.clear();
	qstr.append("totalFaceNum = ");
	sStream<<(faceEnd - faceBegin +1);
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nCorrectJudgeFace = ");
	sStream<<truePos;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nErrorJudgeFace = ");
	sStream<<falsePos;
	sStream>>str;
	qstr.append(str.c_str());
	
	
	qstr.append("totalnotfaceNum = ");
	sStream<<(notfaceEnd - notfaceBegin +1);
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nCorrectJudgeNotface = ");
	sStream<<trueNeg;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nErrorJudgeNotface = ");
	sStream<<falseNeg;
	sStream>>str;
	qstr.append(str.c_str());
	this->ui.yMsgText->append(qstr);
#ifdef DEBUG_UI
	cout<<"totalFace = "<<faceEnd - faceBegin + 1<<endl;
	cout<<"correctJudgeFace = "<<truePos<<endl;
	cout<<"errorJudgeFace = "<<falsePos<<endl;
	cout<<"totalNotface = "<<notfaceEnd - notfaceBegin + 1<<endl;
	cout<<"correctJUdgeNotface = "<<trueNeg<<endl;
	cout<<"errorJudgeNotface = "<<falseNeg<<endl;
#endif//DEBUG_UI
}

void Main::on_pTestFaceChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_pTestFaceChoiceFileBtn_clicked"<<endl;
#endif
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open File"), "./img");  
	this->ui.pTestFaceImgPathText->setText(fileName);
	
}

void Main::on_pTestNotfaceChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_pTestNotFaceChoiceFileBtn_clicked"<<endl;
#endif
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open File"), "./img");  
	this->ui.pTestNotfaceImgPathText->setText(fileName);
}

void Main::on_pTrainFaceChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_pTrainFaceChoiceFileBtn_clicked"<<endl;
#endif
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open File"), "./img");  
	this->ui.pTrainFaceImgPathText->setText(fileName);
}

void Main::on_pTrainNotfaceChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_pTrainNotfaceChoiceFileBtn_clicked"<<endl;
#endif
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open File"), "./img");  
	this->ui.pTrainNotfaceImgPathText->setText(fileName);
}

void Main::on_yStartBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_yStartBtn_clicked"<<endl;
#endif//DEBUG_UI
	Mat img;
	YCbCr fd(img);
	fd.setUseMorphOpen(false);
	if(this->ui.yRealtimeRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"yRealtimeRtb is checked"<<endl;
#endif//DEBUG_UI
		VideoCapture cap(0);
		if(!cap.isOpened())
		{
			QMessageBox::information(this, tr("Error"), tr("VideoCapture is closed !"));  
			return;
		}
		while(char(waitKey(10)) != 'q' && this->ui.yRealtimeRtb->isChecked())
		{
#ifdef DEBUG_UI
			cout<<"yRealtimeRtb is checked : "<<this->ui.yRealtimeRtb->isChecked()<<endl;
#endif//DEBUG_UI
			cap>>img;
			this->dealYCbCrPro(fd,img);
		}
		cap.release();
	}
	else
	{
		QString qImgPath = this->ui.yImgPathText->text();
		string imgPath = qImgPath.toStdString();
#ifdef DEBUG_UI
		cout<<"\tfileRtb is checked"<<endl;
		cout<<"ImgPath = "<<imgPath<<endl;
#endif
		if(imgPath.empty())
		{
			QMessageBox::information(this, tr("Error"), tr("Please input image path !"));  
			return;
		}
		img = imread(imgPath.c_str());
		this->dealYCbCrPro(fd,img);
	}
}

void Main::on_yRealtimeRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_yRealtimeRtb_clicked"<<endl;
#endif//DEBUG_UI
	this->ui.yImgPathText->setEnabled(false);
	this->ui.yChoiceFileBtn->setEnabled(false);
}

void Main::on_yFileRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_yFileRtb_clicked"<<endl;
#endif//DEBUG_UI
	this->ui.yImgPathText->setEnabled(true);
	this->ui.yChoiceFileBtn->setEnabled(true);
}

void Main::on_yChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_yChoiceFileBtn_clicked"<<endl;
#endif//DEBUG_UI
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "./img",  tr("Images (*.png *.xpm *.jpg)"));  
	this->ui.yImgPathText->setText(fileName);
	Mat img = imread(fileName.toStdString().c_str());
	if(img.empty())
	{
		QMessageBox::information(this, tr("Error"), tr("The image you have choiced is in a wrong format !"));
		return;
	}
	else
	{
		this->mat2Label(img,this->ui.ySrcImgLabel);
	}
}

void Main::mat2Label(Mat image,QLabel* label)
{
	if(image.channels() == 3 || image.channels() == 4)
	{
		cvtColor(image,image,CV_BGR2RGB);
	}
	else
	{
		cvtColor(image,image,CV_GRAY2RGB);
	}
    QImage img = QImage((const unsigned char*)(image.data),image.cols,image.rows, image.cols*image.channels(),  QImage::Format_RGB888);  
	label->clear();
	label->setPixmap(QPixmap::fromImage(img));
}

void Main::dealYCbCrPro(YCbCr& fd,const Mat& img)
{
	fd.setImg(img);
	this->mat2Label(img,this->ui.ySrcImgLabel);
	fd.init();
	fd.initOrdinarySkin();
	this->mat2Label(fd.getCacheImg(),this->ui.ySkinImgLabel);
	fd.morph();
	this->mat2Label(fd.getCacheImg(),this->ui.yMorphImgLabel);
	fd.splitArea();
	this->mat2Label(fd.getDrawnImg(),this->ui.ySplitAreaImgLabel);
	fd.selectLegal();
	this->mat2Label(fd.getDrawnImg(),this->ui.yDstImgLabel);
}

void Main::on_cFaceChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_cFaceChoiceFileBtn_clicked"<<endl;
#endif//DEBUG_UI
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open File"), "./img");
	this->ui.cFaceImgPathText->setText(fileName);
}

void Main::on_cNotfaceChoiceFileBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_cNotfaceChoiceFileBtn_clicked"<<endl;
#endif//DEBUG_UI
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Open File"), "./img");
	this->ui.cNotfaceImgPathText->setText(fileName);
}

void Main::on_cStartBtn_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_cStartBtn_clicked"<<endl;
#endif//DEBUG_UI
	String faceDir = this->ui.cFaceImgPathText->text().toStdString();
	String notfaceDir = this->ui.cNotfaceImgPathText->text().toStdString();
	int faceBegin = this->ui.cFaceBeginBox->value();
	int faceEnd = this->ui.cFaceEndBox->value();
	int notfaceBegin = this->ui.cNotfaceBeginBox->value();
	int notfaceEnd = this->ui.cNotfaceEndBox->value();
	if(faceDir.find_last_of("/") != faceDir.length())
	{
		faceDir += "/";
	}
	if(notfaceDir.find_last_of("/") != notfaceDir.length())
	{
		notfaceDir += "/";
	}
#ifdef DEBUG_UI
	cout<<"faceDir = "<<faceDir<<";from "<<faceBegin<<" to "<<faceEnd<<endl;
	cout<<"notfaceDir = "<<notfaceDir<<";from "<<notfaceBegin<<" to "<<notfaceEnd<<endl;
#endif
	int faceNum = faceEnd - faceBegin + 1;
	int notfaceNum = notfaceEnd - notfaceBegin + 1;
	stringstream sStream;
	string str;
	sStream<<faceNum;
	sStream>>str;
	this->ui.cFaceNum->setText(QString::fromStdString(str));
	sStream<<notfaceNum;
	sStream>>str;
	this->ui.cNotfaceNum->setText(QString::fromStdString(str);
	if(this->ui.cAdaBoostRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"AdaBoost is choiced !"<<endl;
#endif
	}else if(this->ui.cPCASVMRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"PCASVM is choiced !"<<endl;
#endif
	}else
	{
#ifdef DEBUG_UI
		cout<<"YCrCb is choiced !"<<endl;
#endif
	}
}

void Main::on_cAdaBoostRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_cAdaBoostRtb_clicked"<<endl;
#endif//DEBUG_UI
	this->ui.cAlgorithm->setText(tr("AdaBoost"));
}

void Main::on_cPCASVMRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_cPCASVMRtb_clicked"<<endl;
#endif//DEBUG_UI
	this->ui.cAlgorithm->setText(tr("PCASVM"));
}

void Main::on_cYCrCbRtb_clicked()
{
#ifdef DEBUG_UI
	cout<<"on_cYCrCbRtb_clicked"<<endl;
#endif//DEBUG_UI
	this->ui.cAlgorithm->setText(tr("YCrCb"));
}

Main::~Main()
{
	if(this->ps != NULL)
	{
		delete this->ps;
		this->ps = NULL;
	}
}
