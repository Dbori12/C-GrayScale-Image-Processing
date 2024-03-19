#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h> // GUI 지원 기능
#include <math.h>

/////////////////////
// 전역 변수부
/////////////////////
HWND hwnd; // 윈도 화면(외부, 물리적)
HDC hdc; // 윈도 화면 (내부, 논리적)

FILE* rfp, * wfp, * cfp;
char fileName[200];
int inH, inW, outH, outW, cirH, cirW;
// 메모리 확보
unsigned char** inImage = NULL, ** outImage = NULL, ** circleImage = NULL;


/////////////////////
// 함수 선언부
/////////////////////
// 공통 함수
void loadImage(); void loadCircleImage();  int getIntValue(); double getDoubleValue(); void printImage(); void printMenu();
void PointMenu(); void GeomatricMenu(); void HistogramMenu(); void AreaMenu();  void EdgeMenu();
void PointSetting(); void GeomatricSetting(); void HistogramSetting(); void AreaSetting();  void EdgeSetting();
void saveImage(); void freeInputMemory(); void freeOutputMemory(); void freeDoubleMemory(double** memory, int h);
void mallocInputMemory(); void mallocOutputMemory(); double** mallocDoubleMemory(int h, int w); void mallocCircleMemory(); void freeCircleMemory();

