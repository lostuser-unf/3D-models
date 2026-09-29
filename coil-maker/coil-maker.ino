// 5 зуб/см = 0.5/мм == 180град / мм == 3200 шаг/мм
#include <math.h>
const float WIRE_D = 0.46; // in mm

const float COIL_LEN = 31.5; // in mm
const float COIL_D = 10.0; // in mm

const int TURNS = 100; //
const int TURNS_BY_LAYER = floor(COIL_LEN/WIRE_D);


// const bool STOP_LAYER = false;// stop after layer ended, for apply isolation

// machine config
const bool DIR_PRESET = LOW; // PRESET TO ENDSTOP!!!!!
const float THREAD_PITCH = 5; // turn/10mm
const int STEPS_TO_360 = 200 *2; // 1.8 stepper motor
const float ONE_STEP_ANGLE = 360 / STEPS_TO_360;
const float STEP_FOR_MM = STEPS_TO_360 * 10 / THREAD_PITCH;

const int LOWERING = 1;
const int STEP_45 = floor(STEPS_TO_360 / 8) * LOWERING;

// boot config
const int STEP_THREAD = 2;
const int DIR_THREAD  = 3;
const int STEP_COIL = 4;
const int DIR_COIL  = 5;
const int EN_DRIVERS = 6;
const int ENDSTOP = 7;

void setup() {
    pinMode(STEP_THREAD, OUTPUT);
    pinMode(DIR_THREAD, OUTPUT);
    pinMode(STEP_COIL, OUTPUT);
    pinMode(DIR_COIL, OUTPUT);
    pinMode(EN_DRIVERS, OUTPUT);
    pinMode(ENDSTOP, INPUT);

    digitalWrite(EN_DRIVERS, HIGH);
    digitalWrite(DIR_THREAD, DIR_PRESET);
    digitalWrite(DIR_COIL, HIGH);
}

/////////////////////////////////////////////

void loop() {
    delay(2000));
    digitalWrite(EN_DRIVERS, LOW);

    goToEndstop();

    for(int i = TURNS, digitalWrite(DIR_THREAD, !DIR_PRESET); i > 0; i -= TURNS_BY_LAYER){
        int turns_need = (i >= TURNS_BY_LAYER) ? TURNS_BY_LAYER : i;
        {
            move_mm(round(STEP_FOR_MM * WIRE_D / 2), STEP_THREAD, 100);
            move(round(STEPS_TO_360/2); STEP_COIL, 100);
        }
        digitalWrite(DIR_THREAD, !DIR_THREAD);
        delay(1000);
    }
    goToEndstop();



    digitalWrite(EN_DRIVERS, HIGH);

    while(!digitalRead(ENDSTOP))
        delay(1000);



}

/////////////////////////////////////////////

void goToEndstop(void){
    digitalWrite(DIR_THREAD, DIR_PRESET);
    for (;!digitalRead(ENDSTOP);){
        step(STEP_THREAD, 100);
    }
    digitalWrite(DIR_THREAD, !DIR_PRESET);
    delay(500);

    for (;digitalRead(ENDSTOP);){
        step(STEP_THREAD, 200);
    }
    delay(500);
}

void move(int step_to_move, int motor, int time_mcs) {
    for (int i = 0; i < step_to_move && !digitalRead(ENDSTOP)
        ; i++) {
        digitalWrite(motor, HIGH);
        delayMicroseconds(time_mcs);
        digitalWrite(motor, LOW);
        delayMicroseconds(time_mcs);
    }
}

void step(int motor, int time_mcs) {
    digitalWrite(motor, HIGH);
    delayMicroseconds(time_mcs);
    digitalWrite(motor, LOW);
    delayMicroseconds(time_mcs);
}

void move_mm(float mm, int motor, int time_mcs){
    if(mm == 0)
        return 0;

    digitalWrite(DIR_THREAD, (mm > 0 ? !DIR_PRESET : DIR_PRESET));
    int steps = floor(STEP_FOR_MM * mm);
    move(steps, motor, time_mcs);
}
