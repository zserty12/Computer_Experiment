#ifndef __ARRAY__
#define __ARRAY__

class Array{
	protected:
		int *data; //배열 포인터
		int len;  //배열 크기
	public:
			Array(int size); //생성자
			~Array(); //소멸자

			int length() const; //배열크기 리턴

			int& operator[](int i); //leftvalue 배열에 원소를 삽입
			int operator[](int i)const;  //rightvalue 배열의 원소 값을 반환

			void print();
};
#endif
