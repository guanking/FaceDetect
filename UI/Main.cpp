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
	this->fd = NULL;
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
			this->ada.setImage(img);
			this->ada.detect();
			this->mat2Label(img,this->ui.aSrcImgLabel);
			this->mat2Label(this->ada.getDrawnImg(),this->ui.aDstImgLabel);
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
		this->ada.setImage(img);
		this->ada.detect();
		this->mat2Label(img,this->ui.aSrcImgLabel);
		this->mat2Label(this->ada.getDrawnImg(),this->ui.aDstImgLabel);
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
	char str[256];
	sprintf(str,"Face Path = %s\nfrom %d to %d\nNotfacePath = %s\nfrom %d to %d\n \
			PCA component is %.2lf%%\nSVM kernel type is %s\n",
			facePath.c_str(),faceBegin,faceEnd,notfacePath.c_str(),notfaceBegin,notfaceEnd,
			pcaPercent/100.0,kernelType.c_str());
	this->ui.yMsgText->setText(tr(str));
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
	qstr.append("\nfrom ");
	qstr.append(str.c_str());
	sStream<<faceBegin;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append(" to ");
	sStream.clear();
	sStream<<faceEnd;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nnotfacePath = ");
	qstr.append(notfacePath.c_str());
	qstr.append("\nfrom ");
	sStream.clear();
	sStream<<notfaceBegin;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append(" to ");
	sStream.clear();
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
	sStream.clear();
	sStream<<(faceEnd - faceBegin +1);
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nCorrectJudgeFace = ");
	sStream.clear();
	sStream<<truePos;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nErrorJudgeFace = ");
	sStream.clear();
	sStream<<falsePos;
	sStream>>str;
	qstr.append(str.c_str());
	
	
	qstr.append("\ntotalnotfaceNum = ");
	sStream.clear();
	sStream<<(notfaceEnd - notfaceBegin +1);
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nCorrectJudgeNotface = ");
	sStream.clear();
	sStream<<trueNeg;
	sStream>>str;
	qstr.append(str.c_str());
	qstr.append("\nErrorJudgeNotface = ");
	sStream.clear();
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
	if(this->fd == NULL)
		this->fd = new YCbCr(img);
	this->fd->setUseMorphOpen(false);
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
			this->dealYCbCrPro(img);
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
		this->dealYCbCrPro(img);
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

