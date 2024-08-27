// Pin Definitions
const int ledPin = LED_BUILTIN;  // Built-in LED
const int buttonPin = 2;         // Push button connected to pin 2

// Morse code timing
const int dotDuration = 200;     // Duration of a dot
const int dashDuration = dotDuration * 3; // Duration of a dash
const int letterSpace = dotDuration * 3;  // Space between letters
const int wordSpace = dotDuration * 7;    // Space between words

// Morse code for "Aruj"
String morseCode = ".- .-. ..- .--- ";  // Morse code for Aruj
String name = "Aruj"; // Name in alphabetic form

bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);        // Initialize the LED pin as an output
  pinMode(buttonPin, INPUT_PULLUP); // Initialize the button pin as an input with pull-up resistor
  Serial.begin(9600);             // Start the serial communication
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Print name in alphabetic form
    Serial.print("Name: ");
    Serial.println(name);
    
    // Print name in Morse code
    Serial.print("Morse Code: ");
    Serial.println(morseCode);
    
    // Blink Morse code
    for (int i = 0; i < morseCode.length(); i++) {
      if (morseCode[i] == '.') {
        blinkDot();
      } else if (morseCode[i] == '-') {
        blinkDash();
      } else if (morseCode[i] == ' ') {
        delay(wordSpace);
      }

      // Check if the button is pressed again to reset
      if (digitalRead(buttonPin) == LOW) {
        i = -1; // Reset the loop index to start from the beginning
      }
    }
  }

  lastButtonState = buttonState;
}

void blinkDot() {
  digitalWrite(ledPin, HIGH);
  delay(dotDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
}

void blinkDash() {
  digitalWrite(ledPin, HIGH);
  delay(dashDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
}