/*  05 / Octubre / 2024    09:15 am - functional version V2.

ARDUIPONIA

funciones en botones y conectado el pin 12 (2do barreno alado dela serigrafia en la placa"LCD Keypad Shied")
para dar la muestra del ciclo de encendido del rele a la tecla LEFT e impresion de temperatura al 
presionar el botn RIGHT , con impresion de reloj y fecha en stand-by

Programa para :
-Arduino UNO .
-"LCD keypad Shield D1 robot " , Display Shield de arduino UNO . 
-DHT 11 temp y hum sensor   1pin = vcc , 2pin = data , 3pin = null , 4pin = gnd .


Con implementacion a proyecto "GARDEN ENGINEERING" , UPT Tecamac .
PIXEL BITS STUDIOS             div: ROBOTIKS & DYNAMICS
Titular y Programador : Pacheco Rojas Julio Cesar , 1323144310 .

Please Note: pin 10 must be set to INPUT to prevent damage to your shield
*/

//Librerias a utilizar .
#include <LiquidCrystal.h>
#include <DHT11.h>
//#include<LiquidCrystal_I2C.h>       // Libreria para trabajar LCD sin botones por protocolo I2C


// Pines que usara el shield .
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // 8 = rs del display , 9 = enabled , 4 5 6 7 = pin digital 4 5 6 7  .
DHT11 dht11(2);                       //metodo de la libreria DHT.h , utilizando como INPUT el pin digital 2 .
//LiquidCrystal_I2C lcd(0x27,16,2);   //Declaracion para trabajar LCD por I2C .
// Variables used by the panel and buttons

int minutos = 59;
int segundos = 55;
int horas = 12;
int dia = 5;
int mes = 5;
int anio = 2024;
int LED = 13;
int lcd_key = 0;
int adc_key_in = 0;

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

//IS logo bit map
byte c1[8] = {
  0b00000,
  0b00000,
  0b00011,
  0b00100,
  0b01000,
  0b01011,
  0b10010,
  0b10010

};

byte c2[8] = {
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00001,
  0b10101

};

byte c3[8] = {
  0b00000,
  0b00000,
  0b11000,
  0b00100,
  0b11010,
  0b00110,
  0b00101,
  0b00101

};

byte c4[8] = {
  0b10010,
  0b10010,
  0b01011,
  0b01000,
  0b00100,
  0b00011,
  0b00000,
  0b00000

};

byte c5[8] = {
  0b10101,
  0b00000,
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b11111,
  0b00000

};

byte c6[8] = {
  0b11001,
  0b01001,
  0b11010,
  0b00010,
  0b00100,
  0b11000,
  0b00000,
  0b00000

};


//gota hydro bit map
byte G[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00010,
  0b00010,
  0b00100,
  0b00100,
  0b01000
};

byte H[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b01000,
  0b01000,
  0b00100,
  0b00100,
  0b00010
};

byte O[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b01000,
  0b00100,
  0b00011

};


byte P[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00010,
  0b00100,
  0b11000
};
//


char pos[10];  //size + 1

void setup() {
  Serial.begin(9600);
  dht11.setDelay(1000);  // delay en al lectura
  //digitalWrite(12, LOW);
  pinMode(LED, OUTPUT);
  lcd.begin(16, 2);       // start the library y la pantalla como tal y define el tamaño del matrix del display 16 columans * 2 filas .
  lcd.createChar(0, c1);  //
  lcd.createChar(1, c2);  //
  lcd.createChar(2, c3);  //    se tienen que crear los caracteres justo antes de usarlos para quer no se traslapen al momento de imprimir
  lcd.createChar(3, c4);  //  estas lines generan el caracter previamente ingresado como bit map
  lcd.createChar(4, c5);  //    con la variable asignada siempre se deve empezar desde 0
  lcd.createChar(5, c6);  //
  lcd.setCursor(0, 0);    // donde empieza el primer caracter generado columna 0 fila 0
  lcd.write((byte)0);
  lcd.write((byte)1);
  //lcd.write(byte(0)); // when calling lcd.write() '0' must be cast as a byte
  lcd.write((byte)2);
  lcd.setCursor(0, 1);  // la linea de abajo de la imagen en bit map columna 0 fila 1 , la de abajo .
  lcd.write((byte)3);
  lcd.write((byte)4);
  lcd.write((byte)5);
  lcd.setCursor(6, 0);
  lcd.print("GARDEN");
  lcd.setCursor(4, 1);
  lcd.print("ENGINEERING");
  delay(5000);
  lcd.clear();           // limpia la pantalla para pasar a la siguiente
  lcd.createChar(6, G);  //
  lcd.createChar(7, H);  //   creacion de caracteres de la gota hydro
  lcd.createChar(8, O);  //
  lcd.createChar(9, P);  //
  lcd.setCursor(0, 0);   //gota inicio linie de arriba 0
  lcd.write((byte)6);
  lcd.write((byte)7);
  lcd.setCursor(0, 1);
  lcd.write((byte)8);
  lcd.write((byte)9);  //fin de gota
  lcd.setCursor(5, 0);
  lcd.print("CONTROL");
  lcd.setCursor(4, 1);
  lcd.print("HIDROPONIA");
  delay(5000);
  lcd.clear();

  for (int i = 0; i < 5; i++)  //Ciclo para parpadeo de mensaje de developer .
  {
    lcd.setCursor(0, 0);
    lcd.print("Developed By : ");
    lcd.setCursor(0, 1);
    lcd.print(" JULIO PACHECO");
    delay(300);
    lcd.clear();
    delay(200);
  }
}

void loop() {  //el reloj se mostrara de standby por estar al principio del loop() y fuera de algun {} .

  temphumserial();

  segundos++;
  if (segundos > 59) {
    minutos++;
    segundos = 0;
  }
  if (minutos > 59) {
    horas++;
    minutos = 0;
    segundos = 0;
  }
  if (horas >= 24) {
    horas = 0;
    minutos = 0;
    segundos = 0;
    dia++;
    //if(horas > 12) horas = horas -12 ;           /// idea de bucle para cambiar entre formato 12/24 hrs


    //condicional ara meses de 31 dias
    if (((mes == 1) || (mes == 3) || (mes == 5) || (mes == 7) || (mes == 8) || (mes == 10) || (mes == 12)) && (dia > 31)) {
      mes++;
      dia = 1;
    }


    //condicional para aumentar el año
    if (mes == 13) {
      mes = 1;
      anio++;
      dia = 1;
    }


    //condicional ara meses de 30 dias
    if (((mes == 4) || (mes == 6) || (mes == 9) || (mes == 11) && (dia > 30))) {
      mes++;
      dia = 1;
    }


    //condicional ara mese de 28 dias  FEBRERO
    if (((mes == 2) && (dia > 28))) {
      mes++;
      dia = 1;
    }
  }

  lcd.clear();
  lcd.print("Hora:");
  lcd.print(" ");
  if (horas < 10) lcd.print("0");
  lcd.print(horas);
  lcd.print(":");
  if (minutos < 10) lcd.print("0");
  lcd.print(minutos);
  lcd.print(":");
  if (segundos < 10) lcd.print("0");
  lcd.print(segundos);
  lcd.setCursor(0, 1);
  lcd.print("Fecha:");
  if (dia < 10) lcd.print("0");
  lcd.print(dia);
  lcd.print("/");
  if (mes < 10) lcd.print("0");
  lcd.print(mes);
  lcd.print("/");
  lcd.print(anio);
  delay(1000);
  lcd.setCursor(0, 1);           // move to the begining of the second line .
  lcd_key = read_LCD_buttons();  // read the buttons .  // como funcion para empezar odarle salida al menu de abajo
                                 /// OPCIONES DE ACCION "MENU" SIN INTERFAZ AL 10/AGO/24
  switch (lcd_key) {             // depending on which button is pushed is the perform an action .
    case btnRIGHT:
      {
        lcd.clear();  //LIMPIARA LA PANTALLA Y LUEGO ARA EL SCROLL DE TEXTO  .
        lcd.setCursor(2, 0);
        lcd.print("TEMPERATURA");
        lcd.setCursor(3, 1);
        lcd.print("& HUMEDAD");
        delay(3000);
        lcd.clear();

        temphumlcd();
        delay(5000);
        lcd.clear();
        break;
      }

    case btnLEFT:
      {
        pinMode(12, OUTPUT);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("CICLO HYDRO ON");
        lcd.setCursor(5, 1);
        lcd.print("1 Min.");
        digitalWrite(LED, HIGH);
        //digitalWrite(12, HIGH);
        delay(60000);  //tiempo de ciclo de prendido pin 12 (RELE de bomba hydoponia)
        digitalWrite(LED, LOW);
        pinMode(12, LOW);
        //delay(500);
        break;
      }
    case btnUP:
      {
        pinMode(10, OUTPUT);  //must be set as INPUT
        digitalWrite(10, HIGH);
        lcd.display();
        digitalWrite(LED, HIGH);
        lcd.clear();                                                             //LIMPIARA LA PANTALLA Y LUEGO ARA EL SCROLL DE TEXTO  .
        for (int positionCounter = 0; positionCounter < 5; positionCounter++) {  //40 para que de toda la vuelta  .
          lcd.setCursor(0, 0);
          lcd.print("De Regreso .");
          //digitalWrite(LED, HIGH);
          lcd.scrollDisplayRight();
          delay(250);
        }
        //delay(3000);
        break;
      }
    case btnDOWN:
      {
        digitalWrite(LED, HIGH);
        lcd.clear();                                                              //LIMPIARA LA PANTALLA Y LUEGO ARA EL SCROLL DE TEXTO  .
        for (int positionCounter = 0; positionCounter < 25; positionCounter++) {  //40 para que de toda la vuelta  .
          lcd.setCursor(7, 0);
          lcd.print("apagando pantalla . . .");
          //digitalWrite(LED, HIGH);
          lcd.scrollDisplayLeft();
          delay(250);
        }
        //delay(3000);
        lcd.noDisplay();
        pinMode(10, OUTPUT);  //must be set as INPUT
        digitalWrite(10, LOW);
        break;
      }
    case btnSELECT:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Program Time : ");
        //dtostrf((millis()/1000),9, 0, pos);  //unique function converts float to string .
        lcd.setCursor(7, 1);
        lcd.print(millis() / 1000);
        lcd.setCursor(1, 1);
        lcd.print(pos);
        delay(3000);

        break;
      }
    case btnNONE:
      {
        //delay(3000)
        //lcd.print("NONE  ");
        digitalWrite(LED, LOW);
        break;
      }
  }
}

void temphumlcd() {
  hum();

  int temp = 0;  //Podria dividirse segun la unidad de medida que se dese convertir   33.8 C = 1 F
  int hum = 0;

  int result = dht11.readTemperatureHumidity(temp, hum);

  if (result == 0) {
    Serial.print("Temperatura : ");
    Serial.print(temp);
    Serial.println("°C   ");

    lcd.setCursor(0, 0);
    lcd.print("Temp : ");
    lcd.print(temp);
    lcd.print(" C   ");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
}


void hum() {
  int hum = 0;
  int temp = 0;  //Podria dividirse segun la unidad de medida que se dese convertir   33.8 C = 1 F

  int result = dht11.readTemperatureHumidity(temp, hum);

  if (result == 0) {

    //Impresion de humedad en porcentaje y en niveles .
    Serial.print("Humedad : ");
    Serial.print(hum);
    Serial.print(" %  ");
    Serial.print("Nivel : ");
    if (hum <= 55) {
      Serial.print("1");
    } else {
      Serial.print("2");
    }
    Serial.println(".");
    //humlvl();
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
  if (result == 0) {

    //Impresion de humedad en porcentaje y en niveles .
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(hum);
    lcd.print("%  ");
    lcd.print("Lvl: ");
    if (hum <= 55) {
      lcd.print("1");
    } else {
      lcd.print("2");
    }
  } else {
    lcd.println(DHT11::getErrorString(result));
  }
}


void temphumserial() {

  int temp = 0;  //Podria dividirse segun la unidad de medida que se dese convertir   33.8 C = 1 F
  int hum = 0;

  int result = dht11.readTemperatureHumidity(temp, hum);

  //Impresion de temperatura en las tres unidades de medida Celsius , Fahrenheit , Kelvin .
  if (result == 0) {
    Serial.print("Temperatura : ");
    Serial.print(temp);
    Serial.print("°C   ");
    Serial.print((temp * 9 / 5) + 32);  // Formula para cpnversion a Fahrenheit .
    Serial.print("°F   ");
    Serial.print(temp + 273.15);  // Formula para cpnversion a Kelvin .
    Serial.print("°K   ");

    //Impresion de humedad en porcentaje y en niveles .
    Serial.print("\t \t Humedad : ");
    Serial.print(hum);
    Serial.print(" %  ");
    Serial.print("Nivel : ");
    if (hum <= 55) {
      Serial.print("1");
    } else {
      Serial.print("2");
    }
    Serial.println(".");
    //humlvl();

  } else {
    Serial.println(DHT11::getErrorString(result));
  }
  //delay(3000);

  //Condicion para la temperatura .
  if (temp >= 24) {
    digitalWrite(13, HIGH);

  } else {
    digitalWrite(13, LOW);
  }
}

// read buttons
int read_LCD_buttons() {
  adc_key_in = analogRead(0);  // read the value  .
  // my buttons when read are centered at these values: 0, 144, 329, 504, 741 .
  // we add approx 50 to those values and check to see if we are close .
  if (adc_key_in > 1000) return btnNONE;  // no Key pressed .
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 250) return btnUP;
  if (adc_key_in < 450) return btnDOWN;
  if (adc_key_in < 650) return btnLEFT;
  if (adc_key_in < 850) return btnSELECT;
  return btnNONE;  // when all others fail return btnNONE .
}
