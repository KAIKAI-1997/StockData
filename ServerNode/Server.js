function getIPAdress() {
    let interfaces = require('os').networkInterfaces();
    for (var devName in interfaces) {
        var iface = interfaces[devName];
        for (var i = 0; i < iface.length; i++) {
            let alias = iface[i];
            if (alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal) {
                // console.log(alias.address);

                return alias.address
            }
        }
    }
}
//————————————————
//版权声明：本文为CSDN博主「佳宝儿」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/weixin_43653175/article/details/84561603 

console.log(getIPAdress())

var net = require('net')
var express = require('express')
const cors = require('cors')

const router = require('./API')

const host = getIPAdress()
const port_node_client = 3070
const port_node_server = 3080

var buf = "not good"

var app = express()

//解决跨域
app.use(cors())

app.use(router)

app.listen(port_node_server, host, function() { 
    console.log('server is listening');
});






