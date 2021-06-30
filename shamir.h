#ifndef SHAMIR_H
#define SHAMIR_H
#include<iostream>
#include<cstdlib> 
#include<ctime>
using namespace std;


const int maxn=100000;
int prime[maxn] ,pnum=0; //pnum:素数数组的大小
bool p[maxn]={0};
//素数生成函数，生成2-max范围内的素数
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

//生成范围在l~r的随机数
void Random(double *a,int n,double l,double r) 
{
	srand(time(0));  //设置时间种子
	for(int i=0;i<n;i++){
        a[i]=double(rand())/double(32767)*(r-l)+l;//生成区间随机数   
		a[i]=int(a[i]*1000);
		a[i]/=1000;
	}
}

void Random1(double *a,int n,int t,int l,int r) 
{
	srand(time(0));  //设置时间种子
	for(int i=0;i<n;i++){
        a[i]=double(rand())/double(32767)*(r-l)+l;//生成区间随机数                  
		a[i]=int(a[i]);
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