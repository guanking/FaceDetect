#ifndef RTIMER_H
#define RTIMER_H
#include"Adaboost.h"
#include"PCASVM.h"
#include"YCbCr.h"
using namespace std;
using namespace cv;

/**
 *AdaBoost算法实时检测
 */
void rTTAdaboost();

/**
 *PCASVM算法实时检测
 */
void rTTPCASVM();

/**
 *YCbCr算法实时检测
 */
void rTTYCbCr();

#endif//RTIMER_H
