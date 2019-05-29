int Temp = 1;
float val;

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT); // this is for triggering relay
  digitalWrite(7, HIGH); // turn fan into an off state
   
}

void loop() {     
     
   val = analogRead(A0); // lets read the output from temp sensor
   float voltage =  (val/1024.0)*500;
   float cel = (voltage);
     

// Serial.print("Current Temp: ");
   Serial.print(cel);
   Serial.println();
   
   
    if (cel > 27 ) {
      digitalWrite(7, LOW); // turn on fan
    } else {
      digitalWrite(7, HIGH); // turn off an
    }

    delay(1000); // stop fan from jerking
     
      
   

}
