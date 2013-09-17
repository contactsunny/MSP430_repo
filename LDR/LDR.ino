//int ldr=11;
//int led=2;

void setup()
{
  pinMode(P2_3,INPUT);
  pinMode(P1_0,OUTPUT);
  
  digitalWrite(P1_0,LOW);
}

void loop()
{
  int ldrValue=analogRead(P2_3);
  analogWrite(P1_0,ldrValue);
  
  delay(500);
}
