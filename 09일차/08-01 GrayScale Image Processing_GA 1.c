#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h> // GUI ���� ���
#include <math.h>

/////////////////////
// ���� ������
/////////////////////
HWND hwnd; // ���� ȭ��(�ܺ�, ������)
HDC hdc; // ���� ȭ�� (����, ����)

FILE* rfp, * wfp, * cfp;
char fileName[200];
int inH, inW, outH, outW, cirH, cirW;
// �޸� Ȯ��
unsigned char** inImage = NULL, ** outImage = NULL, ** circleImage = NULL;


/////////////////////
// �Լ� �����
/////////////////////
// ���� �Լ�
void loadImage(); void loadCircleImage();  int getIntValue(); double getDoubleValue(); void printImage(); void printMenu();
void PointMenu(); void GeomatricMenu(); void HistogramMenu(); void AreaMenu();  void EdgeMenu();
void PointSetting(); void GeomatricSetting(); void HistogramSetting(); void AreaSetting();  void EdgeSetting();
void saveImage(); void freeInputMemory(); void freeOutputMemory(); void freeDoubleMemory(double** memory, int h);
void mallocInputMemory(); void mallocOutputMemory(); double** mallocDoubleMemory(int h, int w); void mallocCircleMemory(); void freeCircleMemory();

// ȭ���� ó��
void equalImage(); void addImage(); void subImage(); void reverseImage();
void bwImage(); void bwAvgImage(); void bwMedImage(); int compare(const unsigned char* a, const unsigned char* b);
void gammaImage(); void paraCapImage(); void paraCupImage();
void andImage(); void orImage(); void xorImage();
// ������ ó��
void zoomOutImage(); void zoomOutAvgImage(); void zoomOutMedImage(); void zoomInImage(); void zoomIn2Image(); void zoomInYSImage();
void rotateImage(); void rotate2Image(); void rotate3Image();
void moveImage(); void mirrorUDImage(); void mirrorLRImage();
// ������׷� ó��
void histoStretchImage(); void endInImage(); void histoEqualImage();
// ȭ�� ���� ó��
void embossImage(); void blurImage(); void SharpImage(); void gausImage(); void hpfSharpImage(); void lpfSharpImage();
// ��輱 ����
void edgeVerImage(); void edgeHorImage(); void edgeHomogenImage(); double doubleABS(double X); void laplacianImage();

