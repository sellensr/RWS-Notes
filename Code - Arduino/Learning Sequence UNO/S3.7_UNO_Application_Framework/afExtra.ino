void buttonFall(){  // ISR to be called by interrupt whenever the button is pushed
  static unsigned long lastSwitch = 0;
  if(micros()-lastSwitch > 300000){    // don't switch on a bounce, or on release
    lastSwitch = micros();
    if(dtTaskMin[CONSOLE_TASK] != 0) dtTaskMin[CONSOLE_TASK] = 0; 
    else dtTaskMin[CONSOLE_TASK] = CONSOLE_TIME; 
  }
}

void blinkPin(){  // changes the status of the onboard LED on each call
  if(digitalRead(BLINK_PIN)) digitalWrite(BLINK_PIN,LOW);
  else digitalWrite(BLINK_PIN,HIGH);
}
