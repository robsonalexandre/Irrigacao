#include <Wire.h>
#include <RTClib.h>
#include <Timer.h>
#include <SimpleDHT.h>

#define pinDHT11 A1
#define Bomba 13
#define Sensor A0
#define SensorOnOff 7
#define minUmidade 85

int bombaEvent;
char data[20], horario[10],
  diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"},
  agenda[][12] = {
    "21:33:00",
    "21:35:00",
    "21:37:00",
    "21:29:00",
    "21:30:00"
  };

RTC_DS3231 rtc;   //RTC_DS1307 rtc;
Timer t;
SimpleDHT11 dht11;

void getDateTime(boolean saidaSerial = false){
  DateTime now = rtc.now();
  sprintf(data, "%s, %02d/%02d/%d", diasDaSemana[now.dayOfTheWeek()], now.day(), now.month(), now.year());
  sprintf(horario, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  if (saidaSerial){
    Serial.println(data);
    Serial.println(horario);
  }
}

void desligaBomba() {
  digitalWrite(Bomba, LOW);  // Desliga o pino=13 do solenoide
  Serial.println(horario);
  Serial.println("Desligando bomba dagua!");
  t.stop(bombaEvent);
}

void setup () {
  pinMode(Bomba, OUTPUT);
  pinMode(SensorOnOff, OUTPUT);
  digitalWrite(Bomba, LOW);                   // Desliga o solenóide
  digitalWrite(SensorOnOff, LOW);             // Desliga o sensor
  
  Serial.begin(115200);
  delay(3000);

  if (! rtc.begin()) {
    Serial.println("RTC NAO esta rodando!");
    while(1);
  }
  /*
  if (rtc.lostPower()) {
    Serial.println("RTC sem bateria, ajustando data e hora!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  /**/
  getDateTime(true);
}

void loop () {
  /* Para uso com o sensor de umidade de solo + Comparador LM393 */
  int umidade;
  /*  Para uso com o sensor DHT11 *\
  byte umidade = 99
  /**/
  byte temperatura;
  getDateTime(false);
  t.update();
  
  //Verifica se está no horário do acionamento
  for (int i = 0; i < sizeof(agenda) - 1; i++){
    if (strcmp(horario, agenda[i]) == 0) {
      digitalWrite(SensorOnOff, HIGH); // Liga pino 7 para acionar o sensor
      delay(2000);  // Aguarda 2s esperando o sensor energizar e realizar a leitura
      /* Para uso com o sensor de umidade de solo + Comparador LM393 */
      umidade = analogRead(Sensor); 
      umidade = map(umidade, 0, 1023, 0, 100);
      /**/
      /*  Para uso com o sensor DHT11 (var umidade deve ser byte) *\
      dht11.read(pinDHT11, &temperatura, &umidade, NULL);
      /**/       
      if (umidade < minUmidade) {  //Se a umidade for menor que 35% liga o solenóide
        digitalWrite(Bomba, HIGH);  //Caso a umidade seja menor que 35% liga o pino=13 do solenoide
        int bombaEvent = t.after(35000, desligaBomba); // Após 35s desligará a solenoide
        Serial.println(horario);
        Serial.print("Umidade: "); Serial.print(umidade); Serial.println("%");
        Serial.print("Temperatura: "); Serial.print(temperatura); Serial.println("*C"); 
        Serial.println("Irrigacao INICIADA!");
      }
    }
  }
}
