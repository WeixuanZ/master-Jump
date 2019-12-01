// This section defines key variables and sets up the pins.
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// Above two lines simply set up display software for these pins, to enable us to use the attached LCD screen (which is attached to output pins 2 through 7).

const int blackSwitch = 8;
const int redSwitch = 9;
// Sets up the two switch input pins that the programme requires in order to operate.

const int output = 13;
// Sets up the position of the final pin needed - the output pin that enters into the circuit.

float voltage;
float jumpTime;
float y = 300;
// Defines some key variables that are used later on in the programme.
// This includes the voltage that is input into the programme that is then converted to jumpTime. There is also an intermediary value, y, which determines how fast the initial animation goes off.


// Following section defines a two-step animation that is used at the start of the programme as a sort of start-up screen.
// It has no actual purpose vis-à-vis the actual functioning of the programme, but the animation looks quite good.
// The animation in question is that of the chess piece in Jump jumping across the LCD screen.
// It works by dividing the LCD display into 8x5 pixel windows.
// It then draws each "frame" separately for the 8x5 windows.

// The full chess piece
byte customChar[8] = {
0b00100,
0b01110,
0b00100,
0b00000,
0b00100,
0b01110,
0b01110,
0b11111
};

// An empty character.
byte customChar2[8] = {
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};

// Group1
byte customChar3b[8] = {
0b00100,
0b00000,
0b00100,
0b01110,
0b01110,
0b11111,
0b00000,
0b00000,
};

byte customChar3a[8] = {
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00100,
0b01110,
};

// Group2
byte customChar4b[8] = {
0b00100,
0b01110,
0b01110,
0b11111,
0b00000,
0b00000,
0b00000,
0b00000
};

byte customChar4a[8] = {
0b00000,
0b00000,
0b00000,
0b00000,
0b00100,
0b01110,
0b00100,
0b00000
};

// Group3
byte customChar5a[8] = {
0b00000,
0b00000,
0b00100,
0b01110,
0b00100,
0b00000,
0b00100,
0b01110
};

byte customChar5b[8] = {
0b01110,
0b11111,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};

// This section is the preliminaries to the running of the programme.
void setup() {
pinMode(blackSwitch, INPUT_PULLUP);
pinMode(redSwitch, INPUT_PULLUP);
// These two lines put a pullup resistor above each of the switches.
// This means that when the button is pressed, there is a low voltage to the input.
// The reason why this is here will be discussed fully elsewhere, but it prevents "floating" between high and low.
pinMode(output, OUTPUT);
// This line simply sets the output pin to being an output.
digitalWrite(output, LOW);
// Preliminarily sets the output to being low.
// When the output is high, that is when the circuit below is triggered, and the phone connected to ground.
Serial.begin(9600);
// Sets data rate in bits per second.
lcd.begin(16, 2);
// Defines the LCD's number of columns and rows:
lcd.print("MasterJump");
// Print the message "Master Jump" (as a sort of title for the load screen) to the LCD.
animation();
// Executes the animated jumping character function.
lcd.clear();
// Clears the LCD of all of the "load screen" detritus.
}

// This section calculates how long a press is needed for the jump to be the correct length.
float timeCalc() {
jumpTime = 277.59 * voltage + 362.45;
return jumpTime;
// Returns the correct time for the Jump.
}


// This section scales the voltage input from the variable resistor and modulates it to display a value between 0 and 5V.
float voltageCalc() {
int sensorValue = analogRead(A0);
// Reads in the analogue sensor reading of the variable resistor in the first analogue pin.
voltage = sensorValue * (5.0 / 1023.0);
// Scales the digital value, so it will be a value between 0 and 5V
return voltage;
// Returns the modulated value.
}

