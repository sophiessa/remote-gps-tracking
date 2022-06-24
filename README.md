## Description
In this project you can find how to make your own _GPS_ tracker that can be monitored over the internet

### Languages used
- [Javascript](https://www.javascript.com/)
- [Dart](https://dart.dev/)
- [C++](https://en.wikipedia.org/wiki/C%2B%2B)
### Other things used
- [Arduino IDE](https://www.arduino.cc/)
- [Flutter](https://flutter.dev/)
- [Node](https://nodejs.org/en/)
-	[Socket.IO](https://socket.io/)
- [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol)
- [2GIS](https://info.2gis.com/)
- [Google Maps](https://www.google.com/maps)

### Server side
To be able to use server side code you have to install dependencies. 
Either run `npm install` inside the server folder, or `npm install dgram` and `npm install socket.io` in your own directory.

**dgram** will let us receive UDP messages from a device and **socket.io** will allow us to send data to the users over TCP.

Open _server/udp-server.js_ to see how to use these libraries.

### Client side
#### Web
To use free api of 2GIS you just need to serve the _client/web/free-2gis.html_ and add its ip in your server web socket.

For google maps you have to acquire an API and place it in an appropriate place that can be found in the code _client/web/paid-google.html_.

#### Mobile
Mobile application for this project was created using _Flutter_. You can find how to integrate google maps and socket.io inside _client/mobile/main.dart_.

### Device side
#### Hardware 
- Arduino Pro Mini
- SIM800L
- NEO-6M

#### Software
- Arduino IDE
- UART
