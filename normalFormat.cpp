#define NORMAL_FORMAT
#ifdef NORMAL_FORMAT
#include<iostream>
#include<string>
#include<string.h>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

/**
 *统一图片大小
 *
 *@param imgDir 要转换的图片文件所在的文件夹
 *@param beginIndex 图片的开始序号
 *@param endIndex 图片的结束序号
 *@param standardImg 作为标准的图片
 *@param suffix 所要处理的文件的后缀名
 */
bool normalFormat(string imgDir,int beginIndex,int endIndex,string standardImg = "",string suffix = ".png")
{
	Mat img;
	char imgPath[256];
	Size size;
	bool hasStandard = false;
	if(standardImg.length() != 0)
	{
		hasStandard = true;
		img = imread(standardImg);
		if(img.empty())
		{
			cout<<standardImg<<" is empty !"<<endl;
			return false;
		}
		size.width = img.cols;
		size.height = img.rows;
	}
	for(int i = beginIndex; i <= endIndex; i++)
	{
		sprintf(imgPath,"%s%d%s",imgDir.c_str(),i,suffix.c_str());
		img = imread(imgPath);
		cout<<endl<<"Process \""<<imgPath<<"\" ... ";
		if(img.empty())
		{
			cout<<imgPath<<" is empty !"<<endl;
			return false;
		}
		if(!hasStandard)
		{
			size = Size(img.cols,img.rows);
			hasStandard = true;
		}
		else
		{
			cout<<"resize "<<img.cols<<" X "<<img.rows;
			resize(img,img,size);
			cout<<" to "<<img.cols<<" X "<<img.rows;
		}
		imwrite(imgPath,img);
	}

	cout<<"\n\nUse standard size "<<size.width<<" X "<<size.height<<endl;
	return true;
}
int main(int argc,char* argv[])
{
	if(argc < 4)
	{
		cout<<"Please input legal params : imgDir,beginIndex,endIndex,standardImg = \"\",suffix = .png"<<endl;
		return 0;
	}
	string imgDir(argv[1]);
	int beginIndex,endIndex;
	sscanf(argv[2],"%d",&beginIndex);
	sscanf(argv[3],"%d",&endIndex);
	if(imgDir.find_last_of("/") != imgDir.length() - 1)
	{
		imgDir += "/";
	}
	cout<<"imgDir : "<<imgDir<<endl;
	cout<<"BeginIndex : "<<beginIndex<<endl;
	cout<<"EndIndex : "<<endIndex<<endl;
	bool result = false;
	if(argc == 5)
	{
		result = normalFormat(imgDir,beginIndex,endIndex,argv[4]);
	}
	else if(argc == 6)
	{
		result = normalFormat(imgDir,beginIndex,endIndex,argv[4],argv[5]);
	}
	else
	{
		result = normalFormat(imgDir,beginIndex,endIndex);
	}

	if(!result)
	{
		cout<<"Ternimal ! translate failed !"<<endl;
	}
	else
	{
		cout<<endl<<"Translate finish !"<<endl;
	}
	return 0;
}

#endif//NORMAL_FORMAT
