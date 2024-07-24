#include "String.h"  //String.h 헤더파일 추가
#include <iostream>  //iostream 헤더 파일 추가
using namespace std;  //namespace 라는 std 사용

int main() { 
	Str a("I'm a girl\n"); //클래스 a를 "I'm a girl" 문자열로 생성
	Str b("I'm a man\n"); //클래스 b를 "I'm a man" 문자열로 생성
	cout << a.contents(); //a의 내용을 contents()함수를 이용해 출력
	cout << b.contents(); //b의 내용을 contents()함수를 이용해 출력
	a = "I'm a boy\n"; //a에 "im a boy\n"를 대입
	cout << a.contents(); //a의 내용 출력
	cout << a.compare("I'm a a") << endl; //"I'm a a"라는 문자열과 비교해 결과값 출력
	Str& c = b;
	cout << a.compare(c) << endl;//클래스 b의 문자열과 클래스 a의 문자열을 비교해 결과값 출력
	return 0; //함수 종료
}