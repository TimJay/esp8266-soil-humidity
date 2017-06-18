#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define PIN 14

ADC_MODE(ADC_VCC);

const char* ssid = "XXXXXX";
const char* password = "XXXXXX";
const char* server = "XXXXXX";
const String topic_prefix = "smartgarden/";
String topic_humidity = "/humidity";
String topic_voltage = "/voltage";

WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient);

void setup() {
	Serial.begin(115200);
	delay(10);
	Serial.println("Starting ...");

	WiFi.begin(ssid, password);
	Serial.print("Connecting ");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println(". connected");
	topic_humidity = topic_prefix + String(ESP.getChipId()) + topic_humidity;
	topic_voltage = topic_prefix + String(ESP.getChipId()) + topic_voltage;
	if (!client.connect((char*) String(ESP.getChipId()).c_str())) {
		abort();
	}
}

void loop() {
	pinMode(PIN, OUTPUT);
	digitalWrite(PIN, LOW);
	delay(100);
	pinMode(PIN, INPUT_PULLUP);
	uint32_t start = ESP.getCycleCount();
	while (!digitalRead(PIN)) {
	}
	String payload_humidity = String(ESP.getCycleCount() - start);
	String payload_voltage = String(ESP.getVcc());
	if (client.connected()) {
		client.publish((char*) topic_humidity.c_str(),
				(char*) payload_humidity.c_str());
		client.publish((char*) topic_voltage.c_str(),
				(char*) payload_voltage.c_str());
	}
	Serial.print("Sent data - humidity ");
	Serial.print(payload_humidity);
	Serial.print(" voltage ");
	Serial.println(payload_voltage);
	ESP.deepSleep(900000000, WAKE_RF_DEFAULT);
}
