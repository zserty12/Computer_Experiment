#include "Header.h"

void count_page(int N, int* ary)
{
	int i;
	for (i = 1; i <= N; i++)
	{
		int temp = i;
		while (temp > 0)
		{
			ary[temp % 10] += 1;
			temp = temp / 10;
		}

	}
}
