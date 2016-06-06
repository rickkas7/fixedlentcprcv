#ifndef __FIXEDLENGTHMSGCLIENT_H
#define __FIXEDLENGTHMSGCLIENT_H

/**
 * Parser for TCP data sent in fixed length messages from a server to a TCPClient
 */
class FixedLengthMsgClient {
public:
	/**
	 * Initialize the object with the fixed size of the message in bytes
	 */
	FixedLengthMsgClient(size_t msgSize);

	virtual ~FixedLengthMsgClient();

	/**
	 * When you make a new connection with TCPClient, call start() to start reading from it
	 */
	void start(TCPClient *client);

	/**
	 * Call parse repeatedly out of loop to check for data. It returns one of the MSG_*
	 * constants defined below.
	 *
	 * MSG_READY means you should handle the data. Use getMsg() to get a pointer to the buffer
	 * where the data is. Note that this data will be overwritten on the next call to parse()
	 * so be sure to copy it if you need to save it.
	 */
	int parse(void);

	/**
	 * Called when you're done parsing the data, usually before you close the connection.
	 */
	void done();

	/**
	 * Convenience method that returns the data as a string of hex bytes (typically used for
	 * debugging binary data to the Serial console)
	 */
	String toHexString();

	/**
	 * Returns the msgSize passed when FixedLengthMsgClient was constructed
	 */
	size_t getMsgSize() const { return msgSize; }

	/**
	 * Returns a pointer to the current message. Valid only if parse() returns MSG_READY and
	 * is overwritten on the next call to parse().
	 */
	byte *getMsg() const { return msg; }

	/**
	 * Returns true if this object believes that reads are still active on the object;
	 * it has been started with start() and the TCP connection is still connected.
	 */
	bool isActive() const { return (client != NULL); }

	static const int MSG_READY = 0;
	static const int MSG_NONE = 1;
	static const int MSG_PARTIAL = 2;
	static const int MSG_ERROR = -1;
	static const int MSG_DISCONNECTED = -2;

private:
	TCPClient *client = NULL;
	size_t msgSize;
	byte *msg;
	size_t msgOffset;
};

#endif /* __FIXEDLENGTHMSGCLIENT_H */
