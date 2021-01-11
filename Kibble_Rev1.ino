
/*
 Advanced Chat Server

 A simple server that distributes any incoming messages to all
 connected clients but the client the message comes from.
 To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 redesigned to make use of operator== 25 Nov 2013
 by Norbert Truchsess

 */

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
//#define MACADDRESS 0x00,0x30,0xD3,0x0E,0xDF,0xF8
//#define MYIPADDR 131,203,15,139
//#define MYIPMASK 255,255,255,0
//#define MYDNS 192,168,1,1
//#define MYGW 192,168,1,1
// telnet defaults to port 23
//#define LISTENPORT 23

//#define ARDUINO
//#define ARDUINO_ARCH_SAM
//#define ACTLOGLEVEL LOG_NONE;
#include "Kibble_Rev1.h"



//#if defined(__MBED__)
//  #include <mbed.h>
//  #include "mbed/millis.h"
//  #define delay(x) wait_ms(x)
//  #define PROGMEM
//  #include "mbed/Print.h"
//#endif

//#include <UIPEthernet.h>
//#include <utility/logging.h>

  //uint8_t mac[6] = {MACADDRESS};
  //uint8_t myIP[4] = {MYIPADDR};
  //uint8_t myMASK[4] = {MYIPMASK};
  //uint8_t myDNS[4] = {MYDNS};
  //uint8_t myGW[4] = {MYGW};
  

//EthernetServer server(LISTENPORT);

//EthernetClient clients[4];
//#define ACTLOGLEVEL LOG_DEBUG

AD5790_C *chip1;
AD5790_C *chip2;

void setup() {
  SerialUSB.begin(9600);
  while(!SerialUSB){
  ;
  } 
  //delay(3000);
  //Set PA7 to high which enumerates the USB Port.  
  PIOA->PIO_PER |= (1<<7); //override peripheral on this pin
  PIOA->PIO_OER |= (1<<7); //set ddr of pin to output
  
  //Set PA77 to High
  PIOA -> PIO_SODR |= (1<<7);
  

  //Setup LED pin
  PIOB->PIO_PER |= (1<<LED_CS); //override peripheral on this pin
  PIOB->PIO_OER |= (1<<LED_CS); //set ddr of pin to output 

  //Set PA12, PA13 and PA16 to inputs, these pins are not used but must be set as inputs due to board mods
  PIOA->PIO_PER |= ((1<<12)|(1<<13)|(1<<16)); //override peripheral on this pin
  PIOA->PIO_ODR |= ((1<<12)|(1<<13)|(1<<16)); //set ddr of pin to input

  

  //Set PC15, PC15, PC16, PC17, PC18, PC19 to outputs  ... these control analog multiplexing
  PIOC->PIO_PER |= ((1<<15)|(1<<16)|(1<<17)|(1<<18)|(1<<19)); //override peripheral on this pin
  PIOC->PIO_OER |= ((1<<15)|(1<<16)|(1<<17)|(1<<18)|(1<<19)); //set ddr of pin to output 

  //Set PC14, PC15, PC16, PC17, PC18 and PC19 to low
  PIOC -> PIO_CODR |= ((1<<14)|(1<<15)|(1<<16)|(1<<18)|(1<<19));

  //Set PC17 to High
  PIOC -> PIO_SODR |= (1<<17);
  
  

  
  //Set up the ENC28J60 Clock source
  //REG_PMC_PCER1 |= 1 << 4;                   // activate clock for PWM controller
  //PIOA->PIO_PDR |= 1<<ETH_CLK;                // activate peripheral functions for pin (disables all PIO functionality)
  //REG_PIOA_ABSR |= 1<< ETH_CLK;              // choose peripheral option B
 // REG_PWM_CLK |= 8;                           // choose clock rate, 0 -> full MCLK as reference 84MHz
 // REG_PWM_CMR0 |= 0 << 9;                    // select clock and polarity for PWM channel (PWM channel 0) -> (CPOL = 0) 
 // REG_PWM_CPRD0 |= 4;                        // initialize PWM period -> T = value/84MHz (value: up to 16bit), value=10 -> 8.4MHz
  //REG_PWM_CDTY0 |= 2;                         // initialize duty cycle, REG_PWM_CPRD0 / value = duty cycle, for 10/5 = 50%
 // REG_PWM_ENA |= 1 << 0;                      // enable PWM on PWM channel (pin 107 = PWML0)

  /*
  // initialize the ethernet device
  //Ethernet.begin(mac,myIP);
  delay(10000);  
  Ethernet.begin(mac,myIP);//,myDNS,myGW,myMASK);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
 
  LogObject.begin(UARTBAUD);
  
  //delay(10000);  
  
   #if ACTLOGLEVEL>LOG_NONE

    #if defined(ARDUINO)
      LogObject.begin(UARTBAUD);
    #endif
    #if defined(__MBED__)
      Serial LogObject(SERIAL_TX,SERIAL_RX);
      LogObject.baud(UARTBAUD);
    #endif
    while (!LogObject)
      {
      ; // wait for serial port to connect. Needed for Leonardo only
      }
  #endif

  #if ACTLOGLEVEL>=LOG_INFO
    LogObject.uart_send_str(F("Chat server listen on:"));
    #if defined(ARDUINO)
      LogObject.print(Ethernet.localIP()[0]);
      LogObject.print(F("."));
      LogObject.print(Ethernet.localIP()[1]);
      LogObject.print(F("."));
      LogObject.print(Ethernet.localIP()[2]);
      LogObject.print(F("."));
      LogObject.print(Ethernet.localIP()[3]);
      LogObject.print(F(":"));
      LogObject.println(LISTENPORT);
    #endif  
    #if defined(__MBED__)
      LogObject.printf("%d.%d.%d.%d:%d\r\n",Ethernet.localIP()[0],Ethernet.localIP()[1],Ethernet.localIP()[2],Ethernet.localIP()[3],LISTENPORT);
    #endif  
  #endif   
  */
    initDACs();
}

