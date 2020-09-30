OBJS=Timer.o example.o
INCDIRS=../

example: ${OBJS}
	g++ -o example ${OBJS}

example.o: example.cpp ../Timer.h
	g++ -c -o example.o -I${INCDIRS} example.cpp

Timer.o: ../Timer.cpp ../Timer.h
	g++ -c -o Timer.o -I${INCDIRS} ../Timer.cpp

clean:
	rm ${OBJS} example

