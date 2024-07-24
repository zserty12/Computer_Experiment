#include "Header.h"
#define _CRT_SECURE_NO_WARNINGS

/*테스트 수와 페이지 수를 입력받고
페이지 수를 세는 count함수와 결과를 출력하는 print함수를 실행하는 main.c */
int main(void)
{
	int T, N;
	scanf("%d", &T); //테스트 케이스 수 입력

	for (int i = 0; i < T; i++) //테스크 케이스 수 만큼 반복
	{
		int cnt[10] = { 0 }; //0~9의 개수를 입력받을 배열 선
		scanf("%d", &N); //케이스별 페이지수 입력
		count_page(N, cnt); //count함수 실행
		print_result(cnt); //print함수 실행
	}
	return 0;
}

