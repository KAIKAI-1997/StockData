const express = require('express')
//const fetch = require('./FETCH')
var https = require("https")
var iconv = require("iconv-lite")
var client = require('./serverclient').client
var encoding = require("encoding")

const router = express.Router()
var URL=""

//获取今天函数
Date.prototype.Format = function(fmt) { //author: meizz 
    var o = {
        "M+": this.getMonth() + 1, //月份 
        "d+": this.getDate(), //日 
        "h+": this.getHours(), //小时 
        "m+": this.getMinutes(), //分 
        "s+": this.getSeconds(), //秒 
        "S": this.getMilliseconds() //毫秒 
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}

function getToday() {
    return new Date().Format("yyyy-MM-dd")
}

function getYestoday() {
    return new Date(new Date().getTime() - 1000 * 60 * 60 * 24).Format("yyyy-MM-dd")
}

//https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh[股票代码],day,[开始日期],[结束日期 yyyy-m-d],[条数],qfq
//api接口localhost/day/:index?startday=YYYY-MM-DD&endday=YYYY-MM-DD&number=30
router.get('/:daytype(day1|day7|day30|day365)/:index', (req, res)=>{
    console.log("connected from " + req.ip + ":"+req.socket.remotePort + " query " + req.headers.host + req.url)

    var number=300
    var data_final = []
    var daytype

    if(req.params.daytype=='day1'){
        daytype=1
    }
    else if(req.params.daytype=='day7'){
        daytype=7
    }
    else if(req.params.daytype=='day30'){
        daytype=30
    }
    else{
        daytype=365
    }

    //创建默认值
    if(req.query.startday)
        startday = req.query.startday
    else
        startday = ""


    if(req.query.endday){
        endday = req.query.endday
    }
    else{    
        endday = getToday()
    }

    if(req.query.number){
        number = req.query.number * daytype
    }
    else{    
        number = daytype*60
    }

    //拼接url
    URL = `https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh${req.params.index},day,${startday},${endday},${number},qfq`
    console.log(URL)
    https.get(URL, function (res2) {  
        var datas = [];  
        var size = 0;  

        //从外部PAI接收数据
        res2.on('data', function (data) {  
            datas.push(data);  
            size += data.length;  
        //process.stdout.write(data);  
        });

        //数据接收完毕后向前端发送json数据  
        res2.on("end", function () {  
            const buff = Buffer.concat(datas, size);  
            var result = iconv.decode(buff, "utf8");//转码//var result = buff.toString();//不需要转编码,直接tostring  
            //console.log("fetching from "+URL)
            
            result= JSON.parse(result)

            //no data response
            if(result["msg"]=="param error" || result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index].length==0){
                var error_return = {
                    "code":503,
                    "data":{},
                    "msg":"param error"
                }
                res.send(error_return)
                return ;
            }

            if(daytype==1){
                data_final = result["data"]["sh"+req.params.index]["qfqday"]
            }
            else if(daytype==7){
                var temp = new Array()
                temp = result["data"]["sh"+req.params.index]["qfqday"]

                for(var i=0; i<Math.floor(temp.length/7); i++){
                    var max_temp=-1, min_temp=999999, dealsum=0
                    for(var j=0; j<7; j++){
                        max_temp = max_temp>temp[i*7+j][3] ? max_temp:temp[i*7+j][3];
                        min_temp = min_temp<temp[i*7+j][4] ? min_temp:temp[i*7+j][4];
                        dealsum=parseFloat(temp[i*7+j][5])
                    }
                    
                    data_final.push([temp[i*7][0], temp[i*7][1], temp[i*7+6][2],  max_temp, min_temp,  ""+dealsum])
                }

            }
            else if(daytype==30){
                var temp = new Array()
                temp = result["data"]["sh"+req.params.index]["qfqday"]

                for(var i=0; i<Math.floor(temp.length/30); i++){
                    var max_temp=-1, min_temp=999999, dealsum=0
                    for(var j=0; j<30; j++){
                        max_temp = max_temp>temp[i*30+j][3] ? max_temp:temp[i*30+j][3];
                        min_temp = min_temp<temp[i*30+j][4] ? min_temp:temp[i*30+j][4];
                        dealsum=parseFloat(temp[i*30+j][5])
                    }
                    
                    data_final.push([temp[i*30][0], temp[i*30][1], temp[i*30+6][2],  max_temp, min_temp,  ""+dealsum])
                }
            }
            else{
                
                var temp = new Array()
                temp = result["data"]["sh"+req.params.index]["qfqday"]
                for(var i=0; i<Math.floor(temp.length/365); i++){
                    var max_temp=-1, min_temp=999999, dealsum=0
                    for(var j=0; j<365; j++){
                        max_temp = max_temp>temp[i*365+j][3] ? max_temp:temp[i*365+j][3];
                        min_temp = min_temp<temp[i*365+j][4] ? min_temp:temp[i*365+j][4];
                        dealsum=parseFloat(temp[i*365+j][5])
                    }
                    
                    data_final.push([temp[i*365][0], temp[i*365][1], temp[i*365+6][2],  max_temp, min_temp,  ""+dealsum])
                }
            }
            
            var front_return = {
                "code":200,
                "data":{
                    "points":data_final,
                    "details":{
                        "index":req.params.index,
                        "type":req.params.daytype,
                        "market":result["data"]["sh"+req.params.index]["qt"]["market"][0],
                        "others":result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index]
                    }
                },
                "msg":""
            }

            var end_fetch = {
                "code":200,
                "data":{
                    "aim":"insert",
                    "points":result["data"]["sh"+req.params.index]["qfqday"],
                    "details":{
                        "index":req.params.index,
                        "type":req.params.daytype,
                        "market":result["data"]["sh"+req.params.index]["qt"]["market"][0],
                        "name":result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index][1]
                    }
                },
                "msg":""
            }

            client.write(JSON.stringify(end_fetch)+"\n")
            res.send(front_return)
        });  
    }).on("error", function (err) {  
        console.log(err.message)
        var error_return = {
            "code":503,
            "data":{},
            "msg":""
        }
        res.send(error_return)
    }); 
})

