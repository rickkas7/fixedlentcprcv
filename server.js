// Run this like:
// node server.js
var net = require('net');

// dataPort is the TCP port we listen on from the Photon. This value is encoded in the Photon code
var dataPort = 7123;

// Size of the fixed-length messages to send
var msgSize = 32;

// Sequence number so it's easy to tell 
var seq = 0;

net.createServer(function (socket) {
	console.log('data connection started from ' + socket.remoteAddress);
	
	// The server sends a 8-bit byte value for each sample. Javascript doesn't really like
	// binary values, so we use setEncoding to read each byte of a data as 2 hex digits instead.
	socket.setEncoding('hex');

	var handler = new SocketHandler(socket);
	handler.run();
	
	socket.on('data', function (data) {
		console.log("got " + count + " bytes " + data.substr(0, 64));
	});
	socket.on('end', function () {
		console.log('data connection ended');
		handler.stop();
	});
	socket.on('error', function (err) {
		console.log('data connection error', err);
		handler.stop();
	});
}).listen(dataPort);


console.log('listening on port ' + dataPort + ' for data');


function FixedMessage(length) {
	this.msgLength = length;
	this.msg = new Buffer(length);
	
	this.generateData = function() {
		// First 4 bytes are the sequence number
		this.msg.writeInt32BE(seq, 0);
		
		// Then random bytes afterwards
		for(var ii = 4; ii < this.msgLength; ii++) {
			var r = Math.floor(Math.random() * 256);
			this.msg.writeUInt8(r, ii);
		}
		console.log("buffer seq=" + seq + " " + this.msg.toString("hex"));
		seq++;
	};
};

function SocketHandler(socket) {
	this.buffers = new Array();
	this.socket = socket;
	
	this.run = function() {
		if (this.buffers.length == 0) {
			this.generateData();
		}
		var elem = this.buffers.shift();
		if (elem) {
			// console.log("sending " + elem.b.length + " bytes");
			this.socket.write(elem.b);
			
			var timerThis = this;
			this.timer = setTimeout(function() {
				timerThis.run();
			}, elem.d);
		}
	};
	
	this.stop = function() {
		if (this.timer) {
			clearTimeout(this.timer);
		}
		this.buffers = new Array();
	};
	
	this.generateData = function() {
		// Randomly generate scenarios
		
		var mode = Math.random();
		if (mode < 0.3) {
			// sending data one byte at a time, random delay 0-100 ms.
			console.log("generating message split into single bytes");
			var msg = new FixedMessage(msgSize);
			msg.generateData();
			
			for(var ii = 0; ii < msgSize; ii++) {
				var b = new Buffer(1);
				b.writeUInt8(msg.msg.readUInt8(ii), 0);
			
				var delay = Math.floor(Math.random() * 10) + 1;
				this.buffers.push({'b':b,'d':delay});
			}
		}
		else
		if (mode < 0.5) {
			// sending multiple blocks (1-20) in one transmission, random delay 0 - 5 s.
			var delay = Math.floor(Math.random() * 5000) + 1;
			var numBlocks = Math.floor(Math.random() * 20) + 1;
			console.log("generating " + numBlocks + " in a single write delay=" + delay);

			var tempArray = new Array();
			for(var ii = 0; ii < numBlocks; ii++) {
				var msg = new FixedMessage(msgSize);
				msg.generateData();
				
				tempArray.push(msg.msg);
			}
			this.buffers.push({'b':Buffer.concat(tempArray), 'd':delay});
			
		}
		else {
			// sending full block, random delay 0 - 5 s.
			var delay = Math.floor(Math.random() * 5000) + 1;
			console.log("generating single message delay=" + delay);

			var msg = new FixedMessage(msgSize);
			msg.generateData();

			this.buffers.push({'b':msg.msg,'d':delay});
		}
	};
};



