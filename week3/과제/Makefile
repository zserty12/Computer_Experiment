cc=g++     #메크로 정의, g++를 이용하여 컴파일 하고 g++ 대신 $(cc)를 사용
target = String   #Makefile 의 최종 목표가 되는 파일 이름

objects = main.o string.o  #Makefile에서 사용되는 오브젝트 파일들

$(target) : $(objects)  #String이라는 target을 만들기 위해 오브젝트 파일들이 필요한 종속적인 구조
	$(cc) $(cflags) -o $(target) $(objects)  #오브젝트파일들을 컴파일하여 String이라는 target파일의 이름을 생성

$(objects) : String.h   #String.h라는 헤더파일을 참조하여 오브젝트 파일 사용

.PHONY : clean  #동일한 이름의 파일 존재로 인한 충돌을 막고 make의 성능을 향상시키기 위해 PHONY 타겟 사용
clean :  #clean할 파일들을 선언 (target 파일들과 오브젝트 파일들)
	rm $(target) $(objects)
