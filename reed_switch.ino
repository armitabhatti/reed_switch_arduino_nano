// Arduino Nano and Reed Switch to Control NPN Transistor

// Pin definitions
#define base 13 // Pin connected to the base of the NPN transistor
#define reed 3  // Pin connected to the non-grounded lead of the reed switch

// Variables
volatile bool reed_closed = false; // State variable to indicate if the reed switch is closed
unsigned long last_state_change = 0; // Tracks the last time the reed switch state changed for debouncing

void setup() {
  // Configure reed pin with an internal pull-up resistor
  // This ensures the pin reads HIGH when the reed switch is open and LOW when closed
  pinMode(reed, INPUT_PULLUP);

  // Configure the base pin as an output to control the transistor
  pinMode(base, OUTPUT);

  // Start with the base pin HIGH, so the transistor is conducting by default
  digitalWrite(base, HIGH);

  // Attach an interrupt to detect a falling edge (reed switch closing)
  // Calls the handle_reed function whenever the reed switch closes
  attachInterrupt(digitalPinToInterrupt(reed), handle_reed, FALLING);
}

// Main loop continuously checks the reed switch state and controls the transistor
void loop() {
  // If the reed switch is detected as closed by the interrupt
  if (reed_closed) {
    // Turn off the transistor by setting the base pin LOW
    digitalWrite(base, LOW);
    // Keep the transistor off for 5 seconds
    delay(5000);
    // Turn the transistor back on by setting the base pin HIGH
    digitalWrite(base, HIGH);
    // Reset the reed switch state to open for the next detection
    reed_closed = false;
  }
}

// Interrupt Service Routine (ISR) for handling the reed switch
void handle_reed() {
  // Get the current time for debouncing logic
  unsigned long current_time = millis();
  
  // Debounce: Ignore signals that occur within 50 milliseconds of the last change
  if (current_time - last_state_change > 50) {
    last_state_change = current_time; // Update the last state change time
    reed_closed = true; // Mark the reed switch as closed
  }
}
