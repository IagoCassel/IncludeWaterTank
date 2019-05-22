#include <Ethernet.h>
#include <UbidotsEthernet.h>
#include <SPI.h>

#define sensor1 5
#define sensor2 6
#define sensor3 7

int valor_s1 = 1, valor_s2 = 1, valor_s3 = 1;

//TOKEN da conta do Ubidots, não se deve alterar
char const * TOKEN = "Insira o token da conta do Ubidots";

//Variável que se deseja guardar no Ubidots
char const * VARIABLE_LABEL_1 = "water";

//MAC do Shield Ethernet, não se deve alterar
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Entra no client do Ubidots pelo TOKEN
Ubidots client(TOKEN);

void setup(){
  
  Serial.begin(9600);
  
  //Define os pinos dos sensores como entrada
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  // Começa e verifica a conexão com a internet
  Serial.print(F("Começando conexao..."));
  if (!Ethernet.begin(mac)) {
    Serial.println(F("Falhou"));
  } else {
    Serial.println(Ethernet.localIP());
  }
  
  //Tempo para o Ethernet Shield iniciar
  delay(2000);
  Serial.println(F("Pronto"));
}

void loop(){
  
  Ethernet.maintain();
  
  //Leitura dos sensores
  int valor_s1 = digitalRead(sensor1);
  int valor_s2 = digitalRead(sensor2);
  int valor_s3 = digitalRead(sensor3);

  //Valor da variável para a porcentagem de água
  float WaterPercentage = 0.0;

  //Checa o nivel e atualiza o valor de envio
  if ((valor_s1 == 0) && valor_s2 == 0 && valor_s3 == 0){
    Serial.print("Nível mais crítico de água, abaixo de 25%\n");
    WaterPercentage = 0.0;
  }

  if ((valor_s1 == 1) && valor_s2 == 0 && valor_s3 == 0){
    Serial.print("Nível alarmante de água, entre 25% e 50%\n");
    WaterPercentage = 25.0;
  }

  if (valor_s2 == 1 && valor_s3 == 0){
    Serial.print("Nível regular de água, entre 50% e 75%\n");
    WaterPercentage = 50.0;
  }

  if (valor_s3 == 1){
    Serial.print("Nível ótimo de água, entre 75% e 100%\n");
    WaterPercentage = 75.0;
  }

  //Adiciona o valor a variável do Ubidots
  client.add(VARIABLE_LABEL_1, WaterPercentage);

  //Envia a informação para o Ubidots
  client.sendAll();
  
  delay(10000);
}
