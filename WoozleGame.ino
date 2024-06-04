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
bool oneison = 0;
bool twoison = 0;
bool threeison = 0;
bool fourison = 0;
bool fiveison = 0;
bool sixison = 0;
bool sevenison = 0;
bool eightison = 0;
bool nineison = 0;

//booleans to keep track of whether something was pressed or the switch was flipped
bool onepressed = 0;
bool twopressed = 0;
bool switchflipped = 0;

//round number. starts at 0 for starting menu
int round = 0;

//whether it's normal or infinite mode.  0 means normal
bool infinite = 0;

//which woozle to randomly summon
int woozleon = 0;

int score = 0;


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

//ISRs for the buttons, since they're the only thing that can handle interrupts
void w1pressed() {
  onepressed = 1;
}

void w2pressed() {
  twopressed = 1;
}


void onepressing() {
  
  onepressed = 0;
}

void twopressing() {
  twopressed = 0;
}

void woozlepolling() {
  if (woozle1.isExpired()) {
    woozle1.repeat();
  }
  if (woozle2.isExpired()) {
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
    break;
    case 2:
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


void loop() {
  switch (round) {
    case 0:
      if (switchflipped) {
        infinite = !infinite;
        switchflipped = 0;
      }
      if (onepressed || twopressed) {
        round++;
      }
      Serial.println("You released the Woozles! Get ready!");
      delay(2000);
    break;
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
  }
}
