#include "controls.hpp"

    Controls:: Controls (double kp, double ki, double kd, ControlType loopType, int lowLimit, int highLimit) {
            this->kP = kp;
            this->kI = ki;
            this->kD = kd;
            this->loopType = loopType;
            this->lowerLimit = lowLimit;
            this->upperLimit = highLimit;
    }

    int Controls::controlWorkLoop(int currentReading, int targetPosition)
    {
        int error = targetPosition - currentReading;

        int derivative = this->lastError - error;
        double controlValue = 0;

        this->integrator += error;

        switch (this->loopType) {
            case p:
                controlValue = this->kP * error;
            break;

            case i:
                controlValue += this->kI * this->integrator;
            break;

            case d:
                controlValue += this->kD * derivative; 
            break;

            case pi:
                controlValue = (this->kP * error) + (this->kI * this->integrator);
            break;

            case pd:
                controlValue = (this->kP * error) + (this->kD * derivative);
            break;

            case pid:
                controlValue = (this->kP * error) + (this->kI * this->integrator) + ( this->kD * derivative );
            break;

        }

        this->lastError = error;

        if ( controlValue > this->upperLimit)
            controlValue = this->upperLimit;

        if ( controlValue < this->lowerLimit )
            controlValue = this->lowerLimit;




        return map(controlValue, this->lowerLimit, this->upperLimit, this->outputLowerLimit , this->outputUpperLimit);
    }
