#include"rtimer.h"
using namespace std;
using namespace cv;

void rTTAdaboost()
{
	cout<<"AdaBoost..."<<endl;
	VideoCapture cap(0);
	if(!cap.isOpened())
	{
		cout<<"VideoCapture is closed !"<<endl;
		return;
	}
	Mat img;
	Adaboost ada;
	while(char(waitKey(10)) != 'q')
	{
		cap>>img;
		ada.setImage(img);
		ada.detect();
		imshow("原图",img);
		imshow("AdaBoost",ada.getDrawnImg());
	}
}

void rTTPCASVM()
{
	cout<<"PCASVM..."<<endl;
	cout<<"Sorry ! No implementation ^_^"<<endl;
}

void rTTYCbCr()
{
	cout<<"YCbCr..."<<endl;
	VideoCapture cap(0);
	if(!cap.isOpened())
	{
		cout<<"VideoCapture is closed !";
		return;
	}
	Mat img;
	YCbCr fd(img);
	while(char(waitKey(10)) != 'q')
	{
		cap>>img;
		imshow("原图",img);
		fd.setImg(img);
		fd.init();
		fd.detect();
		imshow("YCbCr",fd.getDrawnImg());
	}
}

#define TEST
#ifdef TEST
//#define DEBUG
int main()
{
	int choice;
	cout<<"算法选择："<<endl;
	cout<<"\t1、AdaBoost;"<<endl;
	cout<<"\t2、PCASVM;"<<endl;
	cout<<"\t3、YCbCr;"<<endl;
	cout<<"\t0、Quit！"<<endl;
	cout<<"选择：";
	cin>>choice;
	switch(choice)
	{
		case 1:
			rTTAdaboost();
			break;
		case 2:
			rTTPCASVM();
			break;
		case 3:
			rTTYCbCr();
			break;
		default:
			break;
	}
	return 0;
}
#endif//TEST
