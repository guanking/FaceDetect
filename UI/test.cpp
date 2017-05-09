#include"Main.h"
#include<iostream>
using namespace std;
int main(int argc,char* argv[])
{
	QApplication a(argc,argv);
	Main view;
	view.show();
	return a.exec();
}
