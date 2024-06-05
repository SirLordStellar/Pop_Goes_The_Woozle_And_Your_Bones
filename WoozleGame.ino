#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

//these are controlled by left and right buttons, respectively
AsyncDelay woozle1;
AsyncDelay woozle2;
//these are controlled by the touch pads
AsyncDelay woozle3;
AsyncDelay woozle4;
AsyncDelay woozle5;
AsyncDelay woozle6;
AsyncDelay woozle7;
AsyncDelay woozle8;
AsyncDelay woozle9;

//booleans to keep track of the Woozles' presence
bool ison[9] = {0,0,0,0,0,0,0,0,0};

//booleans to keep track of whether something was pressed or the switch was flipped
bool onepressed = 0;
bool twopressed = 0;
bool switchflipped = 0;

//round number. starts at 0 for starting menu
int stagenum = 0;

//whether it's normal or infinite mode.  0 means normal
bool infinite = 0;

//which woozle to randomly summon
int woozleon = 0;

int score = 0;
int scorestart[3] = {3000, 4000, 6000};

int woozlescores[9] = {0,0,0,0,0,0,0,0,0};
int woozlemillis[9] = {0,0,0,0,0,0,0,0,0};
int woozletimeleft[9] = {0,0,0,0,0,0,0,0,0};
int woozleleds[9] = {2,7,0,0,0,0,0,0,0};
bool woozlecooldown[9] = {0,0,0,0,0,0,0,0,0};



void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);

  woozle1.start(1000, AsyncDelay::MILLIS);
  woozle2.start(1000, AsyncDelay::MILLIS);
  woozle3.start(1000, AsyncDelay::MILLIS);
  woozle4.start(1000, AsyncDelay::MILLIS);
  woozle5.start(1000, AsyncDelay::MILLIS);
  woozle6.start(1000, AsyncDelay::MILLIS);
  woozle7.start(1000, AsyncDelay::MILLIS);
  woozle8.start(1000, AsyncDelay::MILLIS);
  woozle9.start(1000, AsyncDelay::MILLIS);
  
  attachInterrupt(4, w1pressed, FALLING);
  attachInterrupt(5, w2pressed, FALLING);
  attachInterrupt(7, modeswitch, CHANGE);

}

void modeswitch() {
  switchflipped = 1;
}

void stealbones() {
  stagenum = 0;
  for (int i = 0; i < 9; i++) {
    ison[i] = 0;
    woozlescores[i] = 0;
    woozlemillis[i] = 0;
    woozletimeleft[i] = 0;
  }
   Serial.print("You got ");
   Serial.print(score);
   Serial.println(" points!");
    onepressed = 0;
    twopressed = 0;
    switchflipped = 0;
    score = 0;
    CircuitPlayground.clearPixels();
}

//ISRs for the buttons, since they're the only thing that can handle interrupts
void w1pressed() {
  onepressed = 1;
}

void w2pressed() {
  twopressed = 1;
}


void onepressing() {
  if (ison[0]) {
    ison[0] = 0;
    score += woozlescores[0];
    CircuitPlayground.setPixelColor(2, 0, 0, 0);
  } else {
    stealbones();
  }
  onepressed = 0;
  woozlecooldown[0] = 1;
  delay(10);
}

void twopressing() {
  if (ison[1]) {
    ison[1] = 0;
    score += woozlescores[1];
    CircuitPlayground.setPixelColor(7, 0, 0, 0);
  } else {
    stealbones();
  }
  twopressed = 0;
  woozlecooldown[1] = 1;
  delay(10);
}

void woozlepolling() {
  if (woozle1.isExpired() && ison[0]) {
    if (woozletimeleft[0] > 1) {
      woozletimeleft[0] -= 1;
    } else {
      stealbones();
    }
    woozle1.repeat();
  } else if (woozle1.isExpired() && woozlecooldown[0]) {
    woozlecooldown[0] = 0;
    woozle1.repeat();
  }
  if (woozle2.isExpired() && ison[1]) {
    if (woozletimeleft[1] > 1) {
      woozletimeleft[1]--;
    } else {
      stealbones();
    }
    woozle2.repeat();
  } else if (woozle2.isExpired() && woozlecooldown[1]) {
    woozlecooldown[1] = 0;
    woozle2.repeat();
  }
  if (woozle3.isExpired()) {
    woozle3.repeat();
  }
  if (woozle4.isExpired()) {
    woozle4.repeat();
  }
  if (woozle5.isExpired()) {
    woozle5.repeat();
  }
  if (woozle6.isExpired()) {
    woozle6.repeat();
  }
  if (woozle7.isExpired()) {
    woozle7.repeat();
  }
  if (woozle8.isExpired()) {
    woozle8.repeat();
  }
  if (woozle9.isExpired()) {
    woozle9.repeat();
  }
}

void woozleapproach() {
  woozleon = random(1, 3);
  switch (woozleon) {
    case 1:
      if (!ison[0] && !woozlecooldown[0]) {
        woozlescores[0] = scorestart[stagenum];
        woozlemillis[0] = millis();
        woozletimeleft[0] = 4 - stagenum;
        woozle1.restart();
        ison[0] = 1;
      }
    break;
    case 2:
      if (!ison[1] && !woozlecooldown[1]) {
        woozlescores[1] = scorestart[stagenum];
        woozlemillis[1] = millis();
        woozletimeleft[1] = 4 - stagenum;
        woozle2.restart();
        ison[1] = 1;
      }
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    case 7:
    break;
    case 8:
    break;
    case 9:
    break;
  }
}

void woozlescoring() {
  for (int i = 0; i < 9; i++) {
    if (ison[i]) {
      woozlescores[i] = millis()-woozlemillis[i];
      woozlescores[i] *= stagenum;
    }
  }
}

void loop() {
  switch (stagenum) {
    case 0:
      if (switchflipped) {
        infinite = !infinite;
        switchflipped = 0;
      }
      if (onepressed || twopressed) {
        stagenum += 1;
        Serial.println("You released the Woozles! Get ready!");
        delay(2000);
        onepressed = 0;
        twopressed = 0;
      }
    break;
    case 1:
      if (random(0,10000 - millis()/100) == 10) {
        woozleapproach();
      }
      if (onepressed) {
        onepressing();
      }
      if (twopressed) {
        twopressing();
      }
      for (int i = 0; i < 9; i++) {
        if (ison[i]) {
          CircuitPlayground.setPixelColor(woozleleds[i], 100, 0 , 100);
        }
      }
      woozlescoring();
      woozlepolling();
    break;
    case 2:
    break;
    case 3:
    break;
  }
}
