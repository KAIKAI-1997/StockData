const fetch = require("./API")

function wait(){
    console.log("waiting")
    return "1"
}

async function main(){
    const fetch = require("./FETCH")
    var abc = await fetch.fetch("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=sh600519,day,2022-03-23,2022-03-24,2700,qfq")
    var c = wait()
    var i = "asd"
    console.log(abc)
    console.log(i)
    console.log(c)
}

main()