#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <thread>
#include <cstdio>
#include <cstdint>
#include "syanten.h"
#include "yuukouhai.h"
using namespace std;

//画像認識に関わるパラメータ
#define B_MAX 255
#define B_MIN 120
#define G_MAX 255
#define G_MIN 120
#define R_MAX 255
#define R_MIN 120
#define BIAS 5
#define COV 46.0f
#define HYPER 0.8f

#define FRAMERATE 30

char *preset_file = "hai/sample.avi";
int d;
int hai[38];//手牌

//画像認識に関わる関数
cv::Point minPoint(std::vector<cv::Point>);//最小の座標を求める
cv::Point maxPoint(std::vector<cv::Point>);//最大の座標を求める

void paste(cv::Mat, cv::Mat, int , int , int , int);

std::string input_file;
cv::Mat input;
cv::VideoCapture cap;
cv::Mat mask;
cv::Rect rect;
cv::Mat tmp_imgs[37];
cv::Mat number[10];
int loop_flag2 = 1;
int i,j;
int ch;
int waittime = 1000/FRAMERATE;

void threadA(){
	while(loop_flag2 == 1){
		cap >> input;
		cv::namedWindow("input");
		cv::imshow("input",input);
		cv::waitKey(waittime);
		if(ch == 27){
			loop_flag2 = 0;
			printf("%s\n", "finish");
		}
		if(input.empty()){
			printf("%s\n", "input end");break;
    }
	}
	loop_flag2 = 0;
	printf("%s\n", "threadA is ended");
}

void threadB(){
	while(loop_flag2 == 1){
		cv::Mat output(input, rect);

  	//テンプレートマッチングノためのリサイズ
  	cv::resize(output, output, cv::Size(), COV/d,COV/d);

  	//探索中の画像可視化
  	//cv::namedWindow("search image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);

  	cv::Mat search_img;
  	output.copyTo(search_img);
		for (j=0; j<37; j++){
			hai[j]=0;
		}
  	//牌の検出
  	for (j=0; j<37; j++) {
    	cv::Mat &tmp_img =  tmp_imgs[j];
    	cv::Mat result_img;

    	//４個まで検出する
    	for (i=0; i<4; i++){

      	//テンプレートマッチング
      	cv::matchTemplate(search_img, tmp_img, result_img, CV_TM_CCOEFF_NORMED);

      	// 最大のスコアの場所を探す
      	cv::Rect roi_rect(0, 0, tmp_img.cols, tmp_img.rows);
      	cv::Point max_pt;
      	double maxVal;
      	cv::minMaxLoc(result_img, NULL, &maxVal, NULL, &max_pt);

      	// 一定スコア以上の時その牌を認識
      	if(maxVal > HYPER){
         	roi_rect.x = max_pt.x;
         	roi_rect.y = max_pt.y;

         	// 探索結果の場所に矩形を描画
         	cv::rectangle(output, roi_rect, cv::Scalar(0,0,255));

         	//探索済みの部分を塗りつぶし
         	cv::rectangle(search_img, roi_rect, cv::Scalar(0,0,0),CV_FILLED);
					if(j == 9 || j == 19 || j == 29){
						hai[j-4]++;
					}
					else{
         		hai[j+1]++;
					}
       	}
     	}
   	}

   	Syanten inst1;
		inst1.clear();
		int sum = 0;
   	inst1.set_tehai(hai);
		for (i = 0; i < 38; i++) {
			sum += hai[i];
		}
		int shantensuu = inst1.NormalSyanten();
   	inst1.set_fuurosuu(4-sum/3);
   	Yuukouhai inst2;
   	inst2.set_tehai(hai);
   	std::vector<int> result1;
   	inst2.NormalYuukou(&result1);
   	//結果
	 	cv::Mat output2 = cv::Mat::zeros(400, 200, CV_8UC3);
	 	for (i = 0; i < result1.size(); i++) {
	 		paste(output2, tmp_imgs[result1[i]-1], 30+40*(i%4), 120+50*(i/4),31,47);
	 	}
	 	cv::Mat syantenmoji = cv::imread("hai/syanten.png",1);
	 	cv::Mat yuukouhaimoji = cv::imread("hai/yuukouhai.png", 1);
		paste(output2,number[shantensuu],30,10,28,32);
	 	paste(output2,syantenmoji,60,10,100,35);
	 	paste(output2,yuukouhaimoji,30,70,100,35);
	 	cv::namedWindow("output");
   	cv::imshow("output", output2);
	}
	loop_flag2 = 0;
	printf("%s\n", "threadB is ended");
}

