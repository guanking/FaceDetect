# FaceDetect
本工程实现三种人脸检测算法：AdaBoost，PCA+SVM，YCbRr
## 运行环境要求：
  要求已经安装opencv和Qt,该工程原始的开发环境为Linux，并且保证g++,make等基本的c/c++环境及命令可用
## 运行
  1. 首先执行qmake -project命令，生成.pro文件；
  2. 然后执行qmake命令，生成Makefile文件；
  3. 在我的电脑上可以直接通过pkg-config opencv --libs命令直接获取到opencv的头文件路径和编译所需要的库文件，将这些东西追加到Makefile文件中的Libs变量后，保证在Qt工程中可以正确调用到opencv；
  4. 执行make命令，会生成可执行文件Facedetect；
  5. 使用./Facedetect执行程序；
## 文件目录介绍
  * UI文件夹中是与界面相关的代码及ui文件，这里.ui文件，我是通过qt designer软件生成的，很好用；
  * cascade文件夹是AdaBoost检测算法所要使用的分类器，里面除了该工程使用到的人脸检测分类器还有其他的物体检测分类器；
  * img文件夹中是程序呢运行所使用的图像文件，里面有人脸和非人脸，图片类型有彩色也有黑白图像，这里的文件有的是网上下载的，还有一个印象中是MIT的彩色人脸图像；
  * test文件中，可以看到程序呢运行的一些截图，是我在做测试的时候保存的截图文件；
  * Adaboost.cpp,Adaboost.h,PCASVM.cpp,PCASVM.h,YCrCb.cpp,YCbCr.h分别是三种算法的实现；
  * normalFormat.cpp需要在文件中设置宏定义才能运行，这个里面程序的功能是统一图片的尺寸，具体用法和参数可以直接看函数注释；
  * rtimer.cpp和rtimer.h是可以直接运行的实时检测程序。
## 其他相关技术
  在界面设计的时候，为了加快速度，我使用了qt designer这个界面设计工具，感觉这个东西很不错，直接拖动就可以设计界面，还可以添加简单的事件；
## 遗留问题
  在PCASVM这个算法的实现过程中，在使用PCA提取主成分的时候，不确定是应该把人脸和非人脸一起提取还是只提取非人脸