//https://ifzq.gtimg.cn/appstock/app/kline/mkline?param=[sh+股票代码],[m1, m5, m15, m30, m60],[],[int取回数目]&_var=[日期 yyyy-m-d]
//api接口localhost/:mtype(m1|m15|m30|m45|m60)/:index?number=xxx&date=xxx
router.get('/:mtype(m1|m5|m15|m30|m60)/:index', (req, res)=>{
    console.log("connected from " + req.ip + ":"+req.socket.remotePort + " query " + req.headers.host + req.url+"\n")

    var number=300
    var date = `2022-3-8`

    //默认30条数据
    if(req.query.number)
        number = req.query.number
    else
        number = 30

    //默认2022-3-8
    if(req.query.date){
        date = req.query.date
    }
    else
        date = getToday()

    URL = `https://ifzq.gtimg.cn/appstock/app/kline/mkline?param=sh${req.params.index},${req.params.mtype},,${number}&_var=${date}`
    console.log(URL)
    https.get(URL, function (res2) {  
        var datas = [];  
        var size = 0;  

        //从外部PAI接收数据
        res2.on('data', function (data) {  
            datas.push(data);  
            size += data.length;  
        //process.stdout.write(data);  
        });

        //数据接收完毕后向前端发送json数据  
        res2.on("end", function () {  
            const buff = Buffer.concat(datas, size);  
            var result = iconv.decode(buff, "utf8");//转码//var result = buff.toString();//不需要转编码,直接tostring  
            //console.log("fetching "+URL)
            

            result= JSON.parse(result)

            //no data response
            if(result["msg"]=="param error" || result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index].length==0){
                var error_return = {
                    "code":503,
                    "data":{},
                    "msg":"param error"
                }
                res.send(error_return)
                return ;
            }


            //set communicate json content
            var front_return = {
                "code":200,
                "data":{
                    "points":result["data"]["sh"+req.params.index][""+req.params.mtype],
                    "details":{
                        "index":req.params.index,
                        "type":req.params.mtype,
                        "market":result["data"]["sh"+req.params.index]["qt"]["market"][0],
                        "others":result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index]
                    }
                },
                "msg":""
            }

            var end_fetch = {
                "code":200,
                "data":{
                    "aim":"insert",
                    "points":result["data"]["sh"+req.params.index][""+req.params.mtype],
                    "details":{
                        "index":req.params.index,
                        "type":req.params.mtype,
                        "market":result["data"]["sh"+req.params.index]["qt"]["market"][0],
                        "name":result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index][1]
                    }
                },
                "msg":""
            }

            //加入换行符表示传输结束
            client.write(JSON.stringify(end_fetch)+"\n")
            res.send(front_return)
        });  
    }).on("error", function (err) {  
        console.log(err.message)
        var front_return = {
            "code":503,
            "data":{},
            "msg":""
        }
        res.send(front_return)
    }); 
})

//https://web.ifzq.gtimg.cn/appstock/app/minute/query?code=[sh+股票代码]
//api接口localhost/min/:index
router.get('/min/:index', (req, res)=>{
    console.log("connected from " + req.ip + ":"+req.socket.remotePort + " query " + req.headers.host + req.url)

    URL = `https://web.ifzq.gtimg.cn/appstock/app/minute/query?code=sh${req.params.index}`
    console.log(URL)

    https.get(URL, function (res2) {  
        var datas = [];  
        var size = 0;  

        //从外部PAI接收数据
        res2.on('data', function (data) {  
            datas.push(data);  
            size += data.length;  
        //process.stdout.write(data);  
        });

        //数据接收完毕后向前端发送json数据  
        res2.on("end", function () {  
            const buff = Buffer.concat(datas, size);  
            var result = iconv.decode(buff, "utf8");//转码//var result = buff.toString();//不需要转编码,直接tostring  
            //console.log("fetching "+URL)
            

            result= JSON.parse(result)

            //no data response
            if(result["msg"]=="code param error"){
                var error_return = {
                    "code":503,
                    "data":{},
                    "msg":"param error"
                }
                res.send(error_return)
                return ;
            }

            //set communicate json content
            var front_return = {
                "code":200,
                "data":{
                    "points":result["data"]["sh"+req.params.index]["data"],
                    "details":{
                        "index":req.params.index,
                        "type":req.params.mtype,
                        "market":result["data"]["sh"+req.params.index]["qt"]["market"][0],
                        "others":result["data"]["sh"+req.params.index]["qt"]["sh"+req.params.index]
                    }
                },
                "msg":""
            }
            //client.write(JSON.stringify(front_return))
            res.send(front_return)
        });  
    }).on("error", function (err) {  
        console.log(err.message)
        var front_return = {
            "code":503,
            "data":{},
            "msg":""
        }
        res.send(front_return)
    }); 
})




module.exports= router

// const app = express()

// app.use('/', router)

// app.listen(80, (req, res)=>{
//     console.log('server running at http://192.168.2.212')
// })
