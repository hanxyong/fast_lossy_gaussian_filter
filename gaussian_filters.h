#pragma once

// Gaussian Kernel calculator
// http://dev.theomader.com/gaussian-kernel-calculator/

// Gaussian Kernel 7x7 sigma=2, floatint point, 2 dimension
const double G_kernel_7x7_Sigma_2_FP_2D[] = {
	0.005084, 0.009377, 0.013539, 0.015302, 0.013539, 0.009377, 0.005084,
	0.009377, 0.017296, 0.024972, 0.028224, 0.024972, 0.017296, 0.009377,
	0.013539, 0.024972, 0.036054, 0.040749, 0.036054, 0.024972, 0.013539,
	0.015302, 0.028224, 0.040749, 0.046056, 0.040749, 0.028224, 0.015302,
	0.013539, 0.024972, 0.036054, 0.040749, 0.036054, 0.024972, 0.013539,
	0.009377, 0.017296, 0.024972, 0.028224, 0.024972, 0.017296, 0.009377,
	0.005084, 0.009377, 0.013539, 0.015302, 0.013539, 0.009377, 0.005084
};

// Gaussian Kernel 7x7 sigma=2, integer (lossy comparing to floating point kernel)
// 2 dimension, SUM = 4096 (for right-shifting 12 bits)
const unsigned int G_kernel_7x7_Sigma_2_INT_2D[] = {
	21, 38, 55, 63, 55, 38, 21,
	38, 71, 102, 116, 102, 71, 38,
	55, 102, 148, 167, 148, 102, 55,
	63, 116, 167, 192, 167, 116, 63,
	55, 102, 148, 167, 148, 102, 55,
	38, 71, 102, 116, 102, 71, 38,
	21, 38, 55, 63, 55, 38, 21
};

// Gaussian Kernel 7x7 sigma=2, integer (more lossy comparing to 2D kernel)
// 1 dimension, SUM = 64 (64x64 = 4096 for right-shifting 12 bits)
// This is for 2-phase gaussian filter
const unsigned int G_kernel_7x7_Sigma_2_INT_1D[] = { 5, 8, 12, 14, 12, 8, 5 };


// This is a normal gaussian filter with floating point kernel
int gaussian_filter_floating_point(unsigned char *input, unsigned char *output, int image_width, int image_height,
	const double gaussian_kernel[], int kernel_size);

// This is a discrete gaussian filter but using right-shift to avoid division
int gaussian_filter_int(unsigned char *input, unsigned char *output, int image_width, int image_height,
	const unsigned int gaussian_kernel[], int kernel_size, int div_shift);

// This uses 2-phase discrete gaussian filter and with lossy edges but gaining very good performance
int gaussian_filter_int_2_phase_lossy(unsigned char *input, unsigned char *output, int image_width, int image_height,
	const unsigned int gaussian_kernel[], int kernel_size, int div_shift, unsigned int *tmpbuf);
