int i=0;
int led=2;
void setup()
{
  Serial.begin(9600);
  Serial.println("Serial test on TI MSP430, by Sunny");
  
  pinMode(led,OUTPUT);
}

void loop()
{
  digitalWrite(led,1);
  Serial.println(i);
  i++;
  delay(500);
  digitalWrite(led,0);
  delay(500);
}
