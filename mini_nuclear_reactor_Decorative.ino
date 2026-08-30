/**
 * @file Arm_Decorative_Cyberpunk.ino
 * @author Rez.R
 * @brief Dual-Channel Advanced Lighting Controller for Cyberpunk Arm Prop.
 * @version 2.0.0
 * @date 2026-08-29
 *
 * @channel YouTube: 5M Workshop
 *
 * ============================================================================
 * PROJECT SPECIFICATIONS
 * ============================================================================
 * @project     Arm Decorative Cyberpunk
 * @description Advanced wearable lighting firmware featuring two fully
 *              independent lighting channels, multiple dynamic effects,
 *              menu-based effect selection, power management and audio
 *              feedback.
 *
 * @platform    Arduino Ecosystem (ATmega328P / Nano / Uno)
 *
 * @hardware
 *              - PWM LED Channel:
 *                30x Blue 1206 SMD LEDs
 *                Controlled through a PNP transistor
 *                PWM Output: Pin 11
 *
 *              - Digital LED Channel:
 *                8x Blue LEDs
 *                Logic: Digital HIGH / LOW
 *                Outputs: Pins 13, 3, 4, 6, 7, 8, 9, 10
 *
 *              - User Input:
 *                Tactile Button
 *                Pin 2
 *                Logic: Active-Low
 *                Internal Pull-Up enabled
 *
 *              - Audio Feedback:
 *                Active Buzzer
 *                Pin 12
 *
 * ============================================================================
 * LIGHTING CHANNELS
 * ============================================================================
 * @channel PWM
 *              Dedicated PWM-controlled channel for the 30x SMD LED array.
 *              Supports brightness-based dynamic effects.
 *
 * @channel DIGITAL
 *              Eight individually addressable digital outputs operating as
 *              a synchronized lighting group.
 *
 * @note        Both lighting channels operate independently during normal
 *              operation. Changing the effect of one channel does not affect
 *              the timing or effect state of the other channel.
 *
 * ============================================================================
 * OPERATIONAL EFFECTS
 * ============================================================================
 * @effect 0    STATIC
 *              Constant illumination.
 *
 * @effect 1    HEARTBEAT
 *              Dual-pulse rhythmic lighting effect.
 *
 * @effect 2    FLICKER
 *              Randomized brightness fluctuation.
 *
 * @effect 3    LIGHTNING
 *              Random high-intensity lightning-style sequence.
 *
 * @effect 4    FADE
 *              Smooth breathing/fading brightness effect.
 *
 * @effect 5    BLINK
 *              Regular pulsed flashing.
 *
 * @effect 6    SPECIAL
 *              Custom multi-level lighting sequence.
 *
 * ============================================================================
 * DIGITAL EFFECTS
 * ============================================================================
 * @effect 0    STATIC
 *              All eight LEDs continuously ON.
 *
 * @effect 1    BLINK
 *              All eight LEDs synchronized flashing.
 *
 * @effect 2    RUN_LEFT
 *              Sequential left-to-right LED movement.
 *
 * @effect 3    RUN_RIGHT
 *              Sequential right-to-left LED movement.
 *
 * @effect 4    CENTER_OUT
 *              Sequential expansion from the center.
 *
 * @effect 5    OUTSIDE_IN
 *              Sequential movement from the outer LEDs toward the center.
 *
 * @effect 6    RANDOM
 *              Randomized single-LED activation.
 *
 * ============================================================================
 * CONTROL SYSTEM
 * ============================================================================
 * @logic Power ON/OFF
 *              Hold the button for 4 seconds to toggle the complete system
 *              power state.
 *
 * @logic Short Press
 *              Changes the currently selected lighting effect.
 *              The newly selected effect is previewed immediately.
 *
 * @logic Menu Entry
 *              Hold the button for 2 seconds while the system is ON to enter
 *              the effect selection menu.
 *
 * @logic Channel Selection
 *              The PWM channel is indicated by blinking the PWM LED array.
 *              The digital channel is indicated by synchronized blinking of
 *              all eight digital LEDs.
 *
 * @logic Menu Navigation
 *              A long press while inside the menu switches between the PWM
 *              and Digital LED channels.
 *
 * @logic Effect Preview
 *              A short press immediately activates the next effect, allowing
 *              the user to visually evaluate the selected effect in real time.
 *
 * @logic Menu Timeout
 *              If no button activity occurs for 10 seconds, the system exits
 *              the selection menu and returns to normal operation.
 *
 * ============================================================================
 * AUDIO FEEDBACK
 * ============================================================================
 * @audio Short Beep
 *              Used for effect selection and menu interaction.
 *
 * @audio Double Beep
 *              Used for channel transitions and menu timeout notification.
 *
 * @audio Long Beep
 *              Used to indicate complete system power ON/OFF.
 *
 * ============================================================================
 * POWER STATES
 * ============================================================================
 * @state OFF
 *              All LED channels are completely disabled.
 *              Short presses and menu commands are ignored.
 *              Only a 4-second button hold can reactivate the system.
 *
 * @state ON
 *              Both lighting channels operate according to their individually
 *              selected effects.
 *
 * ============================================================================
 * DESIGN NOTES
 * ============================================================================
 * @note        The firmware is designed around a non-blocking millis()-based
 *              timing architecture for independent effect execution.
 *
 * @note        The PWM channel is inverted to accommodate the PNP transistor
 *              switching stage.
 *
 * @note        Internal Pull-Up is used for the tactile button, therefore the
 *              button is considered active when the input is LOW.
 *
 * @copyright   Copyright (c) 2026 Rez.R (5M Workshop). All rights reserved.
 * ============================================================================
 */
