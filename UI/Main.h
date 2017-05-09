#ifndef MAIN_H
#define MAIN_H
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
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include"ui_FaceDetactUI.h"
class Main:public QMainWindow
{
	Q_OBJECT
private:
	Ui_MainWindow ui;
public:
	Main(QMainWindow *parent = NULL);
	~Main();
};
#endif
