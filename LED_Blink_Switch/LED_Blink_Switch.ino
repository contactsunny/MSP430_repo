void setup()
{
  pinMode(P1_3,INPUT_PULLUP);
  pinMode(P1_0,OUTPUT);
  pinMode(P1_6,OUTPUT);  
}

void loop()
{
  int buttonValue=digitalRead(P1_3);
  int ledRedState=digitalRead(P1_0);;
  int ledGreenState=digitalRead(P1_6);;
  if(!buttonValue)
  {
    if(ledRedState)
    {
      digitalWrite(P1_6,HIGH);
      digitalWrite(P1_0,LOW);
    }      
    else
    {
      digitalWrite(P1_0,HIGH);
      digitalWrite(P1_6,LOW);
    }
  }
}
