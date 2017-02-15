  #define DEBUG 0
  #define THRESHOLD 20
  #define DEBOUNCE 100
  #define MAT_X 10
  #define MAX_READINGS 20
  #define BAUD 115200
  
  #define SEL0 2
  #define SEL1 3
  #define SEL2 4
  #define SEL3 5
  #define EN 6

  #define MOUT 9
  #define MIN A0

int outpin, i, j  = 0;
int crossing_count = -1;
int pwm_out = 128;
int readings[MAX_READINGS][MAT_X] = {0};

void setup() {
  // put your setup code here, to run once:

  pinMode(MIN, INPUT);
  //pinMode(MOUT, OUTPUT);

  pinMode(SEL0, OUTPUT);
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SEL3, OUTPUT);
  pinMode(EN, OUTPUT);

  Serial.begin(BAUD);  

  digitalWrite(SEL0, 0);
  digitalWrite(SEL1, 0);
  digitalWrite(SEL2, 0);
  digitalWrite(SEL3, 0);
  digitalWrite(EN, 0);

  pinMode(MOUT, OUTPUT);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  readings[MAX_READINGS][MAT_X] = {0};
  for (int i = 0; i < MAT_X; i++){
    readings[crossing_count][i] = read_mux_num(i);
  }

  if (found_crossing(readings[crossing_count], MAT_X)){
    crossing_count++;
    }
  else if (crossing_count > 0){
    Serial.print("Total crossings: ");
    Serial.print(crossing_count);
    Serial.println("\n");
    crossing_count = -1;
    delay(DEBOUNCE);
    print_crossing_counts();
  }

  delay(1);
 
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

bool found_crossing(int crossing_array[], int x){
  for (int i = 0; i < x; i++){
    
      if (crossing_array[i] > THRESHOLD) {
        //Serial.println(crossing_array[i]);
        return 1;
      }
    
  }
  return 0;
}

void print_crossing_counts(){
  int cc = 0;
  while (found_crossing( readings[cc], MAT_X )){
    for (int i = 0; i < MAT_X; i++){
      Serial.print(" ");
      Serial.print(readings[cc][i]);
    }
    Serial.println("");
    cc++;
  }
  Serial.println("===============");
}

