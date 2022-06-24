const dgram  = require('dgram');
const client = dgram.createSocket('udp4');

const io = require('socket.io')(2505, {
    cors: {
            origin: ["CLIENT_IP:CLIENT_PORT"],
    },
});

io.on('connection', (socket) => {
    console.log("User connected: " + socket.id)
    client.on('message', (msg, rinfo) => {
        let gpgga = msg.toString().split(',');
        
        let bus             = gpgga[0];
        let message         = gpgga[1];  
        let time            = gpgga[2];
        let lattitude       = gpgga[3];
        let lat_dir         = gpgga[4];
        let longitude       = gpgga[5];
        let lng_dir         = gpgga[6];
        let satelite_num    = gpgga[7];
        let precision       = gpgga[8];
        let altitude        = gpgga[9];
    
        console.log('From: ');
        console.log(rinfo.address);
        console.log({bus, lattitude, longitude, time, altitude});
        socket.emit('message', {bus, lattitude, longitude, time, altitude});
    });
})


client.bind(3333);