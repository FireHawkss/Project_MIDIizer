# Project goal
This project aims to convert an old digital piano (ARK-2172) to a MIDI controller to improve the sound quality and functionality

## Initial problem
The piano is okay at producing sounds if keys are pressed one by one. However if one wants to press two (or god forbid three!) keys at the same time to play chords on the piano then the sounds quality quickly breaks down.

## Solution
Disconnect the keys of the piano from the outdated controller of the piano and connect it to an arduino Uno (or similar) microcontroller to convert the the key presses into MIDI messages. This way the microcontroller doesn't need to produce the sounds itself and the heavy-lifting can be done by the computer using a VST (Virtual Studio Technology) to produce quality piano sounds

# Practical realisation
## Electronics setup and explanation
Most digital pianos, including this one, use a matrix system to scan for key presses to reduce the amount of cables and inputs needed. In this project we connect the matrix to the Arduino instead of the toaster quality controller in the piano. 

The matrix consists of 16 cables (8 rows, 8 columns) for 61 piano keys. However, the Arduino does not have enough inputs for all 16 cables. So the rows are connected the shift register to reduce the cables needed for the rows from 8 to 3. 

To be able to read the matrix the rows are connected to the shift register (output) and the colums are connected to the microcontroller with pull-down resistors (input). The code sets the rows high one by one and reads the columns. The combination of which row was activated and which column(s) registered an input tells us which key(s) were pressed.

### Used materials
- 1 Trashy digital paino 
- 1 Arduino Uno R4
- Bunch of jumper cables
- 1 SN74HC595N serial-in parallel-out shift register
- 8 10kohm resistors

# Helpful resources
http://www.codetinkerhack.com/2012/11/how-to-turn-piano-toy-into-midi.html
