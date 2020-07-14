
#ifndef CONTROL_HEADERS
#define CONTROL_HEADERS

enum ControlType {
    P, I , D, PI, PID
};
class Controls
{
    private:
        int kP = 0;
        int kI = 0;
        int kD = 0;
        int integrator = 0;
        int lowerLimit = 0;
        int upperLimit = 0;
        int lastError = 0;
        int outputLowerLimit = -128;
        int outputUpperLimit =  128;
        ControlType loopType = P;

    public: 
        Controls(int, int, int, ControlType, int, int  );
        int controlWorkLoop(int, int);
}


#endif