void initDACs(){
  chip1 = new AD5790_C(0); //implements fine current control to the accurate predetermined currents 

   
  if (chip1->AD57XX_Init(AD5790)) SerialUSB.println("AD5790 Chip 1 Init OK"); 
  else SerialUSB.println("AD5790 Chip 1 Init Err"); 
   

  chip2 = new AD5790_C(1); //implements fine coarse control to the accurate predetermined currents 

  
  if (chip2->AD57XX_Init(AD5790)) SerialUSB.println("AD5790 Chip 2 Init OK"); 
  else SerialUSB.println("AD5790 Chip 2 Init Err"); 

  /* Resets the device to its power-on state. */ 
  chip1->AD57XX_SoftInstruction(AD57XX_SOFT_CTRL_RESET);
  chip2->AD57XX_SoftInstruction(AD57XX_SOFT_CTRL_RESET);
  
  

  /* The DAC register is set to use offset binary coding. */ 
  delay(500);
  chip1->AD57XX_Setup(AD57XX_CTRL_BIN2SC);
  delay(500);
  chip2->AD57XX_Setup(AD57XX_CTRL_BIN2SC);

 

  /* Sets the RBuf bit in the control register. This ties the inverting pin to the feedback pin via two parallel on chip 6k8 resistors */
  /* Read Control and then modity result */ 
  long ctrl_result = chip1->AD57XX_GetRegisterValue(AD57XX_REG_CTRL);
  chip1->AD57XX_SetRegisterValue(AD57XX_REG_CTRL, ctrl_result|AD57XX_CTRL_RBUF);

  long ctrl_result2 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_CTRL);
  chip2->AD57XX_SetRegisterValue(AD57XX_REG_CTRL, ctrl_result2|AD57XX_CTRL_RBUF);

  /* Sets the value to which the DAC output is set when CLEAR is enabled.  For a 20 bit dac mid range in 80000 */ 
 
  chip1->AD57XX_SetRegisterValue(AD57XX_REG_CLR_CODE, AD57XX_CLR_CODE_DATA(0x80000)); 
  
  chip2->AD57XX_SetRegisterValue(AD57XX_REG_CLR_CODE, AD57XX_CLR_CODE_DATA(0x80000));

  chip1->AD57XX_EnableOutput(1);
  chip2->AD57XX_EnableOutput(1);

  /* Performs a soft CLEAR operation. */ 
  chip1->AD57XX_SoftInstruction(AD57XX_SOFT_CTRL_CLR); 
  chip2->AD57XX_SoftInstruction(AD57XX_SOFT_CTRL_CLR); 

  chip2->AD57XX_SetRegisterValue(AD57XX_REG_CTRL, 0x12);

  /* Reads and displays the internal registers. */ 
  /* Read DAC. */   
  long result = chip1->AD57XX_GetRegisterValue(AD57XX_REG_DAC); 
  result = (result & 0xFFFFF); 
  SerialUSB.print("DAC1 REG: "); 
  SerialUSB.println(result, HEX); 
  long result2 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_DAC); 
  result2 = (result2 & 0xFFFFF); 
  SerialUSB.print("DAC2 REG: "); 
  SerialUSB.println(result2, HEX); 
  SerialUSB.println();
 
  /* Read Control. */ 
  result = chip1->AD57XX_GetRegisterValue(AD57XX_REG_CTRL); 
  result = (result & 0x3E);  // Only Bits 5 through 1 are holding information. 
  SerialUSB.print("DAC1 CTRL REG: "); 
  SerialUSB.println(result, HEX); 
  result2 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_CTRL); 
  result2 = (result2 & 0x3E);  // Only Bits 5 through 1 are holding information. 
  SerialUSB.print("DAC2 CTRL REG: "); 
  SerialUSB.println(result2, HEX); 
  SerialUSB.println();
 
 
  /* Read ClearCode. */ 
  result = chip1->AD57XX_GetRegisterValue(AD57XX_REG_CLR_CODE); 
  result = (result & 0xFFFFF); 
  SerialUSB.print("DAC1 CLEAR CODE: "); 
  SerialUSB.println(result, HEX); 
  result2 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_CLR_CODE); 
  result2 = (result2 & 0xFFFFF); 
  SerialUSB.print("DAC2 CLEAR CODE: "); 
  SerialUSB.println(result2, HEX); 

  SerialUSB.println("");
  SerialUSB.println("Press M for Command Menu");

 
  
  //chip1->AD57XX_EnableOutput(1);
  //chip2->AD57XX_EnableOutput(1);
  chip1->AD57XX_SetDacValue(0x80000); //zero; does + and - relative to gnd
  chip2->AD57XX_SetDacValue(0x80000); //zero; does + and - relative to gnd
  delay(2000);
  chip1->AD57XX_SetDacValue(0xFFFFF); //zero; does + and - relative to gnd
  chip2->AD57XX_SetDacValue(0xFFFFF); //zero; does + and - relative to gnd
  delay(2000);
  chip1->AD57XX_SetDacValue(0x0); //zero; does + and - relative to gnd
  chip2->AD57XX_SetDacValue(0x0); //zero; does + and - relative to gnd
 
  

  
}



