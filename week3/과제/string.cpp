#include "String.h"  //String.h 헤더파일 추가
#include <cstdlib>   //cstdlib 헤더파일 추가
#include <iostream>  //iostream 헤더파일 추가
#include <cstring>   //string관련 함수 사용을 위한 cstring 헤더파일 추가
using namespace std; //std라는 namespace를 사용

Str::Str(int leng) {  //int leng을 인자로 가지는 생성자
	if (leng < 0) {  //leng값이 0보다 작게 주어지는 경우 예외 처리
		cout << "Error! The length is less than 0!" << endl;
	}
	else {
		str = new char[leng + 1]; //leng+1만큼의 str문자열을 동적할당
		len = leng; //문자열 길이 변수인 len에 leng값 저장
	}
}

Str::Str(const char* neyong) {  //char형의 neyong이라는 문자열을 인자로 갖는 생성자
	len = strlen(neyong);  //neyong 문자열의 길이를 len에 저장
	str = new char[len + 1]; //str문자열을 len+1만큼 동적 할당
	strcpy(str, neyong);  //str문자열에 neyong을 복사
}

Str :: ~Str() { //소멸자 함수
	delete[] str;  //str문자열 배열을 해제
}

int Str::length(void) { //str문자열 길이 반환 함수
	return len;  //len 값 반환
}

char* Str::contents(void) { //str문자열의 내용 반환 함수
	return str;  //str 반환
}

int Str::compare(class Str& a) {  //Str클래스의 문자열 a와의 비교 함수
	return strcmp(str, a.str); //현재 클래스 내의 str과 인자로 전달받은 a라는 클래스의 문자열과 비교하여 결과값 반환
}

int Str::compare(const char* a) {  //인자로 받은 문자열 a와 str 비교함수
	return strcmp(str, a);  //strcmp함수를 사용하여 str과 a를 비교하여 결과값 반환
}

void Str::operator=(const char* a) {  //인자로 받은 a문자열을 대입하는 함수
	delete[] str;  //a의 길이가 str보다 길이가 길면 문자열이 다 대입이 안되므로 먼저 str 문자열 해제
	len = strlen(a); //len에 a 문자열의 길이 저장
	str = new char[len + 1];  //len+1만큼 str 문자열 동적 할당
	strcpy(str, a); //str에 a문자열 복사
	return;
}

void Str::operator=(class Str& a) {  //인자로 받은 클래스의 문자열을 대입하는 함수
	delete[] str; //a.str의 길이가 str보다 길이가 길면 문자열이 다 대입이 안되므로 먼저 str 문자열 해제
	len = strlen(a.str);  //len에 a.str 문자열의 길이 저장
	str = new char[len + 1];  //len+1만큼 str 문자열 동적 할당
	strcpy(str, a.str);  //str에 a.str문자열 복사
	return;
}