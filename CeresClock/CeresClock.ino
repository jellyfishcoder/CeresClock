#include <stdint.h>
#include <TouchScreen.h>
#include <TFT.h>

// Ceres_Clock
// By Alexander Pope
// To run this sketch hook up a Seeed Studio v1 2.8" Touch Shield 
// to a Sparkfun Redboard or an Arduino Uno.

// Time Variables:
int aSec = 0;               // Astrosecound
int aMin = 0;               // Astrominute
int aHr = 1;                // Astrohour
int aDay = 1;               // Astroday (day of mounth)
int dayName;                // Day Name in Char Array
int count = 0; 

// Display Loop Variables:
char* dayNames[] = {"Alpha", "Beta", "Gamma", "Unus", "Duo", "Tres", "Un", "Deux", "Trois"};
char mounth[5] = "Vena";    // HAHA! Only one mounth, but no one will ever know.
boolean update = true;      // To tell the loop to update or not to update the display
char time[6];               // What time is it!?!?
char secounds[3];           // I SAID WHAT TIME IS IT, THAT MEANS SECOUNDS TOO!!!

// Pin Definitions for Seeed Studio TFT Shield v1 (Uno or RedBoard only):
#define YM 14               // (Y-) For shield use pin 14 or A0
#define XM A1               // (X-) For shield use pin A1 only
#define YP A2               // (Y+) For shield use pin A2 only
#define XP 17               // (X+) For shield use pin 17 or A3


void setup() {
    
  // Initialize the TFT Library:
  Tft.init();
  Tft.setDisplayDirect(UP2DOWN);
  // Disable Global Interrupts:
  cli();
  
  // Initialize Timer1:
  TCCR1A = 0;    // Set the Entire TCCR1A Register to 0
  TCCR1B = 0;    // Set the Entire TCCR1B Register to 0
  TCNT1 = 0;      // Initialize counter value to 0
  
  // Set the Compare Match Register to desired timer count:
  OCR1A = 11957;
  // Turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // Enable Timer Compate Interrupt:
  TIMSK1 |= (1 << OCIE1A);
  
  // Enable Global Interrupts:
  sei();
}

void loop() {
  if(update) {
    updatescreen();
  }
}

// Bot to roll over time
void RollOverBot() {
  aSec = 0;               // Since the secounds rolled over, set them to 0.
  if(aMin == 49) {        // If the minutes should roll over,
    aMin = 0;             // then set the minutes to 0,
    
    if(aHr == 10) {       // and check if the hours should roll over.
      aHr = 0;            // If they should, set the hours to 0,
      aDay++;             // and increment the day.
    }
    else {                // If the hours shouldn't roll over,
      aHr++;              // increment the hours.
    }
  }
  else {                  // If the minutes shouldn't roll over,
    aMin++;               // increment the minutes.
  }
  
  update = true;          // Now that we have finished updating things, 
                          // let the loop know it needs to redraw the time
}
  
void updatescreen() {
    // Convert time intigers into time char array:
    time[4] = '0' + (aMin % 10);                                    // Minutes Ones Place
    time[3] = '0' + (aMin / 10);                                    // Minutes Tens Place
    time[2] = ':';                                                  // Hour Minute Separator
    time[1] = '0' + (aHr % 10);                                     // Hours Ones Place
    time[0] = '0' + (aHr / 10);                                     // Hours Tens Place
    
    
    // Convert date intigers into day name and date char array:
    int c = 9;
    while(c > 0) {
      if(aDay % c == 0) {
        dayName = c-1;
        c = 0;
      }
      else {
        c--;
      }
    }
    
    // Draw the time:
    Tft.paintScreenBlack();
    Tft.fillRectangle(0,0,250,325,BLACK);
    Tft.drawString(time,220,15,5,CYAN);                       // Draw the time string
    Tft.drawString(dayNames[dayName], 175, 110, 3, WHITE);    // Draw the day string
    Tft.drawString("CERES MINING CO.", 20, 40, 2, GRAY1);     // Draw the Name
    update = false;
}
    
// Timer 1 Interrupt
ISR(TIMER1_COMPA_vect) {
  if(aSec == 49) {        // If the secounds should roll over,
    RollOverBot();        // then "RELEASE THE ROLLOVER BOT," or just update the time.
  }
  else {                  // If the secounds shouldn't roll over,
    aSec++;               // increment the secounds.
    if(count > 1) {
      Tft.fillRectangle(180,240,90,90,BLACK);
      secounds[2] = '0' + (aSec % 10);                          // Secounds Ones Place  
      secounds[1] = '0' + (aSec / 10);                          // Secounds Tens Place
      secounds[0] = ':';                                        // Minutes Secounds Seperator
      Tft.drawChar(secounds[0],220,210,5,CYAN);                 // Draw the Secounds
      Tft.drawChar(secounds[1],220,245,5,CYAN);
      Tft.drawChar(secounds[2],220,280,5,CYAN);
    }
    else{count++;}
  }
}
