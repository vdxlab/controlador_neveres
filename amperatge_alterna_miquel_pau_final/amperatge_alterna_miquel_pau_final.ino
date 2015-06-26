
// Programa per obtenir l'amperatge d'una carrega de corrent alterna mitjançant el sensor ACS712

//Sensibilidad del amperimetro, en V/A. Mirar Data sheet del sensor ACS712       5A -> 0.1        20A -> 0.185  
#define SensibAmperim  0.185
#define Voltatge 230
#define LED 13

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

int calc_I(float i) {
  int i_neta;
  if ( i < 512 ) i_neta = 512 - i;
  if ( i >= 512 ) i_neta = i - 512;
  return i_neta;
}

void loop() {
  int IntensitatMaxima = 0;
  float Potencia = 0;
  float Amper_Ap = 0;  
  
  //llegim durant 60 ms
  double t_ini = millis();

  while ( t_ini + 60 > millis() )
    IntensitatMaxima = max(IntensitatMaxima, calc_I(analogRead(A0)+2) );

  Serial.print("Temps lectura: ");
  Serial.println(millis()-t_ini);

  Serial.print("Lectura analogica de la Intensitat maxima: ");
  Serial.println(IntensitatMaxima);
  
  // tradueix el valor analogic a valor en ampers, i de la intensitat maxima passa a RMS
  Amper_Ap = (( 2*IntensitatMaxima ) * ( 5 / (1024 * SensibAmperim))) / 1.414214 ;
  Serial.print("Ampers: ");
  Serial.println(Amper_Ap);

// engega un LED si hi ha consum
  if (Amper_Ap > 0.3)                          // mes gran de 0,3 per evitar el "soroll" del sensor    // 0.3*230= 69 W !!!
    digitalWrite (LED, HIGH);                      // engega el LED de la placa si el frigo esta engegat
  else
    digitalWrite (LED, LOW);

  Potencia = Amper_Ap * Voltatge ;
  Serial.print("Potencia en W: ");
  Serial.println(Potencia);
  Serial.println("------------------------------------------------------");

  delay(2000);
}
