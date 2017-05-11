#include"YCbCr.h"

YCbCr::YCbCr(string path)
{
	this->img = imread(path.c_str());
	this->useMorphOpen = true;
}

YCbCr::YCbCr(Mat img)
{
	this->img = img.clone();
	this->useMorphOpen = true;
}

YCbCr::~YCbCr()
{
}

void YCbCr::setUseMorphOpen(bool useMorphOpen)
{
	this->useMorphOpen = useMorphOpen;
}

void YCbCr::setImg(const Mat img)
{
	this->img = img.clone();
}

bool YCbCr::isUseOpen()
{
	return this->useMorphOpen;
}

list<Rect> YCbCr::getFaceAreas()
{
	return this->faceAreas;
}

bool YCbCr::isFace()
{
	return !this->faceAreas.empty();
}

void YCbCr::init()
{
	if(this->img.empty())
	{
		cout<<"Image is empty(In YCbCr face detect)!"<<endl;
		exit(0);
	}
	this->faceAreas.clear();
}

void YCbCr::detect()
{
	this->initOrdinarySkin();
	this->morph();
	this->splitArea();
	this->selectLegal();
}

void YCbCr::initOrdinarySkin()
{
	Mat tImg;
	this->resultImg = Mat(this->img.rows,this->img.cols,CV_8UC1);
	cvtColor(this->img,tImg,CV_BGR2YCrCb);
	vector<Mat> channels;
	split(tImg,channels);
	Mat Cr = channels[1];
	Mat Cb = channels[2];
	for(int i = 0;i < Cr.rows;i++)
	{
		const uchar* curCr = Cr.ptr<uchar>(i);
		const uchar* curCb = Cb.ptr<uchar>(i);
		uchar* curR = this->resultImg.ptr<uchar>(i);
		for(int  j = 0;j < Cr.cols;j++)
		{
			if(curCr[j] > 137 && curCr[j] < 175 && curCb[j] > 100 && curCb[j] < 118)
			{
				curR[j] = MAX_PIXEL_VALUE;
			}
			else
			{
				curR[j] = MIN_PIXEL_VALUE;
			}
		}
	}
#ifdef DEBUG
	imshow("OrdinaryResult",this->resultImg);
	while(char(waitKey(10)) != 'q');
#endif//DEBUG
}

void YCbCr::morphOpen()
{
	erode(this->resultImg,this->resultImg,Mat(),Point(-1,-1),2);
	dilate(this->resultImg,this->resultImg,Mat(),Point(-1,-1),2);
}

void YCbCr::morphClose()
{
	dilate(this->resultImg,this->resultImg,Mat(),Point(-1,-1),2);
	erode(this->resultImg,this->resultImg,Mat(),Point(-1,-1),2);
}

void YCbCr::splitArea()
{
	this->faceAreas.clear();
	Mat vis(this->resultImg.rows,this->resultImg.cols,CV_8UC1);
	vis = UNVISIT;
	for(int i = 0; i < this->resultImg.rows;i++)
	{
		const uchar* curR = this->resultImg.ptr<uchar>(i);
		const uchar* curV = vis.ptr<uchar>(i);
		for(int j = 0;j < this->resultImg.cols; j++)
		{
			if(curR[j] == MAX_PIXEL_VALUE && curV[j] == UNVISIT)
			{
				this->faceAreas.push_back(bfs(this->resultImg,vis,MAX_PIXEL_VALUE,j,i));
#ifdef DEBUG
				imshow("SplitArea",vis);
				while(char(waitKey(10)) != 'q');
#endif//DEBUG
			}
		}
	}
#ifdef DEBUG
	cout<<"\nGet faceAreas,size = "<<this->faceAreas.size()<<endl;
	for(list<Rect>::iterator ele = this->faceAreas.begin();ele != this->faceAreas.end();ele++)
	{
		cout<<*ele<<endl;
	}
#endif//DEBUG
}

