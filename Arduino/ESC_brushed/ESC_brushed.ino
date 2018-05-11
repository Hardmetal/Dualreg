//Simple dual-channel ESC for brushed motors with reverse mode

//#define DEBUG               /*define to use DEBUG mode on serial console with 115200 speed*/
#define CH2                   /*define to use dual-channel mode*/
#define IND        /*define to indicate stop on LED_BUILTIN pin*/

#define a1 5                  /*PWM out for first channel*/
#define a2 9                  /*PWM out for first channel*/
#define ia 6                  /*input for analog-servo-signal for first channel*/

#ifdef CH2
#define b1 11                 /*PWM out for second channel*/
#define b2 10                 /*PWM out for second channel*/
#define ib 7                  /*input for analog-servo-signal for second channel*/
#endif

#define st 120                /*null position of joystic*/
#define delta 10              /*delta for null position of joystic*/
#define pulseInDelay 25000    /*delay for pulseIn function*/

unsigned long a = 0;
int ra = 0;
bool ola = true;

#ifdef CH2
unsigned long b = 0;
int rb = 0;
bool olb = true;
#endif

void setup() {

  pinMode(ia, INPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  analogWrite(a1,0);
  analogWrite(a2,0);
  
#ifdef CH2
  pinMode(ib, INPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  analogWrite(b1,0);
  analogWrite(b2,0);
#endif
#ifdef IND
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
#endif
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("ESC for brushed motor");
  #ifndef CH2
  Serial.println("Single-channel mode");
  #else
  Serial.println("Dual-channel mode");
  #endif
  Serial.println("Debug mode ENABLED");
#endif
}

void loop() {
  a = pulseIn(ia, HIGH, pulseInDelay);
  a = (a - 900)/4.7f;

#ifdef CH2
  b = pulseIn(ib, HIGH, pulseInDelay);
  b = (b - 900)/4.7f;
#endif

  if ((a < 10) or (a>245)){
  ola = false;
  }else{
  ola = true;
  if ((a > (st - delta)) and (a < (st + delta))){
      ola = false;
    }else{
      ra = st - a;
    }
  }

  if (ola == false){
    ra = 0;
    analogWrite(a1,0);
    analogWrite(a2,0);
  }else{
    if(ra == abs(ra)){
      analogWrite(a1,abs(ra)*2);
      analogWrite(a2,0);
    }else{
      analogWrite(a1,0);
      analogWrite(a2,abs(ra)*2);
    }
  }

#ifdef CH2
  if ((b < 10) or (b>245)){
  olb = false;
  }else{
  olb = true;
  if ((b > (st - delta)) and (b < (st + delta))){
      olb = false;
    }else{
      rb = st - b;
    }
  }

  if (olb == false){ 
    rb = 0;
    analogWrite(b1,0);
    analogWrite(b2,0);
  }else{
    if(rb == abs(rb)){
      analogWrite(b1,abs(rb)*2);
      analogWrite(b2,0);
    }else{
      analogWrite(b1,0);
      analogWrite(b2,abs(rb)*2);
    }
  }
#endif
#ifdef IND
#ifndef CH2
  if(!ola) 
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
#else
  if(!ola and !olb) 
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
#endif
#endif
#ifdef DEBUG
#ifndef CH2
  if(a>255)
  Serial.print("NC");
  else
  Serial.print(a);
  Serial.print(" ");
  Serial.println(ra*2);
#else
  if(a>255)
  Serial.print("NC");
  else
  Serial.print(a);
  Serial.print(" ");
  Serial.print(ra*2);
  Serial.print(" ");
  if(b>255)
  Serial.print("NC");
  else
  Serial.print(b);
  Serial.print(" ");
  Serial.print(rb*2);
#endif
#endif
}
