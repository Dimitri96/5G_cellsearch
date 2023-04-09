#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <complex.h> 
#include <stdint.h>



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

	//print the 2049th value of data_re and data_im



	/*//NOISE
	//read in noise data and print error if file cant be opened
	FILE *f1;
	f1 = fopen("noise", "r");
	int sze = 0; // Line counter (result)
    char b; // To store a character read from file
	for (b = getc(f1); b != EOF; b = getc(f1))
	{
        if (b == '\n') // Increment count if this character is newline
            sze = sze + 1;
	}
	//set file pointer to the beginning
    fseek(f1, 0, SEEK_SET);
	

	//define arrays and read in the data 
	float *noise_re = (float *)malloc(sze * sizeof(float));
	float *noise_im = (float *)malloc(sze * sizeof(float));
	char *line1 = NULL;
	size_t len1 = 0;
	ssize_t read1;
	int current_position1 = 0;
	char *token1;
	while((read1 = getline(&line1, &len1, f1)) !=-1)
	{
		token1 = strtok(line1, " + "); //split on " + "
		noise_re[current_position1] = atof(token1); //atof reads in current token value into Array and parses as double
		token1 = strtok(NULL, " + "); //split on " + "
		token1[strlen(token1)-1] = '\0'; //remove the j 
		noise_im[current_position1] = atof(token1);
		current_position1++;
	}
	fclose(f1);*/


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




	//Multiply waveform with complex exponential function
	/*complex x = exp(0);
	for(int i=0; i<size; i++)
	{

		data_re[i] = (data_re[i]*creal(x)) - (data_im[i]*cimag(x));
		data_im[i] = (data_re[i]*cimag(x)) + (data_im[i]*creal(x));
	}*/

	/*for(int i=0; i<10; i++)
	{
		printf("%f + %f\n", data_re[i], data_im[i]);
	}*/



	//add data_re with noise_re and data_im with noise_im
	/*for(int i=0; i<size; i++)
	{
		data_re[i] = data_re[i] + noise_re[i];
		data_im[i] = data_im[i] + noise_im[i];
	}*/

	//correlation
	for(int i = 0; i < size; i++)
	{
		data_im[i] *= -1;
	}

	for(int i = 0; i < count; i++)
	{
		im_data_ref[i] *= -1;
	}

	int sample_rate = 30720000;
	int size_result = 0.025*sample_rate;
	int k = 0;
	float *re_data_result = (float *)malloc(size_result * sizeof(float));
	float *im_data_result = (float *)malloc(size_result * sizeof(float));

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
		/*while(k < size_result/count)
	{
		for(int i = 0; i < size_result; i+=count)
		{	
			for(int j=0; j<count; j++)
			{

				re_data_result[j+i] = (data_re[j+i]*re_data_ref[j]) - (data_im[j+i]*im_data_ref[j]);
				im_data_result[j+i] = (data_re[j+i]*im_data_ref[j]) + (data_im[j+i]*re_data_ref[j]);
			}
		}
		k++;
	}*/


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
	


	//benchmark how fast the computation is done
	/*clock_t start, end;
	float cpu_time_used;
	start = clock();
	//do something
	end = clock();
	cpu_time_used = ((float) (end - start)) / CLOCKS_PER_SEC;
	printf("time used: %f", cpu_time_used);
	printf("\n");*/

	return 0;



}


