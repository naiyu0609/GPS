/// # Description:
// # The sketch for driving the gsm mode via the Arduino board
 
// # Steps:
// #        1. connect D2M and D2
// #        2. connect D3M and D3
// #        3. connect pwrkey and GND

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
int value=0;
long del=7;
int text=0;
int textall=0;
int button = 4;
int LED=5;
int j;
long changeLat;
long changeLon;

void GetGPS (void);
void SentSMS (void);
void changeGPS (void);

char GPS[41];

char incomingByte;
byte gsmDriverPin[2] = {
  2,3};//The default digital driver pins for the GSM and GPS mode
unsigned char PhoneNum[24]={"AT+CMGS=\"+886*********\""};

void setup()
{    
  pinMode(button,INPUT);
  pinMode(button,OUTPUT);

  mySerial.begin(9600);

  digitalWrite(5,LOW);
 //Init the driver pins for GSM function
  for(int i = 0 ; i < 2; i++){
    pinMode(gsmDriverPin[i],OUTPUT);
  }
  digitalWrite(2,LOW);//Enable the GSM mode 
  digitalWrite(3,HIGH);//Disable the GPS mode
  delay(200);
  Serial.begin(9600); //set the baud rate
  delay(8000);//call ready
}
 
void loop()
{  
  char i;
  mySerial.println("AT");
  Serial.println("AT");  
  delay(1000);
  //Send message
  mySerial.println("AT+CGPSPWR=1");
  Serial.println("AT+CGPSPWR=1");
  delay(1000);
  //reset GPS in autonomy mode
  mySerial.println("AT+CGPSRST=1");
  Serial.println("AT+CGPSRST=1");
  delay(1000);
  //digitalWrite(2,HIGH);//Disable the GSM mode 
  //digitalWrite(3,LOW);//Enable the GPS mode
  while(1)
  {
    //　初始化
    if(del==7)
        {
            del=0;
            delay(3000);
            //　發送AT命令 　回傳所要的GPS
            mySerial.println("AT+CGPSINF=4");
            Serial.println("AT+CGPSINF=4");
        }
    //
     else if (del<8)
        {
          if(Serial.available()>0)
            {
            incomingByte = Serial.read();
            //Serial.write(incomingByte);
            value=1;
              /*　將數值存進陣列  */
              if(text==2 && textall<41)
              {
              GPS[textall]=incomingByte;
              textall++;
                if(textall==41 && GPS[40]=='A')
                {
                GetGPS();
                digitalWrite(LED,HIGH);
                if(digitalRead(button)==HIGH)
                  {
                    changeGPS();
                    SentSMS();
                    mySerial.println("Sent Value");
                  }
                }
                else if (textall==41)
                {
                GPS[40]=='0';
                textall=0;
                text=0;                
                mySerial.println("Not Value");
                GetGPS();
                /*　指示燈　*/
                digitalWrite(LED,HIGH);
                delay(200);
                digitalWrite(LED,LOW);
                /*----------*/
                if(digitalRead(button)==HIGH)
                  {
                    mySerial.println("Sent Not Value");
                  }
                }
              }
              /*判斷是否抓到所要的數值*/
              if(text<2)
              {
                if(incomingByte=='4')
                 {
                 text=1;
                 }
               
                 if(text==1 && incomingByte==',')
                 {
                 text=2;
                 }
              }
              

            }
        }
        //　當抓到七個逗點分段
        if (incomingByte==','  && value==1)
        {
        del++;
        value=0;
          if(del==7)
          {
          mySerial.println();
          }
        }
        
        
    }
}  

void GetGPS (void)
{
for (textall=0;textall<41;textall++)
    {
    mySerial.write(GPS[textall]);
         if(textall==40)
         {
             mySerial.println();
         }
    }
textall=0;
text=0;
}

void SentSMS(void)
{
  int i;
  /*
  Serial.println("AT");   
  delay(500);
  //Send message
  Serial.println("AT+CMGF=1");
  delay(500);
  Serial.println("AT+CMGS=\"+886987565484\"");//change the receiver phone number
  delay(500);
  Serial.println("www.waveshare.com");//the message you want to send
  delay(100);
  Serial.write(26);
  delay(5000);
  */
Serial.println("AT");   
  delay(500);
  //Send message
  Serial.println("AT+CMGF=1");
  delay(500);
  Serial.println("AT+CMGS=\"+886931179909\"");//change the receiver phone number
  delay(500);
  Serial.print("You are Here,");
  Serial.print("Latitude:");
  for(j=0;j<2;j++)
  {
  Serial.print(GPS[j]);
  }
  Serial.print(',');
  Serial.print(changeLat);
  
  Serial.print(",");
  Serial.print("Longitude:");
    for(j=14;j<17;j++)
  {
  Serial.print(GPS[j]);
  }
  Serial.print(',');
  Serial.print(changeLon);
  
  Serial.println(".");
  delay(100);
  Serial.write(26);
  delay(15000);
  for (i=0;i<10;i++)
  {
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
  }
}

void changeGPS (void)
{
changeLat=0;
changeLon=0;

changeLat=changeLat+((GPS[2]-48)*10000000);
changeLat=changeLat+((GPS[3]-48)*1000000);
changeLat=changeLat+((GPS[5]-48)*100000);
changeLat=changeLat+((GPS[6]-48)*10000);
changeLat=changeLat+((GPS[7]-48)*1000);
changeLat=changeLat+((GPS[8]-48)*100);
changeLat=changeLat+((GPS[9]-48)*10);
changeLat=changeLat+((GPS[10]-48));
changeLat=changeLat/60;

changeLon=changeLon+((GPS[17]-48)*10000000);
changeLon=changeLon+((GPS[18]-48)*1000000);
changeLon=changeLon+((GPS[20]-48)*100000);
changeLon=changeLon+((GPS[21]-48)*10000);
changeLon=changeLon+((GPS[22]-48)*1000);
changeLon=changeLon+((GPS[23]-48)*100);
changeLon=changeLon+((GPS[24]-48)*10);
changeLon=changeLon+((GPS[25]-48));
changeLon=changeLon/60;
}
