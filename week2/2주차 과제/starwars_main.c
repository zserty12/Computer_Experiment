#include "Header.h"
#define _CRT_SECURE_NO_WARNINGS

/*�׽�Ʈ ���� ������ ���� �Է¹ް�
������ ���� ���� count�Լ��� ����� ����ϴ� print�Լ��� �����ϴ� main.c */
int main(void)
{
	int T, N;
	scanf("%d", &T); //�׽�Ʈ ���̽� �� �Է�

	for (int i = 0; i < T; i++) //�׽�ũ ���̽� �� ��ŭ �ݺ�
	{
		int cnt[10] = { 0 }; //0~9�� ������ �Է¹��� �迭 ��
		scanf("%d", &N); //���̽��� �������� �Է�
		count_page(N, cnt); //count�Լ� ����
		print_result(cnt); //print�Լ� ����
	}
	return 0;
}

