// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);


//Para la descarga del numero de usuarios de ZEsarUX
Pebble.addEventListener("appmessage", function(e) {

  var req = new XMLHttpRequest();

  req.onload = function() {

    Pebble.sendAppMessage({
      0: req.responseText
    });

  };

  req.onerror = function() {

    Pebble.sendAppMessage({
      0: "Error"
    });

  };

  req.open(
    "GET",
    "http://51.83.33.13/zesarux_yesterday_users.txt",
    true
  );

  req.send();
});
