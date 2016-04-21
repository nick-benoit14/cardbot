
/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

class CardbotServo{
  public:
    CardbotServo(int pin, bool orientation){
      p_servo = new Servo;
      m_orient = orientation;
//TODO switched directions for forward
// Sort out forward motion and turning 
      m_forward = (m_orient == true) ? 0 : 180;
      m_reverse = (m_orient == true) ? 180 : 0;
      m_stop = 90;
      m_currentState = m_stop;
     }

     void attach(int pin){p_servo->attach(pin);}
     
     ~CardbotServo(){delete p_servo;}
      
     void write(){p_servo->write((int)m_currentState);}
     
     void setPosition(int pos){
        if(pos < 0){m_currentState = m_reverse;}
        else if(pos > 0){m_currentState = m_forward;}
        else{m_currentState = m_stop;}
      }  

      int getForward(){return m_forward;}
      int getReverse(){return m_reverse;}
      int getStop(){return m_stop;}
         
     private:
     char m_currentState;
     Servo* p_servo;
     bool m_orient;
     int m_forward;
     int m_reverse;
     int m_stop;
  };
  

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "mmhmm";

ESP8266WebServer server(80);
CardbotServo servo_left(5, true);  // create servo object to control a servo
CardbotServo servo_right(4, false);  // create servo object to control a servo


/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
// TODO serve control page
void handleRoot() {
	server.send(200, "text/html", "<html><head><meta name='viewport' content='width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no'><style>body,html{margin:0}.input{width:100%;height:100%;background-color:#021B27}.input_block{padding-top:100px}.input_block_row{display:flex;flex-wrap:wrap;justify-content:center}.input_block_row_button{width:40%;height:20%;margin:10px;background-color:#021B27;border:3px solid #F5991D;border-radius:25px;color:#F5991D;font-size:150%;transition:background-color .5s}.input_block_row_button:hover{background-color:#F5991D;color:#021B27}</style></head><body><div class='input'><div class='input_block'><div class='input_block_row'><button id='w' class='input_block_row_button'>Forward</button></div><div class='input_block_row'><button id='a' class='input_block_row_button'>Left</button> <button id='d' class='input_block_row_button'>Right</button></div><div class='input_block_row'><button id='s' class='input_block_row_button'>Reverse</button></div></div></div></body><script>function sendCommand(n,e){var t=new XMLHttpRequest;switch(n){case TURN:console.log(address+'/command?turn='+e),t.open('GET',address+'/command?turn='+e)}t.addEventListener('load',reqListener),t.send()}function reqListener(){console.log(this.responseText)}function bindButtonEvents(n){n.forEach(function(n){document.getElementById(n).addEventListener('click',function(){sendCommand('TURN',n)})})}const address='http://192.168.4.1',TURN='TURN';bindButtonEvents(['w','s','a','d']);</script></html>");
}

void handleCommand(){

  const int F = 1;
  const int R = -1;
  const int S = 0;
  
  char dir = server.arg(0)[0];
  switch(dir){
    case 'w':
      servo_left.setPosition(F);
      servo_right.setPosition(F);
    break;

    case 's':
      servo_left.setPosition(R);
      servo_right.setPosition(R);
    break;
   
    case 'a':
      servo_left.setPosition(F);
      servo_right.setPosition(R);
    break;
      
    case 'd':
      servo_left.setPosition(R);
      servo_right.setPosition(F);
    break;

    case 'q':
     servo_left.setPosition(S);
     servo_right.setPosition(S);
    break;
  }
   server.send(200);

}




void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/command", handleCommand);
	server.begin();
	Serial.println("HTTP server started");
  
  servo_left.attach(5);              
  servo_right.attach(4);             
}

void loop() {
 servo_left.write();              
 servo_right.write(); 
 server.handleClient();
}

 

