/*
 * ATTINY85 avec potentiomètre et écriture sur port USB comme un clavier.
 * Envoi les données sur Excel ou Notepad
 * 
 *     //All pins are capable of Digital output, though P5 is 3 V at HIGH instead of 5 V
    pinMode(0, OUTPUT); //0 is P0, 1 is P1, 2 is P2, etc. - unlike the analog inputs, for digital outputs the pin number matches.
    digitalWrite(0,HIGH);

        //All pins are capable of digital input.
    pinMode(0, INPUT); //0 is P0, 1 is P1, 2 is P2, etc. - unlike the analog inputs, for digital inputs the pin number matches.
    sensorValue = digitalRead(1); // retunes HIGH or LOW

         //You need not set pin mode for analogRead - though if you have set the pin to
    //output and later want to read from it then you need to set pinMode(0,INPUT);
    //where 0 is the physical pin number not the analog input number.
    sensorValue = analogRead(1); // 1 = Read P2; 2 = Read P4; 3 = Read P3; 0 = Read P5

    //P0, P1, and P4 are capable of hardware PWM (analogWrite).
    pinMode(0, OUTPUT); //0 is P0, 1 is P1, 4 is P4 - unlike the analog inputs, 
    analogWrite(0,255); //Turn the pin on full (100%)
    delay(1000);
    analogWrite(0,128); //Turn the pin on half (50%)
    delay(1000);
    analogWrite(0,0);   //Turn the pin off (0%)
    delay(1000);

  P3 and P4 to communicate with USB

  DigiSpark => ATtiny85
    p0    => pin 5: PB 0 (MOSI)
    p1    => pin 6: PB 1 (MISO)
    p2    => pin 7: PB 2 (SCK)
    p5    => pin 1: PB 5 (RESET)

 */


#include "DigiKeyboard.h"
//#include "EEPROM.h"

int const longAppli = 10;
int const longProfil = 30;
int const longPWD = 14;
int const nbAppli = 6;
int const sensorCat = 1024 / nbAppli; // nb de catégories
int index = 0; // Index pour l'application, le profil et le pwd

// chaines de caractères pour conversion clavier US en FR
char *chaineEntree = "azertyuiopqsdfghjklm<wxcvbn,;:!AZERTYUIOPQSDFGHJKLM>WXCVBN?./1234567890$-=+&é()_'µ°¨£$çà";
char *chaineSortie = "qwertyuiopasdfghjkl; zxcvbnm,./QWERTYUIOPASDFGHJKL: ZXCVBNM<>!@#$%^&*()]6=+125-8        ";

int zz;
int EEsize = 512; // size in bytes of your board's EEPROM

int sensorValue = 0; // pour le potentiometre
int sensorValuePrev = 0;
int buttonValue0 = 0; // pour le bouton P0
int buttonValue1 = 0; // pour le bouton P1

char appli[nbAppli][longAppli] = {
  "gmail",
  "Windows",
  "AS400",
  "ADP",
  "JDE",
  "LinkedIn"
};

char profil[nbAppli][longProfil] = {
  "xxxxxxxx@gmail.com",
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx@gmail.com"
};

char password[nbAppli][longPWD] = {
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx",
  "xxxxxxxx"
};


void setup() {
  // don't need to set anything up to use DigiKeyboard
pinMode(1, INPUT); //LED on Model A but 1 is P1 in INPUT
pinMode(0, INPUT); //0 is P0, 1 is P1, 2 is P2, etc. - unlike the analog inputs, for digital inputs the pin number matches.
    
//EEPROM.write
 // DigiKeyboard.delay(1000);
 
}


void loop() {
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  
  
  // You need not set pin mode for analogRead
  sensorValue = analogRead(1); // 1 = Read P2; 2 = Read P4; 3 = Read P3; 0 = Read P5
  buttonValue0 = digitalRead(0);
  buttonValue1 = digitalRead(1);

  // test if potentiometor is modified. And change the value
  if(sensorValue/sensorCat != sensorValuePrev/sensorCat) {
    index = sensorValuePrev/sensorCat;
    DigiKeyboard.sendKeyStroke(74, MOD_SHIFT_LEFT); 
    DigiKeyboard.sendKeyStroke(42); 
    sensorValuePrev = sensorValue;
    index = sensorValue/sensorCat;
    if(index == nbAppli) index = nbAppli - 1;
    
    //printStr("Appli = ");
    //DigiKeyboard.delay(100);
    printStr(appli[index]); // Affiche le nom de l'application dans la zone de saisie
    //DigiKeyboard.sendKeyStroke(KEY_ENTER);
    
  } else {

  // test if button is pressed. And send profil and password
  if(buttonValue0 == 1) {
    /*
    if (strcmp(appli[index], "gmail") == 0) {
      DigiKeyboard.sendKeyStroke(74, MOD_SHIFT_LEFT); 
      DigiKeyboard.sendKeyStroke(42); 
      printStr(profil[index]);
      DigiKeyboard.sendKeyStroke(KEY_ENTER);
      delay(1000);
      DigiKeyboard.sendKeyStroke(43); // TAB
      DigiKeyboard.sendKeyStroke(43); // TAB
      DigiKeyboard.sendKeyStroke(43); // TAB
      printStr(password[index]);
      DigiKeyboard.sendKeyStroke(KEY_ENTER);
    } else {
    */
    DigiKeyboard.sendKeyStroke(74, MOD_SHIFT_LEFT); // Sélectionne toute la ligne depuis le début
    DigiKeyboard.sendKeyStroke(42); // efface la ligne
    printStr(profil[index]);
    DigiKeyboard.sendKeyStroke(43); // TAB
    printStr(password[index]);
    DigiKeyboard.sendKeyStroke(KEY_ENTER);
    //}
  } else {
    if(buttonValue1 == 1) {
     //DigiKeyboard.sendKeyStroke(74, MOD_SHIFT_LEFT); 
     // DigiKeyboard.sendKeyStroke(42);
     //DigiKeyboard.sendKeyStroke(KEY_ENTER);
    printStr("Realise par F.Dordain");

     
    } 
    
  }
  }


}

void sendChaine1(char chaine[110]) {
  long posCar = 0;
  char transChaine[110];
  strcpy(transChaine, chaine);
  int len = strlen(transChaine);
  char *pst;
    for(int i=0; i<len; i++) {
      pst = strchr(chaineEntree, transChaine[i]);
      if(pst != NULL) {
        // caractère trouvé
        posCar = pst - chaineEntree;
        transChaine[i] = chaineSortie[posCar];
      } 
    }

  DigiKeyboard.print(transChaine);

}

// fonction pour test
void sendValue(int value) {
  int val = value / 1000;
  sendKey(val);
  val = (value / 100) % 10;
  sendKey(val);
  val = (value / 10) % 10;
  sendKey(val);
  val = value % 10;
  sendKey(val);
  
}

// fonction pour test
void sendKey(int value) {
   if(value == 0) DigiKeyboard.sendKeyStroke(KEY_0,MOD_SHIFT_LEFT);
        else DigiKeyboard.sendKeyStroke(KEY_1+(value-1),MOD_SHIFT_LEFT);
}

// Conversion chaine US <-> FR
void printStr(char *str) {
  char c = str[0];
  byte i = 0;
  long posCar = 0;
  char *pst; // position dans chaine
  DigiKeyboard.update();
  DigiKeyboard.sendKeyStroke(0); //this is generally not necessary but with some older systems it seems to prevent missing the first character after a delay
  while (c != 0) {
    if (c==' ')           DigiKeyboard.sendKeyStroke(KEY_SPACE);
    if (c=='0')           DigiKeyboard.sendKeyStroke(KEY_0,MOD_SHIFT_LEFT);
    if (c>='1' && c<='9') DigiKeyboard.sendKeyStroke(KEY_1+(c-'1'),MOD_SHIFT_LEFT);
    if (c=='$')           DigiKeyboard.sendKeyStroke(48);
    if (c==';')           DigiKeyboard.sendKeyStroke(54);
    if (c=='.')           DigiKeyboard.sendKeyStroke(99);
    if (c=='=')           DigiKeyboard.sendKeyStroke(46);
    if (c=='@')           DigiKeyboard.sendKeyStroke(39, MOD_ALT_RIGHT);
    if (c=='-')           DigiKeyboard.sendKeyStroke(35);
    if (c=='a') c='q'; else if (c=='q') c='a';
    if (c=='A') c='Q'; else if (c=='Q') c='A';   
    if (c=='z') c='w'; else if (c=='w') c='z';   
    if (c=='Z') c='W'; else if (c=='W') c='Z';  
    if (c=='m')           DigiKeyboard.sendKeyStroke(51);
    if (c=='M')           DigiKeyboard.sendKeyStroke(51, MOD_SHIFT_LEFT);
    if (c>='A' && c<='Z' && c!='M') DigiKeyboard.sendKeyStroke(KEY_A+(c-'A'), MOD_SHIFT_LEFT);
    if (c>='a' && c<='z' && c!='m') DigiKeyboard.sendKeyStroke(KEY_A+(c-'a'));
        
    i++;
    c = str[ i ];
  }
}



