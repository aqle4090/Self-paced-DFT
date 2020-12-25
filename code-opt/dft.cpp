#include<math.h>
#include "dft.h"
#include"coefficients8.h"

void dft(volatile DTYPE* real_sample, volatile DTYPE* imag_sample, volatile DTYPE* real_output, volatile DTYPE* imag_output)
{
	int i, j;
	DTYPE w;
	//DTYPE c, s;
	DTYPE c_0, s_0, c_1, s_1;

	// Temporary arrays to hold the intermediate frequency domain results
	DTYPE temp_real[SIZE];
	DTYPE temp_imag[SIZE];

	// Calculate each frequency domain sample iteratively
	for (i = 0; i < SIZE; i += 1) {
		temp_real[i] = 0;
		temp_imag[i] = 0;

		// (2 * pi * i)/N
		//w = (2.0 * 3.141592653589  / SIZE) * (DTYPE)i;

		// Calculate the jth frequency sample sequentially
		for (j = 0; j < SIZE; j += 2) {
			// Utilize HLS tool to calculate sine and cosine values
            //#pragma HLS pipeline II=2
			//c = cos_table[i * j % N];
			//s = sin_table[i * j % N];
			c_0 = cos_coefficients_table[i * j % SIZE];
			s_0 = sin_coefficients_table[i * j % SIZE];
		    c_1 = cos_coefficients_table[i * (j + 1) % SIZE];
			s_1 = sin_coefficients_table[i * (j + 1) % SIZE];
			// Multiply the current phasor with the appropriate input sample and keep
			// running sum
			temp_real[i] += (real_sample[j] * c_0 - imag_sample[j] * s_0) + (real_sample[j + 1] * c_1 - imag_sample[j + 1] * s_1);
			temp_imag[i] += (real_sample[j] * s_0 + imag_sample[j] * c_0) + (real_sample[j + 1] * s_1 + imag_sample[j + 1] * c_1);
		}
	}

	for (i = 0; i < SIZE; i += 1) {
		real_output[i] = temp_real[i];
		imag_output[i] = temp_imag[i];
	}
}
