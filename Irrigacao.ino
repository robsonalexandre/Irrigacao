#include <Wire.h>
#include "RTClib.h"
int umidade, ano, mes, dia, hora, minuto, segundo;
char texto[3], data[20], horario[10], strUmidade[14];
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
char agenda[5][12] = {"12:48:00", "12:49:00", "12:50:00", "12:51:00", "12:52:00"};
RTC_DS3231 rtc;//RTC_DS1307 rtc;

void setup () {
  pinMode(13, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(13, LOW);// Desliga o solenóide
  digitalWrite(7, LOW); // Desliga o sensor
  
  Serial.begin(115200);
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
}

void loop () {  
  DateTime now = rtc.now();
  
  dia = now.day();
  mes = now.month();
  ano = now.year();
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();

  sprintf(data, "%s, %02d/%02d/%d", diasDaSemana[now.dayOfTheWeek()], dia, mes, ano);
  sprintf(horario, "%02d:%02d:%02d", hora, minuto, segundo);
  Serial.println(data);
  Serial.println(horario);

  umidade = analogRead(A0);
  umidade = map(umidade, 0, 1023, 0, 100);

  sprintf(strUmidade, "Umidade: %d%%", umidade);
  Serial.println(strUmidade);

          //Verifica se está no horário do acionamento

  for (int i = 0; i < sizeof(agenda) - 1; i++){
    if (strcmp(horario, agenda[i]) == 0) {
      digitalWrite(7, HIGH); // Liga pino 7 para acionar o sensor
      delay(5000);
      if (umidade < 35) {  //Se a umidade for menor que 30% liga o solenóide
        digitalWrite(13, HIGH);  //Caso a umidade seja menor que 35% liga o pino=13 do solenoide
        Serial.println("Irrigacao INICIADA!");
      }
    }
  }

  delay(1000);
}
