#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <fstream>

#include "main.h"
#include "nvbase.h"
#include "ene_program.h"

using namespace cv;
using namespace std;

#define LED_COUNT 22
#define LED_HALF_COUNT (LED_COUNT/2)

bool g_wait = false;
//unsigned char g_product = 0;
bool g_main_thread_exit = false;
std::mutex g_mutex_wait;
std::condition_variable g_led_set_color_ok; // 条件变量, 指示LED 灯已经设置成功
std::condition_variable g_image_process_ok; // 条件变量, 指示完成抓拍，已经处理好图片
std::fstream g_aging_file;
int g_Led_Color = WHITE;


//----		Red		Green	Blue	White
//hmin		156		35		100		0
//hmax		180		77		124		180
//smin		43		43		43		0
//smax		255		255		255		30
//vmin		46		46		46		221
//vmax		255		255		255		255
HsvColor g_HsvColor[AllColor] = {
	//{hmin,hmax,hlow,hhight,smin,smax,slow,shight,vmin,vmax,vlow,vhight}
	{0,   180, 125, 180, 0,  30,  2,   30,  221, 255, 221, 255},			// White
	{156, 180, 159, 180, 43, 255, 149, 255, 46,  255, 148, 255},		// Red
	{35,  77,  35,  77,  43, 255, 43,  255, 46,  255, 136,  255},			// Green
	{100, 124, 100, 124, 43, 255, 43,  255, 46,  255, 176,  255}		// Blue
};

#if false
#define SPLIT_BGR true

void rgb_less_250_set_to_0(Mat& src)
{
	for (int row = 0; row < src.rows; row++)
	{
		for (int col = 0; col < src.cols; col++)
		{
			int b = src.at<Vec3b>(row, col)[0];
			int g = src.at<Vec3b>(row, col)[1];
			int r = src.at<Vec3b>(row, col)[2];

			if (r < 250)
			{
				src.at<Vec3b>(row, col)[0] = 0;
				src.at<Vec3b>(row, col)[1] = 0;
				src.at<Vec3b>(row, col)[2] = 0;
			}
		}
	}
}