#define PWM_PIN 11
#define BUTTON_PIN 2
#define BUZZER_PIN 12

const byte ledPins[8] = {
  13, 3, 4, 6, 7, 8, 9, 10
};

#define PWM_INVERTED true

const unsigned long SELECT_TIME = 2000UL;
const unsigned long POWER_TIME = 4000UL;
const unsigned long MENU_TIMEOUT = 10000UL;
const unsigned long DEBOUNCE_TIME = 30UL;

bool systemOn = true;

bool rawButton = HIGH;
bool stableButton = HIGH;

unsigned long buttonChangedTime = 0;
unsigned long pressStartTime = 0;

bool selectActionDone = false;
bool powerActionDone = false;

enum MenuMode {
  NORMAL,
  PWM_SELECT,
  DIGITAL_SELECT
};

MenuMode menuMode = NORMAL;

unsigned long menuTimer = 0;

const byte PWM_EFFECT_COUNT = 7;
const byte DIGITAL_EFFECT_COUNT = 7;

byte pwmEffect = 0;
byte digitalEffect = 0;

bool pwmPreview = false;
bool digitalPreview = false;

unsigned long pwmTimer = 0;
byte pwmStep = 0;

int fadeValue = 0;
int fadeDirection = 5;

bool pwmToggle = false;

unsigned long digitalTimer = 0;
byte digitalStep = 0;

bool digitalToggle = false;

enum BuzzerMode {
  BUZZER_OFF,
  BUZZER_SINGLE,
  BUZZER_DOUBLE,
  BUZZER_LONG
};

BuzzerMode buzzerMode = BUZZER_OFF;
unsigned long buzzerTimer = 0;


