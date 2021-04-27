#define scl 8
#define sda 9

int initialized = 0;
int count = 0;
bool a[8] = {0, 0, 1, 1, 1, 1, 0, 0};
int currentBit = 0;
bool scl_val = 1;
int data = 24;
bool address_sent = 0;
int counter = 201;



int c;
void setup()
{
  Serial.begin(9600);
  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);
  digitalWrite(scl, HIGH);
  digitalWrite(sda, HIGH);
  
  delay(2);
  digitalWrite(sda, LOW);
  
  
}
void sendBit(bool msg)
{
  digitalWrite(sda, msg);
  
}

void loop()
{
  count++;
  if(count == 100)
  {
    count = 0;
    if (counter >= 0)
    {
      //Toggle the clock
      scl_val = !scl_val;
      digitalWrite(scl, scl_val);

      //Send the adress when the clock is low
      if(counter==0&&scl_val == LOW )
      {
        Serial.print("0\t:Stopping condition");
        digitalWrite(sda, LOW);
        counter=-1;
      }
      else if(scl_val == LOW && initialized == 0 && currentBit <8)
      {
        Serial.print(bitRead(data, 7 - currentBit));
        digitalWrite(sda, bitRead(data, 7 - currentBit));
        currentBit++;
      }
      //Check if address was completly sent
      if(scl_val == HIGH && currentBit == 8 && initialized == 0)
      {
        initialized = 1;
      }

      //The slave must send an ack at this step
      if(scl_val == LOW && currentBit == 8 && initialized == 1)
      {
       
        initialized = 2;
      }

      //Receive the ack at this step when th clk is high
      if(scl_val == HIGH && currentBit == 8 && initialized == 2)
      {
         pinMode(sda, INPUT_PULLUP);
        c=digitalRead(sda);
        if(c==LOW)
        {
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
       
        }
      }
    }
    
  }
}
