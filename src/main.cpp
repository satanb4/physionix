#include "EMGData/EMGdata.h"

int main(int argc, char* argv[]) {
	std::cout << "Starting application";
	EMGdata emgdata;
	emgdata._start();
	getchar();
	emgdata._stop();
	return 0;
}
