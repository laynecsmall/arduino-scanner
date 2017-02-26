
//define system settings
#define DEBUG 0 //enable debug mode, print one row once a second, good for human interatction
#define THRESHOLD 20 //ignore any crossings below this value, needed because shorted pin on soic to wire board
#define DEBOUNCE 1000
#define MUX_SWITCH_WAIT 50 //wait for this many us after switching mux lines
#define MAT_X 10
#define MAX_READINGS 40 //at can rolling speed never seen more than 18 readings
#define BAUD 115200

//define arduino pins
#define SEL0 2
#define SEL1 3
#define SEL2 4
#define SEL3 5
#define EN 6

#define MOUT 9
#define MIN A0

//initialize variables
int outpin, i, j  = 0;
int crossing_count = 0;
int pwm_out = 128;
int readings[MAX_READINGS][MAT_X] = {0};
bool end_scan = 0;
bool crossing_found = 0;

void setup() {
  
  //set arduino pin directions
  pinMode(MIN, INPUT);

  pinMode(SEL0, OUTPUT);
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SEL3, OUTPUT);
  pinMode(EN, OUTPUT);
  
  Serial.begin(BAUD);  

  set_mux(0); //set the mux to pin 0
  digitalWrite(EN, 0); //enable the mux

  pinMode(MOUT, OUTPUT); //write pin high for power to the mat row
  //possibly more than one row for direction sensing

}

void loop() {
  
  if (DEBUG == 0){
    readings[MAX_READINGS][MAT_X] = {0}; //reinitialize array

    for (int i = 0; i < MAT_X; i++){ //read a row
      readings[crossing_count][i] = read_mux_num(i);
    }
    
    crossing_found = found_crossing(readings[crossing_count], MAT_X); //check for crossings
    
    if (crossing_found){ //if there were useful results in the latest read row
      crossing_count++; //increase the row count, scan again
      end_scan = 0;
      }
    
    else if (!crossing_found && end_scan == 0){ //no scan found, loop once more, see if there's data next time
      end_scan = 1;
    }
    
    else if (crossing_count > 0 && end_scan == 1){ //no useful results in this row, if we've been scanning do finish scanning routine
      //print summary data
      Serial.print("Total crossings: ");
      Serial.print(crossing_count);
      Serial.println("\n");

                  
      print_crossing_counts();

      //reset row count, then wait to debounce 
      crossing_count = 0;
      delay(DEBOUNCE);
    }
  
    delay(0.5); //delay 1ms to produce appropriate number of row scans at can rolling speed
  }

  //debug mode, slow used for human interaction/testing the mat manually
  else if (DEBUG == 1){
    readings[MAX_READINGS][MAT_X] = {0}; //initialize the readings array
    for (int i = 0; i < MAT_X; i++){ //read one row
      readings[0][i] = read_mux_num(i);
    }
    if (found_crossing(readings[0], MAT_X)){ //if there's anything useful on the row, print it
       for (int i = 0; i < MAT_X; i++){
         Serial.print(" ");
         Serial.print(readings[0][i]);
       }
       Serial.println("");
    }
    delay(1000); //wait one second. used to avoide overwhelming amount of data
  }
  
} 

//set the multiplexer out line as appropriate
//num is the target line connected to the analogue read
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

//take a reading from the specifid pad, on x axis
int read_mux_num(int pad){
  set_mux(pad);
  delayMicroseconds(MUX_SWITCH_WAIT); //wait after setting mux lines. May not be needed TODO test this
  return analogRead(MIN);
}

//check a crossing row to see if it contains data above the threshold
//returns 1 if has data, 0 if no data found
bool found_crossing(int crossing_array[], int x){
  for (int i = 0; i < x; i++){
    
      if (crossing_array[i] > THRESHOLD) {
        //Serial.println(crossing_array[i]);
        return 1;
      }
    
  }
  return 0;
}

//print a set of crossings via serial
//crossing data taken from global readings array
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