void main()
{
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL);  // Windows 10 : hwnd

	char inKey = 0;
	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': printImage(); break;
		case '9': break;
		case 'a': case 'A': PointSetting(); break;
		case 'b': case 'B': GeomatricSetting(); break;
		case 'c': case 'C': HistogramSetting(); break;
		case 'd': case 'D': AreaSetting(); break;
		case 'e': case 'E': EdgeSetting(); break;
		default: system("cls"); puts("�߸� �Է��ϼ̽��ϴ�."); break;
		}
	}
	freeInputMemory();
	freeOutputMemory();
}
/////////////////////
// �Լ� ���Ǻ�
/////////////////////
/// ���� �Լ�
//////////////
void printMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.����  1.����  2.�ҷ�����  9. ����");
	printf("\n");
	puts("A.ȭ���� ó�� B.������ ó�� C.������׷� ó�� D.ȭ�� ���� ó�� E.��輱 ����");
}
void PointMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.����  1.����  2.�ҷ�����  9. �ڷΰ���");
	printf("\n");
	puts("A.���� B.��� C.��Ӱ� D.���� E.���");
	printf("\n");
	puts("F.���� G.�Ķ󺼶� CAP�� H.�Ķ󺼶� CUP�� I.AND J.OR K.XOR");
}
void GeomatricMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.����  1.����  2.�ҷ�����  9. �ڷΰ���");
	printf("\n");
	puts("A.�ܾƿ� B.�ܾƿ�(�߾Ӱ�) C.�ܾƿ�(��հ�) D.����(������) E.����(�����) F.����(�缱��)");
	printf("\n");
	puts("G.ȸ��(������) H.ȸ��(�߾�, �����) I.ȸ��(�߾�, �����, ����)");
	printf("\n");
	puts("J.�̵� K.���� �̷��� L.�¿� �̷���");
}
void HistogramMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.����  1.����  2.�ҷ�����  9. �ڷΰ���");
	printf("\n");
	puts("A.������׷� ��Ʈ��Ī B.������ Ž�� C.��Ȱȭ");
}
void AreaMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.����  1.����  2.�ҷ�����  9. �ڷΰ���");
	printf("\n");
	puts("A.������ B.���� C.������ D.����þ� E.������ ���� ������ F.������ ���� ������");
}
void EdgeMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.����  1.����  2.�ҷ�����  9. �ڷΰ���");
	printf("\n");
	puts("A.���� ���� B.���� ���� C.���� ������ ���� D.���ö�þ� ����");
}
void PointSetting()
{
	system("cls");
	char inKey = 0;
	while (inKey != '9') {
		PointMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': printImage(); break;
		case '9': break;
		case 'a': case 'A': equalImage(); break;
		case 'b': case 'B': addImage(); break;
		case 'c': case 'C': subImage(); break;
		case 'd': case 'D': reverseImage(); break;
		case 'e': case 'E': bwAvgImage(); break;
		case 'f': case 'F': gammaImage(); break;
		case 'g': case 'G': paraCapImage(); break;
		case 'h': case 'H': paraCupImage(); break;
		case 'i': case 'I': andImage(); break;
		case 'j': case 'J': orImage(); break;
		case 'k': case 'K': xorImage(); break;
		default: system("cls"); puts("�߸� �Է��ϼ̽��ϴ�."); break;
		}
	}
	system("cls");
}
void GeomatricSetting()
{
	system("cls");
	char inKey = 0;
	while (inKey != '9') {
		GeomatricMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': printImage(); break;
		case '9': break;
		case 'a': case 'A': zoomOutImage(); break;
		case 'b': case 'B': zoomOutAvgImage(); break;
		case 'c': case 'C': zoomOutMedImage(); break;
		case 'd': case 'D': zoomInImage(); break;
		case 'e': case 'E': zoomIn2Image(); break;
		case 'f': case 'F': zoomInYSImage(); break;
		case 'g': case 'G': rotateImage(); break;
		case 'h': case 'H': rotate2Image(); break;
		case 'i': case 'I': rotate3Image(); break;
		case 'j': case 'J': moveImage(); break;
		case 'k': case 'K': mirrorUDImage(); break;
		case 'l': case 'L': mirrorLRImage(); break;
		default: system("cls"); puts("�߸� �Է��ϼ̽��ϴ�."); break;
		}
	}
	system("cls");
}
void HistogramSetting()
{
	system("cls");
	char inKey = 0;
	while (inKey != '9') {
		HistogramMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': printImage(); break;
		case '9': break;
		case 'a': case 'A': histoStretchImage(); break;
		case 'b': case 'B': endInImage(); break;
		case 'c': case 'C': histoEqualImage(); break;
		default: system("cls"); puts("�߸� �Է��ϼ̽��ϴ�."); break;
		}
	}
	system("cls");
}
void AreaSetting()
{
	system("cls");
	char inKey = 0;
	while (inKey != '9') {
		AreaMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': printImage(); break;
		case '9': break;
		case 'a': case 'A': embossImage(); break;
		case 'b': case 'B': blurImage(); break;
		case 'c': case 'C': SharpImage(); break;
		case 'd': case 'D': gausImage(); break;
		case 'e': case 'E': hpfSharpImage(); break;
		case 'f': case 'F': lpfSharpImage(); break;
		default: system("cls"); puts("�߸� �Է��ϼ̽��ϴ�."); break;
		}
	}
	system("cls");
}
void EdgeSetting()
{
	system("cls");
	char inKey = 0;
	while (inKey != '9') {
		EdgeMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey)
		{
		case '0': loadImage(); break;
		case '1': saveImage(); break;
		case '2': printImage(); break;
		case '9': break;
		case 'a': case 'A': edgeVerImage(); break;
		case 'b': case 'B': edgeHorImage(); break;
		case 'c': case 'C': edgeHomogenImage(); break;
		case 'd': case 'D': laplacianImage(); break;
		default: system("cls"); puts("�߸� �Է��ϼ̽��ϴ�."); break;
		}
	}
	system("cls");
}
void printImage()
{
	system("cls");
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			int px = outImage[i][k];
			SetPixel(hdc, k + 50, i + 250, RGB(px, px, px));
		}
	}
}
void loadImage()
{
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("���ϸ�-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (�߿�!) �̹����� ���� ���̸� ����
	rfp = fopen(fileName, "rb");
	// ������ �� ������ ����
	if (rfp == NULL)
	{
		system("cls");
		printf("������ ã�� �� �����ϴ�.\n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(rfp); // �� ���������?  262,144  --> 512
	fclose(rfp);
	// �޸� ����
	freeInputMemory();
	// �Է¿��� ����, �� ���ϱ�
	inH = inW = (int)sqrt(fsize);
	// �޸� �Ҵ�
	mallocInputMemory();

	// ���� --> �޸�
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	equalImage();
}
void loadCircleImage()
{
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/circle256.raw";

	// (�߿�!) �̹����� ���� ���̸� ����
	cfp = fopen(fullName, "rb");
	// ������ �� ������ ����
	if (cfp == NULL)
	{
		system("cls");
		printf("������ ã�� �� �����ϴ�.\n");
		return;
	}
	fseek(cfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(cfp); // �� ���������?  262,144  --> 512
	fclose(cfp);
	// �޸� ����
	freeCircleMemory();
	// �Է¿��� ����, �� ���ϱ�
	cirH = cirW = (int)sqrt(fsize);
	// �޸� �Ҵ�
	mallocCircleMemory();

	// ���� --> �޸�
	cfp = fopen(fullName, "rb");
	for (int i = 0; i < cirH; i++)
		fread(circleImage[i], sizeof(unsigned char), cirW, cfp);
	fclose(cfp);

	equalImage();
}
void saveImage()
{
	if (outImage == NULL)
		return;
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("���ϸ�-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");
	// �޸� --> ���� (���྿)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
	// MessageBox(hwnd, L"���� �Ϸ�", L"���� â", NULL);
	printf("���� �Ϸ�");
}
int getIntValue()
{
	int retValue;
	printf("������-->"); scanf("%d", &retValue);
	return retValue;
}
double getDoubleValue()
{
	double retValue;
	printf("�Ǽ���-->"); scanf("%lf", &retValue);
	return retValue;
}
void freeInputMemory()
{
	if (inImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(inImage[i]);
	free(inImage);
	inImage = NULL;
}
void mallocInputMemory()
{
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
	for (int i = 0; i < inH; i++)
		inImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
}
void freeOutputMemory()
{
	if (outImage == NULL)
		return;
	for (int i = 0; i < outH; i++)
		free(outImage[i]);
	free(outImage);
	outImage = NULL;
}
void mallocOutputMemory()
{
	outImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);
	for (int i = 0; i < outH; i++)
		outImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);
}
double** mallocDoubleMemory(int h, int w)
{
	double** retMemory;
	retMemory = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		retMemory[i] = (double*)malloc(sizeof(double) * w);
	return retMemory;
}
void freeDoubleMemory(double** memory, int h)
{
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;
}
void mallocCircleMemory()
{
	circleImage = (unsigned char**)malloc(sizeof(unsigned char*) * cirH);
	for (int i = 0; i < cirH; i++)
		circleImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * cirW);
}
void freeCircleMemory()
{
	if (circleImage == NULL)
		return;
	for (int i = 0; i < cirH; i++)
		free(circleImage[i]);
	free(circleImage);
	circleImage = NULL;
}
//////////////////
//  ����ó�� �Լ�
////////////////////
void equalImage() // ���Ͽ��� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}
void addImage() // ���ϱ� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	int val = getIntValue();
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
		}
	}
	printImage();
}
void subImage() // ���� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	int val = getIntValue();
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] - val > 0)
				outImage[i][k] = inImage[i][k] - val;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}
