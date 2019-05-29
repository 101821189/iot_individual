/* eslint-disable */

// NOSQL & SOCKETIO & MONGOOSE SETUP
const {mongoose} = require('./server/mongoose');
const {Enter} = require('./server/enter');
const {Exit} = require('./server/enter');
const {Fan} = require('./server/fan');
const {Led} = require('./server/led');
const {Player} = require('./server/player');
const {ObjectId} = require('mongodb');
const bodyParser = require('body-parser');
const SerialPort = require('serialport')
const path = require('path');
const http = require('http');
const express = require('express');
const socketIO = require('socket.io');
const moment = require('moment');

// SERVER LIBRARY & SETUP
const publicPath = path.join(__dirname, 'public');
var app = express();
var server = http.createServer(app)
var io = socketIO(server);
app.use(express.static(publicPath));
app.use(bodyParser.json());

// JOHNNY-FIVE CODE
var five = require("johnny-five");

var board = five.Board();

let timeOut = 0;
let duration = 0;
let lock = false;


// Being Repl instance of johnny-five
board.on('ready', function() {

  //http://johnny-five.io/api/button/
  //http://johnny-five.io/examples/led/
  //http://johnny-five.io/examples/led-rgb/
  //http://johnny-five.io/examples/photoresistor/


  // THIS IS THE SETUP SECTION
  var led = new five.Led(8); // Set pin 13 for LED
  var button = new five.Button(2); // button
  var rgb = new five.Led.RGB({  // RGB
    pins: {
      red: 5,
      green: 6,
      blue: 7
    }
  });

  var photoresistor = new five.Sensor({  // Photresistor
    pin: "A2",
    freq: 250
  });

  // Reading Sockets
  io.on('connection', function (socket) {

    // inject photoresistor into johnny-five raple
    board.repl.inject({
      pot: photoresistor
    });

    // photoresister receiving data
    photoresistor.on("data", function() {
      console.log(this.value);

      if(this.value < 900 ) {
                // Turn on led
            const ledTrigger = () => {
              console.log('trigger');
              led.on();
            }

            // Button will simulate PIR sensor
            button.on("hold", function() {
              console.log('lock', lock);
              if (lock) {
                console.log('motion sensor is locked');
              }
              else {
                console.log( "Button pressed" );
                duration += 1;
                ledTrigger();
              }
            });

            // Can no longer sense motion
            button.on("release", function() {
              if (lock) {
                console.log('motion sensor is locked');
              }
              else {
                // turn off light
                led.off();
                console.log(duration);

                // store data to database
                var ledData = new Led({
                  date: moment(Date.now()).format('MMMM Do YYYY, h:mm:ss a'),
                  duration: duration
                });
                ledData.save().then((doc) => {
                  console.log(doc);
                });

                socket.emit('duration', {duration});

                duration = 0;
              }
            });

            // Subscription to lock button
            socket.on('lock', function(data) {
              console.log('website sent me data', data)
              lock = data.lock;
            });
            socket.emit('lock');

            // Subscription to unlock button
            socket.on('unlock', function(data) {
              console.log('website sent me data', data)
              lock = data.unlock;
            });

            socket.emit('unlock');

      }
    });

   
});

});

/////////  ----- DATABASE CODE
app.get('/Led', (req, res) => {
  console.log('api working');
  Led.find().then((data) => {
    res.send(data);
  })
})


app.post('/Led', (req, res) => {
  // console.log(moment(Date.now()));
  var led = new Led({
      date: moment(Date.now()),
      duration: duration
  });

  led.save().then((doc) => {
      res.send(doc);
  }, (e) => {
      res.status(400).send(e);
  });
});


app.post('/enter', (req, res) => {
  // console.log(moment(Date.now()));
  var enter = new Enter({
      time: moment(),
      count: '100',
      status: 'on'
  });

  enter.save().then((doc) => {
      res.send(doc);
  }, (e) => {
      res.status(400).send(e);
  });
});

app.post('/exit', (req, res) => {
  // console.log(moment(Date.now()));
  var enter = new Enter({
      time: moment(),
      count: 'exit data',
      status: 'on'
  });

  enter.save().then((doc) => {
      res.send(doc);
  }, (e) => {
      res.status(400).send(e);
  });
});

app.post('/fan', (req, res) => {
  console.log(req.body.text);
  var fan = new Fan({
      status: req.body.status,
      temp: req.body.temp
  });

  fan.save().then((doc) => {
      res.send(doc);
  }, (e) => {
      res.status(400).send(e);
  });
});

app.get('/fan', (req, res) => {
  console.log('api working');
  Fan.find().then((temp) => {
    res.send(temp);
  })
})

app.get('/player', (req, res) => {
  console.log('api working');
  Player.find().then((players) => {
    res.send(players);
  })
})

app.post('/player', (req, res) => {
  console.log(req.body);
  var player = new Player({
      name: req.body.name,
      level: req.body.level
  });

  player.save().then((doc) => {
      res.send(doc);
  }, (e) => {
      res.status(400).send(e);
  });
});


app.get('*', function (req, res) {
  res.sendFile(path.join(publicPath, 'index.html'))
});

server.listen(8080, () => {
  console.log("Local host live on 8080");
});
