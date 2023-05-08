// Importing the required modules
const WebSocketServer = require('ws');
const { GlobalKeyboardListener } = require('node-global-key-listener')
const robot = require("robotjs");
const getServerResponseTime = require('get-server-response-time')
const ping = require('ping');
 
// Creating a new websocket server
const wss = new WebSocketServer.Server({ port: 80 })
const key = new GlobalKeyboardListener();

const keyToPressStun = process.argv.slice(2)[0].toUpperCase();
const keyToPressUlt = process.argv.slice(2)[1].toUpperCase();
const keyToEnable = process.argv.slice(2)[2].toUpperCase();
const delay = process.argv.slice(2)[3];

const ALPHA_TIME = 1089;

var lastPressStun = 0;
var lastPressUlt = 0;

var sTime = 0;

var enabled = true;

var leaguePing = 25;
var clientPing = 25;

var ip = "";


function getPings() {
    getLeaguePing();
    getSocketPing();
}

function getLeaguePing() {

    ping.promise.probe('104.160.131.102', {
    }).then(function (res) {
        console.log("league ping " + res.avg);
        leaguePing = res.avg;
    });
}

function getSocketPing() {
    ping.promise.probe(ip, {
    }).then(function (res) {
        clientPing = res.avg;
    });
}
 
// Creating connection using websocket
wss.on("connection", (ws,req) => {
    console.log("new client connected");
    // sending message
    ip = ws._socket.remoteAddress.replace(/^.*:/, '')

    setInterval(getPings, delay)
    
    ws.on("message", data => {
        
    });
    // handling what to do when clients disconnects from server
    ws.on("close", () => {
        console.log("the client has connected");
    });
    // handling client connection error
    ws.onerror = function () {
        console.log("Some Error occurred")
    }

    key.addListener(function (e, down) {
        if (e.name == keyToEnable && e.state == "DOWN") 
        {
            enabled = !enabled;
            console.log("macro enabled: " + enabled);
            return false;
        }
        if (e.name == keyToPressStun && e.state == "DOWN" && enabled)
        {
           console.log("Sending stun key event");
           lastPressStun = Date.now();
           setTimeout(() => {
              ws.send(JSON.stringify({type:'keyRequestStun'}))
           }, ALPHA_TIME + Math.max(leaguePing, clientPing))                        
           return false;
        }
        
        if (e.name == keyToPressUlt && e.state == "DOWN" && enabled)
        {
           console.log("Sending ult key event");
           lastPressUlt = Date.now();    
           ws.send(JSON.stringify({type:'keyRequestUlt'}))          
           return false;
        }
    });
});
console.log("The server is running on port 80");