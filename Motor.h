/*
 * Author: Pramuka Perera
 * Description: Software interface for Polulu Part 3072 Micro-metal Gearmotor 
 */

#define WHEEL_DIAM_MM 90

class Motor {
    public:
        Motor (void);
        void rotate (int rpm); 
        void neutral (int rpm);
        int getRpm (void); 

        int rpm;

    private:
        int pwmA, pwmB, tachA, tachB;
        int tachAMeasuredTime, tachBMeasuredTime;

        void tachAInterruptHandler (void);
        void tachBInterruptHandler (void);
