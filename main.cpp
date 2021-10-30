#include <iostream>
#include "BigInt.h"
using namespace std;
int main() {
	{
		BigInt a = 1;
		for (int i = 1; i < 1000; ++i)
			a *= i;
		cout << a <<  endl;
	}
	{
		BigInt a = 2;
		cout << a.pow(1000) << endl;
	}
	{
		BigInt a, b;
		int c;
		long long d;
		cin >> a >> b >> c >> d;
		cout << a + b << "\n" << a + c << "\n" << a + d << endl;
	}
	return 0;
}