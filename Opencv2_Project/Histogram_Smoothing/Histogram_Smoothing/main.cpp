#include "opencv2\highgui.hpp"
#include <stdio.h>
#include <iostream>
#include "opencv2\imgproc.hpp"
#include "opencv2\core.hpp"

using namespace std;
using namespace cv;


int main()
{
	Mat img_input, img_gray, img_result, img_histogram, img_histogram2;

	//�̹��� ������ �о�ͼ� img_input�� ����
	img_input = imread("ex3.png", IMREAD_COLOR);
	if (img_input.empty())
	{
		cout << "������ �о�ü� �����ϴ�." << endl;
		exit(1);
	}


	//�Է¿����� �׷��̽����� �������� ��ȯ
	img_gray = Mat(img_input.rows, img_input.cols, CV_8UC1);

	for (int y = 0; y < img_input.rows; y++)
	{
		for (int x = 0; x < img_input.cols; x++)
		{
			//img_input���κ��� ���� ��ġ (y,x) �ȼ���
			//blue, green, red ���� �о�´�. 
			uchar blue = img_input.at<Vec3b>(y, x)[0];
			uchar green = img_input.at<Vec3b>(y, x)[1];
			uchar red = img_input.at<Vec3b>(y, x)[2];

			//blue, green, red�� ���� ��, 3���� ������ �׷��̽������� �ȴ�.
			uchar gray = (blue + green + red) / 3.0;

			//MatŸ�� ���� img_gray�� �����Ѵ�. 
			img_gray.at<uchar>(y, x) = gray;
		}
	}


	//�Է� �׷��̽����� ������ ������׷� ���
	int histogram[256] = { 0, };

	for (int y = 0; y < img_input.rows; y++)
	{
		for (int x = 0; x < img_input.cols; x++)
		{
			int value = img_gray.at<uchar>(y, x);
			histogram[value] += 1;
		}
	}

	//�Է� �׷��̽����� ������ ���� ������׷� ���
	int cumulative_histogram[256] = { 0, };
	int sum = 0;

	for (int i = 1; i < 256; i++)
	{
		sum += histogram[i];
		cumulative_histogram[i] = sum;
	}

	//�Է� �׷��̽����� ������ ����ȭ�� ���� ������׷� ���
	float normalized_cumulative_histogram[256] = { 0.0, };
	int image_size = img_input.rows * img_input.cols;

	for (int i = 0; i < 256; i++)
	{
		normalized_cumulative_histogram[i] = cumulative_histogram[i] / (float)image_size;
	}


	//������׷� ��Ȱȭ ���� �� ��� ������ ������׷� ���
	img_result = Mat(img_input.rows, img_input.cols, CV_8UC1);
	int histogram2[256] = { 0, };
	for (int y = 0; y < img_input.rows; y++)
	{
		for (int x = 0; x < img_input.cols; x++)
		{
			img_result.at<uchar>(y, x) = normalized_cumulative_histogram[img_gray.at<uchar>(y, x)] * 255;
			histogram2[img_result.at<uchar>(y, x)] += 1;
		}
	}


	//��� ������ ���� ������׷� ���
	int cumulative_histogram2[256] = { 0, };
	sum = 0;

	for (int i = 1; i < 256; i++)
	{
		sum += histogram2[i];
		cumulative_histogram2[i] = sum;
	}


	//������׷� �׸���
	img_histogram = Mat(300, 600, CV_8UC1, Scalar(0));
	img_histogram2 = Mat(300, 600, CV_8UC1, Scalar(0));

	int max = -1;
	for (int i = 0; i < 256; i++)
		if (max < histogram[i]) max = histogram[i];

	int max2 = -1;
	for (int i = 0; i < 256; i++)
		if (max2 < histogram2[i]) max2 = histogram2[i];

	for (int i = 0; i < 256; i++)
	{
		int histo = 300 * histogram[i] / (float)max;
		int cumulative_histo = 300 * cumulative_histogram[i] / (float)cumulative_histogram[255];

		line(img_histogram, Point(i + 10, 300), Point(i + 10, 300 - histo), Scalar(255, 255, 255));
		line(img_histogram, Point(i + 300, 300), Point(i + 300, 300 - cumulative_histo), Scalar(255, 255, 255));


		int histo2 = 300 * histogram2[i] / (float)max2;
		int cumulative_histo2 = 300 * cumulative_histogram2[i] / (float)cumulative_histogram2[255];

		line(img_histogram2, Point(i + 10, 300), Point(i + 10, 300 - histo2), Scalar(255, 255, 255));
		line(img_histogram2, Point(i + 300, 300), Point(i + 300, 300 - cumulative_histo2), Scalar(255, 255, 255));
	}


	//ȭ�鿡 ��� �̹����� �����ش�.
	imshow("�Է� ����", img_input);
	imshow("�Է� �׷��̽����� ����", img_gray);
	imshow("��� �׷��̽����� ����", img_result);
	imshow("�Է� ������ ������׷�", img_histogram);
	imshow("��Ȱȭ �� ������׷�", img_histogram2);

	//�ƹ�Ű�� ������ ������ ���
	while (cv::waitKey(0) == 0);

	//����� ���Ϸ� ����
	imwrite("img_gray.jpg", img_gray);
	imwrite("img_result.jpg", img_result);
	imwrite("img_histogram.jpg", img_histogram);
	imwrite("img_histogram2.jpg", img_histogram2);
}