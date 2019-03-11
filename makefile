all: test testperform

test: test.cpp Schedule.o Coroutine.o
	g++ -std=c++11 -o test test.cpp Schedule.o Coroutine.o

testperform: testperform.cpp Schedule.o Coroutine.o
	g++ -std=c++11 -o testperform testperform.cpp Schedule.o Coroutine.o

Schedule.o: Schedule.cpp Schedule.h
	g++ -std=c++11 -c Schedule.cpp

Coroutine.o: Coroutine.cpp Coroutine.h
	g++ -std=c++11 -c Coroutine.cpp

.PHONY: clean cleanall

clean:
	rm -rf Schedule.o Coroutine.o

cleanall:
	rm -rf Schedule.o Coroutine.o test testperform