//メイン関数
int main(int argc, char **argv){
  //手牌の初期化
  for(i=0;i<38;i++){
    hai[i] = 0;
  }

  //テンプレートマッチング用の画像データ
  tmp_imgs[0] = cv::imread("hai/1m.jpg", 1);
  tmp_imgs[1] = cv::imread("hai/2m.jpg", 1);
  tmp_imgs[2] = cv::imread("hai/3m.jpg", 1);
  tmp_imgs[3] = cv::imread("hai/4m.jpg", 1);
  tmp_imgs[4] = cv::imread("hai/5m.jpg", 1);
  tmp_imgs[5] = cv::imread("hai/6m.jpg", 1);
  tmp_imgs[6] = cv::imread("hai/7m.jpg", 1);
  tmp_imgs[7] = cv::imread("hai/8m.jpg", 1);
  tmp_imgs[8] = cv::imread("hai/9m.jpg", 1);
  tmp_imgs[9] = cv::imread("hai/0m.jpg", 1);
  tmp_imgs[10] = cv::imread("hai/1p.jpg", 1);
  tmp_imgs[11] = cv::imread("hai/2p.jpg", 1);
  tmp_imgs[12] = cv::imread("hai/3p.jpg", 1);
  tmp_imgs[13] = cv::imread("hai/4p.jpg", 1);
  tmp_imgs[14] = cv::imread("hai/5p.jpg", 1);
  tmp_imgs[15] = cv::imread("hai/6p.jpg", 1);
  tmp_imgs[16] = cv::imread("hai/7p.jpg", 1);
  tmp_imgs[17] = cv::imread("hai/8p.jpg", 1);
  tmp_imgs[18] = cv::imread("hai/9p.jpg", 1);
  tmp_imgs[19] = cv::imread("hai/0p.jpg", 1);
  tmp_imgs[20] = cv::imread("hai/1s.jpg", 1);
  tmp_imgs[21] = cv::imread("hai/2s.jpg", 1);
  tmp_imgs[22] = cv::imread("hai/3s.jpg", 1);
  tmp_imgs[23] = cv::imread("hai/4s.jpg", 1);
  tmp_imgs[24] = cv::imread("hai/5s.jpg", 1);
  tmp_imgs[25] = cv::imread("hai/6s.jpg", 1);
  tmp_imgs[26] = cv::imread("hai/7s.jpg", 1);
  tmp_imgs[27] = cv::imread("hai/8s.jpg", 1);
  tmp_imgs[28] = cv::imread("hai/9s.jpg", 1);
  tmp_imgs[29] = cv::imread("hai/0s.jpg", 1);
  tmp_imgs[30] = cv::imread("hai/1z.jpg", 1);
  tmp_imgs[31] = cv::imread("hai/2z.jpg", 1);
  tmp_imgs[32] = cv::imread("hai/3z.jpg", 1);
  tmp_imgs[33] = cv::imread("hai/4z.jpg", 1);
  tmp_imgs[34] = cv::imread("hai/5z.jpg", 1);
  tmp_imgs[35] = cv::imread("hai/6z.jpg", 1);
  tmp_imgs[36] = cv::imread("hai/7z.jpg", 1);
	number[0] = cv::imread("hai/0.png");
	number[1] = cv::imread("hai/1.png");
	number[2] = cv::imread("hai/2.png");
	number[3] = cv::imread("hai/3.png");
	number[4] = cv::imread("hai/4.png");
	number[5] = cv::imread("hai/5.png");
	number[6] = cv::imread("hai/6.png");
	number[7] = cv::imread("hai/7.png");
	number[8] = cv::imread("hai/8.png");
	number[9] = cv::imread("hai/9.png");

  if(argc == 2){
    input_file = argv[1];
		cap.open(input_file);
  }
  else{
    input_file = preset_file;
		cap.open(input_file);
  }
	if(!cap.isOpened()){
    printf("no input video\n");
    return 0;
  }
	cap >> input;
	cv::Size size(input.size());
	int loop_flag1 = 1;
	while(loop_flag1 == 1){
    cap >> input;
    if(input.empty()){
			printf("%s\n", "input error");
      return 0;
    }

	  //牌部分を検出するためのマスク
	  //inRangeを用いてフィルタリング
	  cv::Scalar s_min = cv::Scalar(B_MIN, G_MIN, R_MIN);
	  cv::Scalar s_max = cv::Scalar(B_MAX, G_MAX, R_MAX);
	  cv::inRange(input, s_min, s_max, mask);
	  mask = ~mask;

	  //手牌部分の検出
	  std::vector<std::vector<cv::Point> > contours;
	  std::vector<cv::Vec4i> hierarchy;
	  cv::findContours(mask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	  //各輪郭の最大最小座標を求める
	  for(i=0;i<contours.size(); i++){
	    cv::Point minP = minPoint(contours.at(i));
	    cv::Point maxP = maxPoint(contours.at(i));
	    if(fabs(minP.x-maxP.x)/fabs(minP.y-maxP.y)>9 && fabs(minP.x-maxP.x)/fabs(minP.y-maxP.y)<12 && fabs(minP.x-maxP.x)>size.width/3.0){
	      //手牌部分の矩形を描く
	      d = maxP.y - minP.y;
	      rect.x = minP.x-BIAS;
	      rect.y = minP.y-BIAS;
	      rect.width = (int)fabs(minP.x-maxP.x)+2*BIAS;
	      rect.height = (int)fabs(minP.y-maxP.y)+2*BIAS;
				//手牌検出後のマスク画像を表示

				loop_flag1 = 0;
	    }
	  }
  }

	std::thread th_a(threadA);
  std::thread th_b(threadB);

  th_a.join();
  th_b.join();

  return 0;
}

//最小座標を求める
cv::Point minPoint(std::vector<cv::Point> contours){
  double minx = contours.at(0).x;
  double miny = contours.at(0).y;
  for(int i=1;i<contours.size(); i++){
    if(minx > contours.at(i).x){
      minx = contours.at(i).x;
    }
    if(miny > contours.at(i).y){
      miny = contours.at(i).y;
    }
  }
  return cv::Point(minx, miny);
}
//最大座標を求める
cv::Point maxPoint(std::vector<cv::Point> contours){
  double maxx = contours.at(0).x;
  double maxy = contours.at(0).y;
  for(int i=1;i<contours.size(); i++){
    if(maxx < contours.at(i).x){
      maxx = contours.at(i).x;
    }
    if(maxy < contours.at(i).y){
      maxy = contours.at(i).y;
    }
  }
  return cv::Point(maxx, maxy);
}

void paste(cv::Mat dst, cv::Mat src, int x, int y, int width, int height) {
	cv::Mat resized_img;
	cv::resize(src, resized_img, cv::Size(width, height));

	if (x >= dst.cols || y >= dst.rows) return;
	int w = (x >= 0) ? std::min(dst.cols - x, resized_img.cols) : std::min(std::max(resized_img.cols + x, 0), dst.cols);
	int h = (y >= 0) ? std::min(dst.rows - y, resized_img.rows) : std::min(std::max(resized_img.rows + y, 0), dst.rows);
	int u = (x >= 0) ? 0 : std::min(-x, resized_img.cols - 1);
	int v = (y >= 0) ? 0 : std::min(-y, resized_img.rows - 1);
	int px = std::max(x, 0);
	int py = std::max(y, 0);

	cv::Mat roi_dst = dst(cv::Rect(px, py, w, h));
	cv::Mat roi_resized = resized_img(cv::Rect(u, v, w, h));
	roi_resized.copyTo(roi_dst);
}
