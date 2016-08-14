#include <pitches.h>

#define resolution 8
#define mains 50 // 60: north america, japan; 50: most other places
long limit=250;
long a;

#define refresh 2 * 1000000 / mains
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };
void setup() {
  Serial.begin(9600);

  // unused pins are fairly insignificant,
  // but pulled low to reduce unknown variables
  for(int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  for(int i = 8; i < 11; i++)
    pinMode(i, INPUT);

  startTimer();
}

void loop() {  
 //Serial.println(time(8, B00000001), DEC);
  Serial.print(" ");
  a= time(9, B00000010);
  Serial.println(time(9, B00000010), DEC);
  Serial.print(" ");
  if(a>limit) 
  buz();
  //Serial.println(time(10, B00000100), DEC);

} 

long time(int pin, byte mask) {
  unsigned long count = 0, total = 0;
  while(checkTimer() < refresh) 
  {
     //Serial.print("in 1st while loop");
    // pinMode is about 6 times slower than assigning
    // DDRB directly, but that pause is important
    pinMode(pin, OUTPUT);
    //Serial.print("in 1st0 while loop");
    PORTB = 0;
    //Serial.print("in 1st1 while loop");
    pinMode(pin, INPUT);
    //Serial.print("in 1st2 while loop");
    //delay(1000);
    while((PINB & mask) == 0)
    {
      count++;//Serial.print("in 2nd while loop");delay(1000);
    }
    total++;
    
  }
  startTimer();
  return (count << resolution) / total;
}

extern volatile unsigned long timer0_overflow_count;

void startTimer() {
  timer0_overflow_count = 0;
  TCNT0 = 0;
}

unsigned long checkTimer() {
  return ((timer0_overflow_count << 8) + TCNT0) << 2;
}


void buz()
{Serial.print("gdf");
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(2, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}


