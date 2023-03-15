#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
		

	}
	

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

//(ac -bd) + i(ad +bc) multiplying two complex numbers



	return 0;
}