void reverseImage() // ���� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}
void bwImage() // ��� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] > 127)
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}
void bwAvgImage() // ��� �˰��� (��հ�)
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	//	��հ� ���
	int hap = 0;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			hap += inImage[i][k];
		}
	}
	int avr = hap / (outH * outW);

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] > avr)
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}
void bwMedImage() // ��� �˰��� (�߾Ӱ�)
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// 2���� �迭 ���� 1���� �迭�� ��ȯ
	unsigned char* ary;
	ary = (unsigned char*)malloc(sizeof(unsigned char) * inH * inW);
	int cnt = 0;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			ary[cnt] = inImage[i][k];
			cnt++;
		}
	}

	// 1���� �迭�� ������ ������ ���� �� �迭�� �߾� �� ���
	qsort(ary, cnt, sizeof(unsigned char), compare);
	int mid = ary[(inH * inW) / 2];

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] > mid)
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}
	free(ary);
	printImage();
}
int compare(const unsigned char* a, const unsigned char* b)
{
	if (b < a)
		return 1;
	else
		return -1;
}
void gammaImage() // ���� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	double gamma = getDoubleValue();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			double m = (double)inImage[i][k];
			outImage[i][k] = (unsigned char)(255.0 * pow((m / 255.0), gamma));
		}
	}
	printImage();
}