void YCbCr::selectLegal()
{
#ifdef DEBUG
	cout<<"\nBefore selectLegal, size = "<<this->faceAreas.size()<<endl;
#endif//DEBUG
	for(list<Rect>::iterator ele = this->faceAreas.begin();ele != this->faceAreas.end();)
	{
		double rate = double(ele->width)/ele->height;
		if(rate < 0.6 || rate > 2.0)
		{
#ifdef DEBUG
			list<Rect>::iterator tele = ele;
#endif//DEBUG
			this->faceAreas.erase(ele++);
#ifdef DEBUG
			cout<<"\tIllegal rate("<<rate<<"),delete "<<*tele<<endl;
#endif//DEBUG
		}
		else
		{
			ele++;
		}
	}
	for(list<Rect>::iterator ele = this->faceAreas.begin();ele != this->faceAreas.end();)
	{
#ifdef DEBUG
		cout<<"\nFind subhole in "<<*ele<<"..."<<endl;
#endif//DEBUG
		int holeNum = 0;
		Mat vis(this->resultImg.rows,this->resultImg.cols,CV_8UC1);
		vis = UNVISIT;
		for(int i = 0; i < ele->width;i++)
		{
			for(int j = 0;j < ele->height;j++)
			{
				int x = ele->x + i;
				int y = ele->y + j;
				if(vis.at<uchar>(y,x) == UNVISIT && this->resultImg.at<uchar>(y,x) == MIN_PIXEL_VALUE)
				{
					this->bfs(this->resultImg,vis,MIN_PIXEL_VALUE,x,y);
					holeNum++;
				}
				if(holeNum == LEGAL_SUB_HOLE_NUM)
				{
					break;
				}
			}
			if(holeNum == LEGAL_SUB_HOLE_NUM)
			{
				break;
			}
		}
		if(holeNum != LEGAL_SUB_HOLE_NUM)
		{
#ifdef DEBUG
			cout<<"\tIllegal holeNum = "<<holeNum;
			cout<<" delete faceArea "<<*ele<<endl;
#endif//DEBUG
			this->faceAreas.erase(ele++);
		}
		else
		{
			ele++;
		}
	}
#ifdef DEBUG
	cout<<"\nafter selectLegal size = "<<this->faceAreas.size()<<endl;
	for(list<Rect>::iterator ele = this->faceAreas.begin();ele != this->faceAreas.end();ele++)
	{
		cout<<*ele<<endl;
	}
#endif//DEBUG
}

Rect YCbCr::bfs(const Mat area,Mat vis,const int& value,const int& x,const int& y)
{
	Point topLeft(area.cols,area.rows);
	Point botRight(0,0);
	const size_t dir = 8;
	int dx[] = {-1,-1,-1, 0, 0, 1, 1, 1};
	int dy[] = {-1, 0, 1,-1, 1,-1, 1, 1};
	Point t(x,y);
	queue<Point> que;
	vis.at<uchar>(y,x) = VISIT;
	que.push(t);
	while(!que.empty())
	{
		Point p = que.front();
		que.pop();
		if(p.x < topLeft.x)
		{
			topLeft.x = p.x;
		}
		if(p.y < topLeft.y)
		{
			topLeft.y = p.y;
		}
		if(p.x > botRight.x)
		{
			botRight.x = p.x;
		}
		if(p.y > botRight.y)
		{
			botRight.y = p.y;
		}

		for(size_t i = 0;i < dir;i++)
		{
			t.x = p.x + dx[i];
			t.y = p.y + dy[i];
			if(t.x >=0 && t.x < area.cols && t.y >= 0 && t.y < area.rows)
			{
				if(vis.at<uchar>(t.y,t.x) == UNVISIT && area.at<uchar>(t.y,t.x) == value)
				{
					vis.at<uchar>(t.y,t.x) = VISIT;
					que.push(t);
				}
			}
		}
	}
	Rect curArea(topLeft,botRight);
#ifdef DEBUG
	cout<<"Find a new Subhole : ";
	cout<<curArea<<endl;
#endif//DEBUG
	return curArea;
}

void YCbCr::morph()
{
	if(this->useMorphOpen)
	{
		this->morphOpen();
	}
	else
	{
		this->morphClose();
	}
#ifdef DEBUG
	imshow("Morph:",this->resultImg);
	while(char(waitKey(10)) != 'q');
#endif//DEBUG
}

Mat YCbCr::getDrawnImg()
{
	Mat tImg = this->img.clone();
	for(list<Rect>::iterator ele = this->faceAreas.begin();ele != this->faceAreas.end();ele++)
	{
		rectangle(tImg,cvPoint(cvRound(ele->x),cvRound(ele->y)),
				cvPoint(cvRound(ele->x + ele->width),cvRound(ele->y + ele->height)),
				Scalar(255,128,0),3,8,0);
	}
#ifdef DEBUG
	imshow("YCbCr getDrawnImg",tImg);
	while(char(waitKey(10)) != 'q');
#endif//DEBUG
	return tImg;
}

const Mat YCbCr::getCacheImg()
{
	return this->resultImg;
}
