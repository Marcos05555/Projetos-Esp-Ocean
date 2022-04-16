#include <PubSubClient.h> //Biblioteca para utilizanção do MQTT
#include <ESP8266WiFi.h> //Biblioteca do ESP8266 wifi
#include <Servo.h> //Biblioteca do Servo

const char* ssid = "Duarte."; //Nome da Rede WIFI que está usando
const char* password = "Kimura-95*L#33Tartaruga-7"; // Senha da Rede Wifi que está usando

const char* topic_sub1 = "ocean-esp8266-verde"; //Topico onde o esp é um assinant
const char* topic_sub2 = "ocean-esp8266-amarelo"; //Topico onde o esp é um assinant
const char* topic_sub3 = "ocean-esp8266-vermelho"; //Topico onde o esp é um assinant
 
const char* mqtt_id = "esp8266-Ocean";//ID do usuario, deve ser unico para cada esp8266

const char* BROKER_MQTT = "mqtt.eclipseprojects.io"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; //Porta do Broker MQTT

char topic;
WiFiClient espClient; //Cria o objeto espClient do tipo WiFiclient
PubSubClient MQTT(espClient); //Instancia o cliente Mqtt passando o objeto espClient

void initMQTT(void); //Função responsavel por iniciar a comunicação com o broker mqtt
void mqtt_callback(char* topic, byte* payload, unsigned int length); //Função responsavel receber os dados do broker mqtt
void reconnectMQTT(void); //Função responsavel por realizar a conexão com o broker mqtt
void VerificaConexaoMQTT(void); //Função responsavel por verificar a conexão com o broker mqtt

Servo meu_servo;  // instância chamada de meu_servo

int LedVerde = 13;
int LedAmarelo = 12;
int LedVermelho = 14;
void setup() { 
  
  Serial.begin(115200); // inicia a serial
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Verifica se a conexão com a rede foi realizada e só vai sair do laço quando a conexão foi realizada
    delay(500);
    Serial.print(".");
   }

  Serial.println("WiFi Connected .... IP Address:"); //IP da rede
  Serial.println(WiFi.localIP());
  pinMode(LedVerde, OUTPUT);
  pinMode(LedAmarelo, OUTPUT);
  pinMode(LedVermelho, OUTPUT);
  
  meu_servo.attach(D4); //inicia o servo na porta D4 do est8266 
  initMQTT(); // Chama a função initMQTT
  
}

void loop() {
  
  VerificaConexaoMQTT(); //Chama a função VerificaConexaoMQTT
  MQTT.loop(); //Necessario para ficar em contato comunicação com o broker

}

void initMQTT(void)
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    String mensagem; // Obtem a string do payload recebido 
    for(int i = 0; i < length; i++) // laço de repetição responsável por converter a mensagem
    {
       char c = (char)payload[i];
       mensagem += c;
    }
    
    Serial.println("Topico da mensagem");
    Serial.println(topic); //Topico da mensagem do Broker
    Serial.println("Mensagem recebida do broker");
    
    if(String(topic) == "ocean-esp8266-verde"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      if(mensagem.toInt() == 1){
        Serial.println(mensagem);
        digitalWrite(LedVerde,HIGH);
      }
      else if(mensagem.toInt() == 0){
        Serial.println(mensagem);
        digitalWrite(LedVerde,LOW);
      }
    }
  FunLed();
}

void reconnectMQTT(void)
{
    while (!MQTT.connected())
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(mqtt_id))
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(topic_sub1);
 
        }
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

void VerificaConexaoMQTT(void)
{
  if (!MQTT.connected())
  reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita   
}

void FunLed(){
  
    if(String(topic) == "ocean-esp8266-verde"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      if(mensagem.toInt() == 1){
        Serial.println(mensagem);
        digitalWrite(LedVerde,HIGH);
      }
      else if(mensagem.toInt() == 0){
        Serial.println(mensagem);
        digitalWrite(LedVerde,LOW);
      }
    }
    if(String(topic) == "ocean-esp8266-amarelo"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      if(mensagem.toInt() == 1){
        Serial.println(mensagem);
        digitalWrite(LedAmarelo,HIGH);
      }
      else if(mensagem.toInt() == 0){
        Serial.println(mensagem);
        digitalWrite(LedAmarelo,LOW);
      }
    }
    if(String(topic) == "ocean-esp8266-vermelho"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      if(mensagem.toInt() == 1){
        Serial.println(mensagem);
        digitalWrite(LedVermelho,HIGH);
      }
      else if(mensagem.toInt() == 0){
        Serial.println(mensagem);
        digitalWrite(LedVermelho,LOW);
      }
    }
}
