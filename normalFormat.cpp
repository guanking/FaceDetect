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
bool normalFormat(string imgDir,int beginIndex,int endIndex,string suffix = ".png")
{
	Mat img;
	char imgPath[256];
	Size size;
	bool first = true;
	for(int i = beginIndex; i <= endIndex; i++)
	{
		sprintf(imgPath,"%s%d%s",imgDir.c_str(),i,suffix.c_str());
		img = imread(imgPath);
		cout<<"Process \""<<imgPath<<"\" ... ";
		if(img.empty())
		{
			cout<<imgPath<<" is empty !"<<endl;
			return false;
		}
		if(first)
		{
			size = Size(img.cols,img.rows);
			first = false;
		}
		else
		{
			cout<<"resize"<<img.size;
			resize(img,img,size);
			cout<<" to "<<img.size;
		}
		imwrite(imgPath,img);
	}
	return true;
}
int main(int argc,char* argv[])
{
	if(argc < 4)
	{
		cout<<"Please input legal params : imgDir,beginIndex,endIndex."<<endl;
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
	normalFormat(imgDir,beginIndex,endIndex);
	return 0;
}

#endif//NORMAL_FORMAT
