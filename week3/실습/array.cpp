#include "Array.h"
#include <cstdlib>
#include<iostream>
using namespace std;
Array::Array(int size) //생성자
{
	if(size<0) //인수로 받은 size값이 0보다 작거나 같으면 오류이므로 에러 처리
	{
		cout << "Error! The size is less than 0" << endl;
	}
	else //size값이 양수이면 new를 사용해서 배열 data를 할당하고 len을 size값으로 초기화
	{
		data = new int[size];
		len = size;
	}
}
Array::~Array() //소멸자 함수 : 할당된 메모리 해제 
{
	delete[] data;
}
int Array::length() const //배열 크기 리턴
{
	return len;
}


int& Array::operator[](int i) //배열에 원소를 삽입
{
	static int tmp;

	if(i >= 0 && i < len)
	{
		return data[i];
	}
	else
	{
		cout << "Array bound error!" << endl;
		return tmp;
	}
}

int Array::operator[](int i) const  //배열의 원소값 반환
{
	if(i >= 0 && i<len)
	{
		return data[i];
	}
	else
	{
		cout << "Array bound error!" << endl;
		return 0;
	}
}
void Array::print() 
{
	int i;
	cout << "[";
	for (i = 0; i < len; i++)
		cout << " " << operator[](i);

	cout << "]";
	cout << endl;
}
