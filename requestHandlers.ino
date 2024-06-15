





void vibrateFunc(AsyncWebServerRequest *request) {
  int intensity = 0;
  if (request->hasParam("intensity")) {
    intensity = request->getParam("intensity")->value().toInt();
  }

  if (request->hasParam("motorNum")) {
    String motorNumStr = request->getParam("motorNum")->value();
    motorNumStr = motorNumStr.substring(1, motorNumStr.length() - 1); // Remove the brackets
    
    int startIndex = 0;
    int endIndex = 0;
    String output;

    while (startIndex < motorNumStr.length()) {
      endIndex = motorNumStr.indexOf(',', startIndex);
      if (endIndex == -1) endIndex = motorNumStr.length(); // If no comma, go to the end of the string
      
      int motorNum = motorNumStr.substring(startIndex, endIndex).toInt();
      startIndex = endIndex + 1; // Move past the comma for the next iteration
      
      if (motorNum >= 0 && motorNum < vibmotorslength) { // Validate motor number
        if (intensity >= minIntensity && intensity <= 255) {
          // Replace analogWrite with ledcWrite for ESP32 if needed
          analogWrite(vibmotors[motorNum], intensity);
          output += "Vibration on pin ";
          output += String(vibmotors[motorNum]);
          output += " set to intensity ";
          output += String(intensity);
          output += "\n";
        } else if (intensity == 0 || intensity < minIntensity) {
          analogWrite(vibmotors[motorNum], 0); // Turn off motor
          output += "Vibration turned off on pin ";
          output += String(vibmotors[motorNum]);
          output += "\n";
        }
      } else {
        output += "Error: Invalid motor index ";
        output += String(motorNum);
        output += "\n";
      }
    }

    if (output.length() == 0) {
      request->send(404, "text/plain", "Vibration Error");
    } else {
      request->send(200, "text/plain", output);
    }
  } else {
    request->send(404, "text/plain", "No motorNum provided");
  }
}

//
//
//void vibrateFunc(AsyncWebServerRequest *request) {
//  int intensity = 0;
//  int motorNum = -1;
//  if (request->hasParam("motorNum")) {
//    String hp = request->getParam("motorNum")->value();
//    motorNum = hp.toInt();
//  }
//  if (request->hasParam("intensity")) {
//    String hp = request->getParam("intensity")->value();
//    intensity = hp.toInt();
//  }
//
//  if ( motorNum >= 0 && motorNum < vibmotorslength) {
//    if (intensity > minIntensity && intensity <= 255) {
//
//      analogWrite(vibmotors[motorNum], intensity);
//      String output = "Vibration on pin " ;
//      output += vibmotors[motorNum];
//      output += " intensity: ";
//      output += intensity;
//      request->send(200, "text/plain", output  );
//    }
//    else if (intensity = 0   || intensity < minIntensity) {
//      analogWrite(vibmotors[motorNum], 0);
//      String output = "Vibration turned Off on pin " ;
//      output += vibmotors[motorNum];
//      request->send(200, "text/plain", output);
//    }
//  }
//  else {
//    request->send(200, "text/plain", "Vibration Error" );
//  }
//}

void servoFunc(AsyncWebServerRequest *request) {
  int servoposition = -1;
  int motorNum = -1;
  if (request->hasParam("motorNum")) {
    String hp = request->getParam("motorNum")->value();
    motorNum = hp.toInt();
  }
  if (request->hasParam("position")) {
    String hp = request->getParam("position")->value();
    servoposition = hp.toInt();
  }

  if ( motorNum >= 0 && motorNum < servoslength) {
    if (servoposition > minPosition && servoposition <= maxPosition) {

      servos[motorNum].write(servoposition);
      String output = "Servo on pin " ;
      output += servopins[motorNum];
      output += " positition: ";
      output += servoposition;
      request->send(200, "text/plain", output  );
    }
    else if (servoposition = -1   || servoposition < minPosition) {
      
      
       servos[motorNum].write(minPosition);
      String output = "Servo moved to min: " ;
      output += servopins[motorNum];
      request->send(200, "text/plain", output);
    }
  }
  else {
    request->send(200, "text/plain", "Vibration Error" );
  }
}
