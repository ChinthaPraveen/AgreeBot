#include <SoftwareSerial.h>
#include <Servo.h>
#include <stdlib.h> 
#define  rx 2
#define  tx 3
SoftwareSerial blue(rx,tx);

char sender[20];  // To hold the sender's phone number
char text[160];   // To hold the message text
#define  GSM_rx 4
#define  GSM_tx 7
SoftwareSerial gsm(GSM_rx,GSM_tx);

int generateRandomInRange(int min, int max);
void configureGSMForSend();
void configureGSMForReceive();
String extractGSMMessage(String cmd);

Servo soil;
Servo seed; 
Servo pump;
int pos = 0; 
#define Delay 400 

#define IN_1  8        
#define IN_2  9       
#define IN_3  11        
#define IN_4  10
#define cutter1 12
#define cutter2 13

#define pump1 5
#define pump2 6

String cmd;
String gsm_cmd;
int moistureValue;
int randomNumbers;

void setup() 
{
 //gsmblue.begin(9600);
 gsm.begin(9600); 
 blue.begin(9600);
 Serial.begin(9600); 
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
 pinMode(pump1, OUTPUT);
 pinMode(pump2, OUTPUT);
 pinMode(cutter1, OUTPUT);
 pinMode(cutter2, OUTPUT);
  soil.attach(A0); //purple
  soil.write(30);
  pump.attach(A1); // Blue
  pump.write(90);
  seed.attach(A2);
  seed.write(2); 
  gsm.listen();
  gsm.println("AT");
delay(1000);
if (gsm.available()) {
    String response = gsm.readString();
    Serial.println("AT Response: " + response);
}

  // gsm.println("AT");
  //   delay(1000);
  //   if (gsm.available()) {
  //       String response = gsm.readString();
  //       Serial.println("GSM Response: " + response);
  //   } else {
  //       Serial.println("GSM module not responding.");
  //   }

// gsm.listen();
// gsm.println("AT");  
//     delay(1000);  // Wait for the GSM module to respond

//     // Check if there's any response from the GSM module
//     if (gsm.available()) {
//         String response = gsm.readString();
//         Serial.println("GSM Response: " + response);  // Print the response
//     } else {
//         Serial.println("No response from GSM module.");
//     }

//     delay(2000);  

  configureGSMForReceive();
}

void seederOn()
{
  while(1)
  {
  for(int i=0; i<=135; i=i+2)
  {
    seed.write(i);
    delay(25);
    if(blue.available()>0)
    {
      return;
    }

}
  for(int i =135; i>=0; i=i-2)
  {
        if(i==0 | i==1)
        {
          seed.write(i);
          continue;
        }
    seed.write(i);
    delay(30);
       if(blue.available()>0)
       {
        return;
       }
  }
  }
}

void seederOff()
{
  seed.write(2);
  delay(60);
}

void soilOff()
{
  for(int i=30; i>=0; i--)
  {
   soil.write(i);
   delay(20);
  }
}

void soilOn()
{
  soil.write(30);
  delay(10);
}

void cutterOn()
{
    digitalWrite(cutter1, HIGH);
    digitalWrite(cutter2,LOW);
    delay(50);
}

void cutterOff()
{
    digitalWrite(cutter1, LOW);
    digitalWrite(cutter2,LOW);
    delay(50);
}

void sprayOn()
{
    digitalWrite(pump1, HIGH);
    digitalWrite(pump2,LOW);
    delay(100);
    while(1)
    {
     for(pos = 40; pos <= 140; pos += 1) 
     { 
       pump.write(pos); 
       delay(15);
       if (blue.available() > 0)
       {
        pump.write(90);
            return;
       }
     }
     for (pos = 140; pos >= 40; pos -= 1) 
     { 
       pump.write(pos); 
       delay(15);
       if (blue.available() > 0)
       {
         pump.write(90);
            return;
       }
     }
    }
}

void sprayOff()
{
    digitalWrite(pump1, LOW);
    digitalWrite(pump2,LOW);
}

void backward()
{ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);     
}

void forward()
{
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
}

void left()
{     
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
}

void right()
{
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
}

void stop()
{  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
}

