var options = {};

// Function to send a message to the Pebble using AppMessage API
function sendMessage(ip) {
  var response;
  var req = new XMLHttpRequest();
  req.open('GET', "http://" + ip + "/api/statusinfo", true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        //console.log(req.responseText);
        response = JSON.parse(req.responseText);
        //var currservice_name = response.currservice_name;
        //console.log(currservice_name);
        Pebble.sendAppMessage({ "currservice_begin": response.currservice_begin, 
                              "currservice_station": response.currservice_station,
                              "currservice_name": response.currservice_name,
                              "currservice_end": response.currservice_end,
                              "currservice_description": response.currservice_description},
          function(e) {
            //console.log("Successfully delivered message");
          },
          function(e) {
            console.log("Unable to deliver message. Error is: " + e.error.message);
          }
        );
      } 
      else {
        console.log("Error");
      }
    }
  };
  req.send(null);
}

// function to inform watch this js is ready
function iamready(){
          Pebble.sendAppMessage({ "ip": "???"},
          function(e) {
            //console.log("Successfully delivered ask for ip message");
          },
          function(e) {
            console.log("Unable to deliver message. Error is: " + e.error.message);
          }
        );
}


// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) { 
                //console.log("JavaScript app ready and running!");
                iamready();
							});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
							function(e) {
								//console.log("Received request for ip " + e.payload.ip);
								sendMessage(e.payload.ip);
							});

// Called when configuration is requested
Pebble.addEventListener("showConfiguration", 
                        function() {
                          //console.log("showing configuration");
                          Pebble.openURL('https://googledrive.com/host/0B0svWMRvX6J2ZnJSN2x3ZWtpQWc/enigmawatchsetup.html?'+encodeURIComponent(JSON.stringify(options)));
                        });

// Called after configuration
Pebble.addEventListener("webviewclosed",
                        function(e) {
                          //console.log("configuration closed");
                          // webview closed
                          //Using primitive JSON validity and non-empty check
                          if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
                            options = JSON.parse(decodeURIComponent(e.response));
                            //console.log("Options = " + JSON.stringify(options));
                            Pebble.sendAppMessage(options,
                                                  function(_event) {
                                                    //console.log("Successfully sent options to Pebble");
                                                  },
                                                  function(_event) {
                                                    console.log("Failed to send options to Pebble.\nError: " + _event.error.message);
                                                  }
                            );
                          } else {
                            console.log("Cancelled");
                          }
                        });