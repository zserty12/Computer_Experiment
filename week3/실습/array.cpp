#include "Array.h"
#include <cstdlib>
#include<iostream>
using namespace std;
Array::Array(int size) //������
{
	if(size<0) //�μ��� ���� size���� 0���� �۰ų� ������ �����̹Ƿ� ���� ó��
	{
		cout << "Error! The size is less than 0" << endl;
	}
	else //size���� ����̸� new�� ����ؼ� �迭 data�� �Ҵ��ϰ� len�� size������ �ʱ�ȭ
	{
		data = new int[size];
		len = size;
	}
}
Array::~Array() //�Ҹ��� �Լ� : �Ҵ�� �޸� ���� 
{
	delete[] data;
}
int Array::length() const //�迭 ũ�� ����
{
	return len;
}


int& Array::operator[](int i) //�迭�� ���Ҹ� ����
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

int Array::operator[](int i) const  //�迭�� ���Ұ� ��ȯ
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
