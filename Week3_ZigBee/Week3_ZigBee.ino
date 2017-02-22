#include <XBee.h>

int val;

// create the XBee object
XBee xbee = XBee();

uint8_t payload[] = { 0, 0, 0, 0 };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40EAE3FA);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int statusLed = 13;
int errorLed = 13;

void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

void readValue()
{
  val = analogRead(1);
  float mv = ( val/1024.0)*5000; 
  float cel = mv/10;

  size_t i;
  size_t m;

  for(i = 0; i<4; i++)
  {
    payload[i] = 0;
  }

  int temp = cel*100;

  Serial.println();
  Serial.println("---");
  Serial.println(temp);

  for(i = 0; i<4; i++)
  {
    //payload[i] = (uint8_t)cel + i;
    
    payload[3 - i] = (uint8_t)(temp % 10);
    temp = temp / 10;
  }
  Serial.println("---");

  /*
  payload[0] = 1;
  payload[1] = 2;
  payload[2] = 3;
  payload[3] = 4;
  */
}

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {   
  // break down 10-bit reading into two bytes and place in payload
  readValue();

  xbee.send(zbTx);

  // flash TX indicator
  flashLed(statusLed, 3, 100);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
    // got a response!

    // should be a znet tx status              
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
        flashLed(statusLed, 4, 100);
      } else {
        // the remote XBee did not receive our packet. is it powered on?
        flashLed(errorLed, 5, 200);
      }
    }
  } else if (xbee.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");  
    //nss.println(xbee.getResponse().getErrorCode());
  } else {
    // local XBee did not provide a timely TX Status Response -- should not happen
    flashLed(errorLed, 6, 200);
  }

  delay(59000);
}

