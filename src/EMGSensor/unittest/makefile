output: I2Cdev.o EMG_sensor.o main.o
	g++ I2Cdev.o EMG_sensor.o main.o -o output
I2Cdev.o: ../I2Cdev.cpp ../I2Cdev.h
	g++ -c ../I2Cdev.cpp
EMG_sensor.o: ../EMG_sensor.cpp ../EMG_sensor.h
	g++ -c ../EMG_sensor.cpp
main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o output