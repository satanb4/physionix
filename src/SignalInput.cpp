// Read EMG signal from Grove EMG sensor connected to Grove Base Hat.
// Developer: Sayan Bandyopadhyay (sayan.bandyopadhyay@yahoo.co.in)
// Date: 2020-04-20


// Compile: g++ -Wall -o SignalInput SignalInput.cpp -lwiringPi
// Run:     sudo ./SignalInput

#include <wiringPi.h> 
#include <stdio.h>
#include <stdlib.h>

#define EMG_PIN 0

int SignalInput(void)
{
    int EMGValue;

    if (wiringPiSetup() == -1)
    {
        cout<<"Setup wiringPi failed!";
        exit(1);
    }

    while (1)
    {
        EMGValue = analogRead(EMG_PIN);
        cout<<"EMG Value: "<<EMGValue<<endl;
        delay(30);
    }

    return 0;
}

