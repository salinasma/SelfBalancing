
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
        int lastError = 0;
        int inputLowerLimit = 0;
        int inputUpperLimit = 0;
        int outputLowerLimit = 0;
        int outputUpperLimit = 0;
        ControlType loopType = p;

    public: 
        double kP = 0;
        double kI = 0;
        double kD = 0;

        Controls (double kp, double ki, double kd, ControlType loopType, int inputLowLimit, int inputHighLimit, int outputLowLimit, int outputHighLimit);
        int controlWorkLoop(int, int);
};


#endif
