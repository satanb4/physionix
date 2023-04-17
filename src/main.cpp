/**
 * @file main.cpp
 * @brief Main file for the EMGdata application
 * @author Midhun, Sayan
 * @version 1.0
 * @date 2023-04-17
 * This is the entrypoint for the EMG application. It calls the functionality from the EMGdata class.
 * Contains the _start and _stop functions.
 * @bug No known bugs.
*/

#include "EMGData/EMGdata.h"

int main(int argc, char* argv[]) {
	std::cout << "Starting application";
	EMGdata emgdata;
	emgdata._start();
	getchar();
	emgdata._stop();
	return 0;
}
