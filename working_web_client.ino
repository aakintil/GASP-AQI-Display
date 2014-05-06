// (Based on Ethernet's WebClient Example)

// Necessary Libraries
#include <SPI.h>
#include <WiFly.h>
#include <TextFinder.h>
#include "Credentials.h"

// Variables
String myStr;
String readString, readString1; 
int x=0;
//char lf=10;
boolean reading = false;
String result; 
long array [] = {
};

// LEDS
int red_led = 5; 
int orange_led = 4; 
int yellow_led = 3; 
int green_led = 2;

int led_array[] = { 
  green_led, yellow_led, orange_led, red_led }; 
int array_length = 4;


// BUTTON VARIABLES
const int button = 8; 
int buttonstate;         // variable for reading the pushbutton status

int prev = 1;

WiFlyClient client("airnow.gov", 80);
TextFinder finder( client );

void setup() {
  //  initialize leds
  for ( int i = 0; i < array_length; i++ ) {
    pinMode( led_array[ i ], OUTPUT );
  } 
  // initialize button
  pinMode( button, INPUT);
  digitalWrite( button, HIGH);
  buttonstate = LOW; 

  // initialize the pushbutton pin as an input:
  pinMode( button, INPUT ); 


  Serial.begin(115200);
  Serial.println("WebClient example at 38400 baud.");

  WiFly.begin();

  if (!WiFly.join("CMU", "xxx")) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }  

//  WiFly.configure(WIFLY_BAUD, 38400);

  Serial.println("connecting...");

  if ( client.connect() ) {
    Serial.println("connected");
    client.println("GET /?action=airnow.local_city&zipcode=15232 HTTP/1.0"); 

    //  TEST AND SEE IF ANY OF THESE ARE NECESSARY
    client.println("Accept:text/html,application/xhtml+xml,application/xml");
    //   ;q=0.9,image/webp,*/*;q=0.8"); 
    client.println("Accept-Encoding:gzip,deflate,sdch"); 
    client.println("Accept-Language:en-US,en;q=0.8,fr;q=0.6"); 
    client.println("Cache-Control:max-age=0"); 
    client.println("Connection:keep-alive"); 
    //   test and see if site will download withtout this
    client.println("Cookie:JSESSIONID=8630230f6d714d2c9b2a7d5c2f55f35754b2; CFID=13570; CFTOKEN=74328870; CFGLOBALS=urltoken%3DCFID%23%3D13570%26CFTOKEN%23%3D74328870%26jsessionid%23%3D8630230f6d714d2c9b2a7d5c2f55f35754b2%23lastvisit%3D%7Bts%20%272014%2D05%2D02%2016%3A20%3A44%27%7D%23timecreated%3D%7Bts%20%272014%2D04%2D28%2017%3A23%3A48%27%7D%23hitcount%3D22%23cftoken%3D74328870%23cfid%3D13570%23");
    // change if you change the server name
    client.println("Host:airnow.gov"); 
    client.println("User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.131 Safari/537.36"); 
    client.println(); 
  } 
  else {
    Serial.println("connection failed");
  }

}

int count = 0;
boolean currentLineIsBlank = false;

int c = 0; 
void loop() {
  // read the state of the pushbutton value:
  buttonstate = digitalRead( button );
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:

  if (client.available()) {
    int index = 0; 
    char c = client.read();
    //    Serial.print(c);  
    //    Serial.println("parsing the html data: "); 

    if (client.connected()) {
      Serial.println("connected..."); 
      finder.find("AQDataSectionTitle"); 
      finder.find("TblInvisible"); 

      //      finder.findUntil("background=", "HealthMessage"); 
      finder.findUntil("center", "AQILegendText"); 
      long value = finder.getValue(); 
      light_appropriate_led( 120 );
      //      Serial.println("============ FOUND ============"); 
      Serial.println( value );  
      client.stop();
      //      client.println("Connection: close");
      //      client.println();
      //      if (value)   digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
  }




  if (!client.connected()) {
    client.stop();
    //    Serial.println("Current data row:" );
    //    Serial.print(readString);
    //    Serial.println();
    //    readString1 = (readString.substring(41,43));
    //    Serial.println();
    //    Serial.print("DPD sec: ");
    //    Serial.println(readString1);
    //    Serial.println("done");
    for(;;);
  }



  if ( buttonstate != prev ) {
    light_show();   
  }
  prev = buttonstate;
}

// take in a parameter: the index of the array to light
void light_show() {
  Serial.println( c ); 
  while ( c < 5 ) {
    for ( int i = 0; i < array_length; i++) {
      digitalWrite( led_array[i], HIGH);
      delay( 300 ) ; 
      digitalWrite( led_array[i], LOW);
    }
    c++; 
  }

  digitalWrite(green_led, HIGH);
  c = 0; 
  delay(10000);
  digitalWrite(green_led, LOW);
}


// make this return the index array for the correct light
void light_appropriate_led( long value ) {
  int val = int( value );
  Serial.println("=============="); 
  Serial.println("method called. the AQI is: " + val ); 

  delay(1000);

  if ( val ) {

    if ( val >= 0 && val <= 50 ) {
      digitalWrite( green_led, HIGH );   // turn the LED on (HIGH is the voltage level)
      //      turn on green light for x amount
    }
    else if ( val >= 51 && val <= 100 ) {
      digitalWrite( yellow_led, HIGH );   // turn the LED on (HIGH is the voltage level)
      //    turn on the yellow light for x amount of time
    }
    else if ( val >= 101 && val <= 150 ) {
      digitalWrite( orange_led, HIGH );   // turn the LED on (HIGH is the voltage level)
      //   turn on the orange light for x amount of time 
    }
    else {
      digitalWrite( red_led, HIGH );   // turn the LED on (HIGH is the voltage level)
      // turn on the red light for x amount of time
    }

  }
  else {
    //    couldn't get a value, so just turn on the green light for x amount
  }
}