void paraCapImage() // �Ķ󺼶� CAP �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			double m = inImage[i][k];
			outImage[i][k] = 255.0 - 255.0 * pow((m / 128.0 - 1.0), 2);
		}
	}
	printImage();
}
void paraCupImage() // �Ķ󺼶� CUP �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			double m = inImage[i][k];
			outImage[i][k] = 255.0 * pow((m / 128.0 - 1.0), 2);
		}
	}
	printImage();
}
void andImage()
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	loadCircleImage();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (inImage[i][k] & circleImage[i][k]);
		}
	}
	printImage();
	freeCircleMemory();
}
void orImage()
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	loadCircleImage();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (inImage[i][k] | circleImage[i][k]);
		}
	}
	printImage();
	freeCircleMemory();
}
void xorImage()
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	loadCircleImage();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (inImage[i][k] ^ circleImage[i][k]);
		}
	}
	printImage();
	freeCircleMemory();
}
void zoomOutImage() // ��� �˰���
{
	// ���� ����
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}
void zoomInImage() // Ȯ�� �˰���(������)
{
	// ���� ����
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k];
		}
	}
	printImage();
}
void zoomIn2Image() // Ȯ�� �˰���(�����)
{
	// ���� ����
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			outImage[i][k] = inImage[(int)(i / scale)][(int)(k / scale)];
		}
	}
	printImage();
}
void zoomInYSImage() // Ȯ�� �˰���(������) + �缱�� ����
{
	// ���� ����
	double scale = (double)getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			double rH = i / scale;
			double rW = k / scale;

			int iH = (int)floor(rH);
			int iW = (int)floor(rW);

			// 0 ~ 1 ������ ���� ������ �Ǽ��� ����
			double sH = rH - iH;
			double sW = rW - iW;
			if (iH < 0 || iH >= (inH - 1) || iW < 0 || iW >= (inW - 1))
			{
				outImage[i][k] = 255;
			}
			else
			{
				double c1 = (double)inImage[iH][iW];
				double c2 = (double)inImage[iH][iW + 1];
				double c3 = (double)inImage[iH + 1][iW + 1];
				double c4 = (double)inImage[iH + 1][iW];

				unsigned char value = (unsigned char)(
					c1 * (1 - sH) * (1 - sW)
					+ c2 * sW * (1 - sH)
					+ c3 * sW * sH
					+ c4 * (1 - sW) * sH);
				outImage[i][k] = value;
			}
		}
	}
	printImage();
}
void rotateImage() // ȸ�� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	int degree = getIntValue();
	double radian = -(degree * 3.141592 / 180);
	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			int xs = i;
			int ys = k;
			int xd = (int)(cos(radian) * xs - sin(radian) * ys);
			int yd = (int)(sin(radian) * xs + cos(radian) * ys);
			if (0 <= xd && xd < outH && 0 <= yd && yd < outW)
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}
void rotate2Image() // ȸ�� �˰��� + �߾�/�����
{
	// �Է� �迭 --> ��� �迭
	int degree = getIntValue();
	double radian = -(degree * 3.141592 / 180);

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys
	// �Է� �� �̹��� �߾� ��ǥ
	int cx = inH / 2;
	int cy = inW / 2;
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			int xd = i;
			int yd = k;
			int xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cy));
			int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cy));
			xs += cx;
			ys += cy;
			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW))
				outImage[xd][yd] = inImage[xs][ys];
			else
				outImage[xd][yd] = 255;
		}
	}
	printImage();
}
void rotate3Image() // ȸ�� Ȯ�� �˰���
{
	// ���� ����
	printf("ȸ�� ���� ");
	int degree = getIntValue();
	double radian = -(degree * 3.141592 / 180);
	double radian90 = -((90 - degree) * 3.141592 / 180);

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH * cos(radian90) + inW * cos(radian));
	outW = (int)(inW * cos(radian) + inH * cos(radian90));
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �� �̹��� �߾� ��ǥ
	int cx = inH / 2;
	int cy = inW / 2;
	// �Է� �� �̹��� �߾� ��ǥ
	int cx2 = outH / 2;
	int cy2 = outW / 2;

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			int xd = i;
			int yd = k;
			int xs = (int)(cos(radian) * (xd - cx2) + sin(radian) * (yd - cy2));
			int ys = (int)(-sin(radian) * (xd - cx2) + cos(radian) * (yd - cy2));
			xs += cx;
			ys += cy;
			if ((0 <= xs && xs < inH) && (0 <= ys && ys < inW))
				outImage[xd][yd] = inImage[xs][ys];
			else
				outImage[xd][yd] = 255;
		}
	}
	printImage();
}
void moveImage() // �̵� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();
	// �Է� �迭 --> ��� �迭
	int val = getIntValue();
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (0 <= i - val && i - val < outH && 0 <= k - val && k - val < outW)
				outImage[i][k] = inImage[i - val][k - val];
		}
	}
	printImage();
}
void mirrorUDImage() // ���� �̷��� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = inImage[inH - i - 1][k];
		}
	}
	printImage();
}
void mirrorLRImage() // �¿� �̷��� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = inImage[i][inW - k - 1];
		}
	}
	printImage();
}
void zoomOutAvgImage() // ���(��հ�) �˰���
{
	// ���� ����
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			int hap = 0;
			int cnt = 0;
			for (int n = -1; n < 2; n++)
			{
				for (int m = -1; m < 2; m++)
				{
					if (0 <= i + n && i + n < inH && 0 <= k + m && k + m < inW)
					{
						hap += (int)inImage[i + n][k + m];
						cnt++;
					}
				}
			}
			double Value = (double)(hap / cnt);
			outImage[(int)(i / scale)][(int)(k / scale)] = (unsigned char)Value;
		}
	}
	printImage();
}
void zoomOutMedImage() // ���(�߾Ӱ�) �˰���
{
	// ���� ����
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 2���� �迭 ���� ���� 1���� �迭 ����
			unsigned char* ary;
			ary = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 3);
			int cnt = 0;
			for (int n = -1; n < 2; n++)
			{
				for (int m = -1; m < 2; m++)
				{
					if (0 <= i + n && i + n < inH && 0 <= k + m && k + m < inW)
					{
						ary[cnt] = inImage[i + n][k + m];
						cnt++;
					}
				}
			}
			// 1���� �迭�� ������ ������ ���� �� �迭�� �߾� �� ���
			qsort(ary, cnt, sizeof(unsigned char), compare);
			int mid = ary[cnt / 2];
			outImage[(int)(i / scale)][(int)(k / scale)] = (unsigned char)mid;
			free(ary);
		}
	}
	printImage();
}
void histoStretchImage() // ������׷� ��Ʈ��Ī �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// ��¥ ����ó�� �˰���
	// new = (old - low) / (high - low) * 255.0
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] < low)
				low = inImage[i][k];
			if (inImage[i][k] > high)
				high = inImage[i][k];
		}
	}

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			int old = inImage[i][k];
			int new = (int)((double)(old - low) / (double)(high - low) * 255.0);
			if (new > 255)
				new = 255;
			if (new < 0)
				new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}
