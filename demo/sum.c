#include <stdio.h>

int main(int argc, char const *argv[]) {
	int a[] = {1, 2, 3, 4};
	int b[] = {5, 6, 0, 1};

	int sum = 0;
	int sum1 = 0;
	int sum2 = 0;

	int c = 10;

	for (int i = 0; i < 4; i++)
	{
		sum1 += b[i];
		sum2 += a[i];


	//sum = sum1 + c*sum2;
	}

	// printf("%s\n", "kanw praksi sum");
	// sum = sum1 + sum2;

	printf("sum=%d\n", sum1*sum2);

	return 0;
}
