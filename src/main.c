#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <complex.h> 
#include <stdint.h>
#include <arm_neon.h>



int main() {

	int delta_f = 0;
	int f_s = 30720000;


	//open file 
	FILE *f;
	f = fopen("data/30720KSPS_dl_signal.sigmf-data", "rb");

	//find length of file 
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	//define size of arrays
	int size;
	size = fsize / 8;

	//define arrays to read in data (data_re, data_im)
	int32_t *re = (int32_t *)malloc(size * sizeof(int32_t));
	int32_t *im = (int32_t *)malloc(size * sizeof(int32_t));
	float *data_re = (float *)malloc(size * sizeof(float));
	float *data_im = (float *)malloc(size * sizeof(float));
	uint64_t temp = 0;

	//read in data from file into arrays
	for(int i = 0; i < size; i++) 
	{	
		fread((void*)(&temp), sizeof(temp), 1, f);
		re[i] = (int32_t)(temp & 0xFFFFFFFF);
		im[i] = (int32_t)((temp >> 32));
		/*if (i == 0){
			printf("temp=%lx re=%d im=%d\n", temp, re[0], im[0]);
		}*/		
	}

	//find biggest value in re and im 
	int max = 0;
	for (int i = 0; i < size; i++) {
		if (re[i] > max) {
			max = re[i];
		}
		if (im[i] > max) {
			max = im[i];
		}
	}

	for (int i = 0; i < size; i++) 
	{
		data_re[i] = (float)re[i] / max;
		data_im[i] = (float)im[i] / max;
	}
	free(re);
	free(im);
	fclose(f);

	
	//REFWAVEFORM
	
	//get size of refWaveform
	FILE *fp;
    int count = 0; // Line counter (result)
    char c; // To store a character read from file
	fp = fopen("refWaveform", "r");
	for (c = getc(fp); c != EOF; c = getc(fp))
	{
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
	}
    //set file pointer to the beginning
	fseek(fp, 0, SEEK_SET);
	
	//refWaveform Arrays
	float re_data_ref[count];
	float im_data_ref[count];
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int current_position = 0;
	char *token;
	while((read = getline(&line, &len, fp)) !=-1)
	{
		token = strtok(line, " + "); //split on " + "
		re_data_ref[current_position] = atof(token); //atof reads in current token value into Array and parses as double
		token = strtok(NULL, " + "); //split on " + "
		token[strlen(token)-1] = '\0'; //remove the j 
		im_data_ref[current_position] = atof(token);
		current_position++;
	}
	fclose(fp);

	for(int i = 0; i < count; i++)
	{
		im_data_ref[i] *= -1;
	}

	int sample_rate = 30720000;
	int size_result = 0.025*sample_rate;
	int k = 0;
	float *re_data_result = (float *)malloc(size_result * sizeof(float));
	float *im_data_result = (float *)malloc(size_result * sizeof(float));

	//benchmark how fast the computation is done
	clock_t start, end;
	float cpu_time_used;
	start = clock();
	//correlate data_re and data_im with re_data_ref and im_data_ref
	for(int i=0; i<size_result; i++)
	{
		re_data_result[i] = 0;
		im_data_result[i] = 0;
		for(int j=0; j<count; j++)
		{
			re_data_result[i] += data_re[j+i]*re_data_ref[j] - data_im[j+i]*im_data_ref[j];
			im_data_result[i] += data_re[j+i]*im_data_ref[j] + data_im[j+i]*re_data_ref[j];
		}
	}
	end = clock();
	cpu_time_used = ((float) (end - start)) / CLOCKS_PER_SEC;
	printf("time used: %f", cpu_time_used);
	printf("\n");

	start = clock();
	int neon_result = size_result/4;
	int neon_count = count/4;
	float *re_data_result1 = (float *)malloc(size_result * sizeof(float));
	float *im_data_result1 = (float *)malloc(size_result * sizeof(float));
	float32x4_t *data_re_vec = (float32x4_t *)malloc(neon_result * sizeof(float32x4_t));
	float32x4_t *data_im_vec = (float32x4_t *)malloc(neon_result * sizeof(float32x4_t));
	float32x4_t *re_data_ref_vec = (float32x4_t *)malloc(neon_count * sizeof(float32x4_t));
	float32x4_t *im_data_ref_vec = (float32x4_t *)malloc(neon_count * sizeof(float32x4_t));
	float32x4_t *re_data_result_vec = (float32x4_t *)malloc(neon_result * sizeof(float32x4_t));
	float32x4_t *im_data_result_vec = (float32x4_t *)malloc(neon_result * sizeof(float32x4_t));
	/*for(int i=0; i<size_result; i++)
	{
		data_re_vec[i] = vdupq_n_f32(data_re);
		data_im_vec[i] = vdupq_n_f32(data_im);
	}
	for(int i=0; i<count; i++)
	{
		re_data_ref_vec[i] = vdupq_n_f32(re_data_ref);
		im_data_ref_vec[i] = vdupq_n_f32(im_data_ref);
	}*/
	//load data into vectors
	for(int i=0; i<neon_result; i++)
	{
		data_re_vec[i] = vld1q_f32(data_re+i*4);
		data_im_vec[i] = vld1q_f32(data_im+i*4);
	}
	for(int i=0; i<neon_count; i++)
	{
		re_data_ref_vec[i] = vld1q_f32(re_data_ref+i*4);
		im_data_ref_vec[i] = vld1q_f32(im_data_ref+i*4);
	}
	//initialize result vectors
	for(int i=0; i<neon_result; i++)
	{
		re_data_result_vec[i] = vdupq_n_f32(0);
		im_data_result_vec[i] = vdupq_n_f32(0);
	}
	for(int i=0; i<neon_result; i++)
	{
		for(int j=0; j<neon_count; j++)
		{
			//re_data_result_vec[i] =  vsubq_f32(vmulq_f32(data_re_vec[j+i], re_data_ref_vec[j]), vmulq_f32(data_im_vec[j+i], im_data_ref_vec[j]));
			//im_data_result_vec[i] =  vaddq_f32(vmulq_f32(data_re_vec[j+i], re_data_ref_vec[j]), vmulq_f32(data_im_vec[j+i], im_data_ref_vec[j]));
			re_data_result_vec[i] = vmlaq_f32(re_data_result_vec[i], data_re_vec[j+i], re_data_ref_vec[j]);
			re_data_result_vec[i] = vmlsq_f32(re_data_result_vec[i], data_im_vec[j+i], im_data_ref_vec[j]);
			im_data_result_vec[i] = vmlaq_f32(im_data_result_vec[i], data_re_vec[j+i], im_data_ref_vec[j]);
			im_data_result_vec[i] = vmlaq_f32(im_data_result_vec[i], data_im_vec[j+i], re_data_ref_vec[j]);
		}
	}
	for(int i=0; i<size_result; i++)
	{
		vst1q_f32(re_data_result1+i*4, re_data_result_vec[i]);
		vst1q_f32(im_data_result1+i*4, im_data_result_vec[i]);
	}
	end = clock();
	cpu_time_used = ((float) (end - start)) / CLOCKS_PER_SEC;
	printf("time used with neon: %f", cpu_time_used);
	printf("\n");


	
	


	//write the results to a text file
	FILE *fp1;
	fp1 = fopen("result_re", "w");
	for(int i=0; i<size_result; i++)
	{
		fprintf(fp1, "%f", re_data_result[i]);
		fprintf(fp1, "\n");
	}
	fclose(fp1);


	FILE *fp2;
	fp2 = fopen("result_im", "w");
	for(int i=0; i<size_result; i++)
	{
		fprintf(fp2, "%f", im_data_result[i]);
		fprintf(fp2, "\n");
	}
	fclose(fp2);

	FILE *fp3;
	fp3 = fopen("result_re1", "w");
	for(int i=0; i<size_result; i++)
	{
		fprintf(fp3, "%f", re_data_result1[i]);
		fprintf(fp3, "\n");
	}
	fclose(fp3);


	FILE *fp4;
	fp4 = fopen("result_im1", "w");
	for(int i=0; i<size_result; i++)
	{
		fprintf(fp4, "%f", im_data_result1[i]);
		fprintf(fp4, "\n");
	}
	fclose(fp4);

	return 0;



}