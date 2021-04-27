#define scl 8
#define sda 9
#define sclSlave 7
#define sdaSlave 6

bool reading = false;
bool prev = 1;
int numReceived = 0;
int bits[9];
int dataSlave = 0;
bool ack_wait = 0;
int flagAdress=0;

int initialized = 0;
int count = 0;
bool a[8] = {0, 0, 1, 1, 1, 1, 0, 0};
int currentBit = 0;
bool scl_val = 1;
bool scl_val1=0;
int data = 24;
bool address_sent = 0;
int counter = 201;
bool turn=0;
bool startComm=false;

int c;
void setup()
{
  Serial.begin(9600);
  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);
  digitalWrite(scl, HIGH);
  digitalWrite(sda, HIGH);
  
  
  
  digitalWrite(sda, LOW);
  turn=1;
  pinMode(sdaSlave, INPUT_PULLUP);
  pinMode(sclSlave, INPUT_PULLUP);
  Serial.begin(9600);
  
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  TCCR1B |=(1<<CS10);
  TCCR1B |=(1<<CS12);

  TCCR1B |=(1<<WGM12);
  OCR1A=64;//245
  TIMSK1 |=(1<<OCIE1A);
}
void sendBit(bool msg)
{
  digitalWrite(sda, msg);
  
}

ISR(TIMER1_COMPA_vect)
{

  PINB|=(1<<0);
  //Serial.println("   ISR...");
  //if (TCNT1==200)
 
  
}
void loop()
{
  //count++;
  //if(count == 100)
  //{
    //count = 0;
    if (counter>=0&&scl_val != digitalRead(scl))
    {
     
      scl_val = digitalRead(scl);
      //Toggle the clock
      //scl_val = !scl_val;
      //digitalWrite(scl, scl_val);

      //Send the adress when the clock is low
      if(counter==0&&scl_val == LOW )
      {
        Serial.print("0\t:Stopping condition");
        digitalWrite(sda, LOW);
        counter=-1;
      }
      else if(scl_val == LOW && initialized == 0 && currentBit <8)
      {
        //turn=1;
        //Serial.print(bitRead(data, 7 - currentBit));
        digitalWrite(sda, bitRead(data, 7 - currentBit));
        currentBit++;
      }
      //Check if address was completly sent
      if(scl_val == HIGH && currentBit == 8 && initialized == 0)
      {
        //turn=1;
        initialized = 1;
      }

      //The slave must send an ack at this step
      if(scl_val == LOW && currentBit == 8 && initialized == 1)
      {
        //turn=1;
        initialized = 2;
      }

      //Receive the ack at this step when th clk is high
      if(scl_val == HIGH && currentBit == 8 && initialized == 2)
      {
        
         pinMode(sda, INPUT_PULLUP);
        c=digitalRead(sda);
        if(c==LOW)
        {
         // turn=0;
          counter --;
          Serial.println("   Ack Rececived...");
          currentBit = 0;
          initialized = 0;
          pinMode(sda, OUTPUT);
          if(address_sent == 0)
          {
            address_sent = 1;
            data = 0;
          }
          else
          {
          data += 1;
          if(data > 255)
          {
            data = 0;
          }
          }
        Serial.print(data);
        Serial.print(":\t");
         //Serial.println("/////////");
        //Serial.println(TCNT1);
       
        }
      }
    
    
  
////////////////////////////////////////////
 // if (turn==1&&scl_val1!= digitalRead(sclSlave))
   //{   
    
    //Serial.println("slave");
    scl_val1 = digitalRead(sclSlave);
     //Serial.println(scl_val1);
    // Serial.println("master");
    // Serial.println(scl_val);
if(startComm==false &&digitalRead(sdaSlave) == LOW)
  {
    startComm=true;
  
 
    int cVal = 0;
    reading=true;
    prev = 1;
    cVal = digitalRead(sclSlave);
  }
  if(digitalRead(sclSlave) == HIGH){
      
      if(prev == 0 && numReceived < 8 && ack_wait == 0){
        //turn=0;
        bits[numReceived] = digitalRead(sdaSlave);
        Serial.print(bits[numReceived]);
        bitWrite(dataSlave,7 - numReceived, bits[numReceived]);
        numReceived++;
        prev = 1;
        if(dataSlave==24)
          flagAdress=1;
        
        if(numReceived == 8)
        {
          pinMode(sdaSlave, OUTPUT);
         // turn=1;
        }
      }
      else if(prev == 0 && numReceived < 8 && ack_wait == 1){
        prev = 1;
        ack_wait = 0;
        pinMode(sdaSlave, INPUT_PULLUP);
        //turn=1;
      }
      
   }
   else{
    
    if(flagAdress==1&&digitalRead(sclSlave) == LOW &&prev == 1 && numReceived == 8){
      //turn=0;
          numReceived = 0;
           ack_wait = 1;
          digitalWrite(sdaSlave, LOW);
          Serial.print("\t:");
          Serial.print(dataSlave);
          Serial.println("   ACK Sent...");
          
    }
    else if(numReceived==8&&flagAdress==0)
       {
        Serial.print("\t Wrong Address !!!!");
        flagAdress=2;
       }
      
       
    prev = 0;
   }
}
}
