/***************************************************************************
 Copyright (c) 2016, Xilinx, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ***************************************************************************/

#include "xf_cvt_color_config.h"

#define ERROR_THRESHOLD 2

int main(int argc, char** argv) {

	uint16_t img_width;
	uint16_t img_height;

	cv::Mat inputimg0, inputimg1, inputimg2, inputimg;
	cv::Mat outputimg0, outputimg1, outputimg2;
	cv::Mat error_img0, error_img1, error_img2;
	cv::Mat refimage, refimg0, refimg1, refimg2;
	cv::Mat refimage0, refimage1, refimage2;

	cv::Mat img;

#if IYUV2NV12

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}
	inputimg1 = cv::imread(argv[2], 0);
	if(!inputimg1.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}
	inputimg2 = cv::imread(argv[3], 0);
	if(!inputimg2.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}

	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;

	int newwidth_uv = inputimg1.cols/2;
	int newheight_uv = inputimg1.rows+inputimg2.rows;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0, CV_8UC1);

	cv::Size S1(newwidth_uv,newheight_uv);
	outputimg1.create(S1, CV_16UC1);

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgInput2(inputimg2.rows,inputimg2.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgOutput1(newheight_uv,newwidth_uv);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo(inputimg1.data);
	imgInput2.copyTo(inputimg2.data);
#if __SDSCC__
	TIME_STAMP_INIT
#endif

	xFiyuv2nv12<XF_8UC1, XF_8UC2, HEIGHT, WIDTH, XF_NPPC1,XF_NPPC1>(imgInput0,imgInput1,imgInput2,imgOutput0,imgOutput1);

#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();

#endif
#if RO

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgInput1(inputimg1.rows,inputimg1.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgInput2(inputimg2.rows,inputimg2.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgOutput1(newheight_uv,newwidth_uv);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo(inputimg1.data);
	imgInput2.copyTo(inputimg2.data);
#if __SDSCC__
	TIME_STAMP_INIT
#endif

	xFiyuv2nv12<XF_8UC1, XF_8UC2, HEIGHT, WIDTH, XF_NPPC8,XF_NPPC4>(imgInput0,imgInput1,imgInput2,imgOutput0,imgOutput1);

#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();

#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_UV.png", outputimg1);

	refimage0 = cv::imread(argv[4],0);
	if(!refimage0.data)
	{
		std::cout << "Can't open Y ref image !!" << std::endl;
		return -1;
	}

	refimage1 = cv::imread(argv[5],-1);
	if(!refimage1.data)
	{
		std::cout << "Can't open UV ref image !!" << std::endl;
		return -1;
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);

#endif
#if IYUV2RGBA

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}
	inputimg1 = cv::imread(argv[2], 0);
	if(!inputimg1.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}
	inputimg2 = cv::imread(argv[3], 0);
	if(!inputimg2.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}

	cv::Size S0(inputimg0.cols,inputimg0.rows);
	outputimg0.create(S0, CV_8UC1);

	img_width = inputimg0.cols;
	img_height = inputimg0.rows; // + inputimg1.rows + inputimg2.rows;

	int newwidth = inputimg0.cols;
	int newheight = inputimg0.rows;

	cv::Size S(newwidth,newheight);
	outputimg0.create(S,CV_8UC4);

	//outputimg_ocv.create(S,CV_8UC4);
	error_img0.create(S,CV_8UC3);

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgInput2(inputimg2.rows,inputimg2.cols);

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(inputimg0.rows,inputimg0.cols);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo(inputimg1.data);
	imgInput2.copyTo(inputimg2.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFiyuv2rgba<XF_8UC1,XF_8UC4,HEIGHT,WIDTH,XF_NPPC1>(imgInput0,imgInput1,imgInput2,imgOutput0);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();

	cvtColor(outputimg0,outputimg0,CV_RGBA2BGR);
	imwrite("out.png", outputimg0);

	refimage = cv::imread(argv[4],1);

	absdiff(outputimg0,refimage,error_img0);

#endif
#endif
#if IYUV2YUV4

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}
	inputimg1 = cv::imread(argv[2], 0);
	if(!inputimg1.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}
	inputimg2 = cv::imread(argv[3], 0);
	if(!inputimg2.data)
	{
		std::cout << "Can't open image !!" << std::endl;
		return -1;
	}

	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;
	int newwidth_u = inputimg1.cols;
	int newheight_u = inputimg1.rows<<2;
	int newwidth_v = inputimg2.cols;
	int newheight_v = inputimg2.rows<<2;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u,newheight_u);
	outputimg1.create(S1,CV_8UC1);

	cv::Size S2(newwidth_v,newheight_v);
	outputimg2.create(S2,CV_8UC1);

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgInput2(inputimg2.rows,inputimg2.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput1(newheight_u,newwidth_u);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput2(newheight_v,newwidth_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo(inputimg1.data);
	imgInput2.copyTo(inputimg2.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFiyuv2yuv4<XF_8UC1,HEIGHT,WIDTH,XF_NPPC1>(imgInput0,imgInput1,imgInput2,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif
#if RO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgInput1(inputimg1.rows,inputimg1.cols);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgInput2(inputimg2.rows,inputimg2.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput1(newheight_u,newwidth_u);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput2(newheight_v,newwidth_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo(inputimg1.data);
	imgInput2.copyTo(inputimg2.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFiyuv2yuv4<XF_8UC1,HEIGHT,WIDTH,XF_NPPC8>(imgInput0,imgInput1,imgInput2,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#endif
	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[4],0);
	if(!refimage0.data)
	{
		printf("unable to open image \n");
		return(1);
	}
	refimage1 = cv::imread(argv[5],0);
	if(!refimage1.data)
	{
		printf("unable to open image \n");
		return(1);
	}
	refimage2 = cv::imread(argv[6],0);
	if(!refimage2.data)
	{
		printf("unable to open image \n");
		return(1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);
	imwrite("error_u.png",error_img1);
	imwrite("error_V.png",error_img2);

#endif
#if NV122IYUV

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		return -1;
	}
	inputimg1 = cv::imread(argv[2], -1);
	if(!inputimg1.data)
	{
		return -1;
	}

	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;

	int newwidth_u_v = inputimg1.cols<<1;
	int newheight_u_v = inputimg1.rows>>1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);

	outputimg2.create(S1,CV_8UC1);

#if NO

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int *)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv122iyuv<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC1,XF_NPPC1>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#else
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

	#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv122iyuv<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC8,XF_NPPC4>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif


	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimg0 = cv::imread(argv[3],0);
	if(!refimg0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimg1 = cv::imread(argv[4],0);
	if(!refimg1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimg2 = cv::imread(argv[5],0);
	if(!refimg2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimg0,error_img0);
	absdiff(outputimg1,refimg1,error_img1);
	absdiff(outputimg2,refimg2,error_img2);
	imwrite("error_Y.png", error_img0);
	imwrite("error_U.png", error_img1);
	imwrite("error_V.png", error_img2);
#endif

#if NV122RGBA

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		return -1;
	}
	inputimg1 = cv::imread(argv[2], -1);
	if(!inputimg1.data)
	{
		return -1;
	}

	int newwidth = inputimg0.cols;
	int newheight = inputimg0.rows;

	cv::Size S0(newwidth,newheight);
	outputimg0.create(S0,CV_8UC4);
	img_width = inputimg0.cols;
	img_height = inputimg0.rows;

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight,newwidth);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv122rgba<XF_8UC1,XF_8UC2,XF_8UC4,HEIGHT,WIDTH,XF_NPPC1>(imgInput0,imgInput1,imgOutput0);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();

#endif

	cvtColor(outputimg0,outputimg0,CV_RGBA2BGR);
	imwrite("out.png", outputimg0);

	refimage = cv::imread(argv[3],1);

	absdiff(outputimg0,refimage,error_img0);

#endif

#if NV122YUV4

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		return -1;
	}
	inputimg1 = cv::imread(argv[2], -1);
	if(!inputimg1.data)
	{
		return -1;
	}

	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;

	int newwidth_u_v = inputimg1.cols<<1;
	int newheight_u_v = inputimg1.rows<<1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);

	outputimg2.create(S1,CV_8UC1);

	img_width = inputimg0.cols;
	img_height = inputimg0.rows; // + inputimg1.rows;

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv122yuv4<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC1,XF_NPPC1>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#else
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv122yuv4<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC8,XF_NPPC4>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);

#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[3],0);
	if(!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 = cv::imread(argv[4],0);
	if(!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 = cv::imread(argv[5],0);
	if(!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);

#endif
#if NV212IYUV

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		return -1;
	}
	inputimg1 = cv::imread(argv[2], -1);
	if(!inputimg1.data)
	{
		return -1;
	}

	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;

	int newwidth_u_v = inputimg1.cols<<1;
	int newheight_u_v = inputimg1.rows>>1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);

	outputimg2.create(S1,CV_8UC1);

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv212iyuv<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC1,XF_NPPC1>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#else
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv212iyuv<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC8,XF_NPPC4>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[3],0);
	if(!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 = cv::imread(argv[4],0);
	if(!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 = cv::imread(argv[5],0);
	if(!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);

#endif

#if NV212RGBA

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		return -1;
	}
	inputimg1 = cv::imread(argv[2], -1);
	if(!inputimg1.data)
	{
		return -1;
	}

	int newwidth = inputimg0.cols;
	int newheight = inputimg0.rows;

	cv::Size S0(newwidth,newheight);
	outputimg0.create(S0,CV_8UC4);

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight,newwidth);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv212rgba<XF_8UC1,XF_8UC2,XF_8UC4,HEIGHT,WIDTH,XF_NPPC1>(imgInput0,imgInput1,imgOutput0);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();

#endif

	cvtColor(outputimg0,outputimg0,CV_RGBA2BGR);
	imwrite("out.png", outputimg0);

	refimage = cv::imread(argv[3],1);

	absdiff(outputimg0,refimage,error_img0);

#endif

#if NV212YUV4

	inputimg0 = cv::imread(argv[1], 0);
	if(!inputimg0.data)
	{
		return -1;
	}
	inputimg1 = cv::imread(argv[2], -1);
	if(!inputimg1.data)
	{
		return -1;
	}

	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;

	int newwidth_u_v = inputimg1.cols<<1;
	int newheight_u_v = inputimg1.rows<<1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);

	outputimg2.create(S1,CV_8UC1);

#if NO
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv212yuv4<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC1,XF_NPPC1>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#else
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput0(inputimg0.rows,inputimg0.cols);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgInput1(inputimg1.rows,inputimg1.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput0.copyTo(inputimg0.data);
	imgInput1.copyTo((unsigned short int*)inputimg1.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFnv212yuv4<XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC8,XF_NPPC4>(imgInput0,imgInput1,imgOutput0,imgOutput1,imgOutput2);

#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[3],0);
	if(!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 = cv::imread(argv[4],0);
	if(!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 = cv::imread(argv[5],0);
	if(!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);

#endif

#if RGBA2YUV4

	
	inputimg = cv::imread(argv[1], 1);

	if(!inputimg.data)
	{
		return -1;
	}
	int newwidth_y = inputimg.cols;
	int newheight_y = inputimg.rows;
	int newwidth_u_v = inputimg.cols;
	int newheight_u_v = inputimg.rows;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);
	outputimg2.create(S1,CV_8UC1);

	cvtColor(inputimg,inputimg,CV_BGR2RGBA);

#if NO
	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);
	imgInput.copyTo(inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif

	xFrgba2yuv4<XF_8UC4,XF_8UC1, HEIGHT, WIDTH, XF_NPPC1>(imgInput,imgOutput0,imgOutput1,imgOutput2);

#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif
	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[2],0);
	if(!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 = cv::imread(argv[3],0);
	if(!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 = cv::imread(argv[4],0);
	if(!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);

#endif

#if RGBA2IYUV

	inputimg = cv::imread(argv[1], 1);
	if(!inputimg.data)
	{
		return -1;
	}
	int newwidth_y = inputimg.cols;
	int newheight_y = inputimg.rows;
	int newwidth_u_v = inputimg.cols;
	int newheight_u_v = inputimg.rows>>2;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);
	outputimg2.create(S1,CV_8UC1);

	cvtColor(inputimg,inputimg,CV_BGR2RGBA);

#if NO
	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput.copyTo(inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFrgba2iyuv<XF_8UC4,XF_8UC1, HEIGHT, WIDTH,XF_NPPC1 >(imgInput,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[2],0);
	if(!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 = cv::imread(argv[3],0);
	if(!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 = cv::imread(argv[4],0);
	if(!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);

	imwrite("out_Y_error.png", error_img0);
	imwrite("out_U_error.png", error_img1);
	imwrite("out_V_error.png", error_img2);

#endif

#if RGBA2NV12

	inputimg0 = cv::imread(argv[1], 1);
	if(!inputimg0.data)
	{
		return -1;
	}
	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;
	int newwidth_uv = inputimg0.cols>>1;
	int newheight_uv = inputimg0.rows>>1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_uv,newheight_uv);
	outputimg1.create(S1,CV_16UC1);

	cvtColor(inputimg0,inputimg0,CV_BGR2RGBA);
	img_height = inputimg0.rows;
	img_width = inputimg0.cols;

#if NO

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg0.rows,inputimg0.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgOutput1(newheight_uv,newwidth_uv);

	imgInput.copyTo(inputimg0.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFrgba2nv12<XF_8UC4,XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC1>(imgInput,imgOutput0,imgOutput1);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();

	imwrite("out_UV.png", outputimg1);
	imwrite("out_y.png", outputimg0);
#endif

	refimage0 = cv::imread(argv[2],0);
	if(!refimage0.data)
	{
		std::cout << "Can't open Y ref image !!" << std::endl;
		return -1;
	}

	refimage1 = cv::imread(argv[3],-1);
	if(!refimage1.data)
	{
		std::cout << "Can't open UV ref image !!" << std::endl;
		return -1;
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);

#endif

#if RGBA2NV21

	inputimg0 = cv::imread(argv[1], 1);
	if(!inputimg0.data)
	{
		return -1;
	}
	int newwidth_y = inputimg0.cols;
	int newheight_y = inputimg0.rows;
	int newwidth_uv = inputimg0.cols>>1;
	int newheight_uv = inputimg0.rows>>1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_uv,newheight_uv);
	outputimg1.create(S1,CV_16UC1);

	cvtColor(inputimg0,inputimg0,CV_BGR2RGBA);

#if NO

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg0.rows,inputimg0.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgOutput1(newheight_uv,newwidth_uv);

	for(int i=0;i<inputimg0.rows;i++)
	{
		for(int j=0;j<inputimg0.cols;j++)
		{
			ap_uint<32> var = inputimg0.at< ap_uint<32> >(i,j);
			imgInput.data[(i*inputimg0.cols)+j] = var;
		}
	}
#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFrgba2nv21<XF_8UC4,XF_8UC1,XF_8UC2,HEIGHT,WIDTH,XF_NPPC1>(imgInput,imgOutput0,imgOutput1);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();

#endif
	imwrite("out_Y.png", outputimg0);
	imwrite("out_VU.png", outputimg1);

	refimage0 = cv::imread(argv[2],0);
	if(!refimage0.data)
	{
		std::cout << "Can't open Y ref image !!" << std::endl;
		return -1;
	}

	refimage1 = cv::imread(argv[3],-1);
	if(!refimage1.data)
	{
		std::cout << "Can't open UV ref image !!" << std::endl;
		return -1;
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);

#endif

#if UYVY2IYUV
	inputimg =cv::imread(argv[1],-1);
	if (!inputimg.data) {
		return -1;
	}
	int newwidth_y = inputimg.cols;
	int newheight_y = inputimg.rows;
	int newwidth_u_v = inputimg.cols;
	int newheight_u_v = inputimg.rows >> 2;

	cv::Size S0(newwidth_y, newheight_y);
	outputimg0.create(S0, CV_8UC1);

	cv::Size S1(newwidth_u_v, newheight_u_v);
	outputimg1.create(S1, CV_8UC1);
	outputimg2.create(S1, CV_8UC1);

#if NO
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT / 4, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT / 4, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput.copyTo((unsigned short int*)inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFuyvy2iyuv<XF_16UC1, XF_8UC1, HEIGHT, WIDTH, XF_NPPC1>(imgInput,imgOutput0, imgOutput1, imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#else
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput2(newheight_u_v,newwidth_u_v);
	imgInput.copyTo((unsigned short int*)inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFuyvy2iyuv<XF_16UC1,XF_8UC1, HEIGHT, WIDTH, XF_NPPC8>(imgInput,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 =cv::imread(argv[2],0);
	if (!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 =cv::imread(argv[3],0);
	if (!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 =cv::imread(argv[4],0);
	if (!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0, refimage0, error_img0);
	absdiff(outputimg1, refimage1, error_img1);
	absdiff(outputimg2, refimage2, error_img2);

	imwrite("out_Y_error.png", error_img0);
	imwrite("out_U_error.png", error_img1);
	imwrite("out_V_error.png", error_img2);

#endif
#if UYVY2NV12

	inputimg = cv::imread(argv[1], -1);
	if(!inputimg.data)
	{
		return -1;
	}
	int newwidth_y = inputimg.cols;
	int newheight_y = inputimg.rows;
	int newwidth_u_v = inputimg.cols>>1;
	int newheight_u_v = inputimg.rows>>1;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_16UC1);

	img_width = inputimg.cols;
	img_height = inputimg.rows;

#if NO
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);

	imgInput.copyTo((unsigned short int*)inputimg.data);


	xFuyvy2nv12<XF_16UC1,XF_8UC1,XF_8UC2,HEIGHT, WIDTH, XF_NPPC1,XF_NPPC1>(imgInput,imgOutput0,imgOutput1);

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();


#else
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput(inputimg.rows,inputimg.cols);
	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgOutput1(newheight_u_v,newwidth_u_v);

	imgInput.copyTo((unsigned short int*)inputimg.data);

	xFuyvy2nv12<XF_16UC1, XF_8UC1,XF_8UC2, HEIGHT, WIDTH, XF_NPPC8, XF_NPPC4>(imgInput,imgOutput0,imgOutput1);

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();


#endif

	imwrite("out_UV.png", outputimg1);
	imwrite("out_Y.png", outputimg0);

	refimage0 = cv::imread(argv[2],0);
	if(!refimage0.data)
	{
		std::cout << "Can't open Y ref image !!" << std::endl;
		return -1;
	}

	refimage1 = cv::imread(argv[3],-1);
	if(!refimage1.data)
	{
		std::cout << "Can't open UV ref image !!" << std::endl;
		return -1;
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
#endif
#if UYVY2RGBA

	inputimg = cv::imread(argv[1], -1);
	if(!inputimg.data)
	{
		return -1;
	}

	int newwidth = inputimg.cols;
	int newheight = inputimg.rows;
	cv::Mat outputimgrgba;
	cv::Size S0(newwidth,newheight);
	outputimg0.create(S0,CV_8UC4);
#if NO

	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight,newwidth);

	imgInput.copyTo((unsigned short int *)inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFuyvy2rgba<XF_16UC1,XF_8UC4, HEIGHT, WIDTH, XF_NPPC1>(imgInput,imgOutput0);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();

	cvtColor(outputimg0,outputimg0,CV_RGBA2BGR);

	imwrite("out.png", outputimg0);

	refimage = cv::imread(argv[2],1);
	if(!refimage.data)
	{
		printf("\nFailed to open reference image\n");
		return -1;
	}
	absdiff(outputimg0,refimage,error_img0);
#endif
#endif
#if YUYV2IYUV
	inputimg = cv::imread(argv[1], -1);
	if(!inputimg.data)
	{
		return -1;
	}
	int newwidth_y = inputimg.cols;
	int newheight_y = inputimg.rows;
	int newwidth_u_v = inputimg.cols;
	int newheight_u_v = inputimg.rows>>2;

	cv::Size S0(newwidth_y,newheight_y);
	outputimg0.create(S0,CV_8UC1);

	cv::Size S1(newwidth_u_v,newheight_u_v);
	outputimg1.create(S1,CV_8UC1);
	outputimg2.create(S1,CV_8UC1);
	img_width = inputimg.cols;
	img_height = inputimg.rows;

#if NO
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC1> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput.copyTo((unsigned short int*)inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFyuyv2iyuv<XF_16UC1,XF_8UC1, HEIGHT, WIDTH, XF_NPPC1>(imgInput,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();

#else
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput1(newheight_u_v,newwidth_u_v);
	xF::Mat<XF_8UC1, HEIGHT/4, WIDTH, XF_NPPC8> imgOutput2(newheight_u_v,newwidth_u_v);

	imgInput.copyTo((unsigned short int*)inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFyuyv2iyuv<XF_16UC1,XF_8UC1, HEIGHT, WIDTH, XF_NPPC8>(imgInput,imgOutput0,imgOutput1,imgOutput2);
#if __SDSCC__
	TIME_STAMP
#endif
	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = imgOutput1.copyFrom();
	outputimg2.data = imgOutput2.copyFrom();
#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_U.png", outputimg1);
	imwrite("out_V.png", outputimg2);

	refimage0 = cv::imread(argv[2],0);
	if(!refimage0.data) {
		printf("Failed to open Y reference image\n");
		return (1);
	}
	refimage1 = cv::imread(argv[3],0);
	if(!refimage1.data) {
		printf("Failed to open U reference image\n");
		return (1);
	}
	refimage2 = cv::imread(argv[4],0);
	if(!refimage2.data) {
		printf("Failed to open V reference image\n");
		return (1);
	}

	absdiff(outputimg0,refimage0,error_img0);
	absdiff(outputimg1,refimage1,error_img1);
	absdiff(outputimg2,refimage2,error_img2);

#endif

#if YUYV2NV12

	inputimg =cv::imread(argv[1],-1);
	if (!inputimg.data) {
		return -1;
	}
	int newwidth_y = inputimg.cols; //>>1;
	int newheight_y = inputimg.rows;
	int newwidth_u_v = inputimg.cols >> 1;
	int newheight_u_v = inputimg.rows >> 1;

	cv::Size S0(newwidth_y, newheight_y);
	outputimg0.create(S0, CV_8UC1);

	cv::Size S1(newwidth_u_v, newheight_u_v);
	outputimg1.create(S1, CV_16UC1);

	img_width = inputimg.cols;
	img_height = inputimg.rows;

#if NO
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC1> imgOutput1(newheight_u_v,newwidth_u_v);

	imgInput.copyTo((unsigned short int*)inputimg.data);


	xFyuyv2nv12<XF_16UC1,XF_8UC1,XF_8UC2, HEIGHT, WIDTH, XF_NPPC1,XF_NPPC1>(imgInput,imgOutput0,imgOutput1);

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();


#else
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC8> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC1, HEIGHT, WIDTH, XF_NPPC8> imgOutput0(newheight_y,newwidth_y);
	xF::Mat<XF_8UC2, HEIGHT/2, WIDTH/2, XF_NPPC4> imgOutput1(newheight_u_v,newwidth_u_v);


	imgInput.copyTo((unsigned short int*)inputimg.data);

	xFyuyv2nv12<XF_16UC1,XF_8UC1,XF_8UC2,HEIGHT, WIDTH, XF_NPPC8,XF_NPPC4>(imgInput,imgOutput0,imgOutput1);

	outputimg0.data = imgOutput0.copyFrom();
	outputimg1.data = (unsigned char*)imgOutput1.copyFrom();


#endif

	imwrite("out_Y.png", outputimg0);
	imwrite("out_UV.png", outputimg1);
	printf("\n Written output images\n");
	refimage0 =cv::imread(argv[2],0);
	if (!refimage0.data) {
		std::cout << "Can't open Y ref image !!" << std::endl;
		return -1;
	}

	refimage1 =cv::imread(argv[3],-1);
	if (!refimage1.data) {
		std::cout << "Can't open UV ref image !!" << std::endl;
		return -1;
	}

	absdiff(outputimg0, refimage0, error_img0);

	absdiff(outputimg1, refimage1, error_img1);
	imwrite("error_Y.png", error_img0);
	imwrite("error_UV.png", error_img1);
#endif

#if YUYV2RGBA
	inputimg = cv::imread(argv[1], -1);
	if(!inputimg.data)
	{
		return -1;
	}
	int newwidth = inputimg.cols;
	int newheight = inputimg.rows;

	cv::Size S0(newwidth,newheight);
	outputimg0.create(S0,CV_8UC4);

#if NO
	xF::Mat<XF_16UC1, HEIGHT, WIDTH, XF_NPPC1> imgInput(inputimg.rows,inputimg.cols);

	xF::Mat<XF_8UC4, HEIGHT, WIDTH, XF_NPPC1> imgOutput0(newheight,newwidth);

	imgInput.copyTo((unsigned short int*)inputimg.data);

#if __SDSCC__
	TIME_STAMP_INIT
#endif
	xFyuyv2rgba<XF_16UC1,XF_8UC4, HEIGHT, WIDTH, XF_NPPC1>(imgInput,imgOutput0);
#if __SDSCC__
	TIME_STAMP
#endif

		outputimg0.data = imgOutput0.copyFrom();


#endif
	cvtColor(outputimg0,outputimg0,CV_RGBA2BGR);
	imwrite("out.png", outputimg0);

	refimage = cv::imread(argv[2],1);
	if(!refimage.data)
	{
		printf("\nFailed to read reference image\n");
		return -1;
	}
	absdiff(outputimg0,refimage,error_img0);

#endif
	double minval, maxval;
	float err_per;
	int cnt;

	minval = 255;
	maxval = 0;
	cnt = 0;
	for (int i = 0; i < error_img0.rows; i++) {
		for (int j = 0; j < error_img0.cols; j++) {
			uchar v = error_img0.at<uchar>(i, j);

			if (v > ERROR_THRESHOLD)
				cnt++;
			if (minval > v)
				minval = v;
			if (maxval < v)
				maxval = v;
		}
	}
	err_per = 100.0 * (float) cnt / (error_img0.rows * error_img0.cols);
	fprintf(stderr,
			"Minimum error in intensity = %f\n\
									Maximum error in intensity = %f\n\
									Percentage of pixels above error threshold = %f\n",
			minval, maxval, err_per);

	if (err_per > 3.0f) 
	{
		printf("\n1st Image Test Failed\n");
		return 1;
	}

#if (IYUV2NV12 || RGBA2NV12 || RGBA2NV21 || UYVY2NV12 || YUYV2NV12)
	minval = 255;
	maxval = 0;
	cnt = 0;
	for (int i = 0; i < error_img1.rows; i++) {
		for (int j = 0; j < error_img1.cols; j++) {
			uchar v = error_img1.at<uchar>(i, j);

			if (v > ERROR_THRESHOLD)
				cnt++;
			if (minval > v)
				minval = v;
			if (maxval < v)
				maxval = v;
		}
	}
	err_per = 100.0 * (float) cnt / (error_img1.rows * error_img1.cols);
	fprintf(stderr,
			"Minimum error in intensity = %f\n\
							Maximum error in intensity = %f\n\
							Percentage of pixels above error threshold = %f\n",
			minval, maxval, err_per);
	if (err_per > 3.0f) 
	{
		printf("\n2nd Image Test Failed\n");
		return 1;
	}

#endif
#if (IYUV2YUV4 || NV122IYUV || NV122YUV4 || NV212IYUV || NV212YUV4 || RGBA2IYUV || RGBA2YUV4 || UYVY2IYUV || YUYV2IYUV)
	minval = 255;
	maxval = 0;
	cnt = 0;
	for (int i = 0; i < error_img2.rows; i++) {
		for (int j = 0; j < error_img2.cols; j++) {
			uchar v = error_img2.at<uchar>(i, j);

			if (v > ERROR_THRESHOLD)
				cnt++;
			if (minval > v)
				minval = v;
			if (maxval < v)
				maxval = v;
		}
	}
	err_per = 100.0 * (float) cnt / (error_img2.rows * error_img2.cols);
	fprintf(stderr,
			"Minimum error in intensity = %f\n\
							Maximum error in intensity = %f\n\
							Percentage of pixels above error threshold = %f\n",
			minval, maxval, err_per);
	if (err_per > 3.0f) 
	{
		printf("\n3rd Image Test Failed\n");
		return 1;
	}
#endif
	/* ## *************************************************************** ##*/
return 0;

}

