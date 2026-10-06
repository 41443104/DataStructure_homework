#include<iostream>
using namespace std;
int A(long m, long n)
{
	long st[10000];
	int top = -1;
	st[++top] = m;
	while (top >= 0)
	{
		m = st[top--];
		if (m == 0)
		{
			n = n + 1;
		}
		else if (n == 0)
		{
			st[++top] = m - 1 ;
			n = 1;
		}
		else
		{
			st[++top] = m - 1; //先把A(m-1,結果)記住
			st[++top] = m; //再算A(m-1,A(m,n-1))裡的A(m,n-1)
			n = n - 1;
		}
	}
	return n;
}
int main()
{
	int x, y;
	cout << "輸入(x,y):" << endl;
	cin >> x >> y;
	cout << A(x,y);
	return 0;
}