
1. 数据库采用Mysql， 端口3060
账号root 密码admin
2. c++ 服务器， 端口3070
3. 中台nodejs服务器，前方http接口，后方TCP连接c++服务器
数据请求api接口
分时：localhost/:mtype(m1|m15|m30|m45|m60)/:index?number=xxx
日K：

当前端接到请求的股票数据，向api
日k线
https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh[股票代码],day,[开始日期],[结束日期 yyyy-m-d],[条数],qfq

分钟k线
https://ifzq.gtimg.cn/appstock/app/kline/mkline?param=[sh+股票代码],[m1, m5, m15, m30, m60],[],[int取回数目]&_var=[日期 yyyy-m-d]

分时线
https://web.ifzq.gtimg.cn/appstock/app/minute/query?code=[sh+股票代码]
请求数据

得到数据格式
"202203111400",//日期
"1730.11",//结束价
"1752.00",//开始价
"1756.82",//最高价
"1718.08",//最低价
"10677.85",//
{},
"8.5001"//换手率

前端API
1.分钟k
localhost/:mtype(m1|m15|m30|m60)/:index?number=xxx&date=YYYY-MM-DD
例：http://192.168.2.212:3080/m15/600519?number=300&date=2022-3-8
？后parameter可以不传
number默认30
date默认今天

2.k线
localhost/:daytype(day1|day7|day30|day365)/:index?startday=YYYY-MM-DD&endday=YYYY-MM-DD&number=30
例：http://192.168.2.212:3080/day1/600519?endday=2022-03-09&number=30
？后parameter可以不传
number默认60
startday为空
endday默认今天
开始日期和结束日期为同一天时无返回数据W

1. 分时数据
http://192.168.2.212:3080/min/:index
例：http://192.168.2.212:3080/min/600519

得到数据后一边将数据返回给前端，另一边传入c++服务器存入数据库

交互json格式
{
    "code":200|503,
    "data":{
        "points":[...],
        "details":{
            "index":"321654",
            "type":"m1|m5|m15|m30|m60",
            "market":"xxxx",
            "name":"xxx"
        }
    },
    "msg":""
}

