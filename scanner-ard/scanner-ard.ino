  #define DEBUG 0
  
  #define SEL0 2
  #define SEL1 3
  #define SEL2 4
  #define SEL3 5
  #define EN 6

  #define MOUT 9
  #define MIN A0

int outpin, i, j = 0;
int pwm_out = 128;
int readings[10][10] = {0};

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
  readings[10][10] = {0};
  for (int i = 0; i < 10; i++){
    readings[0][i] = read_mux_num(i);
  }

  if (found_crossing(readings[0], 10, 10)){
    Serial.println("Found crossing");
  }
  

  if (DEBUG == 1){
  for (int i = 0; i < 10; i++){
    Serial.print(" ");
    Serial.print(readings[0][i]);
  }
  Serial.println("");
  }
  
}

void set_mux(int num){
  //Serial.print("setting mux pins:");
  //Serial.print(num); 
  //Serial.print(" -  ");
  digitalWrite(MOUT, HIGH);
  
  for(i = 0; i < 4; i++ ) 
  {
    int pin = i + 2;
    int obit = ((num >> i) & 1);
    digitalWrite(pin, obit);
    //Serial.print(" ");
    //Serial.print(obit);
  }
  //Serial.print("\n");
}

int read_mux_num(int pad){
  set_mux(pad);
  delayMicroseconds(50);
  return analogRead(MIN);
}

bool found_crossing(int crossing_array[], int x, int y){
  for (int i = 0; i < x; i++){
    
      if (crossing_array[i] > 10) {
        Serial.println(crossing_array[i]);
        return 1;
      }
    
  }
  return 0;
}

