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
#include"ui_FaceDetactUI.h"
class Main:public QMainWindow
{
	Q_OBJECT
private:
	Ui_MainWindow ui;
public:
	Main(QMainWindow *parent = NULL);
	~Main();
public slots:
	void on_aStartBtn_clicked();
	void on_aRealtimeRtb_clicked();
	void on_aFileRtb_clicked();
	void on_aChoiceFileBtn_clicked();
};
#endif
