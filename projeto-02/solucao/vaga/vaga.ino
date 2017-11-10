#include <UIPEthernet.h>
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x42 };
EthernetClient ethClient;

#include <Ultrasonic.h>

// Se não definirmos o nome ONLINE, podemos utilizar uma versão diferente da
// biblioteca que simula MQTT via a comunicação serial
#ifdef ONLINE
#include <PubSubClient.h>
#else
#include <SerialPubSubClient.h>
#endif

// const char* MQTT_SERVER = "test.mosquitto.org";
IPAddress MQTT_SERVER(192, 168, 3, 136);
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "arduino-vaga-42";

const char* topic = "vagas/42";

// PIN de LED para indicar se a vaga está ocupada
const int PIN_OCUPADO = 7;
const int DISTANCIA_MINIMA = 8; // cm

Ultrasonic ultrasonic(8, 9);

// Como nesta unidade não pretendemos receber mensagens, podemos passar o valor NULL no lugar de um callback
PubSubClient client(MQTT_SERVER, NULL, MQTT_PORT, ethClient);

void setup() {
	Serial.begin(9600);
	while (!Serial) {};

	#ifdef ONLINE
	configurarEthernet()

	conectarMQTT()
	#endif
}

void configurarEthernet() {
	if(!Ethernet.begin(mac)) {
		Serial.println("Falha no DHCP");
	};
}

int conectarMQTT() {
	return client.connect(MQTT_CLIENT_ID);
}

void atualizarDistancia(int distancia) {
	if(distancia < DISTANCIA_MINIMA) {
		avisarOcupada(1);
	} else {
		avisarOcupada(0);
	}
}

void avisarOcupada(int ocupada) {
	if(ocupada) {
		digitalWrite(PIN_OCUPADO, HIGH);
	} else {
		digitalWrite(PIN_OCUPADO, LOW);
	}
	publicarOcupadaMQTT(ocupada);
}

void publicarOcupadaMQTT(int ocupada) {
	if (ocupada) {
		client.publish(topic, "1", true);
	} else {
		client.publish(topic, "0", true);
	}
}

// Variavel global para guardar o tempo da última tentativa de conexão
unsigned long ultimaTentativaReconectar = 0;
int checkReconectarMQTT() {
	if(!client.connected()) {
		long agora = millis();
		if(agora - ultimaTentativaReconectar > 5000) {
			Serial.println("reconectando...");
			return conectarMQTT();
		}
		ultimaTentativaReconectar = agora;
	}
}

void loop() {
	checkReconectarMQTT();

	int distancia = ultrasonic.distanceRead();
	atualizarDistancia(distancia);

	// Chamada de método requirido pela biblioteca MQTT
	// ele é necessário para que ela tenha uma oportunidade de processar envios e recebimentos de respostas
	client.loop();

	// intervalo arbitrário entre atualizações
	delay(1500);
}
