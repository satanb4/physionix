//============================================================================
// Name        : EMG.cpp
//============================================================================

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringSerial.h>

/* USER CODE PARAMETERS BEGINS */

#define VREF     3.3        /* Operating Voltage of RPI   */
#define ADC_RES  1023       /* 2^10 Adc resolution */
#define EMG_Pin  10         /* EMG pin number      */

/* Total 5 servos */

#define SERVO1_PIN 0        // GPIO pin 0 (BCM pin 17) for servo 1
#define SERVO2_PIN 1        // GPIO pin 1 (BCM pin 18) for servo 2
#define SERVO3_PIN 2        // GPIO pin 2 (BCM pin 27) for servo 3
#define SERVO4_PIN 3        // GPIO pin 3 (BCM pin 22) for servo 4
#define SERVO5_PIN 4        // GPIO pin 4 (BCM pin 23) for servo 5

/* Range of servo movement */

#define MIN_SERVO_RANGE 0   // minimum Servo range (in degrees)
#define MAX_SERVO_RANGE 180 // maximum Servo range (in degrees)

/* Initial Position of the servo during startup */

int Servo1_Init_Position = 1; // Desired Initial position of servos1;
int Servo2_Init_Position = 1; // Desired Initial position of servos2;
int Servo3_Init_Position = 1; // Desired Initial position of servos3;
int Servo4_Init_Position = 1; // Desired Initial position of servos4;
int Servo5_Init_Position = 1; // Desired Initial position of servos5;

/* USER CODE PARAMETERS ENDS */


#define SERVO_MIN 50   // Minimum pulse width (in microseconds)
#define SERVO_MAX 2500 // Maximum pulse width (in microseconds)

using namespace std;

// Set up the serial port for reading the EMG signals
int serial_port;

// Initialize servo
void init_servo(int servo_pin) {

    pinMode(servo_pin, OUTPUT);
    softPwmCreate(servo_pin, 0, SERVO_MAX);
}

// Move servo to specified angle (in degrees)
void move_servo(int servo_pin, int angle) {

    int pulse_width = (SERVO_MIN + ((SERVO_MAX - SERVO_MIN) / MAX_SERVO_RANGE) * angle);
    softPwmWrite(servo_pin, pulse_width);
}

void setupSerial() {

    serial_port = serialOpen("/dev/ttyUSB0", 9600);
    serialFlush(serial_port);
}

int EMG_Read()
{
    int adc_read;

    if (serialDataAvail(serial_port))
    {
        std::string line = "";
        char ch = serialGetchar(serial_port);
        while (ch != '\n') {
            line += ch;
            ch = serialGetchar(serial_port);
        }
        adc_read = std::stod(line);

        return adc_read;
    }
}

void Init_Servos()
{
    // Initialize WiringPi library
    wiringPiSetup();

    // Initialize servos
    init_servo(SERVO1_PIN);
    init_servo(SERVO2_PIN);
    init_servo(SERVO3_PIN);
    init_servo(SERVO4_PIN);
    init_servo(SERVO5_PIN);

    // Move servos to initial position
    move_servo(SERVO1_PIN, Servo1_Init_Position);
    move_servo(SERVO2_PIN, Servo2_Init_Position);
    move_servo(SERVO3_PIN, Servo3_Init_Position);
    move_servo(SERVO4_PIN, Servo4_Init_Position);
    move_servo(SERVO5_PIN, Servo5_Init_Position);
}

void Servo_movement(int a)
{

	int New_Pos = map_angle(a , 0 , ADC_RES , Min, 180);

    // Move servos to new position
    move_servo(SERVO1_PIN, New_Pos);
    move_servo(SERVO2_PIN, New_Pos);
    move_servo(SERVO3_PIN, New_Pos);
    move_servo(SERVO4_PIN, New_Pos);
    move_servo(SERVO5_PIN, New_Pos);

    // Cleanup servos
    softPwmStop(SERVO1_PIN);
    softPwmStop(SERVO2_PIN);
    softPwmStop(SERVO3_PIN);
    softPwmStop(SERVO4_PIN);
    softPwmStop(SERVO5_PIN);
}

int map_angle(int a, int b, int c, int x, int y)
{
  int angles = 0;

  angles = ((a - b) * (y - x) / (c - b)) + x;

  return angles;
}

/* Main Code BEGINS */

int main() {

	float EMG_mV = 0;
	int EMG_Raw  = 0;

	setupSerial();
	Init_Servos();

	while (1)
	{
	    EMG_Raw = EMG_Read();

	    EMG_mV  = (((float)EMG_Raw) * VREF / (float)ADC_RES);

	    cout << "EMG Volt : " << EMG_mV << "mV\n"; // prints !!!Output!!!

	    Servo_movement(EMG_RAW);
	}

	return 0;
}

/* Main Code ENDS */
