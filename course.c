#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


/* A coloured pixel. */

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} pixel_t;

/* A picture. */

typedef struct {
	pixel_t* pixels;
	size_t width;
	size_t height;
} bitmap_t;

/* Given "bitmap", this returns the pixel of bitmap at the point
("x", "y"). */

static pixel_t* pixel_at(bitmap_t* bitmap, int x, int y)
{
	return bitmap->pixels + bitmap->width * y + x;
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
success, non-zero on error. */

static int save_png_to_file(bitmap_t* bitmap, const char* path)
{
	FILE* fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	size_t x, y;
	png_byte** row_pointers = NULL;
	/* "status" contains the return value of this function. At first
	it is set to a value which means 'failure'. When the routine
	has finished its work, it is set to a value which means
	'success'. */
	int status = -1;
	/* The following number is set by trial and error only. I cannot
	see where it it is documented in the libpng manual.
	*/
	int pixel_size = 4;
	int depth = 8;

	fp = fopen(path, "wb");
	if (!fp) {
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	/* Set up error handling. */

	if (setjmp(png_jmpbuf(png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR(png_ptr,
		info_ptr,
		bitmap->width,
		bitmap->height,
		depth,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */

	row_pointers = png_malloc(png_ptr, bitmap->height * sizeof(png_byte*));
	for (y = 0; y < bitmap->height; ++y) {
		png_byte* row =
			png_malloc(png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size);
		row_pointers[y] = row;
		for (x = 0; x < bitmap->width; ++x) {
			pixel_t* pixel = pixel_at(bitmap, x, y);
			*row++ = pixel->red;
			*row++ = pixel->green;
			*row++ = pixel->blue;
			*row++ = pixel->alpha;
		}
	}

	/* Write the image data to "fp". */

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	"status" to a value which indicates success. */

	status = 0;

	for (y = 0; y < bitmap->height; y++) {
		png_free(png_ptr, row_pointers[y]);
	}
	png_free(png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
	png_destroy_write_struct(&png_ptr, &info_ptr);
png_create_write_struct_failed:
	fclose(fp);
fopen_failed:
	return status;
}

/* Given "value" and "max", the maximum value which we expect "value"
to take, this returns an integer between 0 and 255 proportional to
"value" divided by "max". */

static int pix(int value, int max)
{
	if (value < 0)
		return 0;
	return (int)(256.0 * ((double)(value) / (double)max));
}
float float_rand(float min, float max)
{
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	return min + scale * (max - min);      /* [min, max] */
}

int** create_matrix(int n)
{
	int** a = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
	{
		a[i] = (int*)malloc(n * sizeof(int));
	}
	return a;
}

void fill_rand(int** a, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{

			a[i][j] = rand() % 3;
		}

	}
}

void print_matrix(int** a, int n, bitmap_t fruit)
{
	printf("\n");
	pixel_t* pixel;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (a[i][j] == 0)
			{
				pixel = pixel_at(&fruit, i, j);
				pixel->red = 0;
				pixel->green = 0;
				pixel->blue = 0;
			}
			else
				if (a[i][j] == 1)
				{
					pixel = pixel_at(&fruit, i, j);
					pixel->red = 0;
					pixel->green = 255;
					pixel->blue = 0;
				}
				else
					if (a[i][j] == 2)
					{
						pixel = pixel_at(&fruit, i, j);
						pixel->red = 255;
						pixel->green = 0;
						pixel->blue = 0;
					}
			//printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}


int* create_line(int** a, int n, int tipe)
{
	int i, j;
	int* line = (int*)malloc(n * n * sizeof(int));
	switch (tipe)
	{




	case 0://+
		printf("\n");

		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{

				line[(i * n) + j] = a[i][j];
				//printf("%d ", line[(i*n)+j]);
			}
		}
		printf("\n");

		break;

	case 1://+
		printf("\n");

		for (i = n; i > 0; i--)
		{
			for (j = n; j > 0; j--)
			{

				line[((n * n) - 1) - (((i - 1) * n) + j - 1)] = a[i - 1][j - 1];//- (((i-1)*n)+j-1) //+ ((n*n)-1)-(((i-1)*n)+j-1)
				//printf("%d ", line[((n*n)-1)-(((i-1)*n)+j-1)]);line[((n * n) - 1) - (((i - 1) * n) + j - 1)] = a[i - 1][j - 1];
			}
		}
		printf("\n");

		break;

	case 2://+
		printf("\n");

		for (j = 0; j < n; j++)
		{
			for (i = 0; i < n; i++)
			{

				line[((j * n) + i)] = a[i][j];//- ((i*n)+j) //- (((i-1)*n)+j-1) // + ((j*n)+i)
				//printf("%d ", line[((j*n)+i)]);
			}
		}
		printf("\n");

		break;

	case 3://+
		printf("\n");

		for (j = n; j > 0; j--)
		{
			for (i = n; i > 0; i--)
			{

				line[((n * n) - 1) - (((j - 1) * n) + i - 1)] = a[i - 1][j - 1];//- (((i-1)*n)+j-1) //- ((n*n)-1)-(((i-1)*n)+j-1) //+ ((n*n)-1)-(((j-1)*n)+i-1)
				//printf("%d ", line[((n*n)-1)-(((j-1)*n)+i-1)]);
			}
		}
		printf("\n");

		break;
	}

	return line;
}

void print_line(int* line, int n)
{
	int i;

	for (i = 0; i < n * n; i++)
	{
		printf("%d ", line[i]);
	}
}

void pravila(int* line, float p1, float p2, float p3, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			p1 = float_rand(0.0f, 1.0f);
			p2 = float_rand(0.0f, 1.0f);
			p3 = float_rand(0.0f, 1.0f);

			if (line[(i * n) + j] == 1 && line[((i * n) + j) + 1] == 0 && p1 < 0.02)
			{
				line[((i * n) + j) + 1] = 1;
				line[(i * n) + j] = 0;
				break;
			}

			if (line[(i * n) + j] == 2 && line[((i * n) + j) + 1] == 0 && p1 < 0.02)
			{
				line[((i * n) + j) + 1] = 2;
				line[(i * n) + j] = 0;
				break;
			}

			if (line[(i * n) + j] == 1 && line[((i * n) + j) + 1] == 2 && p1 < 0.02)
			{
				line[((i * n) + j) + 1] = 2;
				line[(i * n) + j] = 1;
				break;
			}


			if (line[(i * n) + j] == 0 && line[((i * n) + j) + 1] == 1 && p1 < 0.02)
			{
				line[((i * n) + j) + 1] = 0;
				line[(i * n) + j] = 1;
				break;
			}

			if (line[(i * n) + j] == 0 && line[((i * n) + j) + 1] == 2 && p1 < 0.02)
			{
				line[((i * n) + j) + 1] = 0;
				line[(i * n) + j] = 2;
				break;
			}

			if (line[(i * n) + j] == 2 && line[((i * n) + j) + 1] == 1 && p1 < 0.02)
			{
				line[((i * n) + j) + 1] = 1;
				line[(i * n) + j] = 2;
				break;
			}

			if (line[(i * n) + j] == 1 && line[((i * n) + j) + 1] == 0 && p2 < 0.1)
			{
				line[((i * n) + j) + 1] = 1;
				break;
			}

			if (line[(i * n) + j] == 2 && line[((i * n) + j) + 1] == 1 && p2 < 0.1)
			{
				line[((i * n) + j) + 1] = 2;
				break;
			}

			if (line[(i * n) + j] == 2 && p3 < 0.01)
			{
				line[(i * n) + j] = 0;
				break;
			}




		}
	}
	printf("\n");
}

void vozvrat_martix(int** a, int* line, int n, int tipe)
{

	int i, j;
	switch (tipe)
	{

	case 0:
		printf("\n");

		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{

				a[i][j] = line[(i * n) + j];// line[(i*n)+j] = a[i][j]; //
				//printf("%d ", a[i][j]);
			}
		}
		printf("\n");

		break;

	case 1:
		printf("\n");

		for (i = n; i > 0; i--)
		{
			for (j = n; j > 0; j--)
			{


				a[i - 1][j - 1] = line[((n * n) - 1) - (((i - 1) * n) + j - 1)];//line[((n*n)-1)-(((i-1)*n)+j-1)] = a[i-1][j-1];
				//printf("%d ", a[i-1][j-1]);
			}
		}
		printf("\n");

		break;

	case 2:
		printf("\n");

		for (j = 0; j < n; j++)
		{
			for (i = 0; i < n; i++)
			{

				a[i][j] = line[((j * n) + i)];// line[((j*n)+i)] = a[i][j];
				//printf("%d ", a[i][j]);
			}
		}
		printf("\n");

		break;

	case 3:
		printf("\n");

		for (j = n; j > 0; j--)
		{
			for (i = n; i > 0; i--)
			{

				a[i - 1][j - 1] = line[((n * n) - 1) - (((j - 1) * n) + i - 1)];//line[((n*n)-1)-(((j-1)*n)+i-1)] = a[i-1][j-1];
				//printf("%d ", a[i-1][j-1]);
			}
		}
		printf("\n");

		break;
	}

}
int podshet(int* line, int n, bool s)
{
	int shixlo = 0;
	for (int i = 0; i < n * n; i++)
	{
		if (s)
		{
			if (line[i] == 1)
			{
				shixlo++;
			}
		}
		else
		{
			if (line[i] == 2)
			{
				shixlo++;
			}
		}


	}


	return shixlo;
}
int main()
{
	// Создание / открытие файла
	FILE* fo;
	bitmap_t fruit;
	int x;
	int y;
	int** a;
	int i, n, hod;
	int counter = 1;
	int shet = 0;


	system("color 2");
	system("chcp 1251");
	system("cls");
	printf("Введите размер стороны поля: ");
	scanf("%d", &n);
	printf("Введите количество ходов: ");
	scanf("%d", &hod);

	/* Create an image. */

	fruit.width = n;
	fruit.height = n;

	fruit.pixels = calloc(sizeof(pixel_t), fruit.width * fruit.height);

	for (y = 0; y < fruit.height; y++) {
		for (x = 0; x < fruit.width; x++) {
			pixel_t* pixel = pixel_at(&fruit, x, y);
			pixel->red = 255;
			pixel->green = 255;
			pixel->blue = 255;
			pixel->alpha = 255;
		}
	}

	save_png_to_file(&fruit, "1.png");

	// pixel = pixel_at(&fruit, 10, 5);
	 //pixel->red = 255;
	 //pixel->green = 0;
	 //pixel->blue = 0;
	 /* Write the image to a file 'fruit.png'. */




	srand(time(NULL));

	int shans = rand() % 4;
	float p1 = float_rand(0.0f, 1.0f);
	printf("xx: %f\n", p1);
	float p2 = float_rand(0.0f, 1.0f);
	printf("xx: %f\n", p2);
	float p3 = float_rand(0.0f, 1.0f);
	printf("xx: %f\n", p3);
	a = create_matrix(n);


	// заполняем случайно а
	fill_rand(a, n);
	//print_matrix(a, n, fruit);
	int* line = create_line(a, n, shans);
	//int* line = create_line(a, n, 1);
	//print_line(line, n);
	//printf("\n");

	// применяем правила к line
	pravila(line, p1, p2, p3, n);
	//print_line(line, n);
	// записываем line в a по такому же принципу
	vozvrat_martix(a, line, n, shans);
	//vozvrat_martix(a, line, n, 1);
	//print_matrix(a, n);

	//print_matrix(a, n, fruit);



	int  hish = 0, jertv = 0;
	bool s = 0;
	fo = fopen("file.txt", "wt");
	fprintf(fo, "хищников жертв\n");
	while (shet != hod)
	{
		system("cls");
		int shans = rand() % 4;
		line = create_line(a, n, shans);
		pravila(line, p1, p2, p3, n);
		hish = podshet(line, n, s);
		s = !s;
		jertv = podshet(line, n, s);
		s = !s;
		vozvrat_martix(a, line, n, shans);
		print_matrix(a, n, fruit);
		sprintf(line, "%08d.png", shet);
		save_png_to_file(&fruit, "2.png");


		// Запись в файл

		//const int N = 20;
		//float m, f;
		//for (i = 0; i <= N; i++) {
			//m = (float)i / N;
			//f = pow(m, 2);
		fprintf(fo, "%d       %d\n", hish, jertv);
		//}


		shet++;

		getchar();

	}
	fclose(fo);


	for (i = 0; i < n; i++)
		free(a[i]);
	free(a);
	free(line);




	return 0;


}