int main001()
{
	// 读取一个正常的灯带
	Mat red_0 = imread("./img/red_bands00.png");
	if (!red_0.data)
	{
		printf("ERROR : could not load red_bands.png .\n");
		return -1;
	}

	//rgb_less_250_set_to_0(red_0);

	imshow("red_0", red_0);
	moveWindow("red_0", 300, 0);

	// 将正常灯带转成灰度图, 作为对比目标
	//Mat red_0_gray;
	//cvtColor(red_0, red_0_gray, COLOR_BGR2GRAY);
	//imshow("red_0_gray", red_0_gray);
	//moveWindow("red_0_gray", 1200, 0);
#if SPLIT_BGR
	vector<Mat> red_0_bgr;
	split(red_0, red_0_bgr);
	imshow("red_0_bgr", red_0_bgr[2]);
	moveWindow("red_0_bgr", 1200, 0);
#else
	Mat red_0_hsv;
	cvtColor(red_0, red_0_hsv, COLOR_BGR2HSV);
	imshow("red_0_hsv", red_0_hsv);
	moveWindow("red_0_hsv", 1200, 0);
#endif

	// 读取灭一个灯的灯带, 并转成灰度图
	Mat red_1 = imread("./img/red_bands1.png");
	if (!red_1.data)
	{
		printf("ERROR : could not load red_bands1.png .\n");
		return -1;
	}

	//rgb_less_250_set_to_0(red_1);

	imshow("red_1", red_1);
	moveWindow("red_1", 300, 200);

	//Mat red_1_gray;
	//cvtColor(red_1, red_1_gray, COLOR_BGR2GRAY);
	//imshow("red_1_gray", red_1_gray);
	//moveWindow("red_1_gray", 1200, 200);
#if SPLIT_BGR
	vector<Mat> red_1_bgr;
	split(red_1, red_1_bgr);
	imshow("red_1_bgr", red_1_bgr[2]);
	moveWindow("red_1_bgr", 1200, 200);
#else
	Mat red_1_hsv;
	cvtColor(red_1, red_1_hsv, COLOR_BGR2HSV);
	imshow("red_1_hsv", red_1_hsv);
	moveWindow("red_1_hsv", 1200, 200);
#endif

	// 读取灭2个灯的灯带, 并转成灰度图
	Mat red_2 = imread("./img/red_bands2.png");
	if (!red_2.data)
	{
		printf("ERROR : could not load red_bands2.png .\n");
		return -1;
	}

	//rgb_less_250_set_to_0(red_2);
	imshow("red_2", red_2);
	moveWindow("red_2", 300, 400);

	//Mat red_2_gray;
	//cvtColor(red_2, red_2_gray, COLOR_BGR2GRAY);
	//imshow("red_2_gray", red_2_gray);
	//moveWindow("red_2_gray", 1200, 400);
#if SPLIT_BGR
	vector<Mat> red_2_bgr;
	split(red_2, red_2_bgr);
	imshow("red_2_bgr", red_2_bgr[2]);
	moveWindow("red_2_bgr", 1200, 400);
#else
	Mat red_2_hsv;
	cvtColor(red_2, red_2_hsv, COLOR_BGR2HSV);
	imshow("red_2_hsv", red_2_hsv);
	moveWindow("red_2_hsv", 1200, 400);
#endif

	// 读取灭3个灯的灯带, 并转成灰度图
	Mat red_3 = imread("./img/red_bands3.png");
	if (!red_3.data)
	{
		printf("ERROR : could not load red_bands3.png .\n");
		return -1;
	}

	//rgb_less_250_set_to_0(red_3);
	imshow("red_3", red_3);
	moveWindow("red_3", 300, 600);

	//Mat red_3_gray;
	//cvtColor(red_3, red_3_gray, COLOR_BGR2GRAY);
	//imshow("red_3_gray", red_3_gray);
	//moveWindow("red_3_gray", 1200, 600);
#if SPLIT_BGR
	vector<Mat> red_3_bgr;
	split(red_3, red_3_bgr);
	imshow("red_3_bgr", red_3_bgr[2]);
	moveWindow("red_3_bgr", 1200, 600);
#else
	Mat red_3_hsv;
	cvtColor(red_3, red_3_hsv, COLOR_BGR2HSV);
	imshow("red_3_hsv", red_3_hsv);
	moveWindow("red_3_hsv", 1200, 600);
#endif

	Mat red_0_histogram, red_1_histogram, red_2_histogram, red_3_histogram;

#if SPLIT_BGR

	int channels = 0;
	int histSize =  256;
	float hranges[] = { 0, 255 };
	const float* ranges[] = { hranges };	

	calcHist(&red_0_bgr[2], 1, &channels, Mat(), red_0_histogram, 1, &histSize, ranges, true, false);
	calcHist(&red_1_bgr[2], 1, &channels, Mat(), red_1_histogram, 1, &histSize, ranges, true, false);
	calcHist(&red_2_bgr[2], 1, &channels, Mat(), red_2_histogram, 1, &histSize, ranges, true, false);
	calcHist(&red_3_bgr[2], 1, &channels, Mat(), red_3_histogram, 1, &histSize, ranges, true, false);

	int hist_h = 400;
	int hist_w = 512;
	int bin_w = hist_w / histSize;

	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

	normalize(red_0_histogram, red_0_histogram, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(red_1_histogram, red_1_histogram, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(red_2_histogram, red_2_histogram, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(red_3_histogram, red_3_histogram, 0, hist_h, NORM_MINMAX, -1, Mat());
	
	double v01 = compareHist(red_0_histogram, red_1_histogram, HISTCMP_CORREL);
	double v02 = compareHist(red_0_histogram, red_2_histogram, HISTCMP_CORREL);
	double v03 = compareHist(red_0_histogram, red_3_histogram, HISTCMP_CORREL);

	std::cout << "0v1 = " << v01 << endl;
	std::cout << "0v2 = " << v02 << endl;
	std::cout << "0v3 = " << v03 << endl;

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(red_0_histogram.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(red_0_histogram.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);

		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(red_1_histogram.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(red_1_histogram.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);

		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(red_2_histogram.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(red_2_histogram.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);

		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(red_3_histogram.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(red_3_histogram.at<float>(i))), Scalar(255, 255, 255), 2, LINE_AA);
	}
	imshow("histogram", histImage);

#else
	// 4. 初始化计算直方图需要的实参(bins, 范围，通道 H 和 S ).
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255     
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	// Use the o-th and 1-st channels     
	int channels[] = { 0, 1 };

	calcHist(&red_0_hsv, 1, channels, Mat(), red_0_histogram, 2, histSize, ranges, true, false);
	calcHist(&red_1_hsv, 1, channels, Mat(), red_1_histogram, 2, histSize, ranges, true, false);
	calcHist(&red_2_hsv, 1, channels, Mat(), red_2_histogram, 2, histSize, ranges, true, false);
	calcHist(&red_3_hsv, 1, channels, Mat(), red_3_histogram, 2, histSize, ranges, true, false);

	normalize(red_0_histogram, red_0_histogram, 0, 1, NORM_MINMAX, -1, Mat());
	normalize(red_0_histogram, red_1_histogram, 0, 1, NORM_MINMAX, -1, Mat());
	normalize(red_0_histogram, red_2_histogram, 0, 1, NORM_MINMAX, -1, Mat());
	normalize(red_0_histogram, red_3_histogram, 0, 1, NORM_MINMAX, -1, Mat());

	double compareHist0v0 = compareHist(red_0_histogram, red_0_histogram, HISTCMP_CORREL);
	double compareHist0v1 = compareHist(red_0_histogram, red_1_histogram, HISTCMP_CORREL);
	double compareHist0v2 = compareHist(red_0_histogram, red_2_histogram, HISTCMP_CORREL);
	double compareHist0v3 = compareHist(red_0_histogram, red_3_histogram, HISTCMP_CORREL);

	std::cout << "0v0 = " << compareHist0v0 << endl;
	std::cout << "0v1 = " << compareHist0v1 << endl;
	std::cout << "0v2 = " << compareHist0v2 << endl;
	std::cout << "0v3 = " << compareHist0v3 << endl;
#endif

	waitKey(0);
	return 0;
}

#endif

typedef unsigned long Ul32;

typedef int(*lpLoadVenderDLL)();
typedef bool(*lpVGA_Read_IC_I2C)(UCHAR ucAddress, UCHAR reg_address, BYTE &rData, UINT iCardNumber, Ul32 ulDDCPort, UCHAR regSize, UCHAR DataSize, Ul32 flags);
typedef bool(*lpVGA_Write_IC_I2C)(UCHAR ucAddress, UCHAR reg_address, UCHAR *rData, UINT iCardNumber, Ul32 ulDDCPort, UCHAR regSize, UCHAR DataSize, Ul32 flags);

lpLoadVenderDLL  LOAD_VENDOR_DLL;
lpVGA_Read_IC_I2C    VGA_READ_IC_I2C;
lpVGA_Write_IC_I2C   VGA_WRITE_IC_I2C;

// LED 灯的地址
BYTE REG[22] = { 0x60, 0x63, 0x66, 0x69, 0x6c, 0x6f, 0x72, 0x75, 0x78, 0x7b, 0x7e
				, 0x81, 0x84, 0x87, 0x8a, 0x8d, 0x90, 0x93, 0x96, 0x99, 0x9c, 0x9f };

BYTE uOffset[12] = { 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00 };

#if 0
void setColor(unsigned char red, unsigned char blue, unsigned char green)
{
	unsigned char buf[90] = { 0 };
	for (size_t i = 0; i < 90; i += 3)
	{
		buf[i] = red;
		buf[i + 1] = blue;
		buf[i + 2] = green;
	}
	for (size_t j = 0; j < 90; j += 30)
	{
		eneWriteRegs(0x8160 + j, &buf[j], 30);
	}
	eneWriteReg(0x8021, 1);
	eneWriteReg(0x802f, 1);
}

void setSingleColor(u8 ledNumlight, u8 ledNumDelight, u8 r, u8 g, u8 b)
{
	u8 bufLight[3] = { r,g,b };	// 要打开的灯颜色
	u8 bufDelight[3] = { 0,0,0 };// 要关掉的灯

	eneWriteRegs(0x8160 + ledNumlight * 3, bufLight, 3);

	eneWriteRegs(0x8160 + ledNumDelight * 3, bufDelight, 3);

	eneWriteReg(0x8021, 1);	// 设置为静态
	eneWriteReg(0x802f, 1);	// 设置启用
}
#else

// 隔离硬件平台, 设置led灯光
void setSignleColor(int led, BYTE r, BYTE g, BYTE b)
{
	//Set Start Address
	uOffset[0] = 0x81;
	uOffset[1] = REG[led];
	VGA_WRITE_IC_I2C(0xCE, 0x0, (BYTE*)uOffset, 0, 1, 1, 2, 1);	//set address

	uOffset[0] = 3;	//rgb size
	uOffset[1] = r;
	uOffset[2] = b;
	uOffset[3] = g;
	//(UCHAR ucAddress, UCHAR reg_address, UCHAR *rData, UINT iCardNumber, Ul32 ulDDCPort, UCHAR regSize, UCHAR DataSize, Ul32 flags)
	VGA_WRITE_IC_I2C((BYTE)0xCE, (BYTE)0x03, (BYTE*)uOffset, 0, 1, 1, 4, 1);

	uOffset[0] = 0x80;
	uOffset[1] = 0x21;
	VGA_WRITE_IC_I2C(0xCE, 0x0, (BYTE*)uOffset, 0, 1, 1, 2, 1);	//set address

	uOffset[0] = 0x01;
	VGA_WRITE_IC_I2C(0xCE, 0x1, (BYTE*)uOffset, 0, 1, 1, 1, 1);	//write data

	uOffset[0] = 0x80;
	uOffset[1] = 0x2F;
	VGA_WRITE_IC_I2C(0xCE, 0x0, (BYTE*)uOffset, 0, 1, 1, 2, 1);	//set address

	uOffset[0] = 0x01;
	VGA_WRITE_IC_I2C(0xCE, 0x01, (BYTE*)uOffset, 0, 1, 1, 1, 1);	//write data
}

// 重置为特定颜色
void resetColor(BYTE r, BYTE g, BYTE b)
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		setSignleColor(i, r, g, b);
	}
}
#endif


#if 0
int main111(int argc, char *argv[], char *envp[])
{
	if (nvi2cinit() != 0)
	{
		printf("i2cb init fail\n");
		system("pause");
		return -1;
	}
	while (1)
	{
		setColor(0, 0, 0);
		u8  colorNum[22];
		colorNum[0] = 23;
		for (size_t i = 1; i < 22; i++)
		{
			colorNum[i] = i - 1;
		}

		for (size_t i = 0, t = 11; i < 11 && t < 22; i++, t++)
		{
			setSingleColor(i, colorNum[i], 255, 255, 255);


			setSingleColor(t, colorNum[t], 255, 255, 255);

			Sleep(100);
		}
		setColor(0, 0, 0);
	}

	//eneReset(); // 重置状态

	return 0;
}
#endif

void setColorThread()
{
	/* 这里存一个灯的映射关系，打开一个灯，需要把前面打开的灯关闭
	 0 - 10
	 1 - 0
	 10 - 9
	 -------------
	 11 - 21
	 12 - 11
	 21 - 20
	 */
	u8 colorNum[22];
	for (u8 i = 1; i < 22; i++)
	{
		colorNum[i] = i - 1;
	}
	colorNum[0] = LED_HALF_COUNT - 1;
	colorNum[LED_HALF_COUNT] = LED_COUNT - 1;

	while (true)
	{
		if (!g_wait)
		{
			for (g_Led_Color = WHITE; g_Led_Color <= BLUE; g_Led_Color++)
			{
				for (size_t i = 0, t = LED_HALF_COUNT; i < LED_HALF_COUNT && t < LED_COUNT; i++, t++)
				{
					// 在子线程的每次loop前判定是否需要退出
					// 主线程退出是随机的, 主线程接收到退出key后, 业务循环逻辑立即退出,进入join子线程状态
					// 子线程即便是恰巧在设置完灯后解锁, 主线程因为不再受理业务, 子线程会在下个循环开始前在此处退出
					if (g_main_thread_exit)
					{
						return;
					}
					g_mutex_wait.lock();
					setSignleColor(colorNum[i], 0, 0, 0);
					setSignleColor(colorNum[t], 0, 0, 0);

					if (g_Led_Color == WHITE)
					{
						setSignleColor(i, 255, 255, 255);
						setSignleColor(t, 255, 255, 255);
					}
					else if (g_Led_Color == RED)
					{
						setSignleColor(i, 255, 0, 0);
						setSignleColor(t, 255, 0, 0);
					}
					else if (g_Led_Color == GREEN)
					{
						setSignleColor(i, 0, 255, 0);
						setSignleColor(t, 0, 255, 0);
					}
					else if (g_Led_Color == BLUE)
					{
						setSignleColor(i, 0, 0, 255);
						setSignleColor(t, 0, 0, 255);
					}
					printf("**************\n");
					Sleep(50);	//灯的颜色真正设置进显卡
					g_wait = true;
					g_mutex_wait.unlock();
					Sleep(50);// Give Main Thread CPU Time
				}

				// 等Main 把活做完
				g_mutex_wait.lock();
				printf("++++++++++++++\n");
				g_mutex_wait.unlock();				
				//while (g_wait) {}

				//if (g_Led_Color >= BLUE)
				//	g_Led_Color = WHITE;
				//else
				//	g_Led_Color++;
			}
		}
	}	
}

void setColorThread2()
{
	/* 这里存一个灯的映射关系，打开一个灯，需要把前面打开的灯关闭
	 0 - 10
	 1 - 0
	 10 - 9
	 -------------
	 11 - 21
	 12 - 11
	 21 - 20
	 */
	u8 colorNum[22];
	for (u8 i = 1; i < 22; i++)
	{
		colorNum[i] = i - 1;
	}
	colorNum[0] = LED_HALF_COUNT - 1;
	colorNum[LED_HALF_COUNT] = LED_COUNT - 1;

	while (true)
	{
		for (g_Led_Color = WHITE; g_Led_Color <= BLUE; g_Led_Color++)
		{
			for (size_t i = 0, t = LED_HALF_COUNT; i < LED_HALF_COUNT && t < LED_COUNT; i++, t++)
			{
				// 在子线程的每次loop前判定是否需要退出
				// 主线程退出是随机的, 主线程接收到退出key后, 业务循环逻辑立即退出,进入join子线程状态
				// 子线程即便是恰巧在设置完灯后解锁, 主线程因为不再受理业务, 子线程会在下个循环开始前在此处退出
				if (g_main_thread_exit)
				{
					return;
				}

				// 一个单生产者-单消费者模型
				std::unique_lock<std::mutex> lock(g_mutex_wait);
				if (g_wait)
				{
					g_image_process_ok.wait(lock);
				}
				//g_mutex_wait.lock();
				setSignleColor(colorNum[i], 0, 0, 0);
				setSignleColor(colorNum[t], 0, 0, 0);

				if (g_Led_Color == WHITE)
				{
					setSignleColor(i, 255, 255, 255);
					setSignleColor(t, 255, 255, 255);
				}
				else if (g_Led_Color == RED)
				{
					setSignleColor(i, 255, 0, 0);
					setSignleColor(t, 255, 0, 0);
				}
				else if (g_Led_Color == GREEN)
				{
					setSignleColor(i, 0, 255, 0);
					setSignleColor(t, 0, 255, 0);
				}
				else if (g_Led_Color == BLUE)
				{
					setSignleColor(i, 0, 0, 255);
					setSignleColor(t, 0, 0, 255);
				}
				printf("**************\n");
				Sleep(50);	//灯的颜色真正设置进显卡
				g_wait = true;
				//g_mutex_wait.unlock();

				// 消费者线程目前不需要关注生产者的状态
				g_led_set_color_ok.notify_all();
				lock.unlock(); // 解锁.
				Sleep(2000);// Give Main Thread CPU Time
			}
		}
	}

}

void setColorThread3()
{
	/* 这里存一个灯的映射关系，打开一个灯，需要把前面打开的灯关闭
	 0 - 10
	 1 - 0
	 10 - 9
	 -------------
	 11 - 21
	 12 - 11
	 21 - 20
	 */
	u8 colorNum[22];
	for (u8 i = 1; i < 22; i++)
	{
		colorNum[i] = i - 1;
	}
	colorNum[0] = LED_HALF_COUNT - 1;
	colorNum[LED_HALF_COUNT] = LED_COUNT - 1;

	while (true)
	{
		for (g_Led_Color = WHITE; g_Led_Color < AllColor; g_Led_Color++)
		{
			for (size_t i = 0, t = LED_HALF_COUNT; i < LED_HALF_COUNT && t < LED_COUNT; i++, t++)
			{
				// 在子线程的每次loop前判定是否需要退出
				// 主线程退出是随机的, 主线程接收到退出key后, 业务循环逻辑立即退出,进入join子线程状态
				// 子线程即便是恰巧在设置完灯后解锁, 主线程因为不再受理业务, 子线程会在下个循环开始前在此处退出
				if (g_main_thread_exit)
				{
					return;
				}

				// 一个单生产者-单消费者模型
				std::unique_lock<std::mutex> lock(g_mutex_wait);
				if (g_wait)
				{
					g_image_process_ok.wait(lock);
				}
				//g_mutex_wait.lock();
				setSignleColor(colorNum[i], 0, 0, 0);
				setSignleColor(colorNum[t], 0, 0, 0);

				if (g_Led_Color == WHITE)
				{
					setSignleColor(i, 255, 255, 255);
					setSignleColor(t, 255, 255, 255);
				}
				else if (g_Led_Color == RED)
				{
					setSignleColor(i, 255, 0, 0);
					setSignleColor(t, 255, 0, 0);
				}
				else if (g_Led_Color == GREEN)
				{
					setSignleColor(i, 0, 255, 0);
					setSignleColor(t, 0, 255, 0);
				}
				else if (g_Led_Color == BLUE)
				{
					setSignleColor(i, 0, 0, 255);
					setSignleColor(t, 0, 0, 255);
				}
				printf("**************\n");
				Sleep(50);	//灯的颜色真正设置进显卡
				g_wait = true;
				//g_mutex_wait.unlock();

				// 消费者线程目前不需要关注生产者的状态
				g_led_set_color_ok.notify_all();
				lock.unlock(); // 解锁.
				Sleep(100);// Give Main Thread CPU Time
			}

			// 等我处理完图片你在换灯
			std::unique_lock<std::mutex> lock(g_mutex_wait);
			if (g_wait)
			{
				g_image_process_ok.wait(lock);
			}
			lock.unlock(); // 解锁.
		}
	}

}

bool openAgingLog()
{
	g_aging_file.open("./aging.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	if (g_aging_file.is_open())
	{
		// get length of file:
		g_aging_file.seekg(0, g_aging_file.end);
		std::streampos length = g_aging_file.tellg();
		if (length == 0)
		{
			// 添加表头
			// PPID|img_name|convert img_name to time string|r,g,b|a8[0]|a8[1]|a8[2]|a8[3]|a8[4]|a8[5]|a8[6]|a8[7]|test result
			g_aging_file << "PPID,"
				<< "img_name,"
				<< "rgb,"
				<< "a8[0],"
				<< "a8[1],"
				<< "a8[2],"
				<< "a8[3],"
				<< "a8[4],"
				<< "a8[5],"
				<< "a8[6],"
				<< "a8[7],"
				<< "test result" << endl;
			g_aging_file.flush();
		}
		return true;
	}
	return false;
}

void setFrameImgThread(void* lpcapture)
{
	Mat frame;
	VideoCapture* capture = (VideoCapture*)lpcapture;

	char key = '0';
	clock_t startTime;
	int file_index = 1;
	while (true)
	{
		(*capture).read(frame);
		if (g_wait)
		{
			g_mutex_wait.lock();
			startTime = clock();//计时开始

			AgingLog aging;
			
			Rect rect(40, 280, 1150, 110);	// 画一个截取框出来	
			Mat img = frame(rect);
			//imshow("imga", img);
			
			Mat frame_clone = frame.clone();
			rectangle(frame_clone, rect, Scalar(255, 255, 255), 5);
			imshow("graphics_card", frame_clone);

			char graphics_card_name[256] = { 0 };
			sprintf_s(graphics_card_name, 256, "./aging_img/%d.png", file_index);
			imwrite(graphics_card_name, frame);

			int count = 0;
			//startTime = clock();
			for (int i = 0; i < img.rows; i++)
			{
				for (int j = 0; j < img.cols; j++)
				{
					uchar* lpdata = img.ptr<uchar>(i, j);
					switch (g_Led_Color)
					{
					case WHITE:
						if (lpdata[2] < R_Threshold
							&& lpdata[1] < G_Threshold
							&& lpdata[0] < B_Threshold)
						{
							lpdata[0] = 0;
							lpdata[1] = 0;
							lpdata[2] = 0;
						}
						else {
							count++;
						}
						break;
					case RED:
						if (lpdata[2] < R_Threshold)
						{
							lpdata[0] = 0;
							lpdata[1] = 0;
							lpdata[2] = 0;
						}
						else {
							count++;
						}
						break;
					case GREEN:
						if (lpdata[1] < G_Threshold)
						{
							lpdata[0] = 0;
							lpdata[1] = 0;
							lpdata[2] = 0;
						}
						else {
							count++;
						}
						break;
					case BLUE:
						if (lpdata[0] < B_Threshold)
						{
							lpdata[0] = 0;
							lpdata[1] = 0;
							lpdata[2] = 0;
						}
						else {
							count++;
						}
						break;
					}

				}
			}			
			//printf("time consuming1 = %d\n", clock() - startTime);
			//startTime = clock();

			// 将img 划分成竖向的8列, 统计每列中r >=240 的点数量
			count = 0;
			for (size_t i = 0; i < img.cols; i++)
			{
				//uchar c; //取出指定通道的颜色值
				//int t;   // 该颜色值对应的过滤阈值
				int block_index = i / (img.cols / 8);	// 该像素点所在区块

				for (size_t j = 0; j < img.rows; j++)
				{
					uchar* lpdata = img.ptr<uchar>(j, i);
					switch (g_Led_Color)
					{
					case WHITE:
						if (lpdata[2] > 0
							&& lpdata[1] > 0
							&& lpdata[0] > 0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					case RED:
						if (lpdata[2] > 0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					case GREEN:
						if (lpdata[1] > 0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					case BLUE:
						if (lpdata[0] > 0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					}
				}
			}
			//printf("time consuming2 = %d\n", clock() - startTime);


			bool b8[8] = { 0 };	// 8个区块中, 各个区块是否有color
			int t[2] = { -1, -1 };	// 记录区块下标
			for (int s = 0; s < 8; s++)
			{
				if (s == 0 && aging.point_block[s] > 100)
				{
					b8[s] = true;
					t[0] = s;
				}
				else if (aging.point_block[s] > 100 && !b8[s - 1])
				{
					b8[s] = true;
					if (t[0] >= 0)
						t[1] = s;
					else
						t[0] = s;
				}
				else if (s == 3 && aging.point_block[s] > 70 && !b8[s - 1])
				{
					b8[s] = true;
					if (t[0] >= 0)
						t[1] = s;
					else
						t[0] = s;
				}
			}

			// 划线
			for (int i = 1; i < 8; i++)
				line(img, Point(img.cols / 8 * i, 0), Point(img.cols / 8 * i, img.rows), Scalar(255, 255, 0));

			aging.PPID = 1111111;
			aging.setColor(g_Led_Color);
			time(&aging.img_name);

			if (t[0] < 0 || t[1] < 0)
			{
				const unsigned int* a8 = aging.point_block;

				char aging_img[256] = { 0 };
				//sprintf_s(aging_img, 256, "./aging_img/%I64u.png", aging.img_name);
				sprintf_s(aging_img, 256, "./aging_img_ok/%d.png", file_index);

				char msg[256] = { 0 };
				sprintf_s(msg, 256, "Fail - %s", aging_img);
				putText(img, msg, Point(0, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "t[0] = %d t[1] = %d", t[0], t[1]);
				putText(img, msg, Point(0, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "a[0] = %d,a[1] = %d,a[2] = %d,a[3] = %d,a[4] = %d,a[5] = %d,a[6] = %d,a[7] = %d"
					, a8[0], a8[1], a8[2], a8[3], a8[4], a8[5], a8[6], a8[7]);
				putText(img, msg, Point(0, 110), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				imwrite(aging_img, img);
				//imshow("img_fail", img);
				//waitKey(1);
				aging.result = false;
			}
			else
			{
				const unsigned int* a8 = aging.point_block;
				char aging_img[256] = { 0 };
				//sprintf_s(aging_img, 256, "./aging_img_ok/%I64u.png", aging.img_name);
				sprintf_s(aging_img, 256, "./aging_img_ok/%d.png", file_index);

				char msg[256] = { 0 };
				sprintf_s(msg, 256, "Success - %s", aging_img);
				putText(img, msg, Point(0, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "t[0] = %d t[1] = %d",t[0], t[1]);
				putText(img, msg, Point(0, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "a[0] = %d,a[1] = %d,a[2] = %d,a[3] = %d,a[4] = %d,a[5] = %d,a[6] = %d,a[7] = %d"
					, a8[0], a8[1], a8[2], a8[3], a8[4], a8[5], a8[6], a8[7]);
				putText(img, msg, Point(0, 110), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				imwrite(aging_img, img);
				aging.result = true;
			}
			file_index++;
			imshow("img", img);

			aging.time_consuming = clock() - startTime;

			//printf("time consuming - %d\n", aging.time_consuming);

			if (g_aging_file.is_open())
			{
				g_aging_file << aging.PPID << ","
					<< aging.img_name << ",\""
					<< aging.color[0] << "," << aging.color[1] << "," << aging.color[2] << "\","
					<< aging.point_block[0] << ","
					<< aging.point_block[1] << ","
					<< aging.point_block[2] << ","
					<< aging.point_block[3] << ","
					<< aging.point_block[4] << ","
					<< aging.point_block[5] << ","
					<< aging.point_block[6] << ","
					<< aging.point_block[7] << ","
					<< aging.result << endl;
				g_aging_file.flush();
			}

			g_wait = false;
			printf("--------------%d\n", aging.time_consuming);
			g_mutex_wait.unlock();
		}
		key = waitKey(30);
		if (key == 0x1b)
		{
			g_main_thread_exit = true;
			break;
		}
		else if (key == 0x20)
		{
			g_mutex_wait.lock();
			waitKey();
			g_mutex_wait.unlock();
		}
	}
}

void setFrameImgThread2(void* lpcapture)
{
	Mat frame;
	VideoCapture* capture = (VideoCapture*)lpcapture;

	char key = '0';
	clock_t startTime;

	while (true)
	{
		(*capture).read(frame);
		if (g_wait)
		{
			//g_mutex_wait.lock();
			std::unique_lock<std::mutex> lock(g_mutex_wait);
			{
				startTime = clock();//计时开始
				(*capture).read(frame); // !important, 确保读取出来的灯是完全点亮的
				AgingLog aging;

				Rect rect(40, 280, 1150, 110);	// 画一个截取框出来	
				Mat img = frame(rect);

				Mat frame_clone = frame.clone();
				rectangle(frame_clone, rect, Scalar(255, 255, 255), 5);
				imshow("graphics_card", frame_clone);

				// 分离成3张单通道image
				Mat channels_m[3];
				split(img, channels_m);
				//cvtColor(img, img, COLOR_BGR2GRAY);
				//imshow("gray_img", img);
				// 直接轉換成二值圖
				switch (g_Led_Color)
				{
				case WHITE:
					cvtColor(img, img, COLOR_BGR2GRAY);
					threshold(img, img, W_Threshold, 255, THRESH_BINARY);
					break;
				case RED:
					threshold(channels_m[2], img, R_Threshold, 255, THRESH_BINARY);
					break;
				case GREEN:
					threshold(channels_m[1], img, G_Threshold, 255, THRESH_BINARY);
					break;
				case BLUE:
					threshold(channels_m[0], img, B_Threshold, 255, THRESH_BINARY);
					break;
				}

				imshow("threshold_img", img);

				//均值滤波
				medianBlur(img, img, 5);

				imshow("medianBlur_img", img);

				//形态学处理
				Mat kernel = getStructuringElement(MORPH_CROSS, Size(4, 4));
				morphologyEx(img, img, MORPH_CLOSE, kernel, Point(-1, -1), 7, BORDER_REPLICATE);

				imshow("morphologyEx_img", img);
				waitKey(1);
				g_wait = false;
				printf("--------------%d\n", clock() - startTime);
				//g_mutex_wait.unlock();
			}
			g_image_process_ok.notify_all();
			lock.unlock();
		}

		key = waitKey(1);
		if (key == 0x1b)	// Esc 键
		{
			g_main_thread_exit = true;
			break;
		}
		else if (key == 0x20) // 空格键
		{
			g_mutex_wait.lock();
			waitKey();
			g_mutex_wait.unlock();
		}
	}
}

void setFrameImgThread3(void* lpcapture)
{
	Mat frame;
	VideoCapture* capture = (VideoCapture*)lpcapture;

	char key = '0';
	clock_t startTime;
	
	int lowhsv[3] = { 0 };	
	int highsv[3] = { 0 };

	while (true)
	{
		(*capture).read(frame);
		if (g_wait)
		{
			//g_mutex_wait.lock();
			std::unique_lock<std::mutex> lock(g_mutex_wait);
			if(g_Led_Color < AllColor)
			{
				const HsvColor& hsv = g_HsvColor[g_Led_Color];
				lowhsv[0] = hsv.h[0] + hsv.h[5];
				lowhsv[1] = hsv.s[0] + hsv.s[5];
				lowhsv[2] = hsv.v[0] + hsv.v[5];

				highsv[0] = hsv.h[0] + hsv.h[6];
				highsv[1] = hsv.s[0] + hsv.s[6];
				highsv[2] = hsv.v[0] + hsv.v[6];

				startTime = clock();//计时开始
				(*capture).read(frame); // !important, 确保读取出来的灯是完全点亮的
				AgingLog aging;

				//Rect rect(40, 280, 1150, 110);	// 画一个截取框出来	
				Rect rect(200, 240, 900, 200);
				Mat img = frame(rect);
				//Mat img = frame.clone();

				Mat frame_clone = frame.clone();
				rectangle(frame_clone, rect, Scalar(255, 255, 0), 5);
				imshow("graphics_card", frame_clone);				

				//均值滤波
				//medianBlur(img, img, 7);
				GaussianBlur(img, img, Size(7, 7), 1.0);
				//imshow("medianBlur_img", img);
				//moveWindow("medianBlur_img", 0, 0);

				Mat hsv_img;
				cvtColor(img, hsv_img, COLOR_BGR2HSV);
				imshow("hsv_img", hsv_img);
				//moveWindow("hsv_img", 0, 150);

				Mat hsv_img_mask;				
				inRange(hsv_img, Scalar(lowhsv[0], lowhsv[1], lowhsv[2]), Scalar(highsv[0], highsv[1], highsv[2]), hsv_img_mask);
				//imshow("inRange_mask", hsv_img_mask);
				//moveWindow("inRange_mask", 0, 500);

				//形态学处理
				Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
				morphologyEx(hsv_img_mask, hsv_img_mask, MORPH_CLOSE, kernel);
				//imshow("MORPH_CLOSE", hsv_img_mask);
				//moveWindow("MORPH_CLOSE", 0, 650);

				morphologyEx(hsv_img_mask, hsv_img_mask, MORPH_OPEN, kernel);
				//imshow("MORPH_OPEN", hsv_img_mask);
				//moveWindow("MORPH_OPEN", 0, 800);

				Mat result = Mat::zeros(img.size(), img.type());
				bitwise_and(img, img, result, hsv_img_mask);
				//imshow("bitwise_and", result);
				//moveWindow("bitwise_and", 900, 0);

				//存储边缘
				vector<vector<Point> > contours;
				vector<Vec4i> hierarchy;

				//找到边缘，注意为什么先把binaryFrame克隆一遍
				Mat tempBinaryFrame = hsv_img_mask.clone();
				findContours(tempBinaryFrame, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));//查找最顶层轮廓

				//存储
				vector<Rect> boundRect;
				boundRect.clear();
				for (int index = 0; index < contours.size(); index++)
				{
					// 第一个轮廓会首先放入队列
					// 第二个轮廓会跟队列中的最后一个轮廓进行比较，看彼此的距离是否超过阈值
					// 如果没有超过阈值，说明同属一个大轮廓，所以合并两个轮廓后，重新放入队列
					// 如果超出阈值， 说明是两个独立的轮廓
					vector<Point> contours_poly;
					approxPolyDP(Mat(contours[index]), contours_poly, 3, true);
					Rect rect = boundingRect(Mat(contours_poly));
					if (rect.width < 20 || rect.height < 10)// 先过滤掉小点
						continue;
					if (boundRect.empty())
					{
						boundRect.push_back(rect);
						continue;
					}
					else
					{
						auto tail = boundRect.rbegin();
						int gap = 0;
						if (tail->x > rect.x)
						{
							gap = abs(tail->x - (rect.x + rect.width));
						}
						else {
							gap = abs(rect.x - (tail->x + tail->width));
						}
						
						if (gap < 60)
						{
							Rect big_rect;
							big_rect.x = min(tail->x, rect.x);
							big_rect.y = min(tail->y, rect.y);
							big_rect.width = max(tail->x + tail->width, rect.x + rect.width) - big_rect.x;
							big_rect.height = max(tail->y + tail->height, rect.y + rect.height) - big_rect.y;
							*tail = big_rect;
						}
						else
						{
							boundRect.push_back(rect);
						}
					}
				}
				if (boundRect.size() != 2)
				{
					//waitKey();
				}
				//得到灯的轮廓
				for (int index = 0; index < boundRect.size(); index++)
				{
					rectangle(result, boundRect[index], Scalar(0, 255, 255), 1);
					//imshow("contours", result);
					//moveWindow("contours", 900, 150);
					//waitKey(1);
				}
				imshow("contours", result);

				waitKey(1);
				g_wait = false;
				printf("--------------%d\n", clock() - startTime);
				//g_mutex_wait.unlock();
			}
			g_image_process_ok.notify_all();
			lock.unlock();
		}

		key = waitKey(1);
		if (key == 0x1b)	// Esc 键
		{
			g_main_thread_exit = true;
			break;
		}
		else if (key == 0x20) // 空格键
		{
			g_mutex_wait.lock();
			waitKey();
			g_mutex_wait.unlock();
		}
	}
}

void renderTrackbarThread()
{
	int empty_w = 400, empty_h = 100;
	Mat empty = Mat::zeros(Size(empty_w, empty_h), CV_8UC3);
	namedWindow("Toolkit");
	imshow("Toolkit", empty);
	int hl = 0, sl = 0, vl = 0;
	int hh = 0, sh = 0, vh = 0;

	char buf[128] = { 0 };
	while (true)
	{
		if (g_main_thread_exit) {
			break;
		}
		if (g_Led_Color >= AllColor)// 防止越界
			continue;
		HsvColor& hsv = g_HsvColor[g_Led_Color];
		createTrackbar("lowHue", "Toolkit", &hsv.h[5], hsv.h[4]);
		createTrackbar("higHue", "Toolkit", &hsv.h[6], hsv.h[4]);

		createTrackbar("lowSat", "Toolkit", &hsv.s[5], hsv.s[4]);
		createTrackbar("higSat", "Toolkit", &hsv.s[6], hsv.s[4]);

		createTrackbar("lowVal", "Toolkit", &hsv.v[5], hsv.v[4]);
		createTrackbar("higVal", "Toolkit", &hsv.v[6], hsv.v[4]);
		
		hl = hsv.h[0] + hsv.h[5];
		sl = hsv.s[0] + hsv.s[5];
		vl = hsv.v[0] + hsv.v[5];

		hh = hsv.h[0] + hsv.h[6];
		sh = hsv.s[0] + hsv.s[6];
		vh = hsv.v[0] + hsv.v[6];
		
		sprintf_s(buf, 128, "lowHSV < higHSV !!!");
		putText(empty, buf, Point(0, empty.rows/4 * 1), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);

		sprintf_s(buf, 128, "Real Low HSV (%d, %d, %d)", hl, sl, vl);
		putText(empty, buf, Point(0, empty.rows/4 * 2), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);

		sprintf_s(buf, 128, "Real High HSV (%d, %d, %d)", hh, sh, vh);
		putText(empty, buf, Point(0, empty.rows/4 * 3), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);
		imshow("Toolkit", empty);
		empty = Mat::zeros(Size(empty_w, empty_h), CV_8UC3);

		waitKey(1);
	}
}

int main()
{
	Mat frame;
	VideoCapture capture(1);
	//capture.set(CAP_PROP_SETTINGS, 1);
	capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CAP_PROP_FRAME_HEIGHT, 720);

	HINSTANCE hDLL;		// Handle to DLL
	hDLL = LoadLibrary(L"VGA_Extra_x64.dll");
	LOAD_VENDOR_DLL = (lpLoadVenderDLL)GetProcAddress(hDLL, "LoadVenderDLL");
	VGA_READ_IC_I2C = (lpVGA_Read_IC_I2C)GetProcAddress(hDLL, "VGA_Read_IC_I2C");
	VGA_WRITE_IC_I2C = (lpVGA_Write_IC_I2C)GetProcAddress(hDLL, "VGA_Write_IC_I2C");

	// 载入dll
	LOAD_VENDOR_DLL();

	// 关闭所有灯
	resetColor(0, 0, 0);

	// 初始化log文件句柄
	if (!openAgingLog())
	{
		printf("open aging.csv error!\n");
		return -1;
	}

	std::thread t1(setColorThread3);
	std::thread t2(setFrameImgThread3, &capture);
	std::thread t3(renderTrackbarThread);
	
#if 0
	//char key = '0';
	//unsigned long index = 0;
	//clock_t startTime;
	while (false)
	{
		capture.read(frame);
		if (g_wait)
		{
			AgingLog aging;
			//printf("_clock1 = %u\n", clock());
			g_mutex_wait.lock();
			startTime = clock();//计时开始
			
			Rect rect(230, 180, 850, 300);	// 画一个截取框出来	
			Mat img = frame(rect);
			imshow("img", img);

			Mat frame_clone = frame.clone();
			rectangle(frame_clone, rect, Scalar(255, 255, 255), 5);
			imshow("graphics_card", frame_clone);

			int count = 0;
			for (int row = 0; row < img.rows; row++)
			{
				for (int c = 0; c < img.cols; c++)
				{
					switch (g_Led_Color)
					{
					case WHITE:
						if (img.at<Vec3b>(row, c)[2] < R_Threshold 
							&& img.at<Vec3b>(row, c)[1] < G_Threshold
							&& img.at<Vec3b>(row, c)[0] < B_Threshold)
						{
							img.at<Vec3b>(row, c)[0] = 0;
							img.at<Vec3b>(row, c)[1] = 0;
							img.at<Vec3b>(row, c)[2] = 0;
						}
						else {
							count++;
						}
						break;
					case RED:
						if (img.at<Vec3b>(row, c)[2] < R_Threshold)
						{
							img.at<Vec3b>(row, c)[0] = 0;
							img.at<Vec3b>(row, c)[1] = 0;
							img.at<Vec3b>(row, c)[2] = 0;
						}
						else {
							count++;
						}
						break;
					case GREEN:
						if (img.at<Vec3b>(row, c)[1] < G_Threshold)
						{
							img.at<Vec3b>(row, c)[0] = 0;
							img.at<Vec3b>(row, c)[1] = 0;
							img.at<Vec3b>(row, c)[2] = 0;
						}
						else {
							count++;
						}
						break;
					case BLUE:
						if (img.at<Vec3b>(row, c)[0] < B_Threshold)
						{
							img.at<Vec3b>(row, c)[0] = 0;
							img.at<Vec3b>(row, c)[1] = 0;
							img.at<Vec3b>(row, c)[2] = 0;
						}else {
							count++;
						}
						break;
					}
					
				}
			}
			//printf("count1 = %d\n", count);

			// 将img 划分成竖向的8列, 统计每列中r >=240 的点数量
			
			count = 0;
			for (size_t i = 0; i < img.cols; i++)
			{
				//uchar c; //取出指定通道的颜色值
				//int t;		// 该颜色值对应的过滤阈值
				int block_index = i / (img.cols / 8);	// 该像素点所在区块

				for (size_t j = 0; j < img.rows; j++)
				{
					//int b = img.at<Vec3b>(j, i)[0];
					//int g = img.at<Vec3b>(j, i)[1];
					//uchar r = img.at<Vec3b>(j, i)[2];
					//if (i < img.cols * 1 / 8 && r >= 240)
					//{
					//	aging.point_block[0]++;
					//}
					//else if (i < img.cols * 2 / 8 && r >= 240)
					//{
					//	aging.point_block[1]++;
					//}
					//else if (i < img.cols * 3 / 8 && r >= 240)
					//{
					//	aging.point_block[2]++;
					//}
					//else if (i < img.cols * 4 / 8 && r >= 240)
					//{
					//	aging.point_block[3]++;
					//}
					//else if (i < img.cols * 5 / 8 && r >= 240)
					//{
					//	aging.point_block[4]++;
					//}
					//else if (i < img.cols * 6 / 8 && r >= 240)
					//{
					//	aging.point_block[5]++;
					//}
					//else if (i < img.cols * 7 / 8 && r >= 240)
					//{
					//	aging.point_block[6]++;
					//}
					//else if (i < img.cols && r >= 240)
					//{
					//	aging.point_block[7]++;
					//}
					
					switch (g_Led_Color)
					{
					case WHITE:
						if (img.at<Vec3b>(j, i)[2] > 0
							&& img.at<Vec3b>(j, i)[1] > 0
							&& img.at<Vec3b>(j, i)[0] > 0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					case RED:				
						if (img.at<Vec3b>(j, i)[2] >0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					case GREEN:
						if (img.at<Vec3b>(j, i)[1] >0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					case BLUE:
						if (img.at<Vec3b>(j, i)[0] >0)
						{
							aging.point_block[block_index]++;
							count++;
						}
						break;
					}
				}
			}
			//printf("count2 = %d\n", count);
			

			bool b8[8] = { 0 };	// 8个区块中, 各个区块是否有白色
			int t[2] = { -1, -1 };	// 记录区块下标
			for (int s = 0; s < 8; s++)
			{
				if (s == 0 && aging.point_block[s] > 100)
				{
					b8[s] = true;
					t[0] = s;
				}
				else if (aging.point_block[s] > 100 && !b8[s - 1])
				{
					b8[s] = true;
					if (t[0] >= 0)
						t[1] = s;
					else
						t[0] = s;
				}
				else if (s == 3 && aging.point_block[s] > 70 && !b8[s - 1])
				{
					b8[s] = true;
					if (t[0] >= 0)
						t[1] = s;
					else
						t[0] = s;
				}
			}

			aging.PPID = 1111111;
			aging.setColor(g_Led_Color);
			time(&aging.img_name);

			if (t[0] < 0 || t[1] < 0) 
			{
				const unsigned int* a8 = aging.point_block;
				char fail_img_name[256] = { 0 };
				sprintf_s(fail_img_name, 256, "./fail_img/%I64u.png", aging.img_name);

				char msg[256] = { 0 };
				sprintf_s(msg, 256, "Fail - %d", index++);
				putText(img, msg, Point(0, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "%s t[0] = %d t[1] = %d", fail_img_name, t[0], t[1]);
				putText(img, msg, Point(0, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "a[0] = %d,a[1] = %d,a[2] = %d,a[3] = %d,a[4] = %d,a[5] = %d,a[6] = %d,a[7] = %d"
					, a8[0], a8[1], a8[2], a8[3], a8[4], a8[5], a8[6], a8[7]);
				putText(img, msg, Point(0, 110), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				imwrite(fail_img_name, img);
				imshow("img_fail", img);
				waitKey(1);
				aging.result = false;
			}
			else 
			{
				const unsigned int* a8 = aging.point_block;
				char fail_img_name[256] = { 0 };
				sprintf_s(fail_img_name, 256, "./fail_img/%I64u.png", aging.img_name);

				char msg[256] = { 0 };
				sprintf_s(msg, 256, "Success - %d", index++);
				putText(img, msg, Point(0, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "%s t[0] = %d t[1] = %d", fail_img_name, t[0], t[1]);
				putText(img, msg, Point(0, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				sprintf_s(msg, 256, "a[0] = %d,a[1] = %d,a[2] = %d,a[3] = %d,a[4] = %d,a[5] = %d,a[6] = %d,a[7] = %d"
					, a8[0], a8[1], a8[2], a8[3], a8[4], a8[5], a8[6], a8[7]);
				putText(img, msg, Point(0, 110), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));

				//imwrite(fail_img_name, img);
				aging.result = true;
			}

			for (int i = 1; i < 8; i++)
				line(img, Point(img.cols / 8 * i, 0), Point(img.cols / 8 * i, img.rows), Scalar(255, 255, 0));

			imshow("imgb", img);

			aging.time_consuming = clock() - startTime;
			
			//printf("time consuming - %d\n", aging.time_consuming);

			if (g_aging_file.is_open())
			{
				//char asctime_img_name[128] = { 0 };
				//tm *lptime = gmtime(&aging.img_name);
				//sprintf_s(asctime_img_name, 128, "%d%02d%02d%02d%02d%02d"
				//	, lptime->tm_year + 1900, lptime->tm_mon, lptime->tm_mday
				//	, lptime->tm_hour, lptime->tm_hour, lptime->tm_sec);

				//char buf[256] = { 0 };
				// PPID|img_name|convert img_name to time string|r,g,b|a8[0]|a8[1]|a8[2]|a8[3]|a8[4]|a8[5]|a8[6]|a8[7]|test result
				g_aging_file << aging.PPID << ","
					<< aging.img_name << ",\""
					<< aging.color[0] << "," << aging.color[1] << "," << aging.color[2] << "\","
					<< aging.point_block[0] << ","
					<< aging.point_block[1] << ","
					<< aging.point_block[2] << ","
					<< aging.point_block[3] << ","
					<< aging.point_block[4] << ","
					<< aging.point_block[5] << ","
					<< aging.point_block[6] << ","
					<< aging.point_block[7] << ","
					<< aging.result << endl;
				g_aging_file.flush();
			}

			g_wait = false;
			printf("--------------\n");
			g_mutex_wait.unlock();
		}

		if (g_main_thread_exit)
		{
			break;
		}
	}
#endif
	t2.join();

	t1.join();
	t3.join();
	FreeLibrary(hDLL);
	g_aging_file.close();
	destroyAllWindows();
	return 0;
}