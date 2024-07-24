#include "String.h"  //String.h ������� �߰�
#include <cstdlib>   //cstdlib ������� �߰�
#include <iostream>  //iostream ������� �߰�
#include <cstring>   //string���� �Լ� ����� ���� cstring ������� �߰�
using namespace std; //std��� namespace�� ���

Str::Str(int leng) {  //int leng�� ���ڷ� ������ ������
	if (leng < 0) {  //leng���� 0���� �۰� �־����� ��� ���� ó��
		cout << "Error! The length is less than 0!" << endl;
	}
	else {
		str = new char[leng + 1]; //leng+1��ŭ�� str���ڿ��� �����Ҵ�
		len = leng; //���ڿ� ���� ������ len�� leng�� ����
	}
}

Str::Str(const char* neyong) {  //char���� neyong�̶�� ���ڿ��� ���ڷ� ���� ������
	len = strlen(neyong);  //neyong ���ڿ��� ���̸� len�� ����
	str = new char[len + 1]; //str���ڿ��� len+1��ŭ ���� �Ҵ�
	strcpy(str, neyong);  //str���ڿ��� neyong�� ����
}

Str :: ~Str() { //�Ҹ��� �Լ�
	delete[] str;  //str���ڿ� �迭�� ����
}

int Str::length(void) { //str���ڿ� ���� ��ȯ �Լ�
	return len;  //len �� ��ȯ
}

char* Str::contents(void) { //str���ڿ��� ���� ��ȯ �Լ�
	return str;  //str ��ȯ
}

int Str::compare(class Str& a) {  //StrŬ������ ���ڿ� a���� �� �Լ�
	return strcmp(str, a.str); //���� Ŭ���� ���� str�� ���ڷ� ���޹��� a��� Ŭ������ ���ڿ��� ���Ͽ� ����� ��ȯ
}

int Str::compare(const char* a) {  //���ڷ� ���� ���ڿ� a�� str ���Լ�
	return strcmp(str, a);  //strcmp�Լ��� ����Ͽ� str�� a�� ���Ͽ� ����� ��ȯ
}

void Str::operator=(const char* a) {  //���ڷ� ���� a���ڿ��� �����ϴ� �Լ�
	delete[] str;  //a�� ���̰� str���� ���̰� ��� ���ڿ��� �� ������ �ȵǹǷ� ���� str ���ڿ� ����
	len = strlen(a); //len�� a ���ڿ��� ���� ����
	str = new char[len + 1];  //len+1��ŭ str ���ڿ� ���� �Ҵ�
	strcpy(str, a); //str�� a���ڿ� ����
	return;
}

void Str::operator=(class Str& a) {  //���ڷ� ���� Ŭ������ ���ڿ��� �����ϴ� �Լ�
	delete[] str; //a.str�� ���̰� str���� ���̰� ��� ���ڿ��� �� ������ �ȵǹǷ� ���� str ���ڿ� ����
	len = strlen(a.str);  //len�� a.str ���ڿ��� ���� ����
	str = new char[len + 1];  //len+1��ŭ str ���ڿ� ���� �Ҵ�
	strcpy(str, a.str);  //str�� a.str���ڿ� ����
	return;
}