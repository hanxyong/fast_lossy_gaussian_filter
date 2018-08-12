#include "gaussian_filters.h"

int gaussian_filter_floating_point(unsigned char *input, unsigned char *output, int image_width, int image_height,
	const double gaussian_kernel[], int kernel_size)
{
	int hksize = kernel_size / 2;

	for (int r = 0; r < image_height; r++) // image row
	{
		for (int c = 0; c < image_width; c++) // image col
		{
			double accSum = 0;
			int targetPix = r * image_width + c;
			for (int m = 0; m < kernel_size; m++)	// kernel row
			{
				for (int n = 0; n < kernel_size; n++) // kernel col
				{
					int cRow = r - hksize + m;
					int cCol = c - hksize + n;
					if (cRow >= 0 && cRow < image_height && cCol >= 0 && cCol < image_width) {
						int cPix = cRow * image_width + cCol;
						accSum += gaussian_kernel[m*kernel_size + n] * input[cPix];
					}
				}
			}
			output[targetPix] = (unsigned int) accSum;
		}
	}

	return 1;
}

int gaussian_filter_int(unsigned char *input, unsigned char *output, int image_width, int image_height,
	const unsigned int gaussian_kernel[], int kernel_size, int div_shift)
{
	int hksize = kernel_size / 2;

	for (int r = 0; r < image_height; r++) // image row
	{
		for (int c = 0; c < image_width; c++) // image col
		{
			unsigned int accSum = 0;
			int targetPix = r * image_width + c;
			for (int m = 0; m < kernel_size; m++)	// kernel row
			{
				for (int n = 0; n < kernel_size; n++) // kernel col
				{
					int cRow = r - hksize + m;
					int cCol = c - hksize + n;
					if (cRow >= 0 && cRow < image_height && cCol >= 0 && cCol < image_width) {
						int cPix = cRow * image_width + cCol;
						accSum += gaussian_kernel[m*kernel_size + n] * input[cPix];
					}
				}
			}
			output[targetPix] = accSum >> div_shift;
		}
	}

	return 1;
}

int gaussian_filter_int_2_phase_lossy(unsigned char *input, unsigned char *output, int image_width, int image_height,
	const unsigned int gaussian_kernel[], int kernel_size, int div_shift, unsigned int *tmpbuf)
{
	int offset = kernel_size / 2;
	for (int i = offset; i < image_width*image_height - offset; i++) {
		tmpbuf[i] = 0;
		for (int n = 0; n < kernel_size; n++)
			tmpbuf[i] += gaussian_kernel[n] * (input[i + (n - offset)]);
	}

	for (int i = offset * image_width; i < image_width*image_height - offset * image_width; i++) {
		unsigned int tempSum = 0;
		for (int n = 0; n < kernel_size; n++)
			tempSum += gaussian_kernel[n] * tmpbuf[i + (n - offset) * image_width];
		output[i] = tempSum >> div_shift;
	}
	return 1;
}