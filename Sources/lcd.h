// lcd.h 
// Eduardo Corpeño
// LCD utility functions

#define lcdE        PTED_PTED7   // Enabled pin of LCD                     
#define lcdEDD      PTEDD_PTEDD7 // Data Direction of Enabled Pin          
#define lcdRS       PTED_PTED6   // RS pin of LCD (Data/Instruction Select)
#define lcdRSDD     PTEDD_PTEDD6 // Data Direction of RS Pin               
#define lcdRW       PTGD_PTGD3   // RW pin of LCD
#define lcdRWDD     PTGDD_PTGDD3 // Data Direction of RW Pin  

#define lcdPort     PTGD         // Port of 4 data bits to lcd connection  
#define lcdPortDD   PTGDD        // Data direction for 4 data pins         


#define OUTNUMBER(character)  (character<<4) // Shift the data                                            
#define PORTMASK   0x0F   // To locate the 4 output pins of the port         

// board constants

#define PushPE  PTAPE          // PushButton Pullup Enables for lines 4 through 7   
  #define Push1    PTAD_PTAD4
  #define Push2    PTAD_PTAD5
  #define Push3    PTAD_PTAD6  
  #define Push4    PTAD_PTAD7

#define DIPPE   PTBPE          // DIP Switch Pullup Enables for lines 4 through 7 
  #define DIP_SW1    PTBD_PTBD4
  #define DIP_SW2    PTBD_PTBD5
  #define DIP_SW3    PTBD_PTBD6  
  #define DIP_SW4    PTBD_PTBD7
  
#define LEDsDD    PTFDD        // LEDs Data Direction
  #define LED1    PTFD_PTFD0
  #define LED2    PTFD_PTFD1
  #define LED3    PTFD_PTFD2  
  #define LED4    PTFD_PTFD3

// ============================================================================
// msDelay
// Wait ms miliseconds
// ============================================================================
#define time 544
void msDelay(char ms){  
  int i,j;
  for(i=0;i<ms;i++)
    for(j=0;j<((time*2));j+=2)
      j=j--;
} 

// ============================================================================
// Indicate "read data" to LCD through falling edge
// ============================================================================
void LCDSend(void) {
  lcdE = 1;    
  lcdE = 0;         
}   
     
// ============================================================================     
//  Send Command to display 
// ============================================================================
void LCDCommand(volatile char dato, volatile char delay) {
      lcdRS = 0; // Command
      lcdPort &= PORTMASK;
      lcdPort |= OUTNUMBER(dato>>4);
      LCDSend();
      lcdPort &= PORTMASK;
      lcdPort |= OUTNUMBER(dato);
      LCDSend();
      msDelay(delay);
}    

// ============================================================================
// Send Character to display 
// ============================================================================
void LCDputchar(char letra) {
  if(letra=='\n'){
      LCDCommand(0xC0,1);
  } else{
      lcdRS = 1;  // Character
      lcdPort &= PORTMASK;
      lcdPort |= OUTNUMBER(letra>>4);
      LCDSend();
      lcdPort &= PORTMASK;
      lcdPort |= OUTNUMBER(letra);
      LCDSend();
      msDelay(1);
  }
}

void LCDString(char *chain){
  while (*chain != '\0') {
    LCDputchar(*(chain++));
  }
}

// ============================================================================
// LCD Clear Screen
// Clears the Screen
// ============================================================================
void LCDClearScreen(){
  LCDCommand(0x1,2);
}

// ============================================================================
// LCDinit()
// initialize the LCD
// ============================================================================
void LCDinit(){                       
  lcdPort=0;        // Lcd port out=0  
  lcdPortDD|=0xf0;  // Bits 3 through 7 of PortG outputs (LCD data nibble)
  
  lcdRW=0;          // Explicitly, RW line to 0 
  lcdRS=0;          // 
  lcdE=0;           //        
  lcdRSDD=1;        // lcd RS = output                      
  lcdEDD=1;         // lcd Enable = output 
  lcdRWDD=1;        // lcd RW = output  
  

  msDelay(110);
  LCDCommand(0x03,5); // required configuration for 2x16 Screen
  LCDCommand(0x03,2); 
  LCDCommand(0x03,2);
  LCDCommand(0x02,2);
  LCDCommand(0x28,2);

  msDelay(110);
  LCDCommand(0x03,5); 
  LCDCommand(0x03,2); 
  LCDCommand(0x03,2);
  LCDCommand(0x02,2);
  LCDCommand(0x28,2);
  msDelay(10);
  LCDCommand(0x06,2);
  LCDCommand(0x0E,2);
  LCDCommand(0x01,2);
}