void endInImage() // ������ Ž�� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// ��¥ ����ó�� �˰���
	// new = (old - low) / (high - low) * 255.0
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (inImage[i][k] < low)
				low = inImage[i][k];
			if (inImage[i][k] > high)
				high = inImage[i][k];
		}
	}

	high -= 50;
	low += 50;

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			int old = inImage[i][k];
			int new = (int)((double)(old - low) / (double)(high - low) * 255.0);
			if (new > 255)
				new = 255;
			if (new < 0)
				new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}
void histoEqualImage() // ��Ȱȭ �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	// 1�ܰ� : �ݵ��� ����(=������׷�)
	int histo[256] = { 0, };
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			histo[inImage[i][k]]++;
	// 2�ܰ� : ���� ������׷� ����
	int sumHisto[256] = { 0, };
	sumHisto[0] = histo[0];
	for (int i = 0; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];
	// 3�ܰ� : ����ȭ�� ������׷� ����
	double normalHisto[256] = { 1.0, };
	for (int i = 0; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW) * 255.0);
	// 4�ܰ� : imImage�� ����ȭ�� ������ ġȯ
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (unsigned char)normalHisto[inImage[i][k]];
		}
	}
	printImage();
}
void embossImage() // ȭ�� ���� ó�� : ������ �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	/////////////////
	// ȸ�� ���� ó��
	/////////////////
	double mask[3][3] = { // ������ ����ũ
		{ -1.0 , 0.0 ,0.0 },
		{ 0.0 , 0.0 ,0.0 },
		{ 0.0 , 0.0 ,1.0 }
	};
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// ȸ�� ����
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// ����ũ(3x3)�� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����)
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			tmpOutImage[i][k] += 127.0;
		}
	}
	// �ӽ� ��� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void blurImage() // ȸ�� ���� ó�� : ���� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	/////////////////
	// ȸ�� ���� ó��
	/////////////////

	// �Է� �迭 --> ��� �迭
	int val = getIntValue();
	if (val == 1)
	{
		for (int i = 0; i < inH; i++)
			for (int k = 0; k < inW; k++)
				outImage[i][k] = inImage[i][k];
		printImage();
		return;
	}

	// ������ ����ũ �Ҵ�
	double** mask = mallocDoubleMemory(val, val);
	for (int i = 0; i < val; i++)
	{
		for (int k = 0; k < val; k++)
		{
			mask[i][k] = 1.0 / (double)(val * val);
		}
	}

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + val - 1, inW + val - 1);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + val - 1; i++)
	{
		for (int k = 0; k < inW + val - 1; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// ȸ�� ����
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// ����ũ(3x3)�� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��
			for (int m = 0; m < val; m++)
				for (int n = 0; n < val; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// �ӽ� ��� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + val - 1);
	freeDoubleMemory(tmpOutImage, outH);
	freeDoubleMemory(mask, val);
	printImage();
}
void SharpImage() // ȸ�� ���� ó�� : ������ �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	/////////////////
	// ȸ�� ���� ó��
	/////////////////
	double mask[3][3] = { // ������ ����ũ
		{ 0 , -1  , 0 },
		{ -1 , 5  , -1 },
		{ 0 , -1  , 0 }
	};
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// ȸ�� ����
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// ����ũ(3x3)�� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// �ӽ� ��� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void gausImage() // ȸ�� ���� ó�� : ����þ� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	/////////////////
	// ȸ�� ���� ó��
	/////////////////
	double mask[3][3] = { // ������ ����ũ
		{ 1. / 16 , 1. / 8  , 1. / 16 },
		{ 1. / 8 , 1. / 4  , 1. / 8 },
		{ 1. / 16 , 1. / 8  , 1. / 16 }
	};
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// ȸ�� ����
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// ����ũ(3x3)�� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// �ӽ� ��� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void hpfSharpImage() // ȸ�� ���� ó�� : ������ ���� ������ �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	/////////////////
	// ȸ�� ���� ó��
	/////////////////
	double mask[3][3] = { // ������ ����ũ
		{ -1. / 9 , -1. / 9  , -1. / 9 },
		{ -1. / 9 , 8. / 9  , -1. / 9 },
		{ -1. / 9 , -1. / 9  , -1. / 9 }
	};
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// ȸ�� ����
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// ����ũ(3x3)�� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// �ӽ� ��� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void lpfSharpImage() // ȸ�� ���� ó�� : ������ ���� ������ �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	/////////////////
	// ȸ�� ���� ó��
	/////////////////
	double mask[3][3] = { // ������ ����ũ
		{ 1. / 9 , 1. / 9  , 1. / 9 },
		{ 1. / 9 , 1. / 9  , 1. / 9 },
		{ 1. / 9 , 1. / 9  , 1. / 9 }
	};
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// ȸ�� ����
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// ����ũ(3x3)�� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = (double)inImage[i][k] - S;
		}
	}
	// �ӽ� ��� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void edgeVerImage() // ��輱 ���� : �������� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	////////////////
	// ȭ�� ���� ó��
	/////////////////
	double mask[3][3] = { {0.0, 0.0, 0.0}, // ������ ����ũ
						  { -1.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** ȸ�� ���� ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3) �� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ����--> ��� ����. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void edgeHorImage() // ��輱 ���� : ������� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	////////////////
	// ȭ�� ���� ó��
	/////////////////
	double mask[3][3] = { {0.0, -1.0, 0.0}, // ������ ����ũ
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** ȸ�� ���� ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3) �� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ����--> ��� ����. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
void edgeHomogenImage() // ��輱 ���� : ���� ������ �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	////////////////
	// ȭ�� ���� ó��
	/////////////////

	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** ȸ�� ���� ***
	double max;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			max = 0.0; // ����� �̵��� ������ �ִ밪 �ʱ�ȭ

			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (doubleABS(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]) >= max)
						max = doubleABS(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]);
					// ����� ��� �� - ����� �ֺ� �ȼ� ���� ���� �� �߿��� �ִ밪�� ã�´�.
				}
			}
			tmpOutImage[i][k] = max;
		}
	}

	// �ӽ� ��� ����--> ��� ����. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}
double doubleABS(double X) // ��輱 ���� : ���ö�þ� �˰���
{
	if (X >= 0)
		return X;
	else
		return -X;
}
void laplacianImage() // ��輱 ���� : ���ö�þ� ó�� �˰���
{
	// �޸� ����
	freeOutputMemory();
	// (�߿�!) ��� �̹����� ũ�⸦ ���� ---> �˰��� ����
	outH = inH;
	outW = inW;
	// �޸� �Ҵ�	
	mallocOutputMemory();

	////////////////
	// ȭ�� ���� ó��
	/////////////////
	double mask[3][3] = { {0.0, 1.0, 0.0}, // ������ ����ũ
						  { 1.0, -4.0, 1.0},
						  { 0.0, 1.0, 0.0} };
	// �ӽ� �޸� �Ҵ�(�Ǽ���)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸𸮸� �ʱ�ȭ(127) : �ʿ�� ��հ�
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** ȸ�� ���� ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ(3x3) �� ������ �߽������� 3x3�� ���ϱ�
			S = 0.0; // ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// ��ó�� (����ũ ���� �հ迡 ����...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// �ӽ� ��� ����--> ��� ����. 
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else
				outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, outH);
	printImage();
}