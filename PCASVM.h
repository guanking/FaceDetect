#ifndef PCASVM_H
#define PCASVM_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include<string>
using namespace std;
using namespace cv;
using namespace cv::ml;

class PCASVM
{
public:
	/**
	 *@param facePath 人脸文件路径
	 *@param faceBegin 人脸图片开始序号
	 *@param faceEnd 人脸图片结束序号
	 *@param notfacePath 非人脸文件路径
	 *@param notfaceBegin 非人脸图片开始序号
	 *@param norfaceEnd 非人脸图片结束序号
	 */
	PCASVM(string facePath,int faceBegin,int faceEnd,string notfacePath,int notfaceBegin,int notfaceEnd);
	/**
	 *析构函数
	 */
	virtual ~PCASVM();
	/**
	 *设置成分百分比
	 */
	void setCptPercent(const double& percent);
	/**
	 *设置SVM使用的核函数
	 */
	void setSVMKernelType(const int & kernelType);
	/**
	 *初始化PCA和SVM
	 */
	void init();
	/**
	 *使用PCA对图片进行成分提取，用训练好的SVM进行检测
	 *该图片是否为人脸
	 *@param faceImg 待检测的图片
	 */
	bool isFace(Mat faceImg);
	/**
	 *和isFace（Mat）功能一样
	 *@param facePath 待检测的图片的路径
	 */
	bool isFace(string facePath);
protected:
	/**
	 *读取人脸和非人脸图像，并对图像进行扁平化处理
	 *生成每个图像对应的标签
	 */
	void initFlatImagesAndLabels();
	/**
	 *使用PCA对扁平化后的图像数据进行主成分提取，生成SVM所需的训练数据
	 */
	void initPCA();
	/**
	 *主成分提取完成之后使用生成的数据惊醒SVM 训练
	 */
	void initSVM();
private:
	string facePath,notfacePath;
	int faceBegin,notfaceBegin,faceEnd,notfaceEnd;
	/**
	 *成分百分比
	 */
	double cptPercent;
	/**
	 *SVM核函数类型
	 */
	int SVMKernelType;
	Ptr<PCA> pca;
	Ptr<SVM> svm;
	/**
	 *扁平化后的图像数据
	 */
	Mat flatImgs;
	/**
	 *使用PCA提取后的训练数据
	 */
	Mat trainData;
	/**
	 *每个图像的标签（标识是否为人脸）
	 */
	Mat trainLabels;
	/**
	 *训练所使用图片的尺寸
	 */
	int normalCols;
	int normalRows;
};

#endif // PCASVM_H
