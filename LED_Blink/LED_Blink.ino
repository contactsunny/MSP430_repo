int led=2;
int led2=14;
int button=5;

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led,LOW);
  
  pinMode(led2, OUTPUT);
  digitalWrite(led2,HIGH);  
  
  pinMode(button,INPUT_PULLUP);
}

void loop()
{
  int buttonValue=digitalRead(button);
  if(!buttonValue)
  {
    digitalWrite(led, HIGH);
    digitalWrite(led2, LOW);
  }
  else
  {
    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
  }
}
