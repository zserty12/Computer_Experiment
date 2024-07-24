#ifndef __ARRAY__
#define __ARRAY__

class Array{
	protected:
		int *data; //�迭 ������
		int len;  //�迭 ũ��
	public:
			Array(int size); //������
			~Array(); //�Ҹ���

			int length() const; //�迭ũ�� ����

			int& operator[](int i); //leftvalue �迭�� ���Ҹ� ����
			int operator[](int i)const;  //rightvalue �迭�� ���� ���� ��ȯ

			void print();
};
#endif
