#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <opencv2/opencv.hpp>
#include "gaussian_filters.h"

using namespace std;
using namespace cv;

bool show_matrix_raw_gray(const char *image_name, unsigned char *image, int width, int height)
{
	cv::Mat matrix(height, width, CV_8UC1);

	memcpy(matrix.data, image, sizeof(unsigned char)*width*height);

	cv::imshow(cv::String(image_name), matrix);
	cv::waitKey(0);
	cv::destroyWindow(cv::String(image_name));

	return true;
}

int main(int argc, char *argv[])
{
	Mat mat = imread(argv[1], IMREAD_GRAYSCALE);
	if (mat.empty())
	{
		printf("Cannot read image, please check the path\n");
		return -1;
	}

	int repetition = 1;
	if(argc > 2)
		repetition = stoi(argv[2]);;

	unsigned char *img_raw = mat.data;

	show_matrix_raw_gray("Original image gray", img_raw, mat.size().width, mat.size().height);

	unsigned char *img_blurred = (unsigned char *)malloc(mat.size().width * mat.size().height);
	clock_t cStart, cEnd;
	double timeRan1, timeRan2, timeRan3;

	// below start running floating point ver. gaussian kernel filter

	cStart = clock();
	for(int i=0;i<repetition;i++)
		gaussian_filter_floating_point(img_raw, img_blurred, mat.size().width, mat.size().height, G_kernel_7x7_Sigma_2_FP_2D, 7);
	cEnd = clock();

	show_matrix_raw_gray("Blurred image - floating point", img_blurred, mat.size().width, mat.size().height);

	timeRan1 = ((double)(cEnd-cStart)) / CLOCKS_PER_SEC;

	printf("\n");
	printf("(1) Running floating point gaussian kernel 7x7\n");
	printf(" Totally taking %.5f secs for %d image(s) processing, average : %.5f sec\n\n", timeRan1, repetition, timeRan1/repetition);


	// below start running int ver. gaussian kernel filter with right-shifting as sum division

	cStart = clock();
	for (int i = 0; i<repetition; i++)
		gaussian_filter_int(img_raw, img_blurred, mat.size().width, mat.size().height, G_kernel_7x7_Sigma_2_INT_2D, 7, 12);
	cEnd = clock();

	show_matrix_raw_gray("Blurred image - Integer", img_blurred, mat.size().width, mat.size().height);

	timeRan2 = ((double)(cEnd - cStart)) / CLOCKS_PER_SEC;

	printf("\n");
	printf("(2) Running integer-division-shift gaussian kernel 7x7\n");
	printf(" Totally taking %.5f secs for %d image(s) processing, average : %.5f sec\n\n", timeRan2, repetition, timeRan2 / repetition);


	// below start running fast int ver. gaussian kernel filter with 2-phase method and right-shifting as sum division

	unsigned int *tmp_buffer = (unsigned int *)malloc(sizeof(unsigned int) * mat.size().width * mat.size().height);

	cStart = clock();
	for (int i = 0; i<repetition; i++)
		gaussian_filter_int_2_phase_lossy(img_raw, img_blurred, mat.size().width, mat.size().height, G_kernel_7x7_Sigma_2_INT_1D, 7, 12, tmp_buffer);
	cEnd = clock();

	show_matrix_raw_gray("Blurred image - Integer Fast", img_blurred, mat.size().width, mat.size().height);

	timeRan3 = ((double)(cEnd - cStart)) / CLOCKS_PER_SEC;

	printf("\n");
	printf("(3) Running Integer Fast gaussian kernel 7x7\n");
	printf(" Totally taking %.5f secs for %d image(s) processing, average : %.5f sec\n\n", timeRan3, repetition, timeRan3 / repetition);


	// summary print
	printf("== Algorithms comparsion ==\n");
	printf("Algorithm (2) is %.1f-times faster than (1)\n", (float)timeRan1/timeRan2);
	printf("Algorithm (3) is %.1f-times faster than (2)\n", (float)timeRan2/timeRan3);
	printf("Algorithm (3) is %.1f-times faster than (1)\n", (float)timeRan1/timeRan3);

	free(img_blurred);
	free(tmp_buffer);
	mat.release();
	return 0;
}