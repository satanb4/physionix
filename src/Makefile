test: ADS1115.o emg_filter.o EMGdata.o main.o
	g++ -Wall -pthread ADS1115.o emg_filter.o EMGdata.o main.o -o physionix -lpigpio -lfftw3
ADS1115.o: EMGSensor/ADS1115.cpp EMGSensor/ADS1115.h
	g++ -c EMGSensor/ADS1115.cpp
emg_filter.o: EMGProcessing/emg_filter.cpp EMGProcessing/emg_filter.h
	g++ -c EMGProcessing/emg_filter.cpp
EMGdata.o: EMGData/EMGdata.cpp EMGData/EMGdata.h
	g++ -c EMGData/EMGdata.cpp
main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o physionix
