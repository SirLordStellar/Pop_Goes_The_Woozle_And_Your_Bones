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

//score
int score = 0;

//what the woozle score starts at for each round
int scorestart[5] = {0, 3000, 4000, 6000, 6000};

//array for the scores
int woozlescores[9] = {0,0,0,0,0,0,0,0,0};

//the number of milliseconds a woozle has been present
int woozlemillis[9] = {0,0,0,0,0,0,0,0,0};

//the number of seconds until the woozle will steal your bones
int woozletimeleft[9] = {0,0,0,0,0,0,0,0,0};

//which LED corresponds to which woozle
int woozleleds[9] = {2,7,6,8,9,0,1,3,4};

//whether there needs to be some delay before this woozle can appear again
bool woozlecooldown[9] = {0,0,0,0,0,0,0,0,0};

//whether there needs to be some delay before a touchpad becomes sensitive again
bool woozletouchcool[7] = {0,0,0,0,0,0,0};

//whether you've let go yet
bool stilltouching[7] = {0,0,0,0,0,0,0};



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

//this one sets the flag to toggle the mode (round progression vs. infinite)
void modeswitch() {
  switchflipped = 1;
}

//this is the function that ends the game, resetting all of the round variables and printing your final score
void stealbones() {
  //resetting all of the arrays that got changed
  for (int i = 0; i < 9; i++) {
    ison[i] = 0;
    woozlescores[i] = 0;
    woozlemillis[i] = 0;
    woozletimeleft[i] = 0;
    woozletouchcool[i] = 0;
    stilltouching[i] = 0;
  }
  //blinky lights
  for (int i = 0; i < 3; i++) {
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 100, 0, 0);
        }
        delay(200);
        CircuitPlayground.clearPixels();
        delay(200);
        }
   Serial.print("You got to stage ");
   Serial.print(stagenum);
   Serial.print(" and you got ");
   Serial.print(score);
   Serial.print(" points!");
   Serial.println("But now you're dead...");
   Serial.println("Press either button to hire your replacement and alert the Woozles to the presence of new bones!");
    //resetting these so it's back to how things were at the start
    onepressed = 0;
    twopressed = 0;
    switchflipped = 0;
    score = 0;
    stagenum = 0;
    CircuitPlayground.clearPixels();
}

//ISRs for the buttons, since they're the only thing that can handle interrupts
void w1pressed() {
  onepressed = 1;
}

void w2pressed() {
  twopressed = 1;
}

//functions themselves with debouncing
void onepressing() {
  if (ison[0]) {
    ison[0] = 0;
    score += woozlescores[0];
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
  } else {
    stealbones();
  }
  twopressed = 0;
  woozlecooldown[1] = 1;
  delay(10);
}

//the polling for if you're touching each touchpad
void touching() {
  if (CircuitPlayground.readCap(6) >= 800 && !woozletouchcool[0] && !stilltouching[0] && !woozlecooldown[2]) {
    stilltouching[0] = 1;
     }
     else if (CircuitPlayground.readCap(6) < 800 && !woozletouchcool[0] && stilltouching[0]) {
      if (ison[2]) {
      score += woozlescores[2];
      woozle3.restart();
      woozletouchcool[0] = 1;
      ison[2] = 0;
      stilltouching[0] = 0;
      woozlecooldown[2] = 1;
    } else {
      stealbones();
    }
    }

  }

