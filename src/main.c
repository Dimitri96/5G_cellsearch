#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

short int max;

void findMaximum(short int arr1[], short int arr2[], int N)
{
    int i;
    // variable to store the maximum element
    short int max_im = arr1[0];
	short int max_re = arr2[0];
    // Traverse the given array
    for (i = 0; i < N; i++)
    {
        // If current element is greater
        // than max then update it
        if (arr1[i] > max_re)
        {
            max_re = arr1[i];
        }

		if (arr2[i] > max_im)
        {
            max_im = arr2[i];
        }

		if (max_re > max_im) 
		{
    		max = max_re;
		}
		else 
		{
    		 max = max_im;
		}
    }
    // Print the maximum element
    /*printf("maximum real element is %d", max_re);
	printf("\n");
	printf("maximum imaginery element is %d", max_im);
	printf("\n");
	printf("maximum element is %d", max);
	printf("\n");*/
}


int main() {
	// 1. first read in the file from the data folder
	//1. define pointers to read in data
	short int *data_im;
	short int *data_re;

	//short int **data_im_start = &data_im;
	//short int **data_re_start = &data_re;
	//2. allocate memory for the data to read in
	FILE *f = fopen("data/30720KSPS_dl_signal.sigmf-data", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f);*/

	short int size = fsize / 4;
	data_im = (short int*) malloc(size*sizeof(short int));
	data_re = (short int*) malloc(size*sizeof(short int));
	double re_Array[size];
	double im_Array[size];

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
	double re_Array_ref[count];
	double im_Array_ref[count];
	fp = fopen("refWaveform", "r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int current_position = 0;
	char *token;
	while((read = getline(&line, &len, fp)) !=-1)
	{
		token = strtok(line, " + "); //split on " + "
		re_Array_ref[current_position] = atof(token); //atof reads in current token value into Array and parses as double
		token = strtok(NULL, " + "); //split on " + "
		token[strlen(token)-1] = '\0'; //remove the j 
		im_Array_ref[current_position] = atof(token);
		current_position++;
	}


	/*for(int i=0; i<10; i++)
	{
		printf("refWaveform:");
		printf("%f + %f", re_Array_ref[i], im_Array_ref[i]);
		printf("\n");
	}*/
	

	//go in steps of 2 bytes and split the data into real and imaginery numbers, each with a size of 2 bytes
	int i;
	for (i=0; i<size; i++)
	{
		fread(&data_re[i], 2, 1, f);
		fseek(f, 2, SEEK_CUR);
		fread(&data_im[i], 2, 1, f);
		fseek(f, 2, SEEK_CUR);
		
	}

	findMaximum(data_re, data_im, size);

	for(int i=0; i<size; i++)
	{
		re_Array[i] = ((double)data_re[i])/max;
		im_Array[i] = ((double)data_im[i])/max;
	}

	/*for(int i=0; i<10; i++)
	{
		printf("Waveform:");
		printf("%f + %f", re_Array[i], im_Array[i]);
		printf("\n");
	}*/
	FILE *fp2;
	fp2 = fopen("Data.txt", "w");
	for(int i=0; i<size; i++)
	{
		fprintf(fp2, "%f + %fi", re_Array[i], im_Array[i]);
		fprintf(fp2, "\n");
	}
	fclose(fp2);



//correlation
double re_Array_result[size];
double im_Array_result[size];

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<count; j++)
		{
		re_Array_result[i] = (re_Array[i]*re_Array_ref[j]) - (im_Array[i]*im_Array_ref[j]);
		im_Array_result[i] = (re_Array[i]*im_Array_ref[j]) + (im_Array[i]*re_Array_ref[j]);
		}
	}

	//find out length of array re_Array_result
	
	/*
	int length = sizeof(re_Array_result)/sizeof(re_Array_result[0]);
	printf("length: %d", length);
	printf("\n");

	//find out and print 2049th position of the array re_Array im_Array
	printf("2049 re im : %f + %fi", re_Array[2048], im_Array[2048]);
	printf("\n");

	//find out and print 1st position of the array ref
	printf("1 ref : %f + %fi", re_Array_ref[0], im_Array_ref[0]);
	printf("\n");

	//find out and print the 2049 position of the array re_Array_result
	printf("2049 result: %f + %fi", re_Array_result[2048], im_Array_result[2048]);
	printf("\n");
	*/


	//print the result of the multiplication
	for(int i=0; i<10; i++)
	{
		printf("result:");
		printf("%f + %fi", re_Array_result[i], im_Array_result[i]);
		printf("\n");
	}

	//write the results to a text file
	FILE *fp1;
	fp1 = fopen("result.txt", "w");
	for(int i=0; i<size; i++)
	{
		fprintf(fp1, "%f + %fi", re_Array_result[i], im_Array_result[i]);
		fprintf(fp1, "\n");
	}
	fclose(fp1);



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

