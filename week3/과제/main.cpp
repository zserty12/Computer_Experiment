#include "String.h"  //String.h ������� �߰�
#include <iostream>  //iostream ��� ���� �߰�
using namespace std;  //namespace ��� std ���

int main() { 
	Str a("I'm a girl\n"); //Ŭ���� a�� "I'm a girl" ���ڿ��� ����
	Str b("I'm a man\n"); //Ŭ���� b�� "I'm a man" ���ڿ��� ����
	cout << a.contents(); //a�� ������ contents()�Լ��� �̿��� ���
	cout << b.contents(); //b�� ������ contents()�Լ��� �̿��� ���
	a = "I'm a boy\n"; //a�� "im a boy\n"�� ����
	cout << a.contents(); //a�� ���� ���
	cout << a.compare("I'm a a") << endl; //"I'm a a"��� ���ڿ��� ���� ����� ���
	Str& c = b;
	cout << a.compare(c) << endl;//Ŭ���� b�� ���ڿ��� Ŭ���� a�� ���ڿ��� ���� ����� ���
	return 0; //�Լ� ����
}