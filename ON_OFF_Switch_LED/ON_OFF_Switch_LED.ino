void setup()
{
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,INPUT);
  
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);  
}

void loop()
{
  int switchValue=digitalRead(11);
  if(switchValue)
  {
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);    
  }
  else
  {
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);    
  }
}
