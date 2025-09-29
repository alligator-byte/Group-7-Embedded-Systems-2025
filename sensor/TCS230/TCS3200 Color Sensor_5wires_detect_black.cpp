#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();


//USES 5 wires at max
#define S0 10
#define S1 11
#define S2 12
#define S3 13
#define OUT 16

#define BLACK_THRESHOLD 8000       // Adjust based on your calibration
#define TIMEOUT_PULSE 20000        // 20 ms timeout for pulseIn

unsigned long pulseDuration;
unsigned int frequency;

// Helper function to update TFT screen
void displayMessage(const char* msg, uint16_t bgColor, uint16_t textColor) {
  tft.fillScreen(bgColor);
  tft.setTextColor(textColor, bgColor);
  tft.setCursor(10, 50);
  tft.println(msg);
}


void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Initialize TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Set frequency scaling to 100% for max frequency output
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  // Select Clear filter (no color filter)
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
}

void loop() {
  // Read pulse duration from sensor output (LOW pulse length)
  pulseDuration = pulseIn(OUT, LOW, TIMEOUT_PULSE);

  // Handle timeout (no pulse detected)
  if (pulseDuration == 0) {
    pulseDuration = TIMEOUT_PULSE;
  }

  // Calculate frequency in Hz
  frequency = 1000000UL / pulseDuration;

  Serial.print("Frequency: ");
  Serial.println(frequency);

  // Determine color detection
  if (frequency > 50000) {
    Serial.println("No valid reading");
    displayMessage("NO READ", TFT_YELLOW, TFT_BLACK);
  } else if (frequency < BLACK_THRESHOLD) {
    Serial.println("Black detected");
    displayMessage("BLACK", TFT_BLACK, TFT_WHITE);
  } else {
    Serial.println("White detected");
    displayMessage("WHITE", TFT_WHITE, TFT_BLACK);
  }

  delay(300); // small delay to stabilize readings
}

/**
 * Good question! The TCS230/TCS3200 sensor typically needs at least 5 pins to function properly:

S0, S1 — frequency scaling control (2 pins)

S2, S3 — color filter selection (2 pins)

OUT — output frequency pulse (1 pin)

That's 5 pins total.

Can you reduce the number of pins used?

Frequency scaling (S0, S1):
You could hardwire these pins to fixed values (for example, both HIGH for 100% scaling) instead of controlling them via MCU pins. This saves 2 GPIOs but you lose runtime control over frequency scaling.

Color filter selection (S2, S3):
Same idea here. If you only care about one filter (like the clear filter for black/white detection), you can tie S2 and S3 directly to GND or VCC to permanently select that filter. This would save 2 GPIOs.

Minimal pin setup for fixed operation:
Signal	Connection
S0	tied HIGH (5V or 3.3V)
S1	tied HIGH
S2	tied LOW
S3	tied LOW
OUT	connected to one GPIO

In this way, you only use 1 GPIO pin to read frequency output.

What you lose by this?

No dynamic control of frequency scaling (but 100% scaling is usually fine).

No ability to switch color filters in code (only detecting clear filter output).

So for your use case — detecting black/white only — this minimal wiring is likely enough.
Summary:
Pins needed	Use GPIO?	Alternative
S0	No	Tie to HIGH (3.3V)
S1	No	Tie to HIGH (3.3V)
S2	No	Tie to LOW (GND)
S3	No	Tie to LOW (GND)
OUT	Yes (1 pin)	Connect to one ADC/digital pin for reading

If you want, I can help you adjust your code for this single-pin read mode and recommend how to wire the sensor accordingly! Would you like that?

You said:
wait summaries to I can understand how reading only bw would work with this
ChatGPT said:

Absolutely! Here’s a simple summary to explain how you can detect black and white using the TCS230 sensor with minimum pins:

How to detect black and white with TCS230 using just one input pin:
1. Wiring (only 1 input pin needed on your ESP32):

S0 = HIGH (tie to 3.3V) — sets frequency scaling to 100% (max output frequency)

S1 = HIGH (tie to 3.3V)

S2 = LOW (tie to GND) — selects Clear filter (no color filtering)

S3 = LOW (tie to GND)

OUT — connect to ONE ESP32 GPIO pin (input) for frequency reading

2. How it works:

The sensor’s output pin (OUT) will produce a frequency signal — the frequency changes depending on the amount of light reflected from the surface.

White surface = more reflected light = higher frequency

Black surface = less reflected light = lower frequency

3. What your code does:

Measures the frequency on the OUT pin by timing the pulse width.

Compares frequency to a threshold:

If frequency > threshold → White detected

Else → Black detected

4. What you lose by doing this:

You cannot change frequency scaling dynamically (fixed at 100%).

You cannot select color filters — only the clear filter is used.

But for basic black/white detection, this is totally fine.

So, with this setup:

You use only 1 GPIO pin to read the sensor output.

All control pins are hardwired to fixed levels for simple operation.

Your code reads frequency and tells if the surface is black or white.

If you want, I can help you with a simple code version that works for this exact wiring and functionality. Just say the word!
 */
