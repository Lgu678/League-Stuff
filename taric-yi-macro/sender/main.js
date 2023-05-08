const { WebSocket } = require('ws');
const { GlobalKeyboardListener } = require('node-global-key-listener')
const robot = require("robotjs");
const key = new GlobalKeyboardListener();
const ws = new WebSocket("ws://"+process.argv.slice(2)[0]+":80");
const keyToEnable = process.argv.slice(2)[1].toUpperCase();
const keyToEnableUlt = process.argv.slice(2)[2].toUpperCase();
var enabled = true;
var enabledUlt = true;
var wsping = 0;
var leagueping = 29;
var lastPress = 0;

ws.on('open', function open() {
  console.log ("connected");

  key.addListener(function (e, down) {
        if (e.name == keyToEnable && e.state == "DOWN") 
        {
            enabled = !enabled;
            console.log("macro enabled: " + enabled);
            return false;
        }

        if (e.name == keyToEnableUlt && e.state == "DOWN") 
        {
            enabledUlt = !enabledUlt;
            console.log("macro enabled ult: " + enabledUlt);
            return false;
        }
    });
});

ws.on('message', function message(data) {
        if (!enabled) {
          return;
        }
        var event = JSON.parse(data);       
        if (event.type == "keyRequestStun") {
          robot.keyTap("E");
        }
        if (event.type == "keyRequestUlt") {
          if (!enabledUlt) {
            return;
          }
          robot.keyTap("R");
          return;
        }
        
});
