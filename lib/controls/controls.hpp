
#ifndef CONTROL_HEADERS
#define CONTROL_HEADERS

#include <Arduino.h>

enum ControlType {
    p, i, d, pi, pd, pid
};

class Controls
{
    private:
        int integrator = 0;
        int lowerLimit = 0;
        int upperLimit = 0;
        int lastError = 0;
        int outputLowerLimit = 0;
        int outputUpperLimit = 250;
        ControlType loopType = p;

    public: 
        double kP = 0;
        double kI = 0;
        double kD = 0;

        Controls(double, double, double, ControlType, int, int);
        int controlWorkLoop(int, int);
};


#endif