void loop() 
     {
        //  gsm.listen();
        //  if (gsm.available() > 0)
        // {
        //  String message = gsm.readString();  // Read the entire incoming data

        // } 
        blue.listen();
        if (blue.available() > 0) 
        {
        cmd = blue.readString(); 
        cmd.trim();
        Serial.print(cmd);
        // Handle the commands
          if (cmd == "forward") 
          {
              forward();
          } 
          else if (cmd == "backward") 
          {
              backward();
          } 
          else if (cmd == "right") 
          {
              right();
          } 
          else if (cmd == "left") 
          {
              left();
          } 
          else if (cmd == "stop") 
          {
              stop();
          } 
          else if (cmd == "soilOn") 
          {
              soilOn();
          } 
          else if (cmd == "soilOff") 
          {
              soilOff();
          } 
          else if (cmd == "cutterOn")
          {
              cutterOn();
          } 
          else if (cmd == "cutterOff") 
          {
              cutterOff();
          } 
          else if (cmd == "seederOn") 
          {
              seederOn();
          }
          else if (cmd == "seederOff")
          {
              seederOff();
          } 
          else if (cmd == "sprayOn") {
              sprayOn();
          } 
          else if (cmd == "sprayOff")
          {
              sprayOff();
          } 
          else
          {
              stop();
          }
        cmd = "";
        delay(Delay); 
        }
     }

int generateRandomInRange(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// Function to configure GSM to receive SMS
void configureGSMForReceive() {
    gsm.println("AT+CNMI=1,2,0,0,0");  
    delay(300);  
}

void configureGSMForSend() {
    gsm.println("AT+CMGF=1");  
    delay(300); 
}

void sendSoilMoistureData(int soilMoisture) {
    // Set SMS mode to text
   // configureGSMForSend();

    // Set recipient's phone number
    gsm.println("AT+CMGS=\"+919912683087\"");  // Replace with the actual phone number
    delay(300);

    // Compose and send the SMS message
    gsm.print("Soil Moisture Level: ");
    gsm.print(soilMoisture);  // The soil moisture data
    delay(100);

    // End the SMS message with Ctrl+Z (ASCII code 26)
    gsm.write(26);
    delay(1000);  // Wait for the message to send

    String response = "";
    while (gsm.available()) {
        response += gsm.readString();
    }
    return response;
}
String extractGSMMessage(String cmd) {
    // Check if the response contains "+CMT:", which indicates an SMS message
    if (cmd.indexOf("+CMT:") >= 0) {
        // Find the index of the first newline after "+CMT:", where the actual message starts
        int startIndex = cmd.indexOf("\n", cmd.indexOf("+CMT:")) + 1;
        // Extract the message part
        if (startIndex > 0) {
            String message = cmd.substring(startIndex);  // Extract message from that point onward
            message.trim();  // Remove any extra spaces or newlines

            // Clean the message by removing unprintable or non-ASCII characters
            String cleanMessage = "";
            for (int i = 0; i < message.length(); i++) {
                char c = message.charAt(i);
                // Only include printable ASCII characters (from space (32) to tilde (126))
                if (c >= 32 && c <= 126) {
                    cleanMessage += c;
                }
            }
            return cleanMessage;  // Return the cleaned message content
        }
    }
    return "";  // Return an empty string if no valid message found
}

// String extractGSMMessage(String cmd)
// {
//     // Check if the response contains "+CMT:", which indicates an SMS message
//     if (cmd.indexOf("+CMT:") >= 0)
//     {
//         // Find the index of the first newline after "+CMT:", where the actual message starts
//         int startIndex = cmd.indexOf("\n", cmd.indexOf("+CMT:")) + 1;
//         // Extract the message part
//         if (startIndex > 0)
//         {
//             String message = cmd.substring(startIndex);  // Extract message from that point onward
//             message.trim();  // Remove any extra spaces or newlines
//             // Clean the message by removing unprintable or non-ASCII characters
//             String cleanMessage = "";
//             for (int i = 0; i < message.length(); i++)
//             {
//                 char c = message.charAt(i);
//                 // Only include printable ASCII characters (from space (32) to tilde (126))
//                 if (c >= 32 && c <= 126)
//                 {
//                     cleanMessage += c;
//                 }
//             }
//             return cleanMessage;  // Return the cleaned message content
//         }
//     }
//     return "";  // Return an empty string if no valid message found
// }