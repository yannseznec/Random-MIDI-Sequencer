/* RANDOM MIDI NOTE SEQUENCER
 by yann seznec july 2017 - http://wwww.yannseznec.com
 no rights reserved
 any questions? email me, yann@yannseznec.com or on twitter @yannseznec

This probably has lots of bugs. among them:
 - i feel like sometimes there is a silent note at the end of each sequence, but i'm not sure
 - i'm not sure if the arrays need to be pre-filled the way they are now
 - this could probably be coded better in general. for example, the 'tempo' is dependent on the 'notelength'. on the plus side, this makes it kinda hard to control.

 This is made with the Teensy microcontroller in mind. It might work for other boards with some adaptations.

 For Teensys, make sure to go to "Tools" -> "USB Type" and select "USB MIDI".

 If you want to use regular MIDI you'll need the MIDI library: https://playground.arduino.cc/Main/MIDILibrary

 You can also just remove the #include <MIDI.h>, and all references to the MIDI library, if you just want to use USB MIDI. 

 Similarly, you can remove the usbMIDI references and just use the MIDI stuff if you want. WHATEVER WORKS.

 This code uses three potentiometers, two switches and a button.

 Connect the button to Ground and Pin 2
 Connect the on/off switch to Ground and Pin 3
 Connect the Super Random Mode switch to Ground and Pin 4
 Help with connecting buttons/switches to arduinos or similar: https://www.arduino.cc/en/Tutorial/Button (I don't use the resistor, I just use INPUT_PULLUP instead)

For the potentiometers, the outside pins of each one connect to 3v and Ground. The middle pins ('wipers') connect to pins 14,15 and 16.
Help with using Potentiometers on arduinos or similar: https://www.arduino.cc/en/tutorial/potentiometer

I've included a status LED connected to pin 13. The code for this isn't very good. It's totally optional.

For using old-fashioned MIDI, follow these instructions: https://www.pjrc.com/teensy/td_libs_MIDI.html
For some versions of Teensy you may need to change what is connected to the various pins in order to access the TX/RX. I usually use the Teensy LC or 3.

*/

#include <MIDI.h>



int numSteps = 4;
int pitch[] = {
  48,60,55,82,48,60,55,82,48,60,55,82,48,60,55,82,48,60,55,82,48,60,55,82
};
int vel[] = {
 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};
int randomTiming[] = {
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};
int totalSteps;
int currentStep;
int previousStep;
int maxSteps = 25; // maximum number of steps
int tempomap;
int noteLength = 800;
int currentMillis;
int previousMillis;
int currentNoteMillis;
int previousNoteMillis;

// the MIDI channel number to send messages
const int channel = 1;



void setup() {

  Serial.begin(38400);
  MIDI.begin(4);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  for (int i = 0; i < numSteps; i++) {
    pitch[i] = int(random(50) + 40);
    vel[i] = 100;
    randomTiming[i] = int(random(500) + 40);
    Serial.println(randomTiming[i]);
  }
}

void loop() {
  tempomap = int(map(analogRead(0), 0, 1024, 1500, 50));
  numSteps = int(map(analogRead(1), 0, 1024, 1, maxSteps));
  noteLength = int(map(analogRead(2), 0, 1024, 10, 800));
  if (digitalRead(3) == 0) {
    currentMillis = millis();
    currentNoteMillis = millis();
    if ((currentMillis - previousMillis) > tempomap) {
      previousMillis = currentMillis;
      totalSteps++;
      currentStep = totalSteps % numSteps;
      //   Serial.println(currentStep);
            
    playNote(currentStep);

      }
    }
    if(digitalRead(2) == 0) {
  randomNotes();
  }
  
}





void playNote(int s) {
      usbMIDI.sendNoteOn(pitch[s], vel[s], channel);
      MIDI.sendNoteOn(pitch[s], vel[s], channel);
      digitalWrite(13, HIGH);
     if(digitalRead(4)==0) {
      delay(randomTiming[s]);
     } else {
      delay(noteLength);
     }
      stopNote(s);
}

void stopNote(int s) {
  usbMIDI.sendNoteOff(pitch[s], vel[s], channel);
  MIDI.sendNoteOff(pitch[s], vel[s], channel);
  digitalWrite(13,LOW);
}

void randomNotes() {
  for (int i = 0; i < numSteps; i++) {
    pitch[i] = int(random(50) + 40);
    vel[i] = int(random(5) * 5);
    randomTiming[i] = int(random(800) + 30);
    Serial.println(randomTiming[i]);
  }
}





