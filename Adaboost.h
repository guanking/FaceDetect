#ifndef ADABOOST_H
#define ADABOOST_H
#include<iostream>
#include<string>
#include<string.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<vector>
using namespace std;
using namespace cv;

/**
功能：使用Adaboost算法进行人脸检测
流程：设置图片，设置分类器，检测，判断是否检测到
*/
class Adaboost
{
private:
	/**
	检测所使用的图片
	*/
	Mat image;
	/**
	存放检测结果
	*/
	vector<Rect> rects;
	/**
	是否改变原图片
	*/
	bool changeSrc ;
	/**
	分类器
	*/
	CascadeClassifier cascade;
public:
	Adaboost();
	/**
	使用确定图片初始化
	@param path 图片路径
	*/
	Adaboost(string path);
	/**
	使用已有图片初始化
	@param image 图片路径
	*/
	Adaboost(Mat image);
	/**
	开始检测
	*/
	void detect();
	/**
	检测完成之后判断是否有人脸
	*/
	bool hasFace();
	/**
	获取当前检测使用的图片
	*/
	Mat getImage();
	/**
	设置当前检测要使用的图片
	*/
	void setImage(Mat img);
	/**
	设置要使用的分类器
	*/
	void setCascade(string path);
	/**
	获取检测到的人脸区域
	*/
	vector<Rect> getImageRects();
	/**
	获取当前所使用的图片的类型
	*/
	string imgType();
	/**
	显示检测结果
	*/
	void show();
	/**
	显示信息
	*/
	string info();
	/**
	设置在检测过程中是否改变原图片
	*/
	void setChangeSrcImage(bool change);
	/**
	获取在检测过程中是否改变了原图片
	*/
	bool getChangeSrcImage();
	/**
	获取标记了人脸的图 
	*/
	Mat getDrawnImg();
protected:
	/**
	初始化
	*/
	void init();
};
#endif
