    

var net = require('net')
import routers from "/API"

const host = "192.168.2.212"
const port_node_client = 3070
const port_node_server = 3080

var buf = "not good"

//绑定node server事件
var server_node = net.createServer(function(connection) { 


    console.log('client connected')
    connection.write('connected!')

    connection.on('end', function() {
        server_node.close()
        console.log('客户端关闭连接')
    })

    connection.on('data', (data)=>{
        Client_node.write(data.toString())
        console.log('fetching')
        })
        
        Client_node.on('data', (data)=>{
            console.log(data.toString())
            buf = data.toString()
            connection.write(data.toString())
        })
    //connection.pipe(connection)
})


//只有在接收到数据时触发
//绑定nodeclient事件
//接收到c++server data后，返回给client

var Client_node = new net.Socket()


Client_node.on('end', () => {
    console.log('disconnected from server');
});


Client_node.connect(port_node_client, host, (req, res)=>{
    //Client_node.write('give me data')
    console.log('c++server connect success')
})

//启动连接
server_node.listen(port_node_server, host, function() { 
    console.log('server is listening');
});





