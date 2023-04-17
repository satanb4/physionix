#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

#define VREF     3.3        /* Operating Voltage of RPI   */
#define ADC_RES  1023       /* 2^10 Adc resolution */
#define EMG_Pin  10         /* EMG pin number      */
#define MIN_SERVO_RANGE 0   // minimum Servo range (in degrees)
#define MAX_SERVO_RANGE 180 // maximum Servo range (in degrees)
#define SERVO_MIN 50   // Minimum pulse width (in microseconds)
#define SERVO_MAX 2500 // Maximum pulse width (in microseconds)

// Define the GPIO pin numbers for the servos
const int servo1_pin = 0;
const int servo2_pin = 1;
const int servo3_pin = 2;
const int servo4_pin = 3;

// Servo angle conversion function
int angleToPulse(int angle) {
    return (angle * 2000 / 180) + 500;
}

// Servo control function
void setServoAngle(int pin, int angle) {
    int pulse = angleToPulse(angle);
    softPwmWrite(pin, pulse);
}

// Servo state functions
void relaxedState() {
    setServoAngle(servo1_pin, 0);
    setServoAngle(servo2_pin, 0);
    setServoAngle(servo3_pin, 0);
    setServoAngle(servo4_pin, 0);
}

void flexState(int angle) {
    setServoAngle(servo1_pin, angle);
    setServoAngle(servo2_pin, angle);
    setServoAngle(servo3_pin, angle);
    setServoAngle(servo4_pin, 0);
}

void rotateState(int angle) {
    setServoAngle(servo1_pin, 0);
    setServoAngle(servo2_pin, 0);
    setServoAngle(servo3_pin, 0);
    setServoAngle(servo4_pin, angle);
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
int map_angle(int a, int b, int c, int x, int y)
{
  int angles = 0;

  angles = ((a - b) * (y - x) / (c - b)) + x;

  return angles;
}
int main() {
    // Initialize WiringPi
    wiringPiSetup();
int EMG_Raw = 0;
int Angle = 0;
    // Initialize servos with WiringPi
    softPwmCreate(servo1_pin, 0, 20000);
    softPwmCreate(servo2_pin, 0, 20000);
    softPwmCreate(servo3_pin, 0, 20000);
    softPwmCreate(servo4_pin, 0, 20000);
    	    EMG_Raw = EMG_Read();
    	    Angle = map_angle (EMG_Raw,0,1023,0,180);

	    EMG_mV  = (((float)EMG_Raw) * VREF / (float)ADC_RES);

	    cout << "EMG Volt : " << EMG_mV << "mV\n"; // prints !!!Output!!!

    

    // Connect your EMG sensor and implement its functionality here
    // Depending on the EMG signal, call one of the following state functions:

    // Example usage based on the filtered EMG value
    //int filteredEmgValue = readFilteredEmgValue();

   if (Angle <= 10) {
       relaxedState();
    } else if (Angle > 10 && Angle <= 45) {
        flexState(45);
    } else if (Angle > 45) {
        rotateState(90);
    }

    return 0;
}
