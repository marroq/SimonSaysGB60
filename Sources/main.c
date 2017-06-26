#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "lcd.h"
#include "stdlib.h"

const byte LEDS   = 4;
const byte LEVEL1  = 6;

void waitKey(char key){
  switch(key){
    case 1: while(!Push1);
            while(Push1);
            break;
    case 2: while(!Push2);
            while(Push2);
            break;
    case 3: while(!Push3);
            while(Push3);
            break;
    case 4: while(!Push4);
            while(Push4);
            break;
  }
}

void config() {
    DisableInterrupts;
    
    /*habilitar LEDs como salida*/
    PTFDD_PTFDD0 = 1;
    PTFDD_PTFDD1 = 1;
    PTFDD_PTFDD2 = 1;
    PTFDD_PTFDD3 = 1;
    
    /*habilitar */
    PTADD_PTADD4 = 0;    
    PTADD_PTADD5 = 0;
    PTADD_PTADD6 = 0;
    PTADD_PTADD7 = 0;
    
    EnableInterrupts;
}

void symonSequency(byte *seq) {
  byte led=0, i;
  
  for(i=0;i<LEVEL1;i++) {
    led = rand() % LEDS + 1;
    seq[i] = led;
  }
}

void showGame(byte *sequency) {
  byte i;
  for (i=0; i<LEVEL1; i++) {
    switch(sequency[i]) {
      case 1:
        PTFD_PTFD0 = 1;
        break;
      case 2:
        PTFD_PTFD1 = 1;
        break;
      case 3:
        PTFD_PTFD2 = 1;
        break;
      case 4:
        PTFD_PTFD3 = 1;
        break;
    }
  }
}

byte play(byte *sequency) {
  byte i=0, stage=0;
  
  for(i=0; i<LEVEL1; i++) {
    waitKey(1);
    if (PTAD_PTAD4 == 1) {
      if (sequency[i] == 1)
        stage++;
    } else if (PTAD_PTAD5 == 1) {
      if (sequency[i] == 2)
        stage++; 
    } else if (PTAD_PTAD6 == 1) {
      if (sequency[i] == 3)
        stage++;
    } else if (PTAD_PTAD7 == 1) {
      if (sequency[i] == 4)
        stage++;
    }
  }
  
  return stage;
}

void main(void) {
  byte *sequency = (byte*)malloc(LEVEL1 * sizeof(byte));
  byte stage=0;
      
  LCDinit();
  config();                   //Configuracion de leds y push button
  symonSequency(sequency);    //Almaceno secuencia del juego
  showGame(sequency);         //Mostrar secuencia al jugador
  
  stage = play(sequency);     //Validacion de secuencia por el jugador            
    
  if (stage == LEVEL1) {      
    LCDString("Correct!");
  } else {
    LCDString("YOU LOSE!");
  }
  
  free(sequency);
}
