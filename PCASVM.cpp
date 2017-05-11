#include "PCASVM.h"
PCASVM::PCASVM(string facePath,int faceBegin,int faceEnd,string notfacePath,int notfaceBegin,int notfaceEnd)
{
	this->facePath = facePath;
	this->faceBegin = faceBegin;
	this->faceEnd  = faceEnd;
	this->notfacePath = notfacePath;
	this->notfaceBegin = notfaceBegin;
	this->notfaceEnd = notfaceEnd;
#ifdef DEBUG
	cout<<"FacePath = "<<this->facePath<<endl;
	cout<<"FaceBegin = "<<this->faceBegin<<endl;
	cout<<"FaceEnd = "<<this->faceEnd<<endl;
	cout<<"NotfacePath = "<<this->notfacePath<<endl;
	cout<<"NotfaceBegin = "<<this->notfaceBegin<<endl;
	cout<<"NotfaceEnd = "<<this->notfaceEnd<<endl;
#endif
	this->SVMKernelType = SVM::LINEAR;
	this->cptPercent = 0.95;
}

void PCASVM::init()
{
	this->initFlatImagesAndLabels();
	this->initPCA();
	this->initSVM();
}
void PCASVM::initFlatImagesAndLabels()
{
	char fileName[126];
	vector<Mat> imgs;
	//read face images and notface images
	for(int i =  this->faceBegin;i <= this->faceEnd; i++)
	{
		sprintf(fileName,"%s%d.png",this->facePath.c_str(),i);
		imgs.push_back(imread(fileName,IMREAD_GRAYSCALE));
	}
	for(int i =  this->notfaceBegin;i <= this->notfaceEnd; i++)
	{
		sprintf(fileName,"%s%d.png",this->notfacePath.c_str(),i);
		imgs.push_back(imread(fileName,IMREAD_GRAYSCALE));
	}
#ifdef DEBUG
	cout<<"Image Total Num = "<<imgs.size()<<endl;
#endif
	//convert images to flatImages
	if(imgs.empty()||imgs[0].empty())
	{
		cout<<"The image-path has no faces"<<endl;
		exit(0);
	}
	this->flatImgs = Mat(imgs.size(),imgs[0].cols*imgs[0].rows,CV_32F);
	this->normalCols = imgs[0].cols;
	this->normalRows = imgs[0].rows;
#ifdef DEBUG
	cout<<"Each image : \n\twidth = "<<this->normalRows<<" ,height = "<<this->normalCols<<endl;
	cout<<"\ttotal piexls = "<<this->normalRows*this->normalCols<<endl;
#endif
	for(size_t i = 0;i < imgs.size();i++)
	{
		imgs[i].reshape(1,1).convertTo(this->flatImgs.row(i),CV_32F);
	}

	//init label
	this->trainLabels = Mat(imgs.size(),1,CV_32SC1);
	int  cur = 0;
	for(int i = this->faceBegin;i <= this->faceEnd;i++)
	{
		this->trainLabels.at<int>(cur,0) = 1;
		cur++;
	}
	for(int i = this->notfaceBegin;i <= this->notfaceEnd;i++)
	{
		this->trainLabels.at<int>(cur,0) = 0;
		cur++;
	}
#ifdef DEBUG
	cout<<"Image label values:"<<endl;
	for(int i = 0;i < this->trainLabels.rows;i++)
	{
		printf("\t%05d %s face\n",i,this->trainLabels.at<int>(i,0) == 1?"is":"isn't");
	}
#endif
}

void PCASVM::initPCA()
{
#ifdef DEBUG
	cout<<"init pca..."<<endl;
	cout<<"Use flat images : "<<this->flatImgs.rows<<" X "<<this->flatImgs.cols<<endl;
#endif // DEBUG
	this->pca = new PCA(this->flatImgs, cv::Mat(), PCA::DATA_AS_ROW, this->cptPercent);
#ifdef DEBUG
	cout<<"Pca status:"<<endl;
	cout<<"\teigenvectors : "<<this->pca->eigenvectors.rows<<" X "<<this->pca->eigenvectors.cols<<endl;
	cout<<"\teigenvalues : "<<this->pca->eigenvalues.rows<<" X "<<this->pca->eigenvalues.cols<<endl;
#endif // DEBUG
	this->trainData = Mat(this->flatImgs.rows,this->pca->eigenvectors.rows,CV_32F);
	//init trainData
	Mat tImg;
	for(int i = 0;i < this->flatImgs.rows;i++)
	{
		tImg = this->pca->project(this->flatImgs.row(i));
		tImg.copyTo(this->trainData.row(i));
	}
#ifdef DEBUG
	cout<<"init pca finished !"<<endl;
#endif // DEBUG
}

void PCASVM::initSVM()
{
#ifdef DEBUG
	cout<<"training data using SVM...... "<<endl;
#endif // DEBUG
	this->svm = SVM::create();
	this->svm->setType(SVM::C_SVC);
	this->svm->setKernel(this->SVMKernelType);
	this->svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	this->svm->train(this->trainData, ROW_SAMPLE, this->trainLabels);
#ifdef DEBUG
	cout<<"training data using SVM finished !"<<endl;
#endif // DEBUG
}

void PCASVM::setCptPercent(const double& percent)
{
	this->cptPercent = percent;
}

void PCASVM::setSVMKernelType(const int& kernelType)
{
	this->SVMKernelType = kernelType;
}

PCASVM::~PCASVM()
{
	//delete this->pca;
	//delete this->svm;
}

bool PCASVM::isFace(Mat faceImg)//gray image
{
	Mat t = faceImg.clone();
	t = t.reshape(1,1);
	t = this->pca->project(t);
	return this->svm->predict(t);
}

bool PCASVM::isFace(string facePath)
{
#ifdef DEBUG
	cout<<"detect "<<facePath<<"......"<<endl;
#endif // DEBUG
	Mat img = imread(facePath.c_str(),IMREAD_GRAYSCALE);
	return this->isFace(img);
}


