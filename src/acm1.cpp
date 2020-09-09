#include <iostream>
#include<cstdlib>
using namespace std;
int compInc(const void *a,const void *b){
	return *(int *)a-*(int *)b;
}
int main(){
int s,w,n;
cin>>s;
int i,j,a[300],sum,x,y;
for (int i = 0; i < 300; i++)
{
	a[i]=1000000;
}

for ( i = 0; i < s; i++)
{
	sum=0;
	cin>>w>>n;
	for ( j = 0; j < n; j++)
	{
		cin>>a[j];
	}
	qsort(a,n,sizeof(int),compInc);
	for ( x = 0,y=n-1;x < y;)
	{
		if (a[x]+a[y]<=w)
		{
			x++;y--;
		}else{
			y--;
		}
		sum++;
	}
	if (x==y)
	{
		sum++;
	}
	cout<<"case" <<i<<":"<<sum<<endl;
}
return 0;
}

