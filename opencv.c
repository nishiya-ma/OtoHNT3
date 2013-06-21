//gcc -lcxcore200 -lcv200 -lhighgui200 -L/?/lib

#include <stdio.h>
#include "cv.h"
#include "highgui.h"

struct player{
	float x, y;
	int isdestroyed;
};

CvCapture *capture = 0;
char crc_win[] = "circles";

int main(){
	int key;
	struct player *p;

	if(opencvInit()==-1)return(1);
	while(1){
		getPlayerPoint(p);
		printf("point:x %.0f ,y %.0f\n",p->x,p->y);
		key = cvWaitKey(10);
		if(key == 'q')break;
	}
	opencvFin();
}

int opencvInit(){
	//capture from camera
	capture = cvCaptureFromCAM(0);
	if(!capture){
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}
printf("captured.\n");
  cvNamedWindow (crc_win,CV_WINDOW_AUTOSIZE);
}

int opencvFin(){
	cvReleaseCapture( &capture );

  cvDestroyWindow (crc_win);
}

int getPlayerPoint(struct player *Player)
{
IplImage *camera,//camera image
				 *gray;  //camera gray
CvMemStorage *storage;
CvSeq *circles = 0;

int i;
float *p;

//	while(1){
		//o get current camera
		camera = cvQueryFrame(capture);
		gray = cvCreateImage(cvSize(camera->width,camera->height),camera->depth,1);
		cvCvtColor(camera, gray, CV_BGR2GRAY);
		//find a circle
		cvSmooth(gray,gray,CV_GAUSSIAN,11,11,0,0);
		storage = cvCreateMemStorage(0);
		//circles = cvHoughCircles(
				//gray,storage,CV_HOUGH_GRADIENT,1,100,20,50,10,MAX(gray->width,gray->height));
		circles = cvHoughCircles(
				gray,storage,CV_HOUGH_GRADIENT,1,100,100,50,10,MAX(gray->width,gray->height));
	//HoughCircles入力パラメータの説明
	// src_img_gray : 入力画像
	//     storage : 検出結果の保存領域
	//     CV_HOUGH_GRADIENT :ハフ変換の種類。2段階ハフ変換
	//     1: 円の中心を求めるときの解像度
	//     100:円検出における中心座標間の最小間隔（微妙にずれた円をたくさん検出するのを防ぐ）
	//     20: パラメータ、Cannyエッジ検出のパラメータになる。
	//     50: パラメータ、中心検出のしきい値
	//     10: 検出すべき円の最小半径
	//     MAX (src_img_gray->width, src_img_gray->height) : 検出すべき円の最大半径（この場合は画像サイズの大きい方）
	
	  for (i = 0; i < circles->total; i++) 
	    { 
	      p = (float *)cvGetSeqElem (circles, i);
				cvCircle (camera, cvPoint (cvRound (p[0]), cvRound (p[1])), 3, CV_RGB (0, 255, 0), -1, 8, 0);
				cvCircle (camera, cvPoint (cvRound (p[0]), cvRound (p[1])), cvRound (p[2]), CV_RGB (MAX(0,255 - 100*i), 0, 0), 3, 8, 0);
				if(i == 0){
					Player->x = p[0];//cvRound(p[0]);
					Player->y = (float)camera->height - p[1];//cvRound(p[1]);
				}
	    }
		//最も円らしいものが0番にある。左上(0,0)
	
	  cvShowImage (crc_win, camera);
	
//	}	//end while

  cvReleaseMemStorage (&storage);

	return(0);
}