#include "controls.hpp"

    Controls:: Controls (int kp, int ki, int kd, ControlType loopType, int lowLimit, int highLimit) {
            this.kP = kp;
            this.kI = ki;
            this.kD = kd;
            ControlType loopType = loopType;
            this.lowerLimit = lowLimit;
            this.upperLimit = highLimit;
    }

    int Controls::controlWorkLoop(int currentReading, int targetPosition)
    {
        int error = targetPosition - currentReading;

        int derivative = this.lastError - error;
        int controlValue = 0;
        this.integrator += error;

        switch (this.loopType) {
            case ControlType.P:
                controlValue = this.kP * error;

            break;

            case ControlType.I:
                controlValue += this.kI * this.integrator;
            break;

            case ControlType.D:
            controlValue +=  (this.kD * derivative ) ; 
            break;

            case ControlType.PI:
                controlValue = ( this.kI * this.integrator )  + ( this.kP * error) ;
            break;

            case ControlType.PID:
                controlValue = ( this.kI * this.integrator )  + ( this.kP * error) + ( this.kD * derivative ) ;
            break;

        }

        this.lastError = error;

        if ( controlValue > this.upperLimit)
            controlValue = this.upperLimit;

        if ( controlValue > this.lowerLimit )
            controlValue = this.lowerLimit;




        return map(controlValue, this.lowerLimit, this.upperLimit, this.outputLowerLimit , this.outputUpperLimit);
    }