// This is the main loop of the programme.
void loop() {
voltageCalc();
timeCalc();
// Constantly updates the scaled voltage reading and associated jump time.
lcd.setCursor(0, 0);
lcd.print(voltage);
lcd.print("V");
// Displays on the LCD the updated voltage reading.
lcd.setCursor(0, 1);
lcd.print(jumpTime);
lcd.print("ms");
// Displays on the second row the updated time reading.

// This is the case where the user has pressed the red switch.
// This case is intended as the actual jump calculation.
if (digitalRead(redSwitch) == LOW) {
   lcd.clear();
   // Clears out all of the normal display.
   lcd.print("Jumping...");
   // Replaces it with a new message saying "Jumping".
   digitalWrite(output, HIGH);
   // Sets the output pin to high - triggering the circuit.
   delay(jumpTime);
   // Delays the correct amount of time for the "press".
   digitalWrite(output, LOW);
   // Sets the output back to low, so that it doesn't just keep on pressing down.
   lcd.clear();
   // Clears the display again to prepare for going back to the normal display.
   delay(300);
   // A delay to prevent button spam.
}

// The case where the black reset switch is pressed down.
// This case is intended as a reset button for when the user dies.
if (digitalRead(blackSwitch) == LOW) {
   lcd.clear();
   // Clears out normal display.
   lcd.setCursor(0, 0);
   lcd.print("Restarting...");
   // Prints the message "Restarting".
   digitalWrite(output, HIGH);
   // Starts the "press".
   delay(800);
   // Holds the "press".
   digitalWrite(output, LOW);
   // Lets go of the "press".
   lcd.clear();
   // Clears the display in anticipation of the usual display.
}
}

// This section is the part that actually executes the jumping piece animation.
void animation() {

// Creating the custom characters from the arrays of bytes defined at the beginning of the code.
lcd.createChar(0, customChar);
lcd.createChar(1, customChar2);
lcd.createChar(2, customChar3a);
lcd.createChar(3, customChar3b);
lcd.createChar(4, customChar4a);
lcd.createChar(5, customChar4b);
lcd.createChar(6, customChar5a);
lcd.createChar(7, customChar5b);

// Rising part of the jumping action, for more natural movements, many frames are drawn
float a = 11; 
// Sets the position at which the piece starts rising.
lcd.setCursor(a, 1);
lcd.write((uint8_t)0);
lcd.setCursor(a, 0);
lcd.write(2);
delay(y - 100);
lcd.setCursor(a, 1);
lcd.write(3);
delay(y - 100);
lcd.setCursor(a, 0);
lcd.write(4);
lcd.setCursor(a, 1);
lcd.write(5);
delay(y - 100);
lcd.setCursor(a, 0);
lcd.write(6);
lcd.setCursor(a, 1);
lcd.write(7);
delay(y - 100);
lcd.setCursor(a, 0);
lcd.write((uint8_t)0);
lcd.setCursor(a, 1);
lcd.write(1);
delay(y - 100);


/*The piece moving horizontally in the upper row, representing the part where the piece is in air.
The for loops simplifies the code, as the character that needs to be printed doesn't change, unlike that in the rising and falling parts.
As Arduino only supports 8 custom characters, the horizontal movement cannot be improved by using multiple frames
*/

for (int a = 11; a < 14; a += 1) {
lcd.setCursor(a, 0);
lcd.write((uint8_t)0);
lcd.setCursor(a, 0);
delay(y);
lcd.write(1);
}


// Falling part of the animation, effectively it's is the mirror image of the rising part, representing the piece falling to the ground 
float b = 14;
// Sets the position at which the piece starts to fall
lcd.setCursor(b, 0);
lcd.write(6);
lcd.setCursor(b, 1);
lcd.write(7);
delay(y - 100);
lcd.setCursor(b, 0);
lcd.write(4);
lcd.setCursor(b, 1);
lcd.write(5);
delay(y - 100);
lcd.setCursor(b, 0);
lcd.write(2);
lcd.setCursor(b, 1);
lcd.write(3);
delay(y - 100);
lcd.setCursor(b, 0);
lcd.write(1);
lcd.setCursor(b, 1);
lcd.write((uint8_t)0);
delay(y);
}


