const fs = require('fs');
const path = require('path');
const url = require('url');
const express = require('express');
const bodyParser = require('body-parser');
const cookieParser = require('cookie-parser');
const chalk = require('chalk');
const app = express();
function FormatDate(epoch) {
    let d = (epoch === undefined) ? new Date() : new Date(epoch)
    let s = ''
    const pad = (n, t = '') => { return n < 10 ? `0${n}${t}` : `${n}${t}` }
    s += pad(d.getMonth() + 1, '/')
    s += pad(d.getDate(), ' ')
    s += pad(d.getHours(), ':')
    s += pad(d.getMinutes(), ':')
    s += pad(d.getSeconds())
    return s
}

function log(req, res, next) {
    console.log()
    console.log(chalk.green('------------'))
    console.log(chalk.white.bgGreen(`${req.protocol.toUpperCase()} ${req.method.toUpperCase()} ${req.path}`))
    const unixdate = new Date().getTime();
    console.log(chalk.green(FormatDate(unixdate)))
    let qs = url.parse(req.url).query;
    qs = qs ? qs.split('&') : [];
    for (const s of qs) console.log(chalk.green(` o ${s}`))
    console.log(chalk.white('headers:'))
    console.log(chalk.white(JSON.stringify(req.headers, null, 4)))
    console.log(chalk.grey('cookies:'))
    console.log(chalk.grey(JSON.stringify(req.cookies, null, 4)))
    console.log(chalk.yellow('body:'))
    console.log(chalk.yellow(JSON.stringify(req.body, null, 2)))
    console.log(chalk.green('----------'))
    next()
}

app.use(bodyParser.json())
app.use(log);
// chrome test
app.get('/json', function (req, res) {
    const body = fs.readFileSync(path.join(__dirname, 'test.json'))
    res.setHeader('Content-Type', 'application/json');
    res.send(body)
})
// chrome test
app.get('/txt', function (req, res) {
    const body = fs.readFileSync(path.join(__dirname, 'test.txt'))
    res.setHeader('Content-Type', 'text/plain');
    res.send(body)
})
// curl -v localhost:8081/err
app.get('/err', function (req, res) {
    res.setHeader('Content-Type', 'application/json')
    res.status(500)
    res.send({ response: "hello" })
})

// curl --header "Content-Type: application/json" --data "{\"username\":\"xyz\"}" localhost:8081/api --request GET
app.get('/api', function (req, res) {
    res.setHeader('Content-Type', 'application/json')
    res.status(200)
    res.send({ response: "hello GET" })
})
// curl --header "Content-Type: application/json" --data "{\"username\":\"xyz\"}" localhost:8081/api --request POST
app.post('/api', function (req, res) {
    res.setHeader('Content-Type', 'application/json')
    res.status(200)
    res.send({ response: "hello POST" })
})
// curl --header "Content-Type: application/json" --data "{\"username\":\"xyz\"}" localhost:8081/api --request PUT
app.put('/api', function (req, res) {
    res.setHeader('Content-Type', 'application/json')
    res.status(200)
    res.send({ response: "hello PUT" })
})
// curl --header "Content-Type: application/json" --data "{\"username\":\"xyz\"}" localhost:8081/api --request PATCH
app.patch('/api', function (req, res) {
    res.setHeader('Content-Type', 'application/json')
    res.status(200)
    res.send({ response: "hello PATCH" })
})
// curl --header "Content-Type: application/json" --data "{\"username\":\"xyz\"}" localhost:8081/api --request DELETE
app.delete('/api', function (req, res) {
    res.setHeader('Content-Type', 'application/json')
    res.status(200)
    res.send({ response: "hello DELETE" })
})

var server = app.listen(8081, function () {
   var host = server.address().address
   var port = server.address().port
   console.log("listening at http://%s:%s", host, port)
})