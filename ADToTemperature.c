/******************************************
function：
in：AD值
out：temperature值
discription：将AD值查表得出温度值
methods：二分法、线性插值

author：Mins
date：2019/03/13
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#define uchar unsigned char

int ADToTemperature(int ad);
float lagelangri(float *x, float *y, float xx);
float Line(int inputAD);
//float Line(int inputAD);
//AD-Temperature table (inverted order)

const unsigned int AD_to_T[171] =
{ 1010,1009,1008,1007,1006,1005,1004,1003,1001,1000,999,997,996,994,993,991,989,987,985,983,981,979,977,974,972,
969,966,964,961,958,954,951,948,944,941,937,933,929,925,921,916,912,907,902,897,892,887,881,876,870,864,858,852,
846,840,833,826,820,813,806,799,791,784,776,769,761,753,745,737,729,721,712,704,695,687,678,669,661,652,643,634,
625,617,608,599,590,581,572,563,554,545,537,528,519,510,502,493,484,476,467,459,451,443,434,426,419,411,403,395,
388,380,373,365,358,351,344,337,331,324,317,311,304,298,292,286,280,274,269,263,258,252,247,242,237,232,227,222,
217,213,208,204,200,195,191,187,183,179,176,172,168,165,161,158,155,151,148,145,142,139,136,133,131,128,125,123,
120,118,115,113,111,108 };




int main()
{
	
	
	int inputAD;
	printf("    ***********************************\n");
	printf("    **********2016物联-陈敏诗**********\n");
	printf("    ***********************************\n");
	printf("    **********AD值-温度值查找**********\n");
	printf("    ******请输入108-1010之间的AD值*****\n");
	scanf_s("%d", &inputAD);

	
	
		if (inputAD == 0)
			exit(0);
		if (inputAD > 1010 || inputAD < 108) {
			printf("输入的AD值超出可选范围，需要重新输入\n\n");
			exit(0);
		}
		
	//最邻近插值
	int outputCloseTem = ADToTemperature(inputAD);
	printf("    最邻近插值结果：%d对应的温度值是%d℃\n", inputAD, outputCloseTem);
	//线性插值
	float outputLineTem = Line(inputAD);
	printf("    线性插值结果：%d对应的温度值是%.1f℃\n", inputAD, outputLineTem);
	//牛顿插值
	int i, j, k, m, z = 0;
	double sum = 0, w = 1, x, b[5][6], cc[2][4];
	for (i = 0; i<5; i++)
	{
		b[i][0] = inputAD + i;//x[i]
		b[i][1] = Line(b[i][0]);//y[i]
		//b[i][1] = ADToTemperature(b[i][0]);//y[i]
	}
	for (j = 2, k = 1; j<6; j++, k++)
	{
		for (i = j - 1; i<5; i++)
		{
			b[i][j] = (b[i - 1][j - 1] - b[i][j - 1]) / (b[i - k][0] - b[i][0]);
		}
	}
	for (m = 0; m<4; m++, z = 0)
	{
		cc[0][m] = b[m + 1][m + 2];
		do {
			w *= (inputAD - b[z][0]);
		} while (z++ != m);
		cc[1][m] = w;
	}
	sum = b[0][1];
	for (m = 0; m<4; m++)
		sum += (cc[0][m] * cc[1][m]);
	printf("    牛顿插值结果：%d对应的温度值是%.1f℃\n\n",inputAD, sum);

	

	//system("PAUSE");   
	//getchar();
	printf("    按任意键退出\n");
	_getch();//等待终端输入任意字符
	return 0;

}

float Line(int inputAD)
{
	////线性插值
	int  tem[171];
	for (int k = 0; k < 171; k++)
	{
		tem[k] = k - 50;
	}
	int u = 0;
	float temper;
	for (int i = 0; i < 171; i++) {
		if (AD_to_T[i] == inputAD) {
			temper = tem[i];
			//printf("当AD值为%d时，对应温度为：%.1d℃\n\n", inputAD, tem[i]);
			break;
		}
		if (AD_to_T[i] < inputAD)
		{
			u = AD_to_T[i + 1] - AD_to_T[i];
			temper = tem[i + 1] - tem[i];
			temper = temper / u;
			temper = (inputAD - AD_to_T[i])*temper + tem[i];
			//printf("当AD值为%d时，对应温度为：%.1f\n\n", inputAD, temper);
			break;
		}
	}
	return temper;
}


float lagelangri(float *x, float *y, float xx)
{
	
	int i, j;
	float *a,yy = 0;
	int n = 3;//二次插值则为3
	a = (float*)malloc(n * sizeof(float));
	for (i = 0; i <= n - 1; i++)
	{
		a[i] = y[i];
		for (j = 0; j <= n - 1; j++)
		{ 
			if (j != i) 
				a[i] *= ((xx - x[j]) / (x[i] - x[j]));
			yy += a[i];
		}
		free(a);
	}
	return yy;
}




int ADToTemperature(int ad)
{
	int iTem;
	int iLowValue;
	int iHighValue;
	uchar i;
	uchar cLimit = 0xFF;
	uchar cBotton, cTop;

	cBotton = 170; cTop = 0;//此处指索引值对应的最大最小值
	/*if (ad < AD_to_T[170])
	{
		printf("您的输入小于最小AD值");
	}
	if (ad > AD_to_T[0])
	{
		printf("您的输大于最大AD值");
	}
	*/
	//二分法查表
	for (i = 85; (cBotton - cTop) != 1;)
	{
		if (ad < AD_to_T[i])
		{
			cTop = i;
			i = (cTop + cBotton) / 2;
		}
		else if (ad > AD_to_T[i])
		{
			cBotton = i;
			i = (cTop + cBotton) / 2;
		}
		else
		{
			iTem = i * 1 - 50;
			return iTem;
		}
	}
	iTem = i * 1 - 50;//*1是温度值的步进
	iLowValue = AD_to_T[cBotton];
	iHighValue = AD_to_T[cTop];
	int Tem = (((ad - iLowValue) * 1) / (iHighValue - iLowValue)) + iTem;
	return Tem;
}