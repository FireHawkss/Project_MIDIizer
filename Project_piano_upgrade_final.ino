#include "MIDI.h"

//Note: if the serial terminal is open, Hairless will not be able read the MIDI data.
MIDI_CREATE_DEFAULT_INSTANCE();
//MIDI 60 is middle C
// Pin Definitions
// columns are connected to
const int col0 = 9;
const int col1 = 8;
const int col2 = 7;
const int col3 = 6;
const int col4 = 5;
const int col5 = 4;
const int col6 = 3;
const int col7 = 2;

// The SN74HC595N is a serial-in parallel-out shift register
/*
Qb --> pin 3
Qc --> pin 4
Qd --> pin 5
Qe --> pin 6
Qf --> pin 7
Qg --> pin 8
Qh --> pin 9
GND --> GND

Vcc --> 5V
Qa --> pin 2
SER --> 12
_OE --> GND
RCLK --> pin 11 (latch)
SRCLK --> pin 10 (clock)
_SRCLR --> 5V
Qh' --> nothing 
*/
const int clock = 10; //SRCLK
const int latch = 11; //RCLK
const int data = 12; //SER

const int lowestMIDINote = 36; // lowest note on the piano, C2
const int MIDI_CHANNEL = 1; //set MIDI channel to 1
uint8_t keyToMidiMap[64];
boolean keyPressed[64];
int noteVelocity = 127;


// use prepared bit vectors instead of shifting bit left everytime
int bits[] = { B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000 };


// 74HC595 shift to next column
void scanRow(int value) {
	digitalWrite(latch, LOW); //Pulls the chips latch low
	shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
	digitalWrite(latch, HIGH); //Pulls the latch high displaying the data
}

