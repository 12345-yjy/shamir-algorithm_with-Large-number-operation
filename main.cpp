#include<iostream>
#include"TLargeFloat.h"
#include<stdio.h>
#include<fstream>
#include<string>
#include<string.h>
#include<ctime>
#include<math.h>
#include<iomanip>      
#include"shamir.h"
using namespace std;
//由于double类型的精度最高不超过16位，经多次测试后，选择使用GIT上开源的高精度浮点数库
//理论上，只要设置的y的精度没有被填满（见txt），t就可以设得更大；提高精度，可以增加t的上限
//由于设点x是随机的，故将key.txt中前t个点用于密钥恢复即可
//n的设置影响x点的大小； n,t增大都会使数据溢出  ； 如果t较小，n可以设得较大
//该精度设置下， n=200,t=100可以成功恢复密钥  
//实际上，若将x设为浮点数，可以使t的上限更大

#ifdef _MY_TIME_CLOCK
#include  <windows.h>
#define clock_t __int64
clock_t CLOCKS_PER_SEC = 0;
inline clock_t clock()
{
    __int64 result;
    if (CLOCKS_PER_SEC == 0)
    {
        QueryPerformanceFrequency((LARGE_INTEGER*)&result);
        CLOCKS_PER_SEC = (clock_t)result;
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&result);
    return  (clock_t)result;
}
#else
#include  <time.h>
#endif

void Debug_toCout(const string& strx, const TLargeFloat& x)//调试输出
{
    cout << strx << endl << std::fixed <<x << endl;
}

TLargeFloat pow1(int x, int i)
{																								
	TLargeFloat res(1.0, TLargeFloat::TDigits(250));								//精度设为250位,精度越高，t的上限越高；
	while (i--)  res = res * TLargeFloat(x, TLargeFloat::TDigits(250));				//t超过上限数据会溢出
	return res;
}

// 生成多项式
double fx(int t, double x, int s, double* a)
{
	double res = s;
	for (int i = 1; i < t; i++)
	{
		res += a[i - 1] * pow(x, i);
	}
	return res;
}
TLargeFloat fx1(int t, double x, int s, double* a)
{
	TLargeFloat res = TLargeFloat(s, TLargeFloat::TDigits(250));
	for (int i = 1; i < t; i++)
	{
		res += TLargeFloat(a[i - 1], TLargeFloat::TDigits(250)) * pow1(x, i);
	}
	return res;
}


//密钥分散函数
bool SecretDistribution(int n, int t, int s, int q, string filename)
{
	if (n >= q || s >= q || t >= q) { printf("SecretDistribution failed!"); return 0; }      //如果n,s,t比q大，密钥分散失败
	double a[1000];
	Random(a, t - 1, t, -2.5, 2.5);        									//随机生成多项式系数 -2 to 2

	int x[2000]; double y[2000];
	generateDiffRandV1(x, n, n / 2);   										//随机生成n个x点
	TLargeFloat y1[1000];
//	for (int i = 0; i < n; i++) y[i] = fx(t, x[i], s, a);    				//计算对应的y值
	for (int i = 0; i < n; i++) {
		y1[i]= TLargeFloat(1, TLargeFloat::TDigits(250));
		y1[i] = fx1(t, x[i], s, a); 
	}

	ofstream fout; 															//写key.txt，保存信息
	fout.open(filename);
	if (fout) {
		fout << "f(x)=" << s;
		for (int i = 0; i < t - 1; i++)
		{
			if (a[i] >= 0) fout << "+";
			fout << a[i] << "*x^" << i + 1;
		}
		fout << endl;
		fout << "q=" << q << endl;
		fout << "t=" << t << endl;
		for (int i = 0; i < n; i++)
		{
			fout << std::fixed << setprecision(0) << x[i] << ' ';
			fout << y1[i] << endl;
		}
		fout.close();
	}

	return 1;  																//密钥分散成功
}



//密钥恢复函数
bool SecretRecovery(string filename)										//从key.txt读取信息
{
	int num, n, t = 0;
	char c[300];
	double x[2000];															//存储n个点，这里是n的上限，可修改
	TLargeFloat y[2000];
	ifstream file;
	file.open(filename);
	seek_to_line(file, 2);
	file.getline(c, sizeof(c));
	t = atoi(&c[2]);														//读取t


	char tmp1[300];
	char tmp2[300];
	int k1 = 0, k2 = 0;
	while (file.getline(c, sizeof(c)))
	{
		int i = 0;
		int j = strlen(c);
		while (c[i] != ' ') i++;
		strncpy_s(tmp1, c, i);
		strncpy_s(tmp2, c + i + 1, j - i - 1);
		x[k1++] = atof(tmp1);												//读取n个点的坐标
		y[k2] = TLargeFloat(0.0,TLargeFloat::TDigits(250));					//y是大数，用TLargeFloat类
		y[k2] = TLargeFloat(tmp2);
		k2++;

		memset(tmp1, '\0', sizeof(tmp1));
		memset(tmp2, '\0', sizeof(tmp2));

	}
	//	Print(x,k1);
	//	Print(y,k2);
	printf("n = %d , t = %d\n\n", k1, t);
	printf("Please input a number between 1 and n:\n");
	scanf_s("%d", &num);
	if (num < t) { printf("Secretrecovery failed!"); return 0; }       //如果num<t，恢复失败

//计算sercet
	stringstream ssm;
	stringstream ssj;
	long double RES;

	LargeFloat_UnitTest();
	TLargeFloat res(0.0, TLargeFloat::TDigits(250));								//浮点数计算精度有限，存在误差
	TLargeFloat t0(0.0, TLargeFloat::TDigits(250));									//使用高精度浮点数：TLargeFloat
	TLargeFloat t1(0.0, TLargeFloat::TDigits(250));
	TLargeFloat tmp(1.0, TLargeFloat::TDigits(250));
	TLargeFloat tmp3(1.0, TLargeFloat::TDigits(250));
	printf("\nThe corresponding shares are: \n");
	for (int j = 0; j < t; j++)
	{	
		printf("point %d is: x=%.0f ",j+1,x[j]);
		printf("y= %.0f\n", y[j].AsFloat());
		for (int m = 0; m < t; m++)
		{	
			if (m != j)
			{
				ssm << setprecision(10) << x[m];
				t0.StrToLargeFloat(ssm.str());
				ssj << setprecision(10) << x[j];
				t1.StrToLargeFloat(ssj.str());

				tmp = tmp * (t0 / (t0 - t1));								
				ssm.clear();												
				ssm.str("");
				ssj.clear();
				ssj.str("");
			}
		}
//		Debug_toCout("tmp3=\n", y[j]);                  //这里取消注释，可以查看密钥恢复的计算过程

		res = res+ tmp * y[j];

//		Debug_toCout("res=\n", res);
		tmp = 1;
	}
	RES = res.AsFloat();
	printf("\nSecret has been recovered successfully!\n");												//恢复成功
	printf("The sercet is: %.0f\n", RES);
	
	return 1;													
}

int  main()
{
	Find_Prime();														//生成2-100000之间的素数，可在shamir.h修改
	srand(time(0));
	int e; e = rand();
	int q; q = prime[30000 + e]; 			   							//q是较大的素数
	printf("q=%d\n", q);

	int n, t, s;														//输入
	string name = "key.txt";
	printf("Please input the total number,threshold,and sercet(<q):\n");
	scanf_s("%d %d %d", &n, &t, &s);

	SecretDistribution(n, t, s, q, name);
	printf("Secret has been shared successfully!\n");
	SecretRecovery(name);

	return 0;
}