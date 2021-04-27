#define scl 8
#define sda 9

bool reading = false;
bool prev = 1;
int numReceived = 0;
int bits[9];
int data = 0;
bool ack_wait = 0;
int flagAdress=0;
void setup() {
  pinMode(sda, INPUT_PULLUP);
  pinMode(scl, INPUT);
  Serial.begin(9600);
  
  while(digitalRead(sda) == HIGH);
    int cVal = 0;
    reading=true;
    prev = 1;
    cVal = digitalRead(scl);
}

void loop() {

    
    if(digitalRead(scl) == HIGH){
      
      if(prev == 0 && numReceived < 8 && ack_wait == 0){
        bits[numReceived] = digitalRead(sda);
        Serial.print(bits[numReceived]);
        bitWrite(data,7 - numReceived, bits[numReceived]);
        numReceived++;
        prev = 1;
        if(data==24)
          flagAdress=1;
        
        if(numReceived == 8)pinMode(sda, OUTPUT);
      }
      else if(prev == 0 && numReceived < 8 && ack_wait == 1){
        prev = 1;
        ack_wait = 0;
        pinMode(sda, INPUT_PULLUP);
        
      }
      
   }
   else{
    
    if(flagAdress==1&&digitalRead(scl) == LOW &&prev == 1 && numReceived == 8){
          numReceived = 0;
           ack_wait = 1;
          digitalWrite(sda, LOW);
          Serial.print("\t:");
          Serial.print(data);
          Serial.println("   ACK Sent...");
          
    }
    else if(flagAdress==0)
       {
        Serial.print("\t Wrong Address !!!!");
        flagAdress=2;
       }
      
       
    prev = 0;
   }
  
}
