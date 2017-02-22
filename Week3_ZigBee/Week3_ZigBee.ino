int val;
int tempPin = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000; 
  float cel = mv/10;

  Serial.print("SN TEMP ");
  Serial.println(cel, 4);
  delay(5000);
}
