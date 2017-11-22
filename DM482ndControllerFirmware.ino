#include <OSCMessage.h>
#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial1);
#endif

int _portAx[] = {0,1};  // analog ports to be read
int _vals[] = {0,0};    // for each port the last value will be stored in this array
int _valsLength = 2;    // constant representing length of the _vals array
int _index = 0;         // next port number to be read
String _messages[] = {"/DM48/slider", "/DM48/pressure"}; // OSC message related to each port
int _touch = 0;

void setup() 
{                
   //begin SLIPSerial just like Serial
  SLIPSerial.begin(460800);   // set this as high as you can reliably run on your platform
}

void loop()                     
{
  // READ SLIDER & PRESSURE
  int val = analogRead(_portAx[_index]);
 
  if (_vals[_index] != val) {
    _vals[_index] = val;
    
    OSCMessage msg(_messages[_index].c_str());
    msg.add(val);
      
    SLIPSerial.beginPacket();
    msg.send(SLIPSerial);
    SLIPSerial.endPacket();
    msg.empty();
  }
  
  _index = (_index + 1) % _valsLength;

  // READ TOUCH
  int a = touchRead(A9);
  int b = touchRead(A8);
  int touch = (a-b)*1000/(a+b);
  if (_touch != touch) {
    _touch = touch;
    OSCMessage msgTouch("/DM48/touch");
    msgTouch.add(_touch);
      
    SLIPSerial.beginPacket();
    msgTouch.send(SLIPSerial);
    SLIPSerial.endPacket();
    msgTouch.empty();
  }
  
  delay(10);
}


