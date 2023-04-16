test: emg_filter.o main.o
	g++ -Wall -pthread emg_filter.o main.o -o emgFilter -lfftw3
emg_filter.o: ../emg_filter.cpp ../emg_filter.h
	g++ -c ../emg_filter.cpp

clean:
	rm *.o emg_filter