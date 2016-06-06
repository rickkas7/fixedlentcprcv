#include "Particle.h"

#include "FixedLengthMsgClient.h"

// Method descriptions are in the .h file

FixedLengthMsgClient::FixedLengthMsgClient(size_t msgSize) : msgSize(msgSize) {
	msg = (byte *)malloc(msgSize);
}

FixedLengthMsgClient::~FixedLengthMsgClient() {
	if (msg != NULL) {
		free(msg);
	}
}

void FixedLengthMsgClient::start(TCPClient *client) {
	this->client = client;
	msgOffset = 0;
}

int FixedLengthMsgClient::parse(void) {
	if (client == NULL) {
		return MSG_DISCONNECTED;
	}

	if (client->connected()) {
		int count = client->read(&msg[msgOffset], msgSize - msgOffset);
		if (count <= 0) {
			// No new data
			return (msgOffset == 0) ? MSG_NONE : MSG_PARTIAL;
		}

		// New data arrived
		msgOffset += (size_t) count;
		if (msgOffset < msgSize) {
			// Still not a full message
			// Serial.printlnf("partial offset=%d size=%d", msgOffset, msgSize);
			return MSG_PARTIAL;
		}

		// Got a full message
		msgOffset = 0;
		return MSG_READY;
	}
	else {
		client = NULL;
		return MSG_DISCONNECTED;
	}
}

void FixedLengthMsgClient::done() {
	client = NULL;
}

String FixedLengthMsgClient::toHexString() {
	String result;
	result.reserve(msgSize * 2);

	for(size_t ii = 0; ii < msgSize; ii++) {
		char hexBuf[4];
		snprintf(hexBuf, sizeof(hexBuf), "%02x", msg[ii]);
		result += hexBuf;
	}

	return result;
}



