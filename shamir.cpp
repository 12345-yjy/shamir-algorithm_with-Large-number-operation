#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<math.h>
#include<iomanip>      //浮点数精度
#include"shamir.h"
using namespace std;
  

//生成多项式
double fx(int t,double x,int s,double *a)
{	
	double res=s;
    for(int i=1;i<t;i++)
	{
		res+=a[i-1]*pow(x,i);
	}
//	res=s+(res-s)/double(pow(10,0.1*t));
	return res;
}


//密钥分散函数
bool SecretDistribution(int n,int t,int s,string filename)
{	
	Find_Prime();													//生成2-100000之间的素数，可在shamir.h修改
	int e; e =rand();
	int q=prime[1000+e];  			   								//q是大于10000的素数
	if(n>=q||s>=q||t>=q)  { printf("SecretDistribution failed!");return 0;}      //如果n,s,t比q大，密钥分散失败
	double a[t-1];					    							//随机生成多项式系数
	Random1(a,t-1,t,-5,5);        									

	double x[n],y[n];
//	int x[n]; double y[n];
	Random(x,n,1,5);   										    	//随机生成n个x点
	for(int i=0;i<n;i++) y[i]=fx(t,x[i],s,a);    					//计算对应的y值


	ofstream fout(filename); 										//写key.txt，保存信息
	if (fout) {
		fout << "f(x)="<<s;
		for(int i=0;i<t-1;i++)   
			{	
				if(a[i]>=0) fout<<"+";
				fout <<a[i]<<"x^"<<i+1; 
			}
		fout<<endl;
		fout << "q="<<q<<endl;
		fout << "t="<<t<<endl;
		for (int i = 0; i < n; i++)
		{
			fout <<std::fixed<<setprecision(3)<< x[i] << ' ';
			fout <<std::fixed<<setprecision(20)<< y[i] << endl;
		}
		fout.close();
	}
	
	return 1;  											//密钥分散成功
}

//密钥恢复函数
bool SecretRecovery(string filename)					//从key.txt读取信息
{	
	int num,n,t=0;
	char c[100];
	double x[100000];									//存储n个点
	double y[100000];

    ifstream file;
	file.open(filename);
	seek_to_line(file,2);        	
	file.getline(c,sizeof(c));     
	t=atoi(&c[2]);										//读取t

	
	char tmp1[200];
	char tmp2[200];
	int k1=0,k2=0;
	while(file.getline(c,sizeof(c)))
	{	
		int i=0;
		int j=strlen(c);
		while(c[i]!=' ') i++;
		strncpy(tmp1,c,i);
		strncpy(tmp2,c+i+1,j-i-1);

		x[k1++]=atof(tmp1);								//读取n个点的坐标
		y[k2++]=atof(tmp2);
	}
	Print(x,k1);
	Print(y,k2);
	printf("n = %d , t = %d\n\n",k1,t);
	printf("Please input a number between 1 and n:\n");
	scanf("%d",&num);
	if(num<t) { printf("Secretrecovery failed!"); return 0;}       //恢复失败

//计算sercet
	double res=0,tmp=1;
	for(int j=0;j<t;j++)
	{
		for(int m=0;m<t;m++)
		{
			if(m!=j)
			tmp*=x[m]/(x[m]-x[j]);								//浮点数计算精度误差无法避免
		}
		res+=y[j]*tmp;
		tmp=1;
	}
	printf("Secret has been recovered successfully!\n");
	printf("The sercet is : %f ,We think it should be %.0f\n",res,res);
	return 1;													//恢复成功
}
 
int main()
{   
    int n,t,s;
	string name="key.txt";
    printf("Please input the total number,threshold,and sercet:\n");
   	scanf("%d %d %d",&n,&t,&s);

	SecretDistribution(n,t,s,name);
	printf("Secret has been shared successfully!\n");

	SecretRecovery(name);
	return 0;
}