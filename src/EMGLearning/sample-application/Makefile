test: emg_learning.o main.o
	g++ -Wall -pthread emg_learning.o main.o -o output
emg_learning.o: ../emg_learning.cpp ../emg_learning.h
	g++ -c -I../Eigen ../emg_learning.cpp
main.o: main.cpp
	g++ -c -I../Eigen main.cpp

clean:
	rm *.o output