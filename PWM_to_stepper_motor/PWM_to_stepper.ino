#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12
int step_number = 0;




const int inp_pin = 2;                // For getting input, i.e., PWM values
//const int out_to_stepper = 3;         // Stepper pulse emitting pin
//const int out_direction_stepper = 4;  // For direction to stepper
volatile long StartTime = 0;
volatile long CurrentTime = 0;
volatile long Pulses = 0;
int PulseWidth = 0;
int prevPulseWidth = 1500;
float carryover = 0;
float carryOverPulseWidth = 0;


//************//

//INFO about stepper motor parameters

float tot_steps = 2038;            // Total steps required for one revolution
float angle_coverage = 180;       // Angle covered by servo for full rotation
float range_pwm = 1000;           // Max - Min possible value from input

//************//



void PulseTimer() {
  CurrentTime = micros();
  if(CurrentTime > StartTime) {
    Pulses = CurrentTime - StartTime;           // we get the difference in time, i.e., the pulse width for "HIGH" value
    StartTime = CurrentTime;
  }
}



int current_pin = 0;
void OneStep(bool dir){
  if(dir == true) current_pin++;
  else current_pin--;
  if(current_pin > 3) current_pin = 0;
  if(current_pin <0) current_pin = 3;
  int pins[4] = {0, 0, 0, 0};
  pins[current_pin] = 1;
  for(int i=0; i<4; i++) {
    if(pins[i] == 0) digitalWrite(STEPPER_PIN_1+i, LOW);
    else digitalWrite(STEPPER_PIN_1+i, HIGH);
  }
}



int tsum = 0;


void setup() {
  Serial.begin(9600);
  pinMode(inp_pin, INPUT_PULLUP);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(inp_pin), PulseTimer, CHANGE);
}

//int myTestPrevTime = micros();
int num = 0;



void loop() {
  num++;
  
  
  if(Pulses <= 2200 && Pulses >= 800) {

    PulseWidth = Pulses;
    Serial.println(PulseWidth);
    float changeInPWM = -prevPulseWidth + PulseWidth;      // the change in PWM compared to the direct previous cycle. we get an integer 
                                                              //which could range from -20 to 20 in most cases(i.e., decent speed requirement).
    float steps_required = (tot_steps/360*angle_coverage/range_pwm*changeInPWM) + carryover;
    int temp = floor(steps_required);
    carryover = steps_required-temp;
    if(abs(temp)<1400) {

        int sum = 0;
        for(int i=0; i<abs(temp); i++) {   
          if(temp<0) OneStep(true);
          else OneStep(false);
          delay(3);
          sum++;
          if(sum>6) break;
        }
        if(temp>0) num+=sum;
        else num-=sum;
        if(sum>=7) {      //  We exit the loop after every 7 steps. This is to ensure that we can constantly get the latest 
                            //required position reading the pwm values. This is the most important part to ensure the system will not have errors.
          if(temp<0) prevPulseWidth-= floor(7/tot_steps*360/angle_coverage*range_pwm);
          else prevPulseWidth+= floor(7/tot_steps*360/angle_coverage*range_pwm);  
        }
        else{
          prevPulseWidth = PulseWidth;
        }
        Serial.print("num: ");
        Serial.println( num);

      }    
  } 
}
