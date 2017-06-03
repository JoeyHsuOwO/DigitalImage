#include<stdio.h>
#include<stdlib.h>

typedef long INT32;
typedef unsigned short int INT16;
typedef unsigned char U_CHAR;

#define UCH(x)	((int) (x))
#define GET_2B(array,offset)  ((INT16) UCH(array[offset]) + \
			       (((INT16) UCH(array[offset+1])) << 8))
#define GET_4B(array,offset)  ((INT32) UCH(array[offset]) + \
			       (((INT32) UCH(array[offset+1])) << 8) + \
			       (((INT32) UCH(array[offset+2])) << 16) + \
			       (((INT32) UCH(array[offset+3])) << 24))
#define FREAD(file,buf,sizeofbuf)  \
  ((size_t) fread((void *) (buf), (size_t) 1, (size_t) (sizeofbuf), (file)))


int main()
{
	FILE *input_file = 0;
	FILE *output_file = 0;

	U_CHAR bmpfileheader[14] = { 0 };
	U_CHAR bmpinfoheader[40] = { 0 };

	//U_CHAR bmpinfoheader[40] = { 0 } ;

	INT32 FileSize = 0;
	INT32 bfOffBits = 0;
	INT32 headerSize = 0;
	INT32 biWidth = 0;
	INT32 biHeight = 0;
	INT16 biPlanes = 0;
	INT16 BitCount = 0;
	INT32 biCompression = 0;
	INT32 biImageSize = 0;
	INT32 biXPelsPerMeter = 0, biYPelsPerMeter = 0;
	INT32 biClrUsed = 0;
	INT32 biClrImp = 0;

	U_CHAR *data, *new_data, color_table[1024];
	int i, j, k, temp;
	int histo_table[256] = { 0 };
	int histo_new_table[256] = { 0 };
	int histo_reverse_table[256] = { 0 };

	/* 開啟檔案 */
	if ((input_file = fopen("Fig3.23(a ).bmp", "rb")) == NULL) {
		fprintf(stderr, "File can't open.\n");
		exit(0);
	}


	FREAD(input_file, bmpfileheader, 14);
	FREAD(input_file, bmpinfoheader, 40);

	if (GET_2B(bmpfileheader, 0) == 0x4D42) /* 'BM' */
		fprintf(stdout, "BMP file.\n");
	else {
		fprintf(stdout, "Not bmp file.\n");
		exit(0);
	}

	FileSize = GET_4B(bmpfileheader, 2);
	bfOffBits = GET_4B(bmpfileheader, 10);
	headerSize = GET_4B(bmpinfoheader, 0);
	biWidth = GET_4B(bmpinfoheader, 4);
	biHeight = GET_4B(bmpinfoheader, 8);
	biPlanes = GET_2B(bmpinfoheader, 12);
	BitCount = GET_2B(bmpinfoheader, 14);
	biCompression = GET_4B(bmpinfoheader, 16);
	biImageSize = GET_4B(bmpinfoheader, 20);
	biXPelsPerMeter = GET_4B(bmpinfoheader, 24);
	biYPelsPerMeter = GET_4B(bmpinfoheader, 28);
	biClrUsed = GET_4B(bmpinfoheader, 32);
	biClrImp = GET_4B(bmpinfoheader, 36);

	printf("FileSize = %ld \n"
		"DataOffset = %ld \n"
		"HeaderSize = %ld \n"
		"Width = %ld \n"
		"Height = %ld \n"
		"Planes = %d \n"
		"BitCount = %d \n"
		"Compression = %ld \n"
		"ImageSize = %ld \n"
		"XpixelsPerM = %ld \n"
		"YpixelsPerM = %ld \n"
		"ColorsUsed = %ld \n"
		"ColorsImportant = %ld \n", FileSize, bfOffBits, headerSize, biWidth, biHeight, biPlanes,
		BitCount, biCompression, biImageSize, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImp);

	if (BitCount != 8) {
		fprintf(stderr, "Not a 8-bit file.\n");
		fclose(input_file);
		exit(0);
	}

	FREAD(input_file, color_table, 1024);

	//
	data = (U_CHAR *)malloc(((biWidth * 1 + 3) / 4 * 4)*biHeight * 1);
	if (data == NULL) {
		fprintf(stderr, "Insufficient memory.\n");
		fclose(input_file);
		exit(0);
	}

	//
	fseek(input_file, bfOffBits, SEEK_SET);
	FREAD(input_file, data, ((biWidth * 1 + 3) / 4 * 4)*biHeight * 1);
	// 
	fclose(input_file);

	// Process the file
	for (i = 0; i < biHeight; i++)
	{
		k = i* ((biWidth * 1 + 3) / 4 * 4);
		for (j = 0; j < biWidth; j++)
		{
			histo_table[data[k]]++;

			k = k + 1;
		}
	}
	for (i = 0; i < biHeight; i++)
	{
		k = i* ((biWidth * 1 + 3) / 4 * 4);
		for (j = 0; j < biWidth; j++)
		{
			histo_new_table[data[k]]++;

			k = k + 1;
		}
	}
	
	int a,b;
	int r=0 ;
	double temp2;
	int y=0 ;//折線圖
	for (i = 0; i < 10; i++)
	{
		y = 7000 * i;
		y = histo_reverse_table[i];
	}
	for (i = 10; i < 20; i++)
	{
		y = (-6000)*i + 130000;
		y = histo_reverse_table[i];
	}
	for (i = 20; i < 190; i++)
	{
		y = (-10000 / 170)*i + (190000 / 17);
		y = histo_reverse_table[i];
	}
	for (i = 190; i < 200; i++)
	{
		y = 1000*i - 190000;
		y = histo_reverse_table[i];
	}
	for (i = 200; i <=255 ; i++)
	{
		y = (-10000 / 55)*i + (510000 / 11);
		y = histo_reverse_table[i];
	}
	for (i = 1; i < 256; i++)
		histo_reverse_table[i] = histo_reverse_table[i - 1] + histo_reverse_table[i];

	for (i = 0; i < 256; i++)
	{
		a = histo_reverse_table[i];
		for (b = r; b <= a; b++)
		{
			histo_new_table[b] = i;	
		}
		r = a;
	}
	temp2 = histo_new_table[255];

	for (i = 0; i < 256; i++)
		histo_new_table[i] = histo_new_table[i] * 255 / temp2;

	
	for (i = 1; i < 256; i++)
		histo_table[i] = histo_table[i - 1] + histo_table[i];

	temp = histo_table[255];
	for (i = 0; i < 256; i++)
		histo_table[i] = histo_table[i] * 255 / temp;

	for (i = 0; i < 256; i++)
		printf("histo_new_table[%d] = %d\n", i, histo_new_table[i]);

	// Process the file
	for (i = 0; i < biHeight; i++)
	{
		k = i* ((biWidth * 1 + 3) / 4 * 4);
		for (j = 0; j < biWidth; j++)
		{
			data[k] = histo_new_table[histo_table[data[k]]];

			k = k + 1;
		}
	}

	//
	/* 開啟新檔案 */
	if ((output_file = fopen("new.bmp", "wb")) == NULL) {
		fprintf(stderr, "Output file can't open.\n");
		exit(0);
	}

	fwrite(bmpfileheader, sizeof(bmpfileheader), 1, output_file);
	fwrite(bmpinfoheader, sizeof(bmpinfoheader), 1, output_file);

	fwrite(color_table, 1024, 1, output_file);

	fwrite(data, ((biWidth * 1 + 3) / 4 * 4)*biHeight * 1, 1, output_file);

	fclose(output_file);
	return 0;
}