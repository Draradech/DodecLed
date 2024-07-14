WebServer server(80);

void setupWeb()
{
  server.on("/control", handleControl);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loopWeb()
{
  server.handleClient();
}

String beginhtml = "<html><head><style>body{text-align: center; font-family: sans-serif;background-color: #55aabb} a{color: #000000;}</style>";
String midhtml = "</head><body><h1>Wifi Led Controller</h1>";
String endhtml = "</body></html>";

void handleControl()
{
  int localmode = -1;
  int led = -1;
  long color = -1;
  
  for(uint8_t i = 0; i < server.args(); i++)
  {
    if(server.argName(i) == "mode")
    {
      localmode = server.arg(i).toInt();
    }
    else if(server.argName(i) == "led")
    {
      led = server.arg(i).toInt();
    }
    else if(server.argName(i) == "color")
    {
      color = strtol(server.arg(i).c_str(), NULL, 16);
    }
  }

  String message = "";
  bool refresh = true;
  switch(localmode)
  {
    case 1:
      refresh = false;
      if (  (led != -1)
         && (color != -1)
         )
      {
        if(mode != 1)
        {
          for(int i = 0; i < NUMLEDS; i++)
          {
            leds[i] = CRGB::Black;
          }
          mode = 1;
        }
        leds[led] = color;
      }
      else
      {
        message = "<br>error evaluating arguments";
      }
      break;
    default:
      if(mode != localmode)
      {
        for(int i = 0; i < NUMLEDS; i++)
        {
          leds[i] = CRGB::Black;
        }
        mode = localmode;
      }
  }
  server.send(200, "text/html",
    beginhtml +
    (refresh ? "<meta http-equiv=\"refresh\" content=\"2; url=/\">" : "") +
    midhtml +
    "<p>Mode " + String(mode) +
    message +
    "</p>" +
    endhtml
  );
}

void handleNotFound()
{
  server.send(200, "text/html",
    beginhtml +
    midhtml +
    "<a href=control?mode=0>Off</a><br>" +
    "<a href=control?mode=1&led=5&color=0xffffff>Mode 1 (set leds)</a><br>" +
    "<a href=control?mode=2>Mode 2</a><br>" +
    "<a href=control?mode=3>Mode 3</a><br>" +
    "<a href=control?mode=4>Mode 4</a><br>" +
    "<a href=control?mode=5>Mode 5</a><br>" +
    "<a href=control?mode=6>Mode 6</a><br>" +
    String(voltage) + " V<br>" +
    endhtml
  );
}