// 화소점 처리
void equalImage(); void addImage(); void subImage(); void reverseImage();
void bwImage(); void bwAvgImage(); void bwMedImage(); int compare(const unsigned char* a, const unsigned char* b);
void gammaImage(); void paraCapImage(); void paraCupImage();
void andImage(); void orImage(); void xorImage();
// 기하학 처리
void zoomOutImage(); void zoomOutAvgImage(); void zoomOutMedImage(); void zoomInImage(); void zoomIn2Image(); void zoomInYSImage();
void rotateImage(); void rotate2Image(); void rotate3Image();
void moveImage(); void mirrorUDImage(); void mirrorLRImage();
// 히스토그램 처리
void histoStretchImage(); void endInImage(); void histoEqualImage();
// 화소 영역 처리
void embossImage(); void blurImage(); void SharpImage(); void gausImage(); void hpfSharpImage(); void lpfSharpImage();
// 경계선 검출
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
		default: system("cls"); puts("잘못 입력하셨습니다."); break;
		}
	}
	freeInputMemory();
	freeOutputMemory();
}
/////////////////////
// 함수 정의부
/////////////////////
/// 공통 함수
//////////////
void printMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.열기  1.저장  2.불러오기  9. 종료");
	printf("\n");
	puts("A.화소점 처리 B.기하학 처리 C.히스토그램 처리 D.화소 영역 처리 E.경계선 검출");
}
void PointMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.열기  1.저장  2.불러오기  9. 뒤로가기");
	printf("\n");
	puts("A.동일 B.밝게 C.어둡게 D.반전 E.흑백");
	printf("\n");
	puts("F.감마 G.파라볼라 CAP형 H.파라볼라 CUP형 I.AND J.OR K.XOR");
}
void GeomatricMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.열기  1.저장  2.불러오기  9. 뒤로가기");
	printf("\n");
	puts("A.줌아웃 B.줌아웃(중앙값) C.줌아웃(평균값) D.줌인(포워딩) E.줌인(백워딩) F.줌인(양선형)");
	printf("\n");
	puts("G.회전(포워딩) H.회전(중앙, 백워딩) I.회전(중앙, 백워딩, 줌인)");
	printf("\n");
	puts("J.이동 K.상하 미러링 L.좌우 미러링");
}
void HistogramMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.열기  1.저장  2.불러오기  9. 뒤로가기");
	printf("\n");
	puts("A.히스토그램 스트레칭 B.엔드인 탐색 C.평활화");
}
void AreaMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.열기  1.저장  2.불러오기  9. 뒤로가기");
	printf("\n");
	puts("A.엠보싱 B.블러링 C.샤프닝 D.가우시안 E.고주파 필터 샤프닝 F.저주파 필터 샤프닝");
}
void EdgeMenu()
{
	puts("\n ## GrayScale Image Processing (GA 1) ##");
	printf("\n");
	puts("0.열기  1.저장  2.불러오기  9. 뒤로가기");
	printf("\n");
	puts("A.수직 에지 B.수평 에지 C.유사 연산자 에지 D.라플라시안 에지");
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
		default: system("cls"); puts("잘못 입력하셨습니다."); break;
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
		default: system("cls"); puts("잘못 입력하셨습니다."); break;
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
		default: system("cls"); puts("잘못 입력하셨습니다."); break;
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
		default: system("cls"); puts("잘못 입력하셨습니다."); break;
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
		default: system("cls"); puts("잘못 입력하셨습니다."); break;
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
	printf("파일명-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (중요!) 이미지의 폭과 높이를 결정
	rfp = fopen(fileName, "rb");
	// 파일을 못 읽을시 리턴
	if (rfp == NULL)
	{
		system("cls");
		printf("파일을 찾을 수 없습니다.\n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);
	// 메모리 해제
	freeInputMemory();
	// 입력영상 높이, 폭 구하기
	inH = inW = (int)sqrt(fsize);
	// 메모리 할당
	mallocInputMemory();

	// 파일 --> 메모리
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	equalImage();
}
void loadCircleImage()
{
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/circle256.raw";

	// (중요!) 이미지의 폭과 높이를 결정
	cfp = fopen(fullName, "rb");
	// 파일을 못 읽을시 리턴
	if (cfp == NULL)
	{
		system("cls");
		printf("파일을 찾을 수 없습니다.\n");
		return;
	}
	fseek(cfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(cfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(cfp);
	// 메모리 해제
	freeCircleMemory();
	// 입력영상 높이, 폭 구하기
	cirH = cirW = (int)sqrt(fsize);
	// 메모리 할당
	mallocCircleMemory();

	// 파일 --> 메모리
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
	printf("파일명-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");
	// 메모리 --> 파일 (한행씩)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
	// MessageBox(hwnd, L"저장 완료", L"저장 창", NULL);
	printf("저장 완료");
}
int getIntValue()
{
	int retValue;
	printf("정수값-->"); scanf("%d", &retValue);
	return retValue;
}
double getDoubleValue()
{
	double retValue;
	printf("실수값-->"); scanf("%lf", &retValue);
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
//  영상처리 함수
////////////////////
void equalImage() // 동일영상 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}
void addImage() // 더하기 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
void subImage() // 빼기 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
void reverseImage() // 반전 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}
void bwImage() // 흑백 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
void bwAvgImage() // 흑백 알고리즘 (평균값)
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	//	평균값 계산
	int hap = 0;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			hap += inImage[i][k];
		}
	}
	int avr = hap / (outH * outW);

	// 입력 배열 --> 출력 배열
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
void bwMedImage() // 흑백 알고리즘 (중앙값)
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 2차원 배열 값을 1차원 배열로 전환
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

	// 1차원 배열을 오름차 순으로 정렬 후 배열의 중앙 값 계산
	qsort(ary, cnt, sizeof(unsigned char), compare);
	int mid = ary[(inH * inW) / 2];

	// 입력 배열 --> 출력 배열
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
void gammaImage() // 감마 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	double gamma = getDoubleValue();
	// 입력 배열 --> 출력 배열
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

void paraCapImage() // 파라볼라 CAP 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
void paraCupImage() // 파라볼라 CUP 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	loadCircleImage();
	// 입력 배열 --> 출력 배열
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
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	loadCircleImage();
	// 입력 배열 --> 출력 배열
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
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	loadCircleImage();
	// 입력 배열 --> 출력 배열
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
void zoomOutImage() // 축소 알고리즘
{
	// 배율 결정
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}
void zoomInImage() // 확대 알고리즘(포워딩)
{
	// 배율 결정
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[(int)(i * scale)][(int)(k * scale)] = inImage[i][k];
		}
	}
	printImage();
}
void zoomIn2Image() // 확대 알고리즘(백워딩)
{
	// 배율 결정
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			outImage[i][k] = inImage[(int)(i / scale)][(int)(k / scale)];
		}
	}
	printImage();
}
void zoomInYSImage() // 확대 알고리즘(포워딩) + 양선형 보간
{
	// 배율 결정
	double scale = (double)getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			double rH = i / scale;
			double rW = k / scale;

			int iH = (int)floor(rH);
			int iW = (int)floor(rW);

			// 0 ~ 1 사이의 값을 가지는 실수형 변수
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
void rotateImage() // 회전 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
void rotate2Image() // 회전 알고리즘 + 중앙/백워딩
{
	// 입력 배열 --> 출력 배열
	int degree = getIntValue();
	double radian = -(degree * 3.141592 / 180);

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys
	// 입력 전 이미지 중앙 좌표
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
void rotate3Image() // 회전 확대 알고리즘
{
	// 배율 결정
	printf("회전 배율 ");
	int degree = getIntValue();
	double radian = -(degree * 3.141592 / 180);
	double radian90 = -((90 - degree) * 3.141592 / 180);

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH * cos(radian90) + inW * cos(radian));
	outW = (int)(inW * cos(radian) + inH * cos(radian90));
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 전 이미지 중앙 좌표
	int cx = inH / 2;
	int cy = inW / 2;
	// 입력 후 이미지 중앙 좌표
	int cx2 = outH / 2;
	int cy2 = outW / 2;

	// 입력 배열 --> 출력 배열
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
void moveImage() // 이동 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
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
void mirrorUDImage() // 상하 미러링 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = inImage[inH - i - 1][k];
		}
	}
	printImage();
}
void mirrorLRImage() // 좌우 미러링 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = inImage[i][inW - k - 1];
		}
	}
	printImage();
}
void zoomOutAvgImage() // 축소(평균값) 알고리즘
{
	// 배율 결정
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
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
void zoomOutMedImage() // 축소(중앙값) 알고리즘
{
	// 배율 결정
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	// 메모리 할당	
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 2차원 배열 값을 담을 1차원 배열 생성
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
			// 1차원 배열을 오름차 순으로 정렬 후 배열의 중앙 값 계산
			qsort(ary, cnt, sizeof(unsigned char), compare);
			int mid = ary[cnt / 2];
			outImage[(int)(i / scale)][(int)(k / scale)] = (unsigned char)mid;
			free(ary);
		}
	}
	printImage();
}
void histoStretchImage() // 히스토그렘 스트레칭 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 진짜 영상처리 알고리즘
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

	// 입력 배열 --> 출력 배열
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
void endInImage() // 엔드인 탐색 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 진짜 영상처리 알고리즘
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

	// 입력 배열 --> 출력 배열
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
void histoEqualImage() // 평활화 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	// 1단계 : 반도수 세기(=히스토그렘)
	int histo[256] = { 0, };
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			histo[inImage[i][k]]++;
	// 2단계 : 누적 히스토그렘 생성
	int sumHisto[256] = { 0, };
	sumHisto[0] = histo[0];
	for (int i = 0; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];
	// 3단계 : 정규화된 히스토그렘 생성
	double normalHisto[256] = { 1.0, };
	for (int i = 0; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW) * 255.0);
	// 4단계 : imImage를 정규화된 값으로 치환
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			outImage[i][k] = (unsigned char)normalHisto[inImage[i][k]];
		}
	}
	printImage();
}
void embossImage() // 화소 영역 처리 : 엠보싱 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	/////////////////
	// 회소 영역 처리
	/////////////////
	double mask[3][3] = { // 엠보싱 마스크
		{ -1.0 , 0.0 ,0.0 },
		{ 0.0 , 0.0 ,0.0 },
		{ 0.0 , 0.0 ,1.0 }
	};
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// 회선 연산
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 마스크(3x3)와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서)
	for (int i = 0; i < outH; i++)
	{
		for (int k = 0; k < outW; k++)
		{
			tmpOutImage[i][k] += 127.0;
		}
	}
	// 임시 출력 배열 --> 출력 배열
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
void blurImage() // 회소 영역 처리 : 블러링 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	/////////////////
	// 회소 영역 처리
	/////////////////

	// 입력 배열 --> 출력 배열
	int val = getIntValue();
	if (val == 1)
	{
		for (int i = 0; i < inH; i++)
			for (int k = 0; k < inW; k++)
				outImage[i][k] = inImage[i][k];
		printImage();
		return;
	}

	// 엠보싱 마스크 할당
	double** mask = mallocDoubleMemory(val, val);
	for (int i = 0; i < val; i++)
	{
		for (int k = 0; k < val; k++)
		{
			mask[i][k] = 1.0 / (double)(val * val);
		}
	}

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + val - 1, inW + val - 1);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + val - 1; i++)
	{
		for (int k = 0; k < inW + val - 1; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// 회선 연산
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 마스크(3x3)와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값
			for (int m = 0; m < val; m++)
				for (int n = 0; n < val; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 임시 출력 배열 --> 출력 배열
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
void SharpImage() // 회소 영역 처리 : 샤프닝 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	/////////////////
	// 회소 영역 처리
	/////////////////
	double mask[3][3] = { // 엠보싱 마스크
		{ 0 , -1  , 0 },
		{ -1 , 5  , -1 },
		{ 0 , -1  , 0 }
	};
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// 회선 연산
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 마스크(3x3)와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// 임시 출력 배열 --> 출력 배열
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
void gausImage() // 회소 영역 처리 : 가우시안 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	/////////////////
	// 회소 영역 처리
	/////////////////
	double mask[3][3] = { // 엠보싱 마스크
		{ 1. / 16 , 1. / 8  , 1. / 16 },
		{ 1. / 8 , 1. / 4  , 1. / 8 },
		{ 1. / 16 , 1. / 8  , 1. / 16 }
	};
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// 회선 연산
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 마스크(3x3)와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// 임시 출력 배열 --> 출력 배열
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
void hpfSharpImage() // 회소 영역 처리 : 고주파 필터 샤프닝 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	/////////////////
	// 회소 영역 처리
	/////////////////
	double mask[3][3] = { // 엠보싱 마스크
		{ -1. / 9 , -1. / 9  , -1. / 9 },
		{ -1. / 9 , 8. / 9  , -1. / 9 },
		{ -1. / 9 , -1. / 9  , -1. / 9 }
	};
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// 회선 연산
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 마스크(3x3)와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = S;
		}
	}
	// 임시 출력 배열 --> 출력 배열
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
void lpfSharpImage() // 회소 영역 처리 : 저주파 필터 샤프닝 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	/////////////////
	// 회소 영역 처리
	/////////////////
	double mask[3][3] = { // 엠보싱 마스크
		{ 1. / 9 , 1. / 9  , 1. / 9 },
		{ 1. / 9 , 1. / 9  , 1. / 9 },
		{ 1. / 9 , 1. / 9  , 1. / 9 }
	};
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
	{
		for (int k = 0; k < inW + 2; k++)
		{
			tmpInImage[i][k] = 127;
		}
	}
	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}
	// 회선 연산
	double S;
	for (int i = 0; i < inH; i++)
	{
		for (int k = 0; k < inW; k++)
		{
			// 마스크(3x3)와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값
			for (int n = 0; n < 3; n++)
				for (int m = 0; m < 3; m++)
					S += tmpInImage[i + n][k + m] * mask[n][m];

			tmpOutImage[i][k] = (double)inImage[i][k] - S;
		}
	}
	// 임시 출력 배열 --> 출력 배열
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
void edgeVerImage() // 경계선 검출 : 수직검출 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {0.0, 0.0, 0.0}, // 엠보싱 마스크
						  { -1.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
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
void edgeHorImage() // 경계선 검출 : 수평검출 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {0.0, -1.0, 0.0}, // 엠보싱 마스크
						  { 0.0, 1.0, 0.0},
						  { 0.0, 0.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
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
void edgeHomogenImage() // 경계선 검출 : 유사 연산자 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////

	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double max;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			max = 0.0; // 블록이 이동할 때마다 최대값 초기화

			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (doubleABS(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]) >= max)
						max = doubleABS(tmpInImage[i + 1][k + 1] - tmpInImage[i + n][k + m]);
					// 블록의 가운데 값 - 블록의 주변 픽셀 값의 절대 값 중에서 최대값을 찾는다.
				}
			}
			tmpOutImage[i][k] = max;
		}
	}

	// 임시 출력 영상--> 출력 영상. 
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
double doubleABS(double X) // 경계선 검출 : 라플라시안 알고리즘
{
	if (X >= 0)
		return X;
	else
		return -X;
}
void laplacianImage() // 경계선 검출 : 라플라시안 처리 알고리즘
{
	// 메모리 해제
	freeOutputMemory();
	// (중요!) 출력 이미지의 크기를 결정 ---> 알고리즘에 의존
	outH = inH;
	outW = inW;
	// 메모리 할당	
	mallocOutputMemory();

	////////////////
	// 화소 영역 처리
	/////////////////
	double mask[3][3] = { {0.0, 1.0, 0.0}, // 엠보싱 마스크
						  { 1.0, -4.0, 1.0},
						  { 0.0, 1.0, 0.0} };
	// 임시 메모리 할당(실수형)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리를 초기화(127) : 필요시 평균값
	for (int i = 0; i < inH + 2; i++)
		for (int k = 0; k < inW + 2; k++)
			tmpInImage[i][k] = 127;

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++)
			tmpInImage[i + 1][k + 1] = inImage[i][k];

	// *** 회선 연산 ***
	double S;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크(3x3) 와 한점을 중심으로한 3x3을 곱하기
			S = 0.0; // 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}
	// 후처리 (마스크 값의 합계에 따라서...)
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// 임시 출력 영상--> 출력 영상. 
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