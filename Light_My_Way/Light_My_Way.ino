

#include <SPI.h>
#include <Servo.h>



String stringBuf;
String IDString;
String degreeString;
int degree;
boolean skip = false;
boolean DEBUG = true;      //boolean for debugging
boolean handleID = true;    //boolean to indicate which string we are processing


Servo myHorizontalServo;
Servo myVerticalServo;

int horizontalPin = 9;
int verticalPin = 10;
int relayPin = 11;
int horizontalDegree = 90;

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  
  //connect servos to output pins
  myHorizontalServo.attach(horizontalPin);
  myVerticalServo.attach(verticalPin);
  myVerticalServo.write(90);
  myHorizontalServo.write(90);
  
  myHorizontalServo.write(19);
  myVerticalServo.write(141);
  
  
  //Below used for opening sequence
  /*
  delay(1500);
  int i = 20;
  for(i = 20; i < 120; i++)
  {
    Serial.println(i);
    delay(20);
    myHorizontalServo.write(i);
    myVerticalServo.write(165-i);
    if(i == 65){ digitalWrite(relayPin, HIGH);}
  }
  
  delay(3000);
  digitalWrite(relayPin, LOW);
  
  myHorizontalServo.write(20);
  myVerticalServo.write(20);
  delay(1000);
  myHorizontalServo.write(55);
  myVerticalServo.write(55);
  digitalWrite(relayPin, HIGH);

  myHorizontalServo.write(90);
  myVerticalServo.write(90);
  //digitalWrite(relayPin, High);
  myHorizontalServo.write(135);
  myVerticalServo.write(135);
  */
  
  
 }


void loop() {

    // While connected
    if (Serial.available() > 0) {

            
      // read the bytes incoming from the client:
      char thisChar = Serial.read();
      
      //we will only process characters that are numbers or part of the protocol
      switch(thisChar){
       case '1':
       case '2':
       case '3':
       case '4':
       case '5':
       case '6':
       case '7':
       case '8':
       case '9':
       case '0':
       case '|':
       case '#':
       case 'b':
          break;
       default:
          skip = true;
          if(DEBUG) {Serial.println("bad char");}
      }

      if(skip){
        skip = false;
        return;
      }


      // echo the bytes to the server as well:
      if(DEBUG) {Serial.write(thisChar);}
      
      //Handle Start of message
      if(thisChar == 'b')
      {
         resetStrings();    //reset strings in the event the last message was clobbered
         return;
      }
      
      //handle id-degree delimeter
      if(thisChar == '|')
      {
         IDString = String(stringBuf);
         stringBuf = String("");
         handleID = false; 
         return;
      }
      
      //handle end of message
      if(thisChar == '#'){
        degreeString = String(stringBuf);
        //server.print(stringBuf);
        degree = stringToNumber(degreeString);
        //myServo.write(stringBuf);
        if(DEBUG){
          Serial.println("");
          Serial.print("ID is");
          Serial.println(IDString);
          Serial.print("Degree is");
          Serial.println(degree);
        }
        processStream();
        resetStrings();
        return;
      }
            
      stringBuf += thisChar;       
      
    }
}



int stringToNumber(String thisString) {
  int i, value = 0, length;
  length = thisString.length();
  if(DEBUG){
     Serial.print("length is");
     Serial.println(length);
  }
  for(i=0; i<length; i++) {
    value = (10*value) + thisString.charAt(i)-(int) '0';;
  }
  return value;
}

void resetStrings(){
 degreeString = String("");
 IDString = String(""); 
 stringBuf = String("");
 handleID = true;
}


void processStream(){

  //handle horizontal servo
  if (IDString == "0"){
    if(DEBUG) {
      Serial.print("I am going to move horizontal to this position ");
      Serial.println(degree);
      Serial.print(horizontalDegree);
    }
    //myHorizontalServo.write(degree);
    if(degree > horizontalDegree){
      for(int i = horizontalDegree; i <= degree; i++){
        myHorizontalServo.write(i);
        if(DEBUG){Serial.print("going to");
        Serial.println(i);
        }
      }
    }
    if(degree < horizontalDegree){
      for(int i = horizontalDegree; i >= degree; i--){
        myHorizontalServo.write(i);
                if(DEBUG){Serial.print("going to");
        Serial.println(i);
        }
      }
    }
    //if(DEBUG){Serial.print
    horizontalDegree = degree;
    return;
   }   
  
  //handle vertical servo
  if (IDString == "1"){
    if(DEBUG) {
      Serial.print("I am going to move vertical to this position ");
      Serial.println(degree);
    }
    myVerticalServo.write(degree);
    return;
   } 
   
  //handle turning flashlight on 
  if (IDString == "2"){
    if(DEBUG) {
      Serial.print("I am going to turn on flashlight");
    }
    digitalWrite(relayPin, HIGH);
    return;
   }    
   
   
  //handle turning flashlight off
  if (IDString == "3"){
    if(DEBUG) {
      Serial.print("I am going to turn off flashlight");
    }
    digitalWrite(relayPin, LOW);
    return;
   }   
}

