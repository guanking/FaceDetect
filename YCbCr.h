#ifndef YCBCR_H
#define YCBCR_H
#include<iostream>
#include<list>
#include<vector>
#include<queue>
#include<string>
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>
using namespace std;
using namespace cv;

#define MAX_PIXEL_VALUE 255
#define MIN_PIXEL_VALUE 0
#define VISIT 255
#define UNVISIT 0
#define LEGAL_SUB_HOLE_NUM 3
class YCbCr
{
private:
	/**
	待检测的图片
	*/
	Mat img;
	/**
	处理过程的缓存图片
	*/
	Mat resultImg;
	/**
	检测到的人脸区域集合
	*/
	list<Rect> faceAreas;
	/**
	形态学处理时是否使用“开”处理
	*/
	bool useMorphOpen;
	/**
	广度优先遍历算法实现对图片区域的选取（使用八连通区域）
	@param area 遍历使用的图像
	@param vis 标记图像中的点是否被访问过
	@param value 要标记的连通区域的值
	@param x 起始点X轴坐标
	@param y 起始点Y轴坐标
	*/
	Rect bfs(const Mat area,Mat vis,const int& value,const int& x = 0,const int& y = 0);
	/**
	形态学开处理
	*/
	void morphOpen();
	/**
	形态学闭处理
	*/
	void morphClose();
public:
	/**
	使用图片所在路径初始化
	*/
	YCbCr(string path);
	/**
	使用指定的检测图片初始化
	*/
	YCbCr(Mat img);
	/**
	析构函数
	*/
	~YCbCr();
	/**
	参数初始化
	*/
	void init();
	/**
	重置检测使用的图像
	*/
	void setImg(const Mat img);
	/**
	设置是否使用形态学“开”处理
	@param useMorphOpen 为true表示使用开处理，false表示使用闭处理
	*/
	void setUseMorphOpen(bool useMorphOpen);
	/**
	返回是否使用开处理
	*/
	bool isUseOpen();
	/**
	detect之后判断是否有人脸
	*/
	bool isFace();
	/**
	获取检测得到的人脸区域集合
	*/
	list<Rect> getFaceAreas();
	/**
	获取标记了人脸的图片
	*/
	Mat getDrawnImg();
	/**
	 *获取处理过程中的图像
	 */
	const Mat getCacheImg();
	/**
	肤色检测
	*/
	void initOrdinarySkin();
	/**
	形态学处理
	*/
	void morph();
	/**
	在肤色检测完成的基础上进行区域划分，检测出所有的人脸候选区域
	*/
	void splitArea();
	/**
	根据子区域的个数（人脸中会有鼻子眼睛形成的黑色区域）和宽高比筛选合法的人脸区域
	*/
	void selectLegal();
	/**
	人脸检测，按顺序进行肤色检测、形态学处理、区域筛选
	*/
	void detect();
};
#endif//YCBCR_H
