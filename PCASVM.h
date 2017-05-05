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
	PCASVM(string facePath,int faceBegin,int faceEnd,string notfacePath,int notfaceBegin,int notfaceEnd);
	virtual ~PCASVM();
	bool isFace(Mat faceImg);
	bool isFace(string facePath);
protected:
	void initFlatImagesAndLabels();
	void initPCA();
	void initSVM();
private:
	string facePath,notfacePath;
	int faceBegin,notfaceBegin,faceEnd,notfaceEnd;
	Ptr<PCA> pca;
	Ptr<SVM> svm;
	Mat flatImgs;
	Mat trainData;
	Mat trainLabels;
	int normalCols;
	int normalRows;
};

#endif // PCASVM_H
