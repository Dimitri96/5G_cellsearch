#include<stdio.h>
#include <stdlib.h>
#include <math.h>

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
    printf("maximum real element is %d", max_re);
	printf("\n");
	printf("maximum imaginery element is %d", max_im);
	printf("\n");
	printf("maximum element is %d", max);
	printf("\n");
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
	float re_Array[size];
	float im_Array[size];

	//refWaveform
	FILE *fp = fopen("refWaveform", "r");
	fseek(fp, 0, SEEK_END);
	long fpsize = ftell(fp);
	fseek(fp, 0, SEEK_SET); 
	printf("fpsize is %ld", fpsize);
	printf("\n");

	short int ref_size = fpsize / 4;
	float ref_re_Array[ref_size];
	float ref_im_Array[ref_size];

	//go in steps of 2 bytes and split the data into real and imaginery numbers, each with a size of 2 bytes
	int i;
	for (i=0; i<size; i++)
	{
		fread(&data_re[i], 2, 1, f);
		fseek(f, 2, SEEK_CUR);
		fread(&data_im[i], 2, 1, f);
		fseek(f, 2, SEEK_CUR);
		
	}

	for (i=0; i<ref_size; i++)
	{
		fread(&ref_re_Array[i], 2, 1, f);
		fseek(fp, 2, SEEK_CUR);
		fread(&ref_im_Array[i], 2, 1, f);
		fseek(fp, 2, SEEK_CUR);
		
	}

	findMaximum(data_re, data_im, size);

	for(int i=0; i<size; i++)
	{
		re_Array[i] = ((float)data_re[i])/max;
		im_Array[i] = ((float)data_im[i])/max;
	}


	
	/*i = 0;
	for (i=0; i<10; i++)
	{
		printf("ref_re_Array %f\n", ref_re_Array[i]);
		printf("ref_im_Array %f\n", ref_im_Array[i]);

	}
	fclose(f);*/

	/*for (int i=0; i<size; i++)
	{
		re_Array[i] = re_Array[i] * exp();
		re_Array[i] = re_Array[i] * exp();
	}*/


	return 0;
}