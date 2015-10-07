#include <Stepper.h>

const int laser = 13;
const int stepsPerRevolution = 200;
const int numberOfReads = 3;

int i = 0;
int read = 0;
int previousAvg = 0;
int reads[numberOfReads] = {0, 0, 0};
int avg = 0;
int initial = 0;
bool stop = false;
bool first = true;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(60);
  Serial.begin(9600);
  pinMode(laser, OUTPUT);
}

void loop() {
  //Another option would be to take an average for the first few iterations
  //then use that metric for all comparisons past this. The constant rising
  //in values over time might become an issue though.
  delay(500);

  if(i > 0){
    first = false;
  }

  read = analogRead(A0);

  if(first){
    for(int j = 0; j < numberOfReads; j++){
      reads[j] = read;
    }

    initial = read;
  }

  if(!stop){
    reads[i] = read;
  }

  //Take the average of the last numberOfReads values
  for(int j = 0; j < numberOfReads; j++){
    avg += reads[j];
  }
  avg = avg/numberOfReads;

  Serial.print("Average:  ");
  Serial.print(avg);
  Serial.print(" Read:  ");
  Serial.println(read);

  //If there was a change of more than 10.
  if((read - avg) > 10){
    stop = true;
    digitalWrite(13, HIGH);
  }
  else{
    stop = false;
    digitalWrite(13, LOW);
  }

  //increment up the index
  i++;
  if(i == numberOfReads){
    i = 0;
  }

  if((read - avg) > 200 && stop){
    for(int j = 0; j < numberOfReads; j++){
      reads[j] = initial;
    }
  }

  //Reset avg for next iteration
  avg = 0;
}

//  myStepper.step(stepsPerRevolution);
//  myStepper.step(stepsPerRevolution, laser);

//  for(int i = 0; i < 12; i++){
//    myStepper.step(2, laser);
//  }
//
//  for(int i = 0; i < 12; i++){
//    myStepper.step(-2, laser);
//  }