//whether the woozles are ready to vivisect you
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
  if (woozle3.isExpired() && ison[2]) {
    if (woozletimeleft[2] > 1) {
      woozletimeleft[2]--;
    } else {
      stealbones();
    }
    woozle3.repeat();
  } else if (woozle3.isExpired() && woozletouchcool[0] && !stilltouching[0]) {
    woozletouchcool[0] = 0;
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

//randomly picks where a woozle will appear
void woozleapproach() {
  woozleon = random(1, 4);
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
      if (!ison[2] && !woozlecooldown[2]) {
        woozlescores[woozleon - 1] = scorestart[stagenum];
        woozlemillis[woozleon - 1] = millis();
        woozletimeleft[woozleon - 1] = 4 - stagenum;
        woozle3.restart();
        ison[woozleon - 1] = 1;
      }
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

void resetti() {
  for (int i = 0; i < 9; i++) {
    ison[i] = 0;
    woozlecooldown[i] = 0;
    woozlemillis[i] = 0;
    woozletimeleft[i] = 0;
    woozletouchcool[i] = 0;
    stilltouching[i] = 0;
  }
  onepressed = 0;
  twopressed = 0;
  switchflipped = 0;
}

void loop() {
  switch (stagenum) {
    case 0:
      //basically the start menu, before the game has started
      if (switchflipped) {
        infinite = !infinite;
        switchflipped = 0;
      }
      if (onepressed || twopressed) {
        if (!infinite) {
        stagenum += 1;
        Serial.println("You released the Woozles! Get ready! 3");
        for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 0, 100, 100);
          }
          delay(1000);
        Serial.println("2");
        
          for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 100, 100, 0);
          }
        delay(1000);
        Serial.println("1");
        onepressed = 0;
        twopressed = 0;
        } else {
          stagenum = 4;
          Serial.println("You released the Woozles and gave them creatine! Why would you do that?! Get ready! 3");
          for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 0, 25, 25);
          }
        delay(1000);
        Serial.println("2");
        for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 25, 25, 0);
          }
        delay(1000);
        Serial.println("1");
        onepressed = 0;
        twopressed = 0;
        }
      } else {
        if (infinite) {
          for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 25, 0, 0);
          }
        } else {
          for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 0, 25, 0);
          }
        }
      }
    break;
    case 1:
      if (random(0,100) == 10) {
        woozleapproach();
      }
      if (onepressed) {
        onepressing();
      }
      if (twopressed) {
        twopressing();
      }
      touching();
      for (int i = 0; i < 9; i++) {
        if (ison[i]) {
          CircuitPlayground.setPixelColor(woozleleds[i], 100, 0 , 100);
        } else {
          CircuitPlayground.setPixelColor(woozleleds[i], 0, 0, 0);
        }
      }
      woozlescoring();
      woozlepolling();
      if (score >= 10000) {
        stagenum = 2;
        score -= 10000;
        resetti();
        Serial.println("You made it to stage 2, and the Woozles are getting stronger.");
        for (int i = 0; i < 3; i++) {
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, 0, 100);
        }
        delay(200);
        CircuitPlayground.clearPixels();
        delay(200);
        }
      }
    break;
    case 2:
      if (random(0,50) == 10) {
        woozleapproach();
      }
      if (onepressed) {
        onepressing();
      }
      if (twopressed) {
        twopressing();
      }
      touching();
      for (int i = 0; i < 9; i++) {
        if (ison[i]) {
          CircuitPlayground.setPixelColor(woozleleds[i], 100, 0 , 100);
        } else {
          CircuitPlayground.setPixelColor(woozleleds[i], 0, 0, 0);
        }
      }
      woozlescoring();
      woozlepolling();
      if (score >= 15000) {
        stagenum = 3;
        score -= 15000;
        resetti();
        Serial.println("You made it to stage 3, and the Woozles are trying creatine.");
        for (int i = 0; i < 3; i++) {
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, 0, 100);
        }
        delay(200);
        CircuitPlayground.clearPixels();
        delay(200);
        }
      }
    break;
    case 3:
    if (random(0,25) == 10) {
        woozleapproach();
      }
      if (onepressed) {
        onepressing();
      }
      if (twopressed) {
        twopressing();
      }
      touching();
      for (int i = 0; i < 9; i++) {
        if (ison[i]) {
          CircuitPlayground.setPixelColor(woozleleds[i], 100, 0 , 100);
        } else {
          CircuitPlayground.setPixelColor(woozleleds[i], 0, 0, 0);
        }
      }
      woozlescoring();
      woozlepolling();
      if (score >= 20000) {
        for (int i = 0; i < 3; i++) {
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, 0, 100);
        }
        delay(200);
        CircuitPlayground.clearPixels();
        delay(200);
        }
        stagenum = 0;
        Serial.print("You beat the game!  You won with ");
        Serial.print(score);
        Serial.println(" points. Press any button to play again, you saucy little masochist!");
        resetti();
        score = 0;
      }
    break;
    case 4:
    //infinite mode.  Has to be accessed from starting menu
      if (random(0,25) == 10) {
        woozleapproach();
      }
      if (onepressed) {
        onepressing();
      }
      if (twopressed) {
        twopressing();
      }
      touching();
      for (int i = 0; i < 9; i++) {
        if (ison[i]) {
          CircuitPlayground.setPixelColor(woozleleds[i], 100, 0 , 100);
        } else {
          CircuitPlayground.setPixelColor(woozleleds[i], 0, 0, 0);
        }
      }
      woozlescoring();
      woozlepolling();
    break;
  }
}
