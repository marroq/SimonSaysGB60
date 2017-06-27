#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "lcd.h"
#include "stdlib.h"

const byte LEDS   = 4;
const byte LEVEL1 = 4;
const byte LEVEL2 = 6;
const byte LEVEL3 = 8;

byte waitKey(){
  while(1) {
    if(!Push1)
      return 1;    
    else if(!Push2)
      return 2;
    else if(!Push3)
      return 3;
    else if(!Push4)
      return 4; 
  }
}

void delayPro(byte seconds){
  byte i=0;
  for (i=0;i<seconds*2;i++) {
    msDelay(250);
  }
}

void config() {
    SOPT_COPE = 0;  //deshabilitar watch dog
    PTFDD |= 0x0F;  //habilitar LEDs como salida
    PTADD |= 0x0;   //habilitar push button
    PTFD |= 0x0F;   //Iniciar LEDs apagados
}

void symonSequency(byte *seq, byte level) {
  byte led=0, i;
  
  for(i=0;i<level;i++) {
    led = (rand() % (LEDS * 1000))/1000 + 1;
    seq[i] = led;
  }
}

void showGame(byte *sequency, byte level) {
  byte i;
  for (i=0; i<level; i++) {
    switch(sequency[i]) {
      case 1:
        PTFD_PTFD0 = 0;
        delayPro(1);
        PTFD_PTFD0 = 1;        
        delayPro(1);
        break;
      case 2:
        PTFD_PTFD1 = 0;
        delayPro(1);
        PTFD_PTFD1 = 1;        
        delayPro(1);
        break;
      case 3:
        PTFD_PTFD2 = 0;
        delayPro(1);
        PTFD_PTFD2 = 1;        
        delayPro(1);
        break;
      case 4:
        PTFD_PTFD3 = 0;
        delayPro(1);
        PTFD_PTFD3 = 1;
        delayPro(1);
        break;
    }
  }
}

byte play(byte *sequency, byte level) {
  byte i=0, stage=0, push=0;
  
  for(i=0; i<level; i++) {
    push = waitKey();
    switch(push){
      case 1:
        PTFD_PTFD0 = 0;
        if (sequency[i] == 1) stage++;
        else i=level;
        delayPro(1);
        PTFD_PTFD0 = 1;
        break;
      case 2:
        PTFD_PTFD1 = 0;
        if (sequency[i] == 2) stage++; 
        else i=level;
        delayPro(1);
        PTFD_PTFD1 = 1;
        break;
      case 3:
        PTFD_PTFD2 = 0;
        if (sequency[i] == 3) stage++;
        else i=level;
        delayPro(1);
        PTFD_PTFD2 = 1;
        break;
      case 4:
        PTFD_PTFD3 = 0;
        if (sequency[i] == 4) stage++;
        else i=level;
        delayPro(1);
        PTFD_PTFD3 = 1;
        break;
    }
  }    
  return stage;
}

void evaluate(byte *sequency, byte *level, byte *stage, byte currentLevel){
  showGame(sequency, currentLevel);           //Mostrar secuencia al jugador
  *stage = play(sequency, currentLevel);      //Validacion de secuencia por el jugador            
  if (*stage == currentLevel) {      
    LCDClearScreen();  
    LCDString("YOU WIN!");
    delayPro(3);    
    *stage = 0;
    (*level)++;
  } else {
    LCDClearScreen();  
    LCDString("YOU LOSE!");    
    *stage = 0;
    delayPro(3);
  }  
}

void generateSeq(int *sequency,int *sequency2,int *sequency3){
  symonSequency(sequency,  LEVEL1);    //Almaceno secuencia primer nivel
  symonSequency(sequency2, LEVEL2);    //Almaceno secuencia segundo nivel
  symonSequency(sequency3, LEVEL3);    //Almaceno secuencia tercer nivel  
}

void main(void) {
  byte *sequency =  (byte*)malloc(LEVEL1 * sizeof(byte));
  byte *sequency2 = (byte*)malloc(LEVEL2 * sizeof(byte));
  byte *sequency3 = (byte*)malloc(LEVEL3 * sizeof(byte));
  byte stage=0, gLevel=1;
      
  LCDinit();
  config();                            //Configuracion de leds y push button
    
  while(1) {
    LCDClearScreen();
    LCDString("PLAY GAME \nLEVEL: ");
    LCDputchar(gLevel + '0');
    switch (gLevel) {
      case 4:
        LCDClearScreen();
        LCDString("FINISH 3 STAGES");
        gLevel=1;
        delayPro(5);
        LCDString("REINICIANDO");
        break;
      case 1:
        generateSeq(sequency, sequency2, sequency3);
        evaluate(sequency, &gLevel, &stage, LEVEL1);  
        break;
      case 2:
        evaluate(sequency2, &gLevel, &stage, LEVEL2);
        break;
      case 3:
        evaluate(sequency3, &gLevel, &stage, LEVEL3);
        break;
    }  
  }
    
  free(sequency);
  free(sequency2);
  free(sequency3);
}
