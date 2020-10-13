#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

const int DIGITAL_INPUTS = 1;

const int MIDI_CHANNEL = 1;
const int NOTE_ON_SPEED = 127;
const int CC_VALUE = 0;
const int FIRST_NOTE = 1;
const int FIRST_CC = 1;
const int FIRST_PC = 1;

bool delayChange = false;
int previous_values[DIGITAL_INPUTS];


int pin_note(int pin_number){
  return FIRST_NOTE + (pin_number - 1);
}

int pin_cc(int pin_number){
  return FIRST_CC + (pin_number - 1);
}

int pin_pc(int pin_number){
  return FIRST_PC + (pin_number - 1);
}

void setValue(int pin_number){
  MIDI.sendNoteOn(pin_note(pin_number), NOTE_ON_SPEED, MIDI_CHANNEL);
  MIDI.sendControlChange(pin_cc(pin_number), CC_VALUE, MIDI_CHANNEL);
  MIDI.sendProgramChange(pin_pc(pin_number), MIDI_CHANNEL);
  delayChange = true;
}

void unsetValue(int pin_number){
  MIDI.sendNoteOff(pin_note(pin_number), 0, MIDI_CHANNEL);
}


void setup() {
  for (int i = 1; i <= DIGITAL_INPUTS; i++){
    pinMode(i, INPUT_PULLUP); 
    previous_values[i-1] = 0; 
  }
  MIDI.begin(MIDI_CHANNEL);
  Serial.begin(115200);
}

void loop() {
  if (delayChange == true){
    delay(1000);
    delayChange = false;
  }

  for(int i = 1; i <= DIGITAL_INPUTS; i++){
    int value = digitalRead(i);
    if (value != previous_values[i-1]){
      if (value == LOW){
        setValue(i);
      }
      else{
        unsetValue(i);
      }
      previous_values[i-1] = value;
    }
  }
  MidiUSB.flush();
}
