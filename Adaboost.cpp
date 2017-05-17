#include"Adaboost.h"
#include<iostream>
#include<string>
#include<string.h>
#include<opencv2/objdetect.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
#include<map>

using namespace std;
using namespace cv;

Adaboost::Adaboost():changeSrc(false)
{
	this->init();
}

Adaboost::Adaboost(string path):changeSrc(false)
{
	this->init();
	this->image = imread(path);
}

Adaboost::Adaboost(Mat image):changeSrc(false)
{
	this->init();
	this->image = image.clone();
}

bool Adaboost::hasFace()
{
	return !this->rects.empty();
}

Mat Adaboost::getImage()
{
	return this->image;
}

void Adaboost::setImage(Mat image)
{
	this->image = image.clone();
}

void Adaboost::setChangeSrcImage(bool change)
{
	this->changeSrc = change;
}

vector<Rect> Adaboost::getImageRects()
{
	return this->rects;
}

bool Adaboost::getChangeSrcImage()
{
	return this->changeSrc;
}

Mat Adaboost::getDrawnImg()
{
	Mat tImg = this->image.clone();
#ifdef DEBUG
	imshow("Before Drawing",tImg);
#endif//DEBUG
	if(tImg.rows < 100 && tImg.cols < 100)
	{
		resize( tImg, tImg, Size(100,100), 0, 0, INTER_LINEAR );
	}
	
	for(vector<Rect>::iterator ele = this->rects.begin();ele != this->rects.end();ele++)
	{
		rectangle(tImg,cvPoint(cvRound(ele->x),cvRound(ele->y)),
				cvPoint(cvRound(ele->x + ele->width),cvRound(ele->y + ele->height)),
				Scalar(255,128,0),3,8,0);
	}
#ifdef DEBUG
	imshow("Adaboost getDrawnImg",tImg);
	while(char(waitKey(10)) != 'q');
#endif//DEBUG
	return tImg;
}

void Adaboost::setCascade(string path)
{
	this->cascade.load(path);
}

void Adaboost::detect()
{
	this->rects.clear();
	if(this->image.empty())
	{
		return;
	}
	Mat grayImage;
	cvtColor(this->image, grayImage,COLOR_BGR2GRAY);
#ifdef DEBUG
	imshow("gray image",grayImage);
#endif//DEBUG
	if(grayImage.rows < 100 && grayImage.cols < 100)
	{
#ifdef DEBUG
		cout<<"Image is too small to detect ! Resize it to 100X100 !"<<endl;
#endif//DEBUG
		resize( grayImage, grayImage, Size(100,100), 0, 0, INTER_LINEAR );
	}
	equalizeHist(grayImage, grayImage);
	cascade.detectMultiScale( grayImage, this->rects);//,1.1, 2, 0|CASCADE_SCALE_IMAGE,Size(50, 50) );
#ifdef DEBUG
	cout<<"\nAdaboost face areas:"<<endl;
	for(vector<Rect>::iterator ele = this->rects.begin(); ele != this->rects.end();ele++)
	{
		cout<<"\t"<<*ele<<endl;
	}
#endif//DEBUG
	if(this->changeSrc)
	{
		this->image = grayImage;
	}
}

void Adaboost::init()
{
	this->cascade.load("./cascade/haarcascade_frontalface_alt.xml");
	while(!rects.empty())
	{
		rects.pop_back();
	}
}

void Adaboost::show()
{
	for ( size_t i = 0; i < this->rects.size(); i++ )
	{
		Rect r = this->rects[i];
		rectangle( this->image, cvPoint(cvRound(r.x), cvRound(r.y)),
			cvPoint(cvRound(r.x + r.width-1), cvRound(r.y + r.height)),
			 Scalar(255,128,0), 3, 8, 0);
	}
	imshow( "image", this->image );
	char c = 'q';
	do
	{
		c = (char)waitKey(10);
	}
	while(c != 'q' && c != 'Q' );
}

string Adaboost::info()
{
	string str="";
	cout<<"Image info:"<<endl;
	cout<<"\twidth = "<<this->image.size().width<<endl;
	cout<<"\theight = "<<this->image.size().height<<endl;
	cout<<"\tchannels = "<<this->image.channels()<<endl;
	cout<<"\ttype = "<<this->imgType()<<endl;
	return str;
}

string Adaboost::imgType()
{
	map<int,string> typeMaps;
	typeMaps[ CV_8UC1 ]="CV_8UC1";
	typeMaps[ CV_8UC2 ]="CV_8UC2";
	typeMaps[ CV_8UC3 ]="CV_8UC3";
	typeMaps[ CV_8UC4 ]="CV_8UC4";

	typeMaps[ CV_8SC1 ]="CV_8SC1";
	typeMaps[ CV_8SC2 ]="CV_8SC2";
	typeMaps[ CV_8SC3 ]="CV_8SC3";
	typeMaps[ CV_8SC4 ]="CV_8SC4";

	typeMaps[ CV_16UC1 ]="CV_16UC1";
	typeMaps[ CV_16UC2 ]="CV_16UC2";
	typeMaps[ CV_16UC3 ]="CV_16UC3";
	typeMaps[ CV_16UC4 ]="CV_16UC4";

	typeMaps[ CV_16SC1 ]="CV_16SC1";
	typeMaps[ CV_16SC2 ]="CV_16SC2";
	typeMaps[ CV_16SC3 ]="CV_16SC3";
	typeMaps[ CV_16SC4 ]="CV_16SC4";

	typeMaps[ CV_32SC1 ]="CV_32SC1";
	typeMaps[ CV_32SC2 ]="CV_32SC2";
	typeMaps[ CV_32SC3 ]="CV_32SC3";
	typeMaps[ CV_32SC4 ]="CV_32SC4";

	typeMaps[ CV_32FC1 ]="CV_32FC1";
	typeMaps[ CV_32FC2 ]="CV_32FC2";
	typeMaps[ CV_32FC3 ]="CV_32FC3";
	typeMaps[ CV_32FC4 ]="CV_32FC4";

	typeMaps[ CV_64FC1 ]="CV_64FC1";
	typeMaps[ CV_64FC2 ]="CV_64FC2";
	typeMaps[ CV_64FC3 ]="CV_64FC3";
	typeMaps[ CV_64FC4 ]="CV_64FC4";
	if(typeMaps.find(this->image.type())!=typeMaps.end())
	{
		return typeMaps[this->image.type()];
	}
	else
	{
		return "Unknow Type !";
	}
}
