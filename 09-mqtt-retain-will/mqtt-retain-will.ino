#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

const int LED_PIN = 2;

// Atualizar ultimo valor para ID do seu Kit para evitar duplicatas
const byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x42 };

const char* server = "test.mosquitto.org";
const int port = 1883
const char* clientId = "arduino-codexp-42";

const char* topic="senai/codexp/42/luz";


void callback(char* topic, byte* payload, unsigned int length) {
	int payloadAsInt = payload[0] - '0';
	if (payloadAsInt) {
		digitalWrite(LED_PIN, HIGH);
		client.publish(topic, "1", true);
	} else {
		digitalWrite(LED_PIN, LOW);
		client.publish(topic, "0", true);
	}
}

EthernetClient ethernetClient;
PubSubClient client(server, port, callback, ethernetClient);

void mqttConnect() {
	if(client.connect(clientId, topic, 0, true, "")) {
		client.subscribe(topic);
	}
	return client.connected();
}

void setup() {
	Serial.begin(9600);
	while (!Serial) {}

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP request failed, trying fixed IP");
    Ethernet.begin(mac, ip);
    Serial.println("Fixed IP:" + String(Ethernet.localIP()));
  } else {
    Serial.print("IP address granted: ");
    Serial.println(Ethernet.localIP());
	}

	pinMode(LED_PIN, OUTPUT);
	mqttConnect();
}

void loop() {
	if(!client.connected()) {
		mqttConnect();
	}
	client.loop();
}
