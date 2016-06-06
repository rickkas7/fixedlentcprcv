#include "Particle.h"

#include "FixedLengthMsgClient.h"

SYSTEM_THREAD(ENABLED);

FixedLengthMsgClient fixedMsg(32);
TCPClient client;
IPAddress serverAddr(192,168,2,4);
const int serverPort = 7123;
const unsigned long reconnectDelay = 10000;

void setup() {
	Serial.begin(9600);
}

void loop() {
	if (!fixedMsg.isActive()) {
		if (client.connect(serverAddr, serverPort)) {
			Serial.println("connected");
			fixedMsg.start(&client);
		}
		else {
			Serial.println("connection failed");
			delay(reconnectDelay);
		}
	}
	else {
		int result = fixedMsg.parse();
		if (result == FixedLengthMsgClient::MSG_ERROR) {
			Serial.printlnf("parse returned error %d, closing connection", result);
			client.stop();
		}
		else
		if (result == FixedLengthMsgClient::MSG_READY) {
			Serial.printlnf("got message %s", fixedMsg.toHexString().c_str());
		}
	}
}