void setup() {

  pinMode(PWM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  for (byte i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  writePWM(0);

  digitalWrite(BUZZER_PIN, LOW);

  randomSeed(analogRead(A0));

  resetPWM();
  resetDigital();

  Serial.begin(9600);
}


void loop() {

  handleButton();

  updateBuzzer();

  if (!systemOn) {
    allLEDsOff();
    return;
  }

  if (menuMode == PWM_SELECT) {

    if (!pwmPreview) {
      showPWMSelection();
    }
    else {
      runPWMEffect();
    }

    return;
  }

  if (menuMode == DIGITAL_SELECT) {

    if (!digitalPreview) {
      showDigitalSelection();
    }
    else {
      runDigitalEffect();
    }

    return;
  }

  runPWMEffect();
  runDigitalEffect();
}


void handleButton() {

  bool reading = digitalRead(BUTTON_PIN);
  unsigned long now = millis();

  if (reading != rawButton) {
    rawButton = reading;
    buttonChangedTime = now;
  }

  if ((now - buttonChangedTime) >= DEBOUNCE_TIME) {

    if (stableButton != rawButton) {

      stableButton = rawButton;

      if (stableButton == LOW) {

        pressStartTime = now;

        selectActionDone = false;
        powerActionDone = false;
      }

      else {

        unsigned long pressTime = now - pressStartTime;

        if (systemOn &&
            !selectActionDone &&
            !powerActionDone &&
            pressTime < SELECT_TIME) {

          shortPress();
        }
      }
    }
  }

  if (stableButton == LOW) {

    unsigned long pressTime = now - pressStartTime;

    if (pressTime >= POWER_TIME &&
        !powerActionDone) {

      powerActionDone = true;

      togglePower();

      return;
    }

    if (!systemOn) {
      return;
    }

    if (pressTime >= SELECT_TIME &&
        !selectActionDone) {

      selectActionDone = true;

      longPressMenu();
    }
  }

  if (menuMode != NORMAL) {

    if (now - menuTimer >= MENU_TIMEOUT) {

      menuMode = NORMAL;

      pwmPreview = false;
      digitalPreview = false;

      resetPWM();
      resetDigital();

      beepDouble();
    }
  }
}


void shortPress() {

  menuTimer = millis();

  if (menuMode == PWM_SELECT) {

    pwmEffect++;

    if (pwmEffect >= PWM_EFFECT_COUNT) {
      pwmEffect = 0;
    }

    pwmPreview = true;

    resetPWM();

    beepShort();

    return;
  }

  if (menuMode == DIGITAL_SELECT) {

    digitalEffect++;

    if (digitalEffect >= DIGITAL_EFFECT_COUNT) {
      digitalEffect = 0;
    }

    digitalPreview = true;

    resetDigital();

    beepShort();

    return;
  }
}


void longPressMenu() {

  menuTimer = millis();

  if (menuMode == NORMAL) {

    menuMode = PWM_SELECT;

    pwmPreview = false;
    digitalPreview = false;

    resetPWM();
    resetDigital();

    beepShort();

    return;
  }

  if (menuMode == PWM_SELECT) {

    menuMode = DIGITAL_SELECT;

    pwmPreview = false;
    digitalPreview = false;

    resetPWM();
    resetDigital();

    beepDouble();

    return;
  }

  if (menuMode == DIGITAL_SELECT) {

    menuMode = PWM_SELECT;

    pwmPreview = false;
    digitalPreview = false;

    resetPWM();
    resetDigital();

    beepShort();

    return;
  }
}


void togglePower() {

  systemOn = !systemOn;

  menuMode = NORMAL;

  pwmPreview = false;
  digitalPreview = false;

  if (!systemOn) {

    allLEDsOff();

    resetPWM();
    resetDigital();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);

  }
  else {

    resetPWM();
    resetDigital();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
  }
}


void writePWM(int value) {

  value = constrain(value, 0, 255);

#if PWM_INVERTED

  analogWrite(PWM_PIN, 255 - value);

#else

  analogWrite(PWM_PIN, value);

#endif
}


void runPWMEffect() {

  unsigned long now = millis();

  switch (pwmEffect) {

    case 0:

      writePWM(240);

      break;

    case 1:

      if (pwmStep == 0) {

        writePWM(240);

        if (now - pwmTimer >= 100) {
          pwmStep = 1;
          pwmTimer = now;
        }
      }

      else if (pwmStep == 1) {

        writePWM(0);

        if (now - pwmTimer >= 50) {
          pwmStep = 2;
          pwmTimer = now;
        }
      }

      else if (pwmStep == 2) {

        writePWM(240);

        if (now - pwmTimer >= 100) {
          pwmStep = 3;
          pwmTimer = now;
        }
      }

      else {

        writePWM(0);

        if (now - pwmTimer >= 800) {
          pwmStep = 0;
          pwmTimer = now;
        }
      }

      break;

    case 2:

      if (now - pwmTimer >= 40) {

        pwmTimer = now;

        writePWM(random(100, 241));
      }

      break;

    case 3:

      if (pwmStep == 0) {

        writePWM(0);

        if (now - pwmTimer >= 3000) {
          pwmStep = 1;
          pwmTimer = now;
        }
      }

      else if (pwmStep == 1) {

        writePWM(240);

        if (now - pwmTimer >= 60) {
          pwmStep = 2;
          pwmTimer = now;
        }
      }

      else {

        writePWM(random(50, 200));

        if (now - pwmTimer >= 200) {

          writePWM(0);

          pwmStep = 0;
          pwmTimer = now;
        }
      }

      break;

    case 4:

      if (now - pwmTimer >= 20) {

        pwmTimer = now;

        fadeValue += fadeDirection;

        if (fadeValue >= 240) {
          fadeValue = 240;
          fadeDirection = -5;
        }

        if (fadeValue <= 0) {
          fadeValue = 0;
          fadeDirection = 5;
        }

        writePWM(fadeValue);
      }

      break;

    case 5:

      if (now - pwmTimer >= 500) {

        pwmTimer = now;

        pwmToggle = !pwmToggle;

        writePWM(
          pwmToggle ? 240 : 0
        );
      }

      break;

    case 6:

      if (now - pwmTimer >= 150) {

        pwmTimer = now;

        pwmStep++;

        if (pwmStep >= 3) {
          pwmStep = 0;
        }

        if (pwmStep == 0) {
          writePWM(240);
        }
        else if (pwmStep == 1) {
          writePWM(50);
        }
        else {
          writePWM(0);
        }
      }

      break;
  }
}


void runDigitalEffect() {

  unsigned long now = millis();

  switch (digitalEffect) {

    case 0:

      setAllDigital(HIGH);

      break;

    case 1:

      if (now - digitalTimer >= 500) {

        digitalTimer = now;

        digitalToggle = !digitalToggle;

        setAllDigital(
          digitalToggle ? HIGH : LOW
        );
      }

      break;

    case 2:

      if (now - digitalTimer >= 120) {

        digitalTimer = now;

        setAllDigital(LOW);

        digitalWrite(
          ledPins[digitalStep],
          HIGH
        );

        digitalStep++;

        if (digitalStep >= 8) {
          digitalStep = 0;
        }
      }

      break;

    case 3:

      if (now - digitalTimer >= 120) {

        digitalTimer = now;

        setAllDigital(LOW);

        digitalWrite(
          ledPins[7 - digitalStep],
          HIGH
        );

        digitalStep++;

        if (digitalStep >= 8) {
          digitalStep = 0;
        }
      }

      break;

    case 4:

      if (now - digitalTimer >= 200) {

        digitalTimer = now;

        setAllDigital(LOW);

        if (digitalStep == 0) {

          digitalWrite(ledPins[3], HIGH);
          digitalWrite(ledPins[4], HIGH);
        }

        else if (digitalStep == 1) {

          digitalWrite(ledPins[2], HIGH);
          digitalWrite(ledPins[5], HIGH);
        }

        else if (digitalStep == 2) {

          digitalWrite(ledPins[1], HIGH);
          digitalWrite(ledPins[6], HIGH);
        }

        else {

          digitalWrite(ledPins[0], HIGH);
          digitalWrite(ledPins[7], HIGH);
        }

        digitalStep++;

        if (digitalStep >= 4) {
          digitalStep = 0;
        }
      }

      break;

    case 5:

      if (now - digitalTimer >= 200) {

        digitalTimer = now;

        setAllDigital(LOW);

        digitalWrite(
          ledPins[digitalStep],
          HIGH
        );

        digitalWrite(
          ledPins[7 - digitalStep],
          HIGH
        );

        digitalStep++;

        if (digitalStep >= 4) {
          digitalStep = 0;
        }
      }

      break;

    case 6:

      if (now - digitalTimer >= 150) {

        digitalTimer = now;

        setAllDigital(LOW);

        byte r = random(0, 8);

        digitalWrite(
          ledPins[r],
          HIGH
        );
      }

      break;
  }
}


void showPWMSelection() {

  unsigned long now = millis();

  setAllDigital(LOW);

  if (now - pwmTimer >= 400) {

    pwmTimer = now;

    pwmToggle = !pwmToggle;

    writePWM(
      pwmToggle ? 240 : 0
    );
  }
}


void showDigitalSelection() {

  unsigned long now = millis();

  writePWM(0);

  if (now - digitalTimer >= 400) {

    digitalTimer = now;

    digitalToggle = !digitalToggle;

    setAllDigital(
      digitalToggle ? HIGH : LOW
    );
  }
}


void resetPWM() {

  pwmTimer = millis();

  pwmStep = 0;

  fadeValue = 0;

  fadeDirection = 5;

  pwmToggle = false;

  writePWM(0);
}


void resetDigital() {

  digitalTimer = millis();

  digitalStep = 0;

  digitalToggle = false;

  setAllDigital(LOW);
}


void allLEDsOff() {

  writePWM(0);

  setAllDigital(LOW);
}


void setAllDigital(byte state) {

  for (byte i = 0; i < 8; i++) {

    digitalWrite(
      ledPins[i],
      state
    );
  }
}


void beepShort() {

  buzzerMode = BUZZER_SINGLE;

  buzzerTimer = millis();

  digitalWrite(
    BUZZER_PIN,
    HIGH
  );
}


void beepDouble() {

  buzzerMode = BUZZER_DOUBLE;

  buzzerTimer = millis();

  digitalWrite(
    BUZZER_PIN,
    HIGH
  );
}


void beepLong() {

  buzzerMode = BUZZER_LONG;

  buzzerTimer = millis();

  digitalWrite(
    BUZZER_PIN,
    HIGH
  );
}


void updateBuzzer() {

  unsigned long now = millis();

  if (buzzerMode == BUZZER_SINGLE) {

    if (now - buzzerTimer >= 100) {

      digitalWrite(
        BUZZER_PIN,
        LOW
      );

      buzzerMode = BUZZER_OFF;
    }

    return;
  }


  if (buzzerMode == BUZZER_DOUBLE) {

    unsigned long t =
      now - buzzerTimer;

    if (t < 100) {

      digitalWrite(
        BUZZER_PIN,
        HIGH
      );
    }

    else if (t < 200) {

      digitalWrite(
        BUZZER_PIN,
        LOW
      );
    }

    else if (t < 300) {

      digitalWrite(
        BUZZER_PIN,
        HIGH
      );
    }

    else {

      digitalWrite(
        BUZZER_PIN,
        LOW
      );

      buzzerMode = BUZZER_OFF;
    }

    return;
  }


  if (buzzerMode == BUZZER_LONG) {

    if (now - buzzerTimer >= 1000) {

      digitalWrite(
        BUZZER_PIN,
        LOW
      );

      buzzerMode = BUZZER_OFF;
    }

    return;
  }
}