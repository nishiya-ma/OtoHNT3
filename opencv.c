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
	//HoughCircles���̓p�����[�^�̐���
	// src_img_gray : ���͉摜
	//     storage : ���o���ʂ̕ۑ��̈�
	//     CV_HOUGH_GRADIENT :�n�t�ϊ��̎�ށB2�i�K�n�t�ϊ�
	//     1: �~�̒��S�����߂�Ƃ��̉𑜓x
	//     100:�~���o�ɂ����钆�S���W�Ԃ̍ŏ��Ԋu�i�����ɂ��ꂽ�~���������񌟏o����̂�h���j
	//     20: �p�����[�^�ACanny�G�b�W���o�̃p�����[�^�ɂȂ�B
	//     50: �p�����[�^�A���S���o�̂������l
	//     10: ���o���ׂ��~�̍ŏ����a
	//     MAX (src_img_gray->width, src_img_gray->height) : ���o���ׂ��~�̍ő唼�a�i���̏ꍇ�͉摜�T�C�Y�̑傫�����j
	
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
		//�ł��~�炵�����̂�0�Ԃɂ���B����(0,0)
	
	  cvShowImage (crc_win, camera);
	
//	}	//end while

  cvReleaseMemStorage (&storage);

	return(0);
}