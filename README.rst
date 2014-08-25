Ubidots-Arduino-WiFly
=====================

Ubidots-Arduino-WiFly is an example with the WiFly shield RN171 to post a value on Ubidots witha a basic function "save_value"

You will need:
--------------
* Arduino uno
* WiFly shield RN171
* WiFly library



A Quick example
----------------
Here's a quick example of how to post a value on Ubidots, with the serial terminal of Arduino:


.. code-block:: cpp

       /*
       Basic sketch for WiFly shield RN171
 
       This is a basic example to post a value on Ubidots with a simple
       Function "save_value".
       
       You need:
       * Arduino 1
       * WiFly Shield RN171
       * WiFly library
       
       Pins' connection
       Arduino       WiFly
        2    <---->    TX
        3    <---->    RX
       
       created 20 Aug. 2014
       by Mateo Velez - Metavix
       
       This example code is in the public domain.
       
       */
      //-------------------------------------------------------------
      //------------------------------Librarys-----------------------
      //-------------------------------------------------------------
      #include <Arduino.h>
      #include <SoftwareSerial.h>
      #include <WiFly.h>
      //-------------------------------------------------------------
      //---------------------Network Configuration-------------------
      //-------------------------------------------------------------
      /* Change the AUTH according to your network settings
         If is open change to WIFLY_AUTH_OPEN
         If is WPA1 change to WIFLY_AUTH_WPA1
         If is WPA1_2 change to WIFLY_AUTH_WPA1
         If is WPA2 change to WIFLY_AUTH_WPA1
      */
      #define SSID      "Atom$House$Medellin"
      #define KEY       "atommed2014"
      #define AUTH      WIFLY_AUTH_WPA2_PSK

      SoftwareSerial uart(2, 3);
      WiFly wifly(uart);
      unsigned long start_millis = 0;

      void setup() {
        Serial.begin(9600);
        Serial.println("------- WIFLY HTTP --------");
        
        uart.begin(9600);
        if (!wifly.isAssociated(SSID)) {
        while (!wifly.join(SSID, KEY, AUTH)) 
        {    
            Serial.println("Failed to join " SSID);
            Serial.println("Wait 0.1 second and try again...");
            delay(100);
         }
          wifly.save();    
        }
      }



      void loop() {
        int reset = 0;
        
        int valor = analogRead(A0);
        while ((save_value(valor)) < 0) 
        {
          reset++;
          if(reset == 10)
          {
            asm volatile ("  jmp 0");  
          }
        }

      //   Uncomment for debug

      //  char r;
      //  while (wifly.receive((uint8_t *)&r, 1, 1000) == 1) 
      //  {    
      //    Serial.print(r);
      //  }
      }
      int save_value(int dato)
      {
        char post_data_buf[32];
        snprintf(post_data_buf, sizeof(post_data_buf), "{\"value\": %d}\r\n", dato);

        char host[20];
        uint16_t port;
        
        
        if (!wifly.connect("things.ubidots.com", 80)) {
          Serial.println("Failed to connect.\r\n");
          return -2;
        }
        
        // Send request
        char buf[200];
        //dont forget change your id and your token
        snprintf(buf, sizeof(buf), "POST /api/v1.6/variables/53beeca07625420bde83ce29/values HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: %d\r\nX-Auth-Token: CCN8FrVulRYGulPTkbaiR9Myx8qN2o\r\nHost: things.ubidots.com\r\n\r\n",strlen(post_data_buf));
        wifly.send(buf);
        
        // Send body
        if (post_data_buf != NULL) {
          wifly.send(post_data_buf);
        }
         wifly.send("\r\n");
        return 0;
      }


Function

save_value()
````````````````````
.. code-block:: cpp

    boolean = ubiclient.save_value(valor)
=======  ============  ===================================
Type     Argument      Description
=======  ============  ===================================
int      valor         The value of your variable
=======  ============  ====================================

Save a value to Ubidots. Returns true upon success. Returns false upon error.
 