void loop() {
 
  uint32_t step = 0; 

  //standard resistor with calibrated value of 99.999967 used to calibrate DAC current setting.
  //At +/- Vref max the output voltage of the current source is 1.26561V, therefore max output current is +/- 12.6561 mA
  const float max_current = 12.65611;
  const float slope = 25.31222/1048575.0;   
  long result1;
  long result2;
  long result3;
  char incomingByte;
  //chip2->AD57XX_SetDacValue(0x80000); //zero; does + and - relative to gnd
  if (SerialUSB.available() > 0) {
    // read the incoming byte:
    incomingByte = SerialUSB.read();
  }
    switch(incomingByte){
      case 'M':{
         SerialUSB.println("M  Display Menu");
         SerialUSB.println("A  Setup Vref for current control");
         SerialUSB.println("B  Enable -Vref branch to summing point");
         SerialUSB.println("C  Set discrete value for fine current control");
         SerialUSB.println("D  Disable DAC1");
         SerialUSB.println("d  Disable DAC2");
         SerialUSB.println("G  generate triangle wave continously...Press S to stop generation");
         SerialUSB.println("R  Ramp course 20 bit DAC current to specified value (from where it is now) ...not working yet");
         SerialUSB.println("r  Ramp fine 20 bit DAC current to specified value (from where it is now)");
         SerialUSB.println("I  Set DAC1 (fine current) to value");
         SerialUSB.println("i  Set DAC2 (coarse current) to value");
         SerialUSB.println("x  Read DAC Registers");
         SerialUSB.println("y  Read Control Registers");
         SerialUSB.println("z  Read Clear Registers");
         break;
      }
      case 'I':{
        
         //DAC2 control
         float j = 0;
         //SerialUSB.println(slope,8);
         SerialUSB.println("Enter a floating point value in mA (+/- 13mA max)"); 
         SerialUSB.setTimeout(10000);
         j= SerialUSB.parseFloat();
      
         //chip2->AD57XX_EnableOutput(1);
         
         if(j>max_current){
          chip2->AD57XX_SetDacValue(0xFFFFF);
          break;
         }
         if(j<-max_current){
          chip2->AD57XX_SetDacValue(0x0);
          break;
         }
         //0x0 = -13ma
         //0x80000 = 0ma
         //0x100000 = 13ma
         //resolution (mA) = slope = 13/0x80000 = 0.000022888 mA

         uint32_t dacVal = 0;
         
         dacVal = round((j+max_current)/(slope));
         SerialUSB.println(dacVal);
         chip2->AD57XX_SetDacValue(dacVal);
         
         break;
      }
      case 'i':{
         //DAC1 control
         float j = 0.0;
         //SerialUSB.println(slope,8);
         SerialUSB.println("Enter a floating point value in mA (+/- 13mA max)"); 
         SerialUSB.setTimeout(10000);
         j= SerialUSB.parseFloat();
      
         //chip1->AD57XX_EnableOutput(1);
         
         if(j>max_current){
          chip1->AD57XX_SetDacValue(0xFFFFF);
          break;
         }
         if(j<-max_current){
          chip1->AD57XX_SetDacValue(0x0);
          break;
         }
         //0x0 = -13ma
         //0x80000 = 0ma
         //0x100000 = 13ma
         //resolution (mA) = slope = 13/0x80000 = 0.000022888 mA

         uint32_t dacVal = 0.0;
         dacVal = round((j+max_current)/(slope));
         SerialUSB.println(dacVal);
         chip1->AD57XX_SetDacValue(dacVal);
         break;
      } 

      case 'x':{
         result1 = chip1->AD57XX_GetRegisterValue(AD57XX_REG_DAC); 
         result1 = (result1 & 0xFFFFF); 
         SerialUSB.print("DAC1 REG: "); 
         SerialUSB.println(result1, HEX); 
         result1 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_DAC); 
         result1 = (result1 & 0xFFFFF); 
         SerialUSB.print("DAC2 REG: "); 
         SerialUSB.println(result1, HEX); 
         SerialUSB.println();
         break;
      }
         
      case 'y':{
         //Read Control.
         result2 = chip1->AD57XX_GetRegisterValue(AD57XX_REG_CTRL); 
         result2 = (result2 & 0x3E);  // Only Bits 5 through 1 are holding information. 
         SerialUSB.print("DAC1 CTRL REG: "); 
         SerialUSB.println(result2, HEX); 
         result2 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_CTRL); 
         result2 = (result2 & 0x3E);  // Only Bits 5 through 1 are holding information. 
         SerialUSB.print("DAC2 CTRL REG: "); 
         SerialUSB.println(result2, HEX); 
         SerialUSB.println();
         break;
      }
      case 'z':{
         //Read ClearCode. 
         result3 = chip1->AD57XX_GetRegisterValue(AD57XX_REG_CLR_CODE); 
         result3 = (result3 & 0xFFFFF); 
         SerialUSB.print("DAC1 CLEAR CODE: "); 
         SerialUSB.println(result3, HEX); 
         result3 = chip2->AD57XX_GetRegisterValue(AD57XX_REG_CLR_CODE); 
         result3 = (result3 & 0xFFFFF); 
         SerialUSB.print("DAC2 CLEAR CODE: "); 
         SerialUSB.println(result3, HEX); 
         break;
      }
      case 'A':{
         //Control for IC16
        //PC18 is M1_A0
        //PC19 is M1_A1
        //--------------------------
        //M1_A1    M1_A0    Vref-       Vref+
        //  0        0     -10 V        +10 V
        //  0        1       0 V         DAC2
        //  1        0     +10 V        -10 V
        //  1        1      0V V          0 V
        SerialUSB.println("Enter '1'  to set Vref- to -10V and Vref+ to +10V ");
        SerialUSB.println("Enter '2'  to set Vref- to 0V and Vref+ to DAC2 voltage");
        SerialUSB.println("Enter '3'  to set Vref- to +10V and Vref+ to -10V");
        SerialUSB.println("Enter '4'  to set Vref- to 0V and Vref+ to 0V");
        SerialUSB.println("Enter 'S'  Return to previous menu");

        char s;
         while(1){
            s= SerialUSB.read();
            if(s=='S') break;
            switch(s){
              case '1':
                PIOC -> PIO_CODR |= (1<<18); //MSB
                PIOC -> PIO_CODR |= (1<<19); //LSB
                break;
              case '2':
                PIOC -> PIO_SODR |= (1<<18);
                PIOC -> PIO_CODR |= (1<<19);
                break;
              case '3':
                PIOC -> PIO_CODR |= (1<<18);
                PIOC -> PIO_SODR |= (1<<19);
                break;
              case '4':
                PIOC -> PIO_SODR |= (1<<18);
                PIOC -> PIO_SODR |= (1<<19);
                break;
              default:
                continue;
            }
         }
        break;
      }
      case 'B':
        
         SerialUSB.println("Enter '0'  to disable -vref branch into current summer ");
         SerialUSB.println("Enter '1'  to enable -vref branch into current summer");
         SerialUSB.println("Enter 'S'  Return to previous menu");
         //M3_A0 (from schmatic net label) is PC14
        char t;
         while(1){
            t= SerialUSB.read();
            if(t=='S') break;
            switch(t){
              case '0':
                PIOC -> PIO_CODR |= (1<<14);
                break;
              case '1':
                PIOC -> PIO_SODR |= (1<<14);
                break;
              default:
                continue;
            }
         }
         break;
         
      case 'C':{
         SerialUSB.println("Enter '1'  to set current value 1 ~= 12ma ");
         SerialUSB.println("Enter '2'  to set current value 2");
         SerialUSB.println("Enter '3'  to set current value 3");
         SerialUSB.println("Enter '4'  to set current value 4");
         SerialUSB.println("Enter '5'  to set current value 5");
         SerialUSB.println("Enter '6'  to set current value 6");
         SerialUSB.println("Enter '7'  to set current value 7");
         SerialUSB.println("Enter '8'  to set current value 8");
         SerialUSB.println("Enter 'S'  Return to previous menu");

         char s;
         while(1){
            s= SerialUSB.read();
            if(s=='S') break;
            switch(s){
              case '1':
                PIOC -> PIO_CODR |= (1<<16); //MSB
                PIOC -> PIO_CODR |= (1<<15);
                PIOC -> PIO_CODR |= (1<<17); //LSB
                break;
              case '2':
                PIOC -> PIO_CODR |= (1<<16);
                PIOC -> PIO_CODR |= (1<<15);
                PIOC -> PIO_SODR |= (1<<17);
                break;
              case '3':
                PIOC -> PIO_CODR |= (1<<16);
                PIOC -> PIO_SODR |= (1<<15);
                PIOC -> PIO_CODR |= (1<<17);
                break;
              case '4':
                PIOC -> PIO_CODR |= (1<<16);
                PIOC -> PIO_SODR |= (1<<15);
                PIOC -> PIO_SODR |= (1<<17);
                break;
              case '5':
                PIOC -> PIO_SODR |= (1<<16);
                PIOC -> PIO_CODR |= (1<<15);
                PIOC -> PIO_CODR |= (1<<17);
                break;
              case '6':
                PIOC -> PIO_SODR |= (1<<16);
                PIOC -> PIO_CODR |= (1<<15);
                PIOC -> PIO_SODR |= (1<<17);
                break;
              case '7':
                PIOC -> PIO_SODR |= (1<<16);
                PIOC -> PIO_SODR |= (1<<15);
                PIOC -> PIO_CODR |= (1<<17);
                break;
              case '8':
                PIOC -> PIO_SODR |= (1<<16);
                PIOC -> PIO_SODR |= (1<<15);
                PIOC -> PIO_SODR |= (1<<17);
                break;
              default:
                continue;
            }
         }
         break;
      }
    
      case 'R':{
      
         //String k = SerialUSB.readString();
         //SerialUSB.println("Enter a current between -12 and 12.  This is the final value in mA"); 
         //// Read the DAC to get the current value; 
         //long current_dac_value = chip1->AD57XX_GetRegisterValue(AD57XX_REG_DAC); 
         //current_dac_value = (current_dac_value & 0xFFFFF); 
         //SerialUSB.print("DAC start value: ");
         //SerialUSB.print(current_dac_value,DEC); 
         break;
      }
      case 'E':{
        
      }
      //case 'G':
      //uint32_t dacVal__ = 0;
       //  while(1){
       //     char u= SerialUSB.read();
       //     if(u=='S') break;
       //     
            // generate a triangle signal.  
       //     if (dacVal__ >= (0x00100000 - step)) { 
       //        step = -256; 
       //     } 
       //     else if (dacVal__ <= (0x00080000 - step)) { 
        //       step = 256; 
        //    } 
        //     dacVal__ += step; 
        //     chip1->AD57XX_SetDacValue(dacVal__); 
        //     delayMicroseconds(1000); 
        // }
         
       //  break;  
         
          
      case 'd':{
         chip2->AD57XX_EnableOutput(0);
         SerialUSB.println("Output Disabled");
         break; 
      }     
      case 'e':{
         chip2->AD57XX_EnableOutput(1);
         SerialUSB.println("Output Enabled");
         break; 
      }
    }
  
    // say what you got:
    //Serial.println(incomingByte, DEC);
}
  
  //SerialUSB.print(".");
  //delay(100);
  //ASSERT_LED_CS;
  //Set PC17 to High
  //PIOC -> PIO_SODR |= (1<<17);
  //PIOA -> PIO_SODR = (1<<ENC28J60_CONTROL_CS);         
    
  //delay(100);
  //RELEASE_LED_CS;
  //Set PC17 to High
  //PIOC -> PIO_CODR |= (1<<17);
  //PIOA -> PIO_CODR = (1<<ENC28J60_CONTROL_CS); 
  // wait for a new client:
  
  

  /*
  EthernetClient client = server.available();

  if (client)
    {
    //check whether this client refers to the same socket as one of the existing instances:
    bool newClient = true;
    uint8_t i=0;
    while ((i<4) && (clients[i]!=client))
      {i++;}
    if (i<4) {newClient = false;}

    if (newClient)
      {
      
      #if ACTLOGLEVEL>=LOG_INFO
        LogObject.uart_send_strln(F("New client try connect"));
      #endif
      
    //Search unused client:
    uint8_t j=0;
   while ((j<4) && (clients[j]))
     {j++;}
   if (j>=4)
   {
       
     #if ACTLOGLEVEL>=LOG_INFO
          LogObject.uart_send_strln(F("Too many client"));
        #endif
     client.stop();
   }
   else
   {
        
         #if ACTLOGLEVEL>=LOG_INFO
          LogObject.uart_send_str(F("Save client to ID:"));
          LogObject.uart_send_decln(j);
        #endif
        clients[j] = client;
        // clead out the input buffer:
        client.flush();
        
        #if ACTLOGLEVEL>=LOG_INFO
          LogObject.uart_send_strln(F("We have a new client"));
        #endif
        client.println(F("Hello, client!"));
        client.print(F("my IP: "));
        client.println(Ethernet.localIP());
        }
      }

    if (client.available() > 0)
      {
      i=0;
      while ((i<4) && (clients[i]!=client))
        {i++;}
      #if ACTLOGLEVEL>=LOG_INFO
        LogObject.uart_send_str(F("Message received from client ID:"));
        LogObject.uart_send_decln(i);
      #endif
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to all other connected clients:
      for (uint8_t j=0;j<4;j++)
        {
        if (clients[j] && clients[j]!=client)
          {
          #if ACTLOGLEVEL>=LOG_INFOi
            LogObject.uart_send_str(F("Message forwarded to client ID:"));
            LogObject.uart_send_decln(j);
          #endif
          clients[j].write(thisChar);
          }
        }
      // echo the bytes to the server as well:
      #if ACTLOGLEVEL>=LOG_INFO
        #if defined(ARDUINO)
          LogObject.write(thisChar);
        #endif  
        #if defined(__MBED__)
          LogObject.putc(thisChar);
        #endif  
      #endif
    }
  }
  for (uint8_t i=0;i<4;i++) {
    if (!(clients[i].connected())) {
      // client.stop() invalidates the internal socket-descriptor, so next use of == will allways return false;
      clients[i].stop();
    }
  }
  */
//}

//#if defined(__MBED__)
//}
//#endif
