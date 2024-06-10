# You've uploaded Woozles onto your Circuit Playground Express!  Why would you do that?!  Now they're going to try to steal your bones and your only method of stopping them is your Hunny pressure washer.  When a light lights up, that means a Woozle is there, and you need to either touch its touch pad or press its button to use your pressure washer and blast them away.  They're probably fine.  But don't press a button when a Woozle isn't there or you'll be executed immediately for wasting Hunny.

## Rules
1. If the capacitance sensor corresponding to a Woozle has not been pressed within 3, 2, or 1 (depends on round number) seconds of it changing color, it’ll steal your bones
2. Two LEDs will be linked to the buttons instead, so you’ll have to press the buttons to pressure wash them away with hunny
3. As the rounds increase, the Frequency of Woozle Appearance will increase
4. In the infinite mode, the round settings will be set to maximum
5. You will earn more points from pressing the buttons sooner, and these points will determine when the round ends.  Each Woozle will have an associated value starting at 3000 in the first round and decreasing by 1 for every millisecond between when they appear and when you pressure wash them.
6. The score from each Woozle pressure washed will be multiplied by the round number, and the starting number for the timing of pressure washing them 8. will be 3000, 4000, or 6000, depending on the round.
7. If you press the button when there is not a Woozle, you will instantly be executed for wasting hunny (you lose)
When you press a button, the game starts and the mode cannot be changed until you lose or, if you’re in round progression mode, you reach the score threshold of the third stage.


## Inputs
# Capacitance Sensors (A1-7)
  The capacitance sensors will be used to turn their respective LEDs off when they light up.  If the player is touching a sensor (threshold of 800), a flag will be changed that will tell the board to execute a function and react depending on whether the LED was lit.  Unfortunately, these need polling because capacitance doesn’t work for interrupts.

# Buttons
  The buttons will be used in the same way as the capacitance sensors, potentially with a mechanic where they must be held for some number of seconds to make the light cycle through the rainbow and then turn off.

# Switch
  The switch will be used to toggle between round progression mode and infinite mode.  The default will be round progression mode and flipping the switch, no matter which side it starts on, will toggle it.  You can only change the round before the game starts, though.


## Outputs
# LEDs
  The LEDs will be used to represent a Woozle’s presence (lights up a random color) or, when paused, the game mode.  At the start, the game will be paused and all LEDs will be lit up.  If it’s purple, the game is in round progression mode.  If it is red, the game is in infinite mode.

# Serial Monitor
  The Serial Monitor will be used to say when the game has started, when the stage has changed, when you’ve lost, what score you got after you lost, or whether you’ve won.