void Main::dealYCbCrPro(const Mat& img)
{
	this->fd->setImg(img);
	this->mat2Label(img,this->ui.ySrcImgLabel);
	this->fd->init();
	this->fd->initOrdinarySkin();
	this->mat2Label(this->fd->getCacheImg(),this->ui.ySkinImgLabel);
	this->fd->morph();
	this->mat2Label(this->fd->getCacheImg(),this->ui.yMorphImgLabel);
	this->fd->splitArea();
	this->mat2Label(this->fd->getDrawnImg(),this->ui.ySplitAreaImgLabel);
	this->fd->selectLegal();
	this->mat2Label(this->fd->getDrawnImg(),this->ui.yDstImgLabel);
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
	int notfaceEnd = this->ui.pTrainNotfaceEndBox_2->value();
	if(faceDir.length() == 0)
	{
		QMessageBox::information(this, tr("Error"), tr("Please choice face directory !")); 
		return;
	}
	if(notfaceDir.length() == 0)
	{
		QMessageBox::information(this, tr("Error"), tr("Please choice notface directory !")); 
		return ;
	}
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
	this->ui.cFaceNumText->setText(QString::fromStdString(str));
	sStream.clear();
	sStream<<notfaceNum;
	sStream>>str;
	this->ui.cNotfaceNum->setText(QString::fromStdString(str));
	int64 timerBegin = getTickCount();//timer
	if(this->ui.cAdaBoostRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"AdaBoost is choiced !"<<endl;
#endif
		this->compareDetect(faceDir,faceBegin,faceEnd,notfaceDir,notfaceBegin,notfaceEnd,&Main::compareAdaBoost);
	}else if(this->ui.cPCASVMRtb->isChecked())
	{
#ifdef DEBUG_UI
		cout<<"PCASVM is choiced !"<<endl;
#endif
		if(this->ps == NULL)
		{
			QMessageBox::information(this, tr("Error"), tr("Please train before test !")); 
			return;
		}
		this->compareDetect(faceDir,faceBegin,faceEnd,notfaceDir,notfaceBegin,notfaceEnd,&Main::comparePCASVM);
	}else
	{
#ifdef DEBUG_UI
		cout<<"YCrCb is choiced !"<<endl;
#endif
		if(this->fd == NULL)
		{
			this->fd = new YCbCr(Mat());
			this->fd->setUseMorphOpen(false);
		}
		this->compareDetect(faceDir,faceBegin,faceEnd,notfaceDir,notfaceBegin,notfaceEnd,&Main::compareYCrCb);
	}
	int64 timerEnd = getTickCount();
	char timerStr[256];
	sprintf(timerStr,"%s\n\t%.2f ms","Total Time:",1000 * (timerEnd - timerBegin) / getTickFrequency());
	QMessageBox::information(this, tr("Finish"), tr(timerStr)); 
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

void Main::compareDetect(const string& faceDir,const int& faceBegin,const int& faceEnd,const string& notfaceDir,const int& notfaceBegin,const int& notfaceEnd,void (Main::*detectPro)(Mat& src,int& cnt))
{
	int truePos = 0;
	int falseNeg = 0;
	char path[256];
	Mat img;
	for(int i = faceBegin;i <= faceEnd; i++)
	{
		sprintf(path, "%s%d.png",faceDir.c_str(),i);
		img = imread(path);
		if(img.empty())
		{
			sprintf(path, "%s%d.jpg",faceDir.c_str(),i);
			img = imread(path);
		}
		if(img.empty())
		{
			QMessageBox::information(this, tr("Error"), tr(path)); 
			return;
		}
#ifdef DEBUG_UI
		cout<<path;
#endif//DEBUG_UI
		(this->*detectPro)(img,truePos);
	}
	for(int  i = notfaceBegin;i <= notfaceEnd; i++)
	{
		sprintf(path,"%s%d.png",notfaceDir.c_str(),i);
		img = imread(path);
		if(img.empty())
		{
			sprintf(path,"%s%d.jpg",notfaceDir.c_str(),i);
			img = imread(path);
		}
		if(img.empty())
		{
			QMessageBox::information(this, tr("Error"), tr(path)); 
			return;
		}
#ifdef DEBUG_UI
		cout<<path;
#endif//DEBUG_UI
		(this->*detectPro)(img,falseNeg);
	}
	int faceNum = faceEnd - faceBegin + 1;
	int notfaceNum = notfaceEnd - notfaceBegin + 1;
	string str;
	stringstream sStream;
	sStream<<faceNum + notfaceNum;
	sStream>>str;
	this->ui.cResultTable->item(0,0)->setText(QString::fromStdString(str));
	sStream.clear();
	sStream<<truePos;
	sStream>>str;
	this->ui.cResultTable->item(0,1)->setText(QString::fromStdString(str));
	sStream.clear();
	sStream<<falseNeg;
	sStream>>str;
	this->ui.cResultTable->item(0,2)->setText(QString::fromStdString(str));
	char preStr[32];
	sprintf(preStr,"%.2f%%",truePos * 100.0 / faceNum);
	this->ui.cResultTable->item(0,3)->setText(QString::fromStdString(preStr));
	sprintf(preStr,"%.2f%%",(notfaceNum - falseNeg) * 100.0 / notfaceNum);
	this->ui.cResultTable->item(0,4)->setText(QString::fromStdString(preStr));
	sprintf(preStr,"%.2f%%",(faceNum - truePos) * 100.0 / faceNum);
	this->ui.cResultTable->item(0,5)->setText(QString::fromStdString(preStr));
	sprintf(preStr,"%.2f%%",falseNeg * 100.0 / notfaceNum);
	this->ui.cResultTable->item(0,6)->setText(QString::fromStdString(preStr));
}

void Main::compareAdaBoost(Mat& src,int& cnt)
{
		waitKey(10);
		this->ada.setImage(src);
		this->ada.detect();
		this->mat2Label(src,this->ui.cSrcImgLabel);
		this->mat2Label(this->ada.getDrawnImg(),this->ui.cDstImgLabel);
		if(this->ada.hasFace())
		{
			cnt++;
		}
#ifdef DEBUG_UI
			string debug_str = this->ada.hasFace()?" is face !":" isn't face !";
			cout<<debug_str<<endl;
#endif//DEBUG_UI
}

void Main::comparePCASVM(Mat& src,int& cnt)
{
	waitKey(10);
	this->mat2Label(src,this->ui.cSrcImgLabel);
	if(src.channels() != 1)
	{
#ifdef DEBUG_UI
		cout<<" translate to gray image ";
#endif//DEBUG_UI
		cvtColor(src,src,CV_BGR2GRAY);
	}
	this->mat2Label(src,this->ui.cDstImgLabel);
	if(this->ps->isFace(src))
	{
		cnt++;
	}
#ifdef DEBUG_UI
			string debug_str = this->ps->isFace(src)?" is face !":" isn't face !";
			cout<<debug_str<<endl;
#endif//DEBUG_UI
}
void Main::compareYCrCb(Mat& src,int& cnt)
{
	waitKey(10);
	this->fd->setImg(src);
	this->mat2Label(src,this->ui.cSrcImgLabel);
	this->fd->init();
	this->fd->detect();
	this->mat2Label(this->fd->getDrawnImg(),this->ui.cDstImgLabel);
	if(this->fd->isFace())
	{
		cnt++;
	}
#ifdef DEBUG_UI
			string debug_str = this->fd->isFace()?" is face !":" isn't face !";
			cout<<debug_str<<endl;
#endif//DEBUG_UI
}

Main::~Main()
{
	if(this->ps != NULL)
	{
		delete this->ps;
		this->ps = NULL;
	}
	if(this->fd != NULL)
	{
		delete this->fd;
		this->fd = NULL;
	}
}
