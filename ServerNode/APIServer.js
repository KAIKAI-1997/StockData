const express = require('express')

const router = express.Router()

router.get('/get', (req, res)=>{
    const query = req.query

    res.send({
        status: 0,
        msg: '请求成功',
        data: query
    })

})

router.post('/post', (req, res)=>{
    const body = req.body

    res.send({
        status:0,
        msg:'POST请求成功',
        data:body
    })

})

module.exports = router

// router.listen(80, (req, res)=>{
//     console.log('server running at http://192.168.2.212')
// })