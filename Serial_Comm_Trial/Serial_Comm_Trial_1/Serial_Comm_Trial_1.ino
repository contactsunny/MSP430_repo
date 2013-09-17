//#include<SoftwareSerial.h>

//SoftwareSerial port(3,4);

int led=2;
//int in;

void setup()
{
  Serial.begin(9600);
  pinMode(led,OUTPUT);
}

void loop()
{
//  if(Serial.available()>0)
//  {
//    char in=Serial.read();
//    digitalWrite(led,HIGH);
//    Serial.print("Incoming: ");
//    Serial.println(in);
//    digitalWrite(led,LOW);
//  }
  Serial.println("Hi");
  delay(300);
}
