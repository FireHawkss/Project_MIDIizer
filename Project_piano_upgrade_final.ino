#include "MIDI.h"

//Note: if the serial terminal is open, Hairless will not be able read the MIDI data.
MIDI_CREATE_DEFAULT_INSTANCE();
//MIDI 60 is middle C

// Pin Definitions
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
// columns are connected to
const int column0 = 9; //P303
const int column1 = 8; //P304
const int column2 = 7; //P107
const int column3 = 6; //P106
const int column4 = 5; //P102
const int column5 = 4; //P103
const int column6 = 3; //P104
const int column7 = 2; //P105

const int clock = 10; //SRCLK
const int latch = 11; //RCLK
const int data = 12; //SER

const int lowestMIDINote = 36; // lowest note on the piano, C2
const int NUM_KEYS = 61; // 61 keys on the piano (but 8*8 = 64 keys in the matrix)
const int MIDI_CHANNEL = 1; //set MIDI channel to 1
uint8_t keyToMidiMap[64];
boolean keyPressed[64];
int groupValue[8];
int noteVelocity = 127;

int keynum = 0;

char str[64];//pure debugging string


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
	//each key on the piano gets numbered from left to right, from 0 to 63
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

	pinMode(column0, INPUT);
	pinMode(column1, INPUT);
	pinMode(column2, INPUT);
	pinMode(column3, INPUT);
	pinMode(column4, INPUT);
	pinMode(column5, INPUT);
	pinMode(column6, INPUT);
	pinMode(column7, INPUT);

	MIDI.begin(MIDI_CHANNEL);
	Serial.begin(115200); 

	delay(1000);

}

void loop() {

	for (int row = 0; row < 8; row++)  {
		//delay(1000);
		
		// shift scan matrix to following row
		scanRow(bits[row]);

		// check if any keys were pressed - columns will have HIGH output in this case
		
		groupValue[0] = digitalRead(column0);
		groupValue[1] = digitalRead(column1);
		groupValue[2] = digitalRead(column2);
		groupValue[3] = digitalRead(column3);
		groupValue[4] = digitalRead(column4);
		groupValue[5] = digitalRead(column5);
		groupValue[6] = digitalRead(column6);
		groupValue[7] = digitalRead(column7);
		
		/*
		//low-level read of GPIO pins !danger, board specific!
		groupValue[0] = ((R_PORT3->PIDR & bit(3)) == bit(3)); //P303
		groupValue[1] = ((R_PORT3->PIDR & bit(4)) == bit(4)); //P304
		groupValue[2] = ((R_PORT1->PIDR & bit(7)) == bit(7)); //P107
		groupValue[3] = ((R_PORT1->PIDR & bit(6)) == bit(6)); //P106
		groupValue[4] = ((R_PORT1->PIDR & bit(2)) == bit(2)); //P102
		groupValue[5] = ((R_PORT1->PIDR & bit(3)) == bit(3)); //P103
		groupValue[6] = ((R_PORT1->PIDR & bit(4)) == bit(4)); //P104
		groupValue[7] = ((R_PORT1->PIDR & bit(5)) == bit(5)); //P105
		*/

		//GPIOA->IDR&0x02)==0x02

		// process if any combination of keys pressed
		for (int col = 0; col < 8; col++) {
			keynum = row + col*8; //calculate key number based on row and column

			//sprintf(str, "groupValue[%d] = %d \n",col,groupValue[col]);
			//Serial.print(str);

			if (keynum >= NUM_KEYS) {
				continue; // skip if keynum exceeds the number of keys (matrix is 8*8=64 but we have 61 keys)
			}
			
			if (groupValue[col] != 0 && !keyPressed[keynum]) {
				keyPressed[keynum] = true;
				MIDI.sendNoteOn(keyToMidiMap[keynum], noteVelocity, MIDI_CHANNEL);
				//debug_row_and_col(row, col, keyToMidiMap[keynum]);
			}			
			else if (groupValue[col] == 0 && keyPressed[keynum]) {
				keyPressed[keynum] = false;
				MIDI.sendNoteOff(keyToMidiMap[keynum], 0, MIDI_CHANNEL);
				//debug_row_and_col(row, col, keyToMidiMap[keynum]);
			}
		}	
	}
}

void debug_row_and_col(int row, int col, int key) {
	
	char str2[64];

	sprintf(str2, "col: %d, row: %d, key: %d\n", col, row, key);
	Serial.print(str2);	
}