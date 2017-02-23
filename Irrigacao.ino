#include <Wire.h>
#include "RTClib.h"
int umidade;
String hora, minuto, segundo, str_umidade;
RTC_DS1307 rtc;

void setup () {
  pinMode(13, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(13, LOW);// Desliga o solenóide
  digitalWrite(7, LOW); // Desliga o sensor


  Serial.begin(9600);

//   #ifdef AVR
//      Wire.begin();
//      #else
//      Wire1.begin();
//      #endif

  rtc.begin();


  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//Serial.print("inicialização");
    }
}

void loop () {
  umidade = analogRead(A0);
  umidade = map(umidade, 0, 1023, 0, 100);
  DateTime now = rtc.now();
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();
  String horario = hora + ":" + minuto + ":" + segundo;
//  Serial.print(now.hour()); // Mostra a hora
//  Serial.print(":");        // mostra separação da hora
//  Serial.println(now.minute(),DEC);//mostra os minutos
  Serial.println(horario);
  //Serial.println(now.year(), DEC); // Mostra anos
  str_umidade = umidade + "%";
  Serial.println(str_umidade); //Mostra o valor da umidade

    ///////////////////////////////////////////////////////////////////////=
//////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////=
//////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////=
//////////////////////////////////////////////////////////

          //Verifica se está no horário do acionamento

  if (( now.hour() == 23 && now.minute() == 28)) {
    Serial.println("Ligado");
    digitalWrite(7, HIGH); // Liga pino 7 para acionar o sensor
    if (umidade < 35) {  //Se a umidade for maior que 700 desliga o solenóide
      digitalWrite(13, HIGH);  //Caso a umidade seja menor que 35% liga o pino=13 do solenoide
    } else {
      digitalWrite(13, LOW);// Desliga o pino 13 do solenoide
    }
  }
          //Desliga a acionamento na parte da manha
              
  if (( now.hour() == 23 && now.minute() >= 29)) {
    Serial.println("Desligado");
    digitalWrite(7, LOW);  //Desliga o pino 7
    digitalWrite(13, LOW); //Desliga pino 13
  }
  delay(1000);
}
