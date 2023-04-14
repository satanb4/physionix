test: emg_filter.o
	g++ -Wall -pthread emg_filter.o -o emgFilter -lfftw3
emg_filter.o: ../emg_filter.cpp ../include/emg_filter.h
	g++ -c ../emg_filter.cpp

clean:
	rm *.o