#include"Main.h"
#include<iostream> 
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
using namespace std;

Main::Main(QMainWindow *mainView ):QMainWindow(mainView)
{
	this->ui.setupUi(this);
	this->ui.retranslateUi(this);
}

Main::~Main()
{
}