void setup() {
	
	// Map scan matrix buttons/keys to actual Midi note number. Lowest num 36 corresponds to C2 MIDI note.
	for (int i = 0; i < 8; i++) {
		keyToMidiMap[i*8] = lowestMIDINote + i*8 + 0;
		keyToMidiMap[i*8 + 1] = lowestMIDINote + i*8 + 1; 
		keyToMidiMap[i*8 + 2] = lowestMIDINote + i*8 + 2;
		keyToMidiMap[i*8 + 3] = lowestMIDINote + i*8 + 3;
		keyToMidiMap[i*8 + 4] = lowestMIDINote + i*8 + 4;
		keyToMidiMap[i*8 + 5] = lowestMIDINote + i*8 + 5;
		keyToMidiMap[i*8 + 6] = lowestMIDINote + i*8 + 6;
		keyToMidiMap[i*8 + 7] = lowestMIDINote + i*8 + 7;
  }

	// setup pins output/input mode
	pinMode(data, OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(latch, OUTPUT);

	pinMode(col0, INPUT);
	pinMode(col1, INPUT);
	pinMode(col2, INPUT);
	pinMode(col3, INPUT);
	pinMode(col4, INPUT);
	pinMode(col5, INPUT);
	pinMode(col6, INPUT);
	pinMode(col7, INPUT);

	MIDI.begin(MIDI_CHANNEL);
	Serial.begin(115200); 

	delay(1000);

}

void loop() {

	for (int row = 0; row < 8; row++)  {
		//delay(1000);
		
		// shift scan matrix to following row
		scanRow(bits[row]);

		// check if any keys were pressed - columns will have HIGH output in this case corresponding
		int groupValue0 = digitalRead(col0);
		int groupValue1 = digitalRead(col1);
		int groupValue2 = digitalRead(col2);
		int groupValue3 = digitalRead(col3);
		int groupValue4 = digitalRead(col4);
		int groupValue5 = digitalRead(col5);
		int groupValue6 = digitalRead(col6);
		int groupValue7 = digitalRead(col7);		

		// process if any combination of keys pressed
		if (groupValue0 != 0 || groupValue1 != 0 || groupValue2 != 0
				|| groupValue3 != 0 || groupValue4 != 0 || groupValue5 != 0
				|| groupValue6 != 0 || groupValue7 != 0) {

			if (groupValue0 != 0 && !keyPressed[row]) {
				keyPressed[row] = true;
				MIDI.sendNoteOn(keyToMidiMap[row], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(0, row, keyToMidiMap[row]);
			}

			if (groupValue1 != 0 && !keyPressed[row + 8]) {
				keyPressed[row + 8] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 8], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(1, row, keyToMidiMap[row + 8]);
			}

			if (groupValue2 != 0 && !keyPressed[row + 16]) {
				keyPressed[row + 16] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 16], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(2, row, keyToMidiMap[row + 16]);
			}

			if (groupValue3 != 0 && !keyPressed[row + 24]) {
				keyPressed[row + 24] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 24], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(3, row, keyToMidiMap[row + 24]);
			}

			if (groupValue4 != 0 && !keyPressed[row + 32]) {
				keyPressed[row + 32] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 32], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(4, row, keyToMidiMap[row + 32]);
			}

			if (groupValue5 != 0 && !keyPressed[row + 40]) {
				keyPressed[row + 40] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 40], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(5, row, keyToMidiMap[row + 40]);
			}

			if (groupValue6 != 0 && !keyPressed[row + 48]) {
				keyPressed[row + 48] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 48], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(6, row, keyToMidiMap[row + 48]);
			}

			if (groupValue7 != 0 && !keyPressed[row + 56]) {
				keyPressed[row + 56] = true;
				MIDI.sendNoteOn(keyToMidiMap[row + 56], noteVelocity, MIDI_CHANNEL);
				debug_row_and_col(7, row, keyToMidiMap[row + 56]);
			}
		}

		// process if any combination of keys are released
		if (groupValue0 == 0 && keyPressed[row]) {
			keyPressed[row] = false;
			MIDI.sendNoteOff(keyToMidiMap[row], 0, MIDI_CHANNEL);
			debug_row_and_col(0, row, keyToMidiMap[row]);
		}

		if (groupValue1 == 0 && keyPressed[row + 8]) {
			keyPressed[row + 8] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 8], 0, MIDI_CHANNEL);
			debug_row_and_col(1, row, keyToMidiMap[row + 8]);
		}

		if (groupValue2 == 0 && keyPressed[row + 16]) {
			keyPressed[row + 16] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 16], 0, MIDI_CHANNEL);
			debug_row_and_col(2, row, keyToMidiMap[row + 16]);
		}

		if (groupValue3 == 0 && keyPressed[row + 24]) {
			keyPressed[row + 24] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 24], 0, MIDI_CHANNEL);
			debug_row_and_col(3, row, keyToMidiMap[row + 24]);
		}

		if (groupValue4 == 0 && keyPressed[row + 32]) {
			keyPressed[row + 32] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 32], 0, MIDI_CHANNEL);
			debug_row_and_col(4, row, keyToMidiMap[row + 32]);
		}

		if (groupValue5 == 0 && keyPressed[row + 40]) {
			keyPressed[row + 40] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 40], 0, MIDI_CHANNEL);
			debug_row_and_col(5, row, keyToMidiMap[row + 40]);
		}

		if (groupValue6 == 0 && keyPressed[row + 48]) {
			keyPressed[row + 48] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 48], 0, MIDI_CHANNEL);
			debug_row_and_col(6, row, keyToMidiMap[row + 48]);
		}

		if (groupValue7 == 0 && keyPressed[row + 56]) {
			keyPressed[row + 56] = false;
			MIDI.sendNoteOff(keyToMidiMap[row + 56], 0, MIDI_CHANNEL);
			debug_row_and_col(7, row, keyToMidiMap[row + 56]);
		}
	}
}

void debug_row_and_col(int col, int row, int key) {
	/*
	Serial.print("col: ");
	Serial.print(col);
	Serial.print(", row: ");
	Serial.print(row);
	Serial.print(", key: ");
	Serial.print(key);
	Serial.print("\n");
	*/
}