char input;
boolean check = false;
static char endMarker = '\n';
const byte DATA_MAX_SIZE = 32;
char data[DATA_MAX_SIZE]; 
int count = 0;
String msg;
char c;
String readString;


void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT); 
}

void loop(){
  
  //Serial.println("hello from arduino");
  //delay(1000);

  memset(data, 32, sizeof(data));

  if (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ',') {
      Serial.println(readString); //prints string to serial port out
      msg = readString; // lets store the string :)
      readString=""; //clears variable for new input      
     }  
    else {     
      readString += c; //makes the string readString
    }
  }

//if (msg == 'this is from') {
//      Serial.println("hello node");
//    }

//  Serial.println('im getting here');
//  Serial.println(msg);
//  delay(4000);

  
//  
//while(Serial.available() > 0) {
//      Serial.print("from incoming: ");
//      data[count] = Serial.read();
//      Serial.println(input);
////      delay(1000);
//      count+=1;
//}
//
//for(int i = 0; i < count; i++) {
//    Serial.println(data[i]);
//    msg += data[i];
////    delay(1000);
//  }
//count = 0;
//
//Serial.println(msg);
//delay(3000);


  

//
//
//      if (input == endMarker) {
//
//          if (input == '1')  {
//            Serial.println("got it");
//            check = true;
//          }
//          if (input == '0')  {
//            Serial.println("got it");
//            check = false;
//          }
//
//          
//        }
//        
//      
//      Serial.println(input);
//      delay(1000);
//}


    
//   Serial.println("got it");
//   check = true;
//   
//   if (check) {
//      Serial.println( "serial received" );
//      digitalWrite(13, HIGH);
//      delay(1000);
//      
//    } 
//   else {
//      Serial.println( "serial not received" );
//      digitalWrite(13, LOW);
//      delay(1000);
//    }
//    
}
