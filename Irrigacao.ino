#include <Wire.h>
#include "RTClib.h"

#include "Timer.h"

#define Bomba 13
#define Sensor 7

int umidade = 30, ano, mes, dia, hora, minuto, segundo, bombaEvent;
char data[20], horario[10], strUmidade[14],
  diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"},
  agenda[5][12] = {"18:33:00", "18:35:00", "18:37:00", "18:39:00", "18:40:00"};

RTC_DS3231 rtc;   //RTC_DS1307 rtc;
Timer t;

void setup () {
  pinMode(Bomba, OUTPUT);
  pinMode(Sensor, OUTPUT);
  digitalWrite(Bomba, LOW);                   // Desliga o solenóide
  digitalWrite(Sensor, LOW);                  // Desliga o sensor
  
  Serial.begin(115200);
  Serial.println("Comunicacao serial...");
  delay(3000);

  if (! rtc.begin()) {
    Serial.println("RTC NAO esta rodando!");
    while(1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC sem bateria, ajustando data e hora!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void desligaBomba() {
  digitalWrite(Bomba, LOW);  // Desliga o pino=13 do solenoide
  Serial.println(horario);
  Serial.println("Desligando bomba dagua!");
  t.stop(bombaEvent);
}

void loop () {  
  DateTime now = rtc.now();
  t.update();
  
  dia = now.day();
  mes = now.month();
  ano = now.year();
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();

  sprintf(data, "%s, %02d/%02d/%d", diasDaSemana[now.dayOfTheWeek()], dia, mes, ano);
  sprintf(horario, "%02d:%02d:%02d", hora, minuto, segundo);
  /*
  Serial.println(data);
  Serial.println(horario);
  */
          //Verifica se está no horário do acionamento
  for (int i = 0; i < sizeof(agenda) - 1; i++){
    if (strcmp(horario, agenda[i]) == 0) {
      digitalWrite(Sensor, HIGH); // Liga pino 7 para acionar o sensor
      delay(2000);  // Aguarda 2s para esperar o sensor energizar e realizar a leitura
      /*
      umidade = analogRead(A0); 
      umidade = map(umidade, 0, 1023, 0, 100);
      */
      if (umidade < 35) {  //Se a umidade for menor que 35% liga o solenóide
        digitalWrite(Bomba, HIGH);  //Caso a umidade seja menor que 35% liga o pino=13 do solenoide
        int bombaEvent = t.after(35000, desligaBomba); // Após 35s desligará a solenoide
        Serial.println(horario);
        sprintf(strUmidade, "Umidade: %d%%", umidade);
        Serial.println(strUmidade);
        Serial.println("Irrigacao INICIADA!");
      }
    }
  }
}
