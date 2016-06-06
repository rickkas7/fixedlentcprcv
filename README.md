# fixedlentcprcv
*Particle Photon sample code for reading from a TCPClient fixed length messages*

I created a simple code sample in reference to this [Particle Community Forum post] (https://community.particle.io/t/tcpclient-available-read-methods-supported-scenario/23424/).

The Photon main code is in fixedlentcprcv.cpp.

The class to easily handle fixed length messages in a TCPClient stream is in FixedLengthMsgClient.cpp and .h. The .h file contains comments that explain the methods.

There's also a test server for node.js in server.js. The server exercises a bunch of things including:

* Sending message one a time
* Sending a bunch of messages (1-20) at once
* Sending a message a byte at a time

This should exercise the functionality of the message class.

Note that this should not be used with serial; it assumes that the transport mechanism is reliable and has no way to resynchronzing the start of messages as would be required for serial.

https://github.com/rickkas7/fixedlentcprcv

