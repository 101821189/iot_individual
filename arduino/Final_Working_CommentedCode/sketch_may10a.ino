#include <IRremote.h>

int Temp = 1;
float val;                      // lets read the output from temp sensor
const byte DATA_MAX_SIZE = 32;  
char data[DATA_MAX_SIZE];       // char array with maximum 32 data elements
String msg;                     //stored the user input from the serial 
String readString;              //read string
String temp;                    //real-time value from the temp sensor
float setTemp;                  //Turn fan based of set temp
boolean turnOn;                 //for temp sensor and fans
boolean turnOnIR;               //for the memory game
float f;                        //use to change the value of settemp
const int RECV_PIN = 9;
const int buzzer = 10;
const int ledGreen = 11;
const int ledRed = 12;
const int ledYellow = 22;
const int ledBlue = 24;

int playerID = 789;
long randNumber = 5;
int userAnswer = -1;
int gameLevel = 1;
int progressCheck = 0;
int toneBuzz = 1289;
int delayBuzz = 1500;
bool IDCheck = false;
int roundflag = 1;
//display number on the 7 segement kit
int num_array[10][7] = {  { 1, 1, 1, 1, 1, 1, 0 }, // 0
  { 0, 1, 1, 0, 0, 0, 0 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 1, 1, 1, 1, 0, 0, 1 }, // 3
  { 0, 1, 1, 0, 0, 1, 1 }, // 4
  { 1, 0, 1, 1, 0, 1, 1 }, // 5
  { 1, 0, 1, 1, 1, 1, 1 }, // 6
  { 1, 1, 1, 0, 0, 0, 0 }, // 7
  { 1, 1, 1, 1, 1, 1, 1 }, // 8
  { 1, 1, 1, 0, 0, 1, 1 }   // 9
};   

IRrecv irrecv(RECV_PIN);
decode_results results;

char floatbuf[32]; // make this at least big enough for the whole string

void setup() {
  Serial.begin(9600);
  pinMode(26, OUTPUT); // this is for triggering relay
  digitalWrite(26, HIGH); // turn fan into an off state
  setTemp = 30; // default temp
  turnOn = false;
  turnOnIR = false;
  f = 0.00;
  analogRead(12);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  randomSeed(analogRead(0));
  pinMode(buzzer, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledBlue, OUTPUT);
}

void loop() {     

  memset(data, 32, sizeof(data));

  // Read serial inputs 
  if (Serial.available()) {
    char c = Serial.read();       //gets one byte from serial buffer
    if (c == ',') {
      Serial.println(readString); //prints string to serial port out
      msg = readString;           // lets store the string :)
      temp = readString;          // store string for converting to float
                                  // convert serial input to float
      readString="";              //clears variable for new input      
     }  
    else {     
      readString += c;            //makes the string readString
    }
  }


  
  // if Message Sent by Website is = "turn on fan";
  if (msg == "turn on fan" ) {
      turnOn = true;
      turnOnIR = false;
    }
    // if Message Sent by Website is = "turn on ir";
  else if (msg == "turn on ir") {
      turnOnIR = true;
      turnOn = false;
    }
  else {
    
    }

  // Parse the string input & convert to float
  temp.toCharArray(floatbuf, sizeof(floatbuf));
  f = atof(floatbuf);

  // Set the trigger for temp
  if (f > 0 ) { 
    setTemp = f; 
  }

  
    
  // Fan Algorithm is now running  
  if(turnOn){
          
          //
          //  * Note *
          //  * Expect a 2 seconds delay
          //  * before the fan will turn off & on
          //
         val = analogRead(A12); // lets read the output from temp sensor
         float voltage =  (val/1024.0)*500;
         float cel = (voltage);
               
      ///Serial.print("Current Temp: ");
         Serial.print("temp:");
         Serial.print(cel);
         Serial.println();
         
         // Turn fan based of set temp 
          if (cel > setTemp) {
            digitalWrite(26, LOW); // turn on fan
          } 
          else if (cel < setTemp){
            digitalWrite(26, HIGH); // turn off fan
          }
          else {
             digitalWrite(26, HIGH); // turn off fan
          }
          
          delay(1000); // stop fan from jerking
          
        
        
    }

      //memory game is now running
      if (turnOnIR) {
           Serial.println("game IR is now running");
           memoryGame();
    
      }
     
}

//1.generate random number
//2.the buzzer beeping
//3.taking input from the user(ir receive)
//4.compare the answer, and show the result
void memoryGame() {
     if (roundflag == 4)                      //restart a new memory game, roundflag change back to 1
              roundflag = 1;
            do {                              //the game will stuck in the loop until roundflag is equal to 4 
              if (roundflag == 1)             //the 1st part that the memory game should do
              {
                randNumber = GenerateRandom(); //generate random number
                Serial.print("answer:");
                Serial.print(randNumber);
                Serial.println();
                buzz();                         //start beeping base on the generated number 
                roundflag++;                    //move to next step
              }
          
              if(roundflag==2)                  //taking input from the user and compare it
              {      digitalWrite(ledBlue, HIGH); // Turn the LED on
              
              takeInput(1);                     //receive the answer and compare the answer 
              
             }
                  if (roundflag == 3)         //will do this part after asnwer has been compare and show the correct anwser 
                  {
                    delay(5000);
                              resetLED();       //turn off all led
                    roundflag++;                //roundflag = 4;
                  }
            } while (roundflag != 4);           //jump out the loop if roundflag = 4
  
  }


//compare the user input from the ir controller with the generated random number
//then show the corresponding led light(red, green)and correct answer(7segement)  
//update the level progress  
void compare(int input)
{
  resetLED();                       //all led turn off
  userAnswer = input;
  if (input == randNumber)          //compare the random number with the user input(from ir controller)
  {
    //Serial.println("Correct");
      digitalWrite(ledGreen, HIGH); // Turn the LED on
    progressCheck++;                //record the numbers of answer correct, if got 3 correct correct answer consecutively(level will progress)  
    levelProgress();                //this function will update the level if got 3 correct correct answer consecutively
    Serial.print("correct:");
    Serial.print(gameLevel);
    Serial.println();
  }
  else                              //if the user input is not same as the random number
  {
    //Serial.println("Wrong");
          digitalWrite(ledRed, HIGH); // Turn the LED on
    gameLevel = 1;                    //the game level change(reset) back to 1
    progressCheck = 0;                //the numbers of correct answer reset back to 0  
    Serial.print("wrong:");
    Serial.print(gameLevel);
      Serial.println();
    delayBuzz = 1500;                 //time between the tone is reset back to level 1
  }
  if (randNumber == 10)               //the 7 segement will show 0 if the correct answer is 10
    Num_Write(0);
  else
    Num_Write(randNumber);            //show the correct answer by using 7 segment
  roundflag++;                        //the roundflag = 3, ready to move to next step
}
//Generate random, DONE
int GenerateRandom()
{
  int value = 0;
  for (int a = 0; a < 8; a++)
  {
    value = value + (analogRead(a));    //combine all the value that read from the analog pin
  }
  value = (value % 10) + 1;             //devide the value until it is less than 10, then +1 so we the number is between 1 to 10
  return value;
}

//Check unique ID
void uniqueID(int input)
{
  if (input == playerID)              //if the user input is equal to the player id IDCheck = true
  { //Serial.println("Correct ID");
    IDCheck = true;
  }
  else                                //if the user input is not equal to the player id IDCheck = false
  {
    //Serial.println("Wrong ID");
    IDCheck = false;
  }
}

//Collect input from the user
//if MaxChars == 1, we will used to compare the answer from memory game
//if MaxChars == any numbers, we will used verified the unique id (user id) 
void takeInput(int MaxChars) 
{
  if (irrecv.decode(&results)) {    //if recieve data from the ir receiver, run the switch statement
    switch (results.value) {
      case 0xFF22DD:
        //Serial.println("|<<");
        break;
      case 0xFF02FD:
        //Serial.println(">>|");
        break ;
      case 0xFF6897:
        //Serial.println("0");
        if (MaxChars == 1)      
          compare(10);
        else
          uniqueID(10);
        break ;
      case 0xFF30CF:
        //Serial.println("1");
        if (MaxChars == 1)
          compare(1);
        else
          uniqueID(1);
        break ;
      case 0xFF18E7:
        //Serial.println("2");
        if (MaxChars == 1)
          compare(2);
        else
          uniqueID(2);
        break ;
      case 0xFF7A85:
        //Serial.println("3");
        if (MaxChars == 1)
          compare(3);
        else
          uniqueID(3);
        break ;
      case 0xFF10EF:
        //Serial.println("4");
        if (MaxChars == 1)
          compare(4);
        else
          uniqueID(4);
        break ;
      case 0xFF38C7:
        //Serial.println("5");
        if (MaxChars == 1)
          compare(5);
        else
          uniqueID(5);
        break ;
      case 0xFF5AA5:
        //Serial.println("6");
        if (MaxChars == 1)
          compare(6);
        else
          uniqueID(6);
        break ;
      case 0xFF42BD:
        //Serial.println("7");
        if (MaxChars == 1)
          compare(7);
        else
          uniqueID(7);
        break ;
      case 0xFF4AB5:
        //Serial.println("8");
        if (MaxChars == 1)
          compare(8);
        else
          uniqueID(8);
        break ;
      case 0xFF52AD:
        //Serial.println("9");
        if (MaxChars == 1)
          compare(9);
        else
          uniqueID(9);
        break ;
    }

    irrecv.resume();
  }
}

//Buzz the buzzer based on level
void buzz()
{
  for (int i = 0; i < randNumber; i++)    //the loop will break based on the random number generated.
  {
    tone(buzzer, toneBuzz);
    delay(delayBuzz);
    noTone(buzzer);
    delay(delayBuzz);
  }
}

//Level up the game, DONE
void levelProgress()
{
  if (progressCheck == 3)     //if got 3 correct answer consecutively, update the difficulty level and shorter the time between the tone
  {
    gameLevel++;
    delayBuzz /= gameLevel;
    progressCheck = 0;        //reset the correct answer it back to 0
    //Serial.println("Level up! \nCurrent level:");
    //Serial.println(gameLevel);
          digitalWrite(ledYellow, HIGH); // Turn the yellow LED on
  }
}

//display specific number on the 7 segement kit based on the parameter
void Num_Write(int number)
{
  int pin = 2;
  for (int j = 0; j < 7; j++) {               //this loop will ensure which segement in the 7 segment display should be light up based on the num_array 
    digitalWrite(pin, num_array[number][j]);
    pin++;
  }
}

//Reset all LED, turning off all led
void resetLED()
{
      digitalWrite(ledGreen, LOW); // Turn the LED off
      digitalWrite(ledRed, LOW); // Turn the LED off
      digitalWrite(ledBlue, LOW); // Turn the LED off
      digitalWrite(ledYellow, LOW); // Turn the LED off
}
