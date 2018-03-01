#include <Keypad.h>

#define Password_Length 5 //extra character 

char Data[Password_Length];
char Master[Password_Length] = "2017"; //change password here
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
char inchar; //Will hold the incoming character from the Serial Port.

const byte ROWS = 4; //4 rows
const byte COLS = 4; //4 columns
//label each key
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connect keypad row wires to defined pins below.
byte rowPins[ROWS] = { 2, 3, 4, 5 };
// Connect keypad column wires to defined pins below.
byte colPins[COLS] = { 6, 7, 8, 9 }; 

// Create the Keypad
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


int gsm = 9
int led1 = 13; //indicator light
int led2 = A0; //indicator light --> analog pin utilized as a DO
int vehicle = 11; //relay for the vehicle
int light = 12; //relay for the outlet that the light is connected to 
void setup()
{
  // prepare the digital output pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  Serial.begin(19200);
  pinMode(vehicle, OUTPUT);
  pinMode(light, OUTPUT);
  digitalWrite(vehicle, HIGH);
  digitalWrite(light, HIGH);
  Serial.print("pin state low\n");

//gsm shield initialization sequence
  pinMode(gsm, OUTPUT); 
  digitalWrite(gsm,LOW);
  delay(1000);
  digitalWrite(gsm,HIGH);
  delay(2000);
  digitalWrite(gsm,LOW);

  delay(3000); // give time for GSM module to register on network.
  Serial.println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  Serial.println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt 
  delay(200);
}
void loop() 
{
  customKey= customKeypad.getKey();
  if (customKey)
  {
    Data[data_count] = customKey;
    Serial.println(Data[data_count]);
    data_count++;
  }
  if(data_count == Password_Length-1)
  {
    Serial.println("Password is ");

    if(!strcmp(Data, Master))
    {
      Serial.println("Good");
      digitalWrite(vehicle, LOW);
    }
    else
    {
      Serial.println("Bad");
      digitalWrite(vehicle, HIGH);
    }
    delay(1000);
    clearData();
  }


  //If #a1b1 comes as sms, all led's should light up.
  if(Serial.available() >0)
  {
    inchar=Serial.read(); 
    if (inchar=='#') //serial read of first character
    {
      delay(10);
      inchar=Serial.read(); 
      //first led
      if (inchar=='a')//closed
      {
        delay(10);
        inchar=Serial.read();
        if (inchar=='0')//closed
        {
          digitalWrite(led1, LOW);
          delay(500);
          digitalWrite(vehicle,HIGH);
          Serial.print("Relay 1 LOW\n");
          // delay(1000);
          //  Serial.println("AT+CMGD=1,4");                   //Delete all message
          delay(500);
          Serial.println("AT+CMGF=1");
          delay(500);
          Serial.println("AT+CMGS=\"13108743782\"");//receiver phone number
          delay(500);
          Serial.print("Vehicle DISABLED");    //the message you want to send
          delay(500);
          Serial.write(26);
          delay(500);
        }
        else if (inchar=='1')
        {
          digitalWrite(led1, HIGH);
          delay(500);
          digitalWrite(vehicle, LOW);
          Serial.print("Relay 1 HIGH\n");
          // delay(1000);
          //    Serial.println("AT+CMGD=1,4");                   //Delete all message
          delay(500);
          Serial.println("AT+CMGF=1");
          delay(500);
          Serial.println("AT+CMGS=\"13108743782\"");//receiver phone number
          delay(500);
          Serial.print("Vehicle ENABLED");    //the message you want to send
          delay(500);
          Serial.write(26);
          delay(500);
        }

        delay(10);
        inchar=Serial.read();
        if(inchar=='b')
        {
          delay(10);
          inchar=Serial.read();
          if (inchar=='0')
          {
            digitalWrite(led2, LOW);
            delay(500);
            digitalWrite(light, HIGH);
            Serial.print("Relay 2 LOW\n");
            //Serial.println("AT+CMGD=1,4");                   //Delete all message
            delay(500);
            Serial.println("AT+CMGF=1");
            delay(500);
            Serial.println("AT+CMGS=\"13108743782\"");// receiver phone number
            delay(500);
            Serial.print("Light OFF");    //the message you want to send
            delay(500);
            Serial.write(26);
            delay(500);
            Serial.print("Light OFF");
          }
          else if (inchar=='1')
          {
            digitalWrite(led2, HIGH);
            delay(500);
            digitalWrite(light, LOW);
            Serial.print("Relay 2 HIGH\n");
            // Serial.println("AT+CMGD=1,4");                   //Delete all message
            delay(500);
            Serial.println("AT+CMGF=1");
            delay(500);
            Serial.println("AT+CMGS=\"13108743782\"");// receiver phone number
            delay(500);
            Serial.print("Light ON");//the message you want to send
            delay(500);
            Serial.write(26);
            delay(500);
            Serial.print("Light ON");
          }
        }
      } 
      else if (inchar=='b')
      {
        delay(10);
        inchar=Serial.read();
        if (inchar=='0')//closed
        {
          digitalWrite(led2, LOW);
          delay(500);
          digitalWrite(light,HIGH);
          Serial.print("Relay 1 LOW\n");
          // delay(1000);
          //  Serial.println("AT+CMGD=1,4");                   //Delete all message
          delay(500);
          Serial.println("AT+CMGF=1");
          delay(500);
          Serial.println("AT+CMGS=\"13108743782\"");// receiver phone number
          delay(500);
          Serial.print("Light OFF");    //the message you want to send
          delay(500);
          Serial.write(26);
          delay(500);
        }
        else if (inchar=='1')
        {
          digitalWrite(led2, HIGH);
          delay(500);
          digitalWrite(light, LOW);
          Serial.print("Relay 2 HIGH\n");
          // delay(1000);
          //    Serial.println("AT+CMGD=1,4");                   //Delete all message
          delay(500);
          Serial.println("AT+CMGF=1");
          delay(500);
          Serial.println("AT+CMGS=\"13108743782\"");//receiver phone number
          delay(500);
          Serial.print("Light ON");    //the message you want to send
          delay(500);
          Serial.write(26);
          delay(500);
        }

      }


      Serial.println("AT+CMGD=1,4");
      
    }

  }
}

void clearData()
{
  while(data_count !=0)
  {
    Data[data_count--] = 0;

  }
  return;
}



