#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <complex.h> 
#include <stdint.h>


//function to read in data 4 bytes at a time 
void read_data(FILE *f, int16_t *re, int16_t *im, int size) {
	for (int i = 0; i < size; i++) {
		int16_t temp = 0;
		fread(&re[i], sizeof(temp), 1, f);
		fread(&im[i], sizeof(temp), 1, f);
	}
}

int main() {

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
	int64_t *all = (int64_t *)malloc(size * sizeof(int64_t));
	float *data_re = (float *)malloc(size * sizeof(float));
	float *data_im = (float *)malloc(size * sizeof(float));

	//read in data from file into arrays
	for(int i = 0; i < size; i++) 
	{	
		uint64_t temp = 0;
		fread((void*)(&temp), sizeof(temp), 1, f);
		all[i] = temp;
		re[i] = (int32_t)(temp & 0xFFFFFFFF);
		im[i] = (int32_t)((temp >> sizeof(temp)/2) & 0xFFFFFFFF);
	}

	//scale the arrays (arrays / max value of arrays)
	int32_t max = 0;
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

	//print re an im arrays 
	for (int i = 0; i < 10; i++) {
		printf("%f %f\n", data_re[i], data_im[i]);
	}


	//multiply data_re and data_im by exponential complex function
	/*double data_re_exp[size];
	double data_im_exp[size];
	for(int i = 0; i < size; i++) 
	{
		data_re_exp[i] = data_re[i] * cos(2 * M_PI * i / size) - data_im[i] * sin(2 * M_PI * i / size);
		data_im_exp[i] = data_re[i] * sin(2 * M_PI * i / size) + data_im[i] * cos(2 * M_PI * i / size);
	}*/


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
    // Close the file
    fclose(fp);
	
	//refWaveform Arrays
	double re_data_ref[count];
	double im_data_ref[count];
	fp = fopen("refWaveform", "r");
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

/*
	//correlation
	double re_data_result[size];
	double im_data_result[size];

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<count; j++)
		{
		re_data_result[i] = (data_re_exp[i]*re_data_ref[j]) - (data_im_exp[i]*im_data_ref[j]);
		im_data_result[i] = (data_re_exp[i]*im_data_ref[j]) + (data_im_exp[i]*re_data_ref[j]);
		}
	}

	

	//write the results to a text file
	FILE *fp1;
	fp1 = fopen("result.txt", "w");
	for(int i=0; i<size; i++)
	{
		fprintf(fp1, "%f + %fi", re_data_result[i], im_data_result[i]);
		fprintf(fp1, "\n");
	}
	fclose(fp1);
*/


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

