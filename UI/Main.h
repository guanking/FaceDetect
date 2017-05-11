#ifndef MAIN_H
#define MAIN_H

#include <Qt3Support/Q3ComboBox>
#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3TextEdit>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
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
#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <string>
#include <map>
#include "ui_FaceDetactUI.h"
#include "../Adaboost.h"
#include "../PCASVM.h"
#include "../YCbCr.h"

using namespace std;
using namespace cv;

class Main:public QMainWindow
{
	Q_OBJECT
private:
	Ui_MainWindow ui;
	PCASVM* ps;
	map<string,int> SVMKernelTypes;
	void mat2Label(Mat mat,QLabel* label);
	void dealYCbCrPro(YCbCr& fd,const Mat& img);
public:
	Main(QMainWindow *parent = NULL);
	~Main();
public slots:
	/**
	 *Adaboost算法对应监听借口实现
	 */
	void on_aStartBtn_clicked();
	void on_aRealtimeRtb_clicked();
	void on_aFileRtb_clicked();
	void on_aChoiceFileBtn_clicked();

	/**
	 *PCASVM算法对应监听接口实现
	 */
	void on_pStartTrain_clicked();
	void on_pStartTest_clicked();
	void on_pTestFaceChoiceFileBtn_clicked();
	void on_pTestNotfaceChoiceFileBtn_clicked();
	void on_pTrainFaceChoiceFileBtn_clicked();
	void on_pTrainNotfaceChoiceFileBtn_clicked();
	/**
	 *YCbCr算法对应监听接口实现
	 */
	void on_yStartBtn_clicked();
	void on_yRealtimeRtb_clicked();
	void on_yFileRtb_clicked();
	void on_yChoiceFileBtn_clicked();
};
#endif
