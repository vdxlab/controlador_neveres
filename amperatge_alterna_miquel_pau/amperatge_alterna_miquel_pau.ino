
// Programa per obtenir l'amperatge d'una carrega de corrent alterna mitjançant el sensor ACS712


float IntensitatMaxima = 0;
float Amperimetre = 0;               
float Amper_Ap = 0;            //Sortida
float SensibAmperim = 0.1;              //Sensibilidad del amperimetro, en V/A. Mirar Data sheet del sensor ACS712
float Voltatge = 230;
float Potencia = 0;


void setup()
{
 
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

int calc_I(float i) {
  int i_neta;
  if ( i < 512) i_neta= 512-i
 
}

void loop()
{
  IntensitatMaxima = analogRead(A0);
 
  Serial.print("Lectura analogica de la Intensitat en el primer instant: ");
  Serial.println(IntensitatMaxima);

//   IntensitatMaxima = IntensitatMaxima + 2 ;       //para corregir la desviacion del sensor ACS712 que tiende a poner el cero en 510
  
  if (IntensitatMaxima < 512)          // para evitar que el maximo elimine las intensidades negativas
  {
int Diferencia = 0;
    Diferencia = 512 - IntensitatMaxima;
IntensitatMaxima = IntensitatMaxima + (Diferencia * 2) - 1;
  }
 
for (int i=0; i<60; i++)                 

// n medidas para encontrar la intensidad maxima de la onda sinoidal
// Cal mirar si ajustant els temps entre mesures es pot reduir el numero de mesures i disminuir l'error
                                         
{
Amperimetre = analogRead(A0);

// Amperimetre = Amperimetre +2 ;        //para corregir la desviacion del sensor ACS712 que tiende a poner el cero en 510

if (Amperimetre < 512)          // para evitar que el maximo elimine las intensidades negativas
  {
int Diferencia = 0;
    Diferencia = 512 - Amperimetre;
Amperimetre = Amperimetre + (Diferencia * 2) - 1;
  }

 
IntensitatMaxima = max(IntensitatMaxima, Amperimetre);

delay(21);                    // un ciclo de onda, con AC de 50 Hz, dura 20 ms. Quan dura l'execucio del for?

}

  Serial.print("Lectura analogica de la Intensitat maxima: ");
  Serial.println(IntensitatMaxima);
  
// tradueix el valor analogic a valor en ampers, i de la intensitat maxima passa a RMS

Amper_Ap = (( IntensitatMaxima - 512 ) * ( 5 / (1024 * SensibAmperim))) / 1.414214 ;

Serial.print("Ampers: ");
Serial.println(Amper_Ap);

// engega un LED si hi ha consum

if (Amper_Ap > 0.3)                          // mes gran de 0,3 per evitar el "soroll" del sensor    // 0.3*230= 69 W !!!
{
digitalWrite (13, HIGH);                      // engega el LED de la placa si el frigo esta engegat
}
else
{
  digitalWrite (13, LOW);
}

Potencia = Amper_Ap * Voltatge ;

Serial.print("Potencia en W: ");
Serial.println(Potencia);


delay(2000);

}
