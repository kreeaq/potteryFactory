#include <Wire.h>

#define LED RED_LED

byte frame[3];        // Odebrana ramka danych

float h = 0.01;        // Okres całkowania. 0.1h oznacza, że w ciągu 10 iteracji minie jedna godzina w rzeczywistości

int T = 0;          // Obliczona wartość temperatury
double T0 = 20.0;      // Warunek początkowy [st.C]
double M = 1000;     // Masa wsadu zawartego w komorze [kg]
double v = 1.0;        // Prędkość konwojera 
double q = 1.0;       // Dopływ gazu [m3/h]
double n = 7.0;        // Współczynnik nadmiaru powietrza do gazu
double m = 300.0;      // Masa wsadu przypadająca na jednostkę długości konwojera [kg/s]
double c = 0.8*1000; // Ciepło właściwe wsadu [J/(kg*m3)]
double c_s = 1000.0;   // Ciepło właściwe spalin [J/(kg*m3)]
double Ro_s = 1.0;     // Gęstość spalin
double Tp = 1500.0;    // Temperatura płomienia [st.C] - WARTOŚĆ ZADANA

int inc = 0;        // Pierwsza iteracja        

void setup()
{
  T = T0;
  
  Serial.begin(9600);           // start serial for output
  
  Wire.setModule(0);
  Wire.begin(5);                // join i2c bus with address 
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  
  pinMode(LED, OUTPUT);  
}

void loop()
{
  /*
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
  *//*
  Serial.print("q: ");  
  Serial.println(q);
  Serial.print("Tp: ");    
  Serial.println(Tp);
  Serial.print("T: ");    
  Serial.println(T);   */

  Serial.println((double)(frame[0]));
  Serial.println((double)(frame[2]));

}

// functia executes wheneer data is received from master
// this function egistered as an event, see setup()
void receiveEvent(int howMany)
{
    float G;
    
    frame[0] = Wire.read();
    frame[1] = Wire.read();
    frame[2] = Wire.read();

    //q = (int)(frame[0] / 0.1275);
    //Tp = (int)(frame[2] / 0.1275);
    q = ((double)(frame[0]) / 0.1275);
    Tp = ((double)(frame[2]) / 0.1275);
    //n = q*7;
    T = (inc==0) ? T0:T; //JeÄąâ€şli to poczĂ„â€¦tek symulacji, to ustaw T na T0
    //T = T + 1/(M*c)*((n+1)*q*Ro_s*c_s*(Tp-T) - m*v*c*(T-T0))*h; //GÄąâ€šÄ‚Ĺ‚wne rÄ‚Ĺ‚wnanko na temperaturĂ„â„˘ w piecu
    G = 1/(M*c)*((n+1)*q*Ro_s*c_s*(Tp-T) - m*v*c*(T-T0));
    T = T + G*h;
    
    inc = (inc == 0) ? 1 : inc; //Ustaw inc na 1 ÄąÄ˝eby potem nie ustawiaÄąâ€šo T rÄ‚Ĺ‚wne T0
    inc = 1;
  
}

// function that executes whenever data is requested bymaster
// this function is registered as an event, see setup()
void requestEvent() 
{
  
  Wire.write(T);

}


