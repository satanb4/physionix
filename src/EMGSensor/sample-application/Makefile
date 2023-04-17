test: ADS1115.o main.o
	g++ -Wall -pthread ADS1115.o main.o -o emgSensor -lpigpio
ADS1115.o: ../ADS1115.cpp ../ADS1115.h
	g++ -c ../ADS1115.cpp
main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o sample.exe
