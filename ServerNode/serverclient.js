var net = require('net')

const host = "192.168.2.212"
const port_node_client = 3070
const port_node_server = 3080

//client server
var Client_node = new net.Socket()


Client_node.on('end', () => {
    console.log('disconnected from server');
});

Client_node.on('error', () => {
    console.log('c++ server goes wrong');
});


Client_node.connect(port_node_client, host, ()=>{
    //Client_node.write('give me data')
    console.log('c++server connect success')
})

module.exports.client = Client_node
