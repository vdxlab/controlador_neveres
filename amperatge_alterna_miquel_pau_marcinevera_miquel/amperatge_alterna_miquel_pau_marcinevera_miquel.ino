
// Programa per obtenir l'amperatge d'una carrega de corrent alterna mitjançant el sensor ACS712
// modificacio marcinevera del gatzem

//Sensibilidad del amperimetro, en V/A. Mirar Data sheet del sensor ACS712   5A -> 0.185  20A -> 0.100  30A -> 0.066
#define SensibAmperim  0.1
#define Voltatge 230
#define LED 13
#define MSG_ON "ences"
#define MSG_OFF "apagat"

//  int vegada = 0;
  double temps_inicial;
  double temps_final;
  boolean ences = 0;
  boolean abans = 0;
  String Missatge;

/*  
  double t_ences_i = 0;
  double t_apagat_i = 0;
  double t_ences = 0;
  double t_apagat = 0;
 */

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  temps_inicial = millis ();
}

int calc_I(float i) {
  int i_neta;
  if ( i < 512 ) i_neta = 512 - i;
  if ( i >= 512 ) i_neta = i - 512;
  return i_neta;
}

void loop() { 
  int IntensitatMaxima = 0;
  float Amper_Ap = 0;  

  //llegim durant 60 ms
  double t_ini = millis();

  while ( t_ini + 70 > millis() )
    IntensitatMaxima = max(IntensitatMaxima, calc_I(analogRead(A0)+2) );

  // tradueix el valor analogic a valor en ampers, i de la intensitat maxima passa a RMS
  Amper_Ap = (( IntensitatMaxima ) * ( 5 / (1024 * SensibAmperim))) / 1.414214 ;

  // Serial.print("Lectura analogica de la Intensitat maxima: ");
  // Serial.println(IntensitatMaxima);

  //  Serial.print("Ampers: ");
  //  Serial.println(Amper_Ap);

  abans = ences;

  // ences o apagat?
  if (Amper_Ap > 0.3)
    ences = 1; 
  else
    ences = 0;

  // engega un LED si hi ha consum
  if (ences)                          
    digitalWrite (LED, HIGH);                      
  else
    digitalWrite (LED, LOW);

  // Potencia = Amper_Ap * Voltatge ;
  // Serial.print("Potencia en W: ");
  // Serial.println(Potencia);
  // Serial.println();

  if (abans)
    Missatge = MSG_ON;
  else
    Missatge = MSG_OFF;

  // hi ha hagut un canvi 
  if (abans != ences) {
    temps_final = millis () - temps_inicial;
    temps_inicial = millis();

    Serial.print("Temps ");
    Serial.print(Missatge);
    Serial.print(" : ");
    Serial.print(temps_final / 1000); 
    Serial.println(" minuts");

    // Serial.print(temps_final / 1000); 
    // Serial.println(" segons"); 
  }
}
