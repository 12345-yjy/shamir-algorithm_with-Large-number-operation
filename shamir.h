#ifndef SHAMIR_H
#define SHAMIR_H
#include<iostream>
#include<fstream>
#include<cstdlib> 
#include<ctime>
using namespace std;


const int maxn=1000000;
int prime[maxn] ,pnum=0; //pnum:素数数组的大小
bool p[maxn]={0};
//素数生成函数，生成2-max范围内的素数(筛选法)
void Find_Prime()   
{
    for(int i=2;i<maxn;i++)
    {
        if(!p[i])
        {
            prime[pnum++]=i;
            for(int j=i+i;j<maxn ;j+=i)
            {
                p[j]=true;
            }
        }
    }
}

void swap(int *a, int *b)
{
	if(*a != *b){                 // 异或操作 交换两个数字的位置
		*a ^= *b;
		*b ^= *a;
		*a ^= *b;
	}
    
}
//生成不重复随机数
void generateDiffRandV1(int a[], int n, int k)           
{
    int i;
    time_t t;
 
    for (i = 0; i < n; i++){
        a[i] = i+1;
    }
 
    srand((int)time(&t));
    for (i = 0; i < k; i++){
        swap(&a[i], &a[i+rand()%(n-i)]);
    }
}
//用于生成多项式系数
void Random(double *a,int n,int t,double l,double r) 
{
	srand(time(0));  //设置时间种子
	for(int i=0;i<n;i++){
        a[i]=double(rand())/double(32767)*(r-l)+l;//生成区间随机数
		a[i]=int(a[i]);               
//		a[i]=a[i]/pow(10,0.5*i);
	}
}

//定位到txt文件的某一行
ifstream & seek_to_line(ifstream & in, int line)
									//将打开的文件in，定位到line行。
{
	int i;
	char buf[1024];
	in.seekg(0, ios::beg);  		//定位到文件开始。
	for (i = 0; i < line; i++)
	{
		in.getline(buf, sizeof(buf));//读取行。
	}
	return in;
}

void Print(double *a,int n)      	//打印函数
{
	for(int i=0;i<n;i++)
		cout<<a[i]<<" ";
	cout<<endl;
}

#endif