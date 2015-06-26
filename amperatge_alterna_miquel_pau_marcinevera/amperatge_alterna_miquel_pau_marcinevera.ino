// amperatge_alterna_miquel_pau_marcinevera.ino
// Programa per obtenir l'amperatge d'una carrega de corrent alterna mitjançant el sensor ACS712
// modificacio marcinevera del gatzem

//Sensibilidad del amperimetro, en V/A. Mirar Data sheet del sensor ACS712   5A -> 0.185  20A -> 0.100  30A -> 0.066
#define SensibAmperim  0.1
#define Voltatge 230
#define LED 13
#define zero 512

  int ences = 0;
  int vegada = 0;
  double t_ences_i = 0;
  double t_apagat_i = 0;
  double t_ences = 0;
  double t_apagat = 0;
  double t_t_ences=0;
  double t_t_apagat=0;

void setup() {
  Serial.begin(9600);
  delay (100);
  pinMode(LED, OUTPUT);
}

int calc_I(float i) {
  int i_neta;
  if ( i < zero ) i_neta = zero - i;
  if ( i >= zero ) i_neta = i - zero;
  return i_neta;
}

void loop() {
  int IntensitatMaxima = 0;
  float Potencia = 0;
  float Amper_Ap = 0;  
  
  //llegim durant 66 ms
  double t_ini = millis();
//  Serial.print("milis actuals ");
//  Serial.println(t_ini);
  Serial.print("segons actuals ");
  Serial.println(t_ini/1000);

  while ( t_ini + 70 > millis() )
    IntensitatMaxima = max(IntensitatMaxima, calc_I(analogRead(A0)) );
//    IntensitatMaxima = (analogRead(A0));    

//  Serial.print("Temps lectura en milisegons: ");
//  Serial.println(millis()-t_ini);

//  Serial.print("Lectura analogica de la Intensitat maxima: ");
//  Serial.println(IntensitatMaxima);
//  Serial.print(IntensitatMaxima);
  
  // tradueix el valor analogic a valor en ampers, i de la intensitat maxima passa a RMS
  Amper_Ap = (( IntensitatMaxima ) * ( 5 / (1024 * SensibAmperim))) / 1.414214 ;
  Serial.print("                              Ampers: ");
  Serial.print(Amper_Ap);

// engega un LED si hi ha consum
  if (Amper_Ap > 0.3)                          // mes gran de 0,3 per evitar el "soroll" del sensor    // 0.3*230= 69 W !!!
    digitalWrite (LED, HIGH);                      // engega el LED de la placa si el frigo esta engegat
  else
    digitalWrite (LED, LOW);

  Potencia = Amper_Ap * Voltatge ;
  Serial.print("                              Potencia en W: ");
  Serial.println(Potencia);




/*
//-------------------------------------------------------------------------------------------------------------------------------------
//temps ences 
  if (Amper_Ap > 0.3) {                  //cas1        // mes gran de 0,3 per evitar el "soroll" del sensor    // 0.3*230= 69 W !!!
    if ( ences == 0) {
      Serial.print("puttttaaaaaaaaaaaaaa: ");
      t_ences_i = millis();
      ences = 1;
      vegada = vegada + 1;
    }
    if ( ences == 1)                     //cas2
      t_ences = millis() - t_ences_i ;
  }

// temps apagat                                                          //if ( Amper_Ap < 0.3 ) {
  else {                                //cas3
    if ( ences == 1) {
      t_apagat_i = millis();
      ences = 0;
    }
    if ( ences == 0)                     //cas4
      t_apagat = millis() - t_apagat_i ;
  }
//-------------------------------------------------------------------------------------------------------------------------------------
*/


if (Amper_Ap > 0.3 && ences==0) {        // cas1
  Serial.print ("                                                                                                         ENCESA");
  Serial.println();
  t_apagat = millis() - t_apagat_i ;
  t_t_apagat = t_t_apagat + t_apagat;
  t_ences_i = millis();
  ences = 1;
  vegada = vegada+1;
}

if (Amper_Ap > 0.3 && ences == 1) {        // cas2
//  Serial.print ("continua ences");
//  Serial.println();
  t_ences = millis() - t_ences_i ;
}

if (Amper_Ap < 0.3 && ences == 1) {        // cas3
  Serial.print ("                                                                                                         APAGADA");
  Serial.println();
  t_ences = millis() - t_ences_i ;
  t_t_ences = t_t_ences + t_ences;
  t_apagat_i = millis();
  ences = 0;
}

if (Amper_Ap < 0.3 && ences == 0) {        // cas4
//  Serial.print ("continua apagat");
//  Serial.println();
  t_apagat = millis() - t_apagat_i ;
}





  Serial.print("ences: ");
  Serial.println(ences);

//  Serial.print("temps ences_inicial: ");
//  Serial.println(t_ences_i/1000);

  Serial.print("minuts temps ences: ");
  Serial.print(t_ences/60000);

  Serial.print("                       minuts temps total ences: ");
  Serial.println(t_t_ences/60000);


//  Serial.print("temps apagat_inicial: ");
//  Serial.println(t_apagat_i/1000);

  Serial.print("minuts temps apagat: ");
  Serial.print(t_apagat/60000);

  Serial.print("                      minuts temps total apagat: ");
  Serial.println(t_t_apagat/60000);

  Serial.print("vegades que s'ha ences la nevera: ");
  Serial.println(vegada);
  Serial.println("---------------------------------------------------------------------------------------------------");
//  Serial.println();

 
  delay(582);
}
