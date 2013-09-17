int dataIn=11;
int led=10;

void setup()
{
  pinMode(dataIn,INPUT);
  pinMode(led,OUTPUT);
  
  digitalWrite(led,LOW);
}

void loop()
{
  int dataValue=digitalRead(dataIn);
  if(dataValue)
    digitalWrite(led,HIGH);
  else
    digitalWrite(led,LOW);
}
