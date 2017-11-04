const mosca = require('mosca');
const path = require('path');

const mqttServer = new mosca.Server({
  port: 1883,
  http: {
    port: 8080,
    static: './public/'
  }
});

mqttServer.on('ready', () => console.log('server ready'));
mqttServer.on('published', (packet, client) => {
  console.log('publish', packet);
  console.log('client', client);
});
