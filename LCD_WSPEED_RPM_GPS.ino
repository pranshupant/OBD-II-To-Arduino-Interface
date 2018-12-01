//#include <SD.h>
//#include <SPI.h>
#include <SoftwareSerial.h>
#define RxD 8  //Arduino pin connected to Tx of HC-05
#define TxD 9 //Arduino pin connected to Rx of HC-05

const double SPF = 5.15; // 3.6 * 0.228 * 2 * PI
unsigned long ti = 0;
long wspeed = 0;
long wheelspeed(unsigned long ti);

int gps_Pic;
int gps_Val;

char SendCommand0[10]= "atz";
char SendCommand1[10]= "atsp0";
char SendCommand2[10] = "010c";
//char SendCommand3[10] = "0111";

char inByte;
int t=0;
long erpm=0;
long throttle=0;
long RPM();
//long wheelspeed();

//File myFile;
//int pinCS = 10; // Pin 10 on Arduino Uno


SoftwareSerial blueToothSerial(RxD,TxD);



void setup() {

  Serial.begin(115200);
  delay(500);
  Serial.print("baud=38400");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.end();

  Serial.begin(38400);
  
  blueToothSerial.begin(38400);//setup via commands
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  digitalWrite(2, HIGH);

//  pinMode(pinCS, OUTPUT);
//  pinMode(1, INPUT);

  //SD.begin();

   blueToothSerial.write(SendCommand0);
   blueToothSerial.write('\r');
   delay(500);
   blueToothSerial.write(SendCommand1);
   blueToothSerial.write('\r');
   delay(500);
   blueToothSerial.write(SendCommand2);
   blueToothSerial.write('\r');
   delay(1500);

 
 // pinMode(rpm_Pin, INPUT);
// nexInit();
  
}

void loop() {

        erpm=RPM();
        Serial.print("RPM.val=");
        Serial.print(erpm);
        Serial.write(0xff);
        Serial.write(0xff);
        Serial.write(0xff);

        delay(250);  

//****************HALL EFFECT**************************************//


    ti = millis();
    wspeed = wheelspeed(ti);
    Serial.print("SPEED.val=");
    Serial.print(wspeed);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);




//**************************************GPS**********************************************//

 if (analogRead(A2)>130 && analogRead(A2)<260)
  {
    gps_Pic = 19;
    gps_Val = 0;//N
 
  }

  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
 
   if ( analogRead(A2)>0 && analogRead(A2)<127)
  {
    gps_Pic = 13; //1
    gps_Val = 1;
  }
  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
    if ( analogRead(A2)>272 && analogRead(A2)<400)
  {
    gps_Pic = 14;//2
    gps_Val = 2;
  }

  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
    if ( analogRead(A2)>426 && analogRead(A2)<550)
  {
    gps_Pic = 15;//3
    gps_Val = 3;
  }

  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
    if ( analogRead(A2)>577 && analogRead(A2)<700)
  {
    gps_Pic = 16;//4
    gps_Val = 4;
  
  }
  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
    if ( analogRead(A2)>730 && analogRead(A2)<860)
  {
    gps_Pic = 17;  //5
    gps_Val = 5;
  }
  
  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
   if ( analogRead(A2)>885 && analogRead(A2)<1023)
  {
    gps_Pic = 18;    //6 
    gps_Val = 6;
  }

  Serial.print("pic 0,0,");
  Serial.print(gps_Pic);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff); 

  delay(250);


//       myFile = SD.open("test.txt",FILE_WRITE);
//        
//        if (myFile) {
//          myFile.print(erpm);
//          myFile.print(",");
//          myFile.print(gps_Val);
//          myFile.println();
//          myFile.close();
//        }
  
}

    

long RPM()
{
    char *ptr;
    long rpm;
    char inData[16];
    char outData[4];

    int i=0;
    blueToothSerial.write(SendCommand2);
    blueToothSerial.write('\r');
    delay(250);

    while(blueToothSerial.available()>0) {
      inByte = blueToothSerial.read();

     // Serial.println(inByte);
      //Serial.print("kjaat");
      inData[i]=inByte;
      i++;
    }
    for (int i = 0; i < 9; i++) {
        if (inData[i]== '4'&& inData[++i]=='1') {
            --i;
            outData[0]=inData[i+6];
           // Serial.println(outData[0]);
            outData[1]=inData[i+7];
            //Serial.println(outData[1]);
            outData[2]=inData[i+9];
           // Serial.println(outData[2]);
            outData[3]=inData[i+10];
            //Serial.println(outData[3]); 
        }/* code */
    }
   /* outData[0]=inData[4];
    outData[1]=inData[5];
    outData[2]=inData[6];
    outData[3]=inData[7];*/

    /*Serial.println(outData[0]);
    Serial.println(outData[1]);
    Serial.println(outData[2]);
    Serial.println(outData[3]);*/

    rpm = strtol(outData, &ptr, 16);
    rpm/=4;
    return rpm;
}


long wheelspeed(unsigned long ti)
{
    unsigned long totaltime = 0;
    unsigned long t1 = 0;
    unsigned long t2 = 0;
    unsigned long tf = 0;

    long lspeed = 0;
    double deltime = 0;
    double dspeed = 0;
    int count = 1;
    while(totaltime < 500)
    {
        tf = millis();
        totaltime = tf - ti;

        t1 = t2;

        if (digitalRead(2) == LOW && count != 0)
        {
            t2 = millis();
            deltime = t2 - t1;
            deltime /= 1000.00;
            if (deltime > 0 && deltime < 6000)
            {
                dspeed = SPF / deltime;
            }
            count = 0;
        }
        else if (digitalRead(2) == HIGH)
        {
            count = 1;
        }
    }
    lspeed = dspeed;
    return lspeed;
}
