  
  #define SEL0 2
  #define SEL1 3
  #define SEL2 4
  #define SEL3 5
  #define EN 6

  #define MOUT 9
  #define MIN A0

int outpin, i = 0;
int pwm_out = 128;
void setup() {
  // put your setup code here, to run once:

  pinMode(MIN, INPUT);
  //pinMode(MOUT, OUTPUT);

  pinMode(SEL0, OUTPUT);
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SEL3, OUTPUT);
  pinMode(EN, OUTPUT);

  Serial.begin(9600);  

  digitalWrite(SEL0, 0);
  digitalWrite(SEL1, 0);
  digitalWrite(SEL2, 0);
  digitalWrite(SEL3, 0);
  digitalWrite(EN, 0);

  pinMode(MOUT, OUTPUT);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    outpin = Serial.parseInt();
    Serial.println(outpin);
    set_mux(outpin);
  }
    
  Serial.println(analogRead(MIN));
  
  delay(1000);
  }

void set_mux(int num){
  Serial.print("setting mux pins:");
  Serial.print(num); 
  Serial.print(" -  ");
  
  for(i = 0; i < 4; i++ ) 
  {
    int pin = i + 2;
    int obit = ((num >> i) & 1);
    digitalWrite(pin, obit);
    Serial.print(" ");
    Serial.print(obit);
  }
  Serial.print("\n");
}

