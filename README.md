# Mini-reactor
```text
###############################################################################
#                                                                             #
#      ███╗   ███╗██╗███╗   ██╗██╗        ███╗   ██╗██╗   ██╗ ██████╗         #
#      ████╗ ████║██║████╗  ██║██║        ████╗  ██║██║   ██║██╔════╝         #
#      ██╔████╔██║██║██╔██╗ ██║██║        ██╔██╗ ██║██║   ██║██║              #
#      ██║╚██╔╝██║██║██║╚██╗██║██║        ██║╚██╗██║██║   ██║██║              #
#      ██║ ╚═╝ ██║██║██║ ╚████║██║        ██║ ╚████║╚██████╔╝╚██████╔╝        #
#      ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝        ╚═╝  ╚═══╝ ╚═════╝  ╚═════╝         #
#                                                                             #
#                         [ 5M WORKSHOP ]                                     #
###############################################################################

PROJECT: Mini Nuclear Reactor Decorative
VERSION: 1.0.0
AUTHOR:  Rez.R
DATE:    2026-08-30

===============================================================================
[ PROJECT OVERVIEW ]
===============================================================================
A compact decorative Mini Nuclear Reactor prop featuring a futuristic 
reactor-core lighting system.

The project is designed as a visual sci-fi / cyberpunk prop and uses 
multiple blue LEDs to simulate the glowing energy core of a miniature 
nuclear reactor.

The lighting controller generates dynamic illumination patterns to create
the appearance of an active and unstable reactor core while maintaining
simple and reliable hardware control.

===============================================================================
[ HARDWARE CONFIGURATION ]
===============================================================================
- MCU:        Arduino Nano (ATmega328P)
- CORE LEDs:  8x Blue 3mm LEDs
- SMD LEDs:   ~30x Blue 1206 SMD LEDs (Parallel Array)
- CONTROL:    Arduino Digital / PWM Outputs
- LOGIC:      5V TTL

NOTE:
The LED configuration should use appropriate current-limiting resistors.
Do not connect a large parallel LED array directly to a single Arduino
GPIO pin if the total current exceeds the ATmega328P pin limits.

===============================================================================
[ OPERATIONAL LOGIC ]
===============================================================================
The reactor lighting system is controlled by a software-based animation
engine designed to simulate the behavior of an energized reactor core.

The firmware controls multiple lighting channels and generates dynamic
patterns to create a realistic reactor effect.

[ REACTOR STATES ]

* STANDBY:
  Reactor core remains inactive with all illumination disabled.

* STARTUP:
  LEDs gradually increase in intensity to simulate reactor activation.

* ACTIVE:
  Core illumination remains active with controlled dynamic brightness.

* ENERGY PULSE:
  Periodic brightness pulses simulate energy circulating through
  the reactor core.

* UNSTABLE:
  Randomized flickering and rapid intensity changes simulate an
  unstable reactor condition.

* OVERLOAD:
  High-frequency flashing sequence representing a critical reactor
  energy state.

* SHUTDOWN:
  Controlled fade-out sequence returning the reactor to standby.

===============================================================================
[ LIGHTING MODES ]
===============================================================================
- OFF         : Reactor completely inactive
- STARTUP     : Progressive reactor ignition sequence
- ACTIVE      : Stable glowing reactor core
- PULSE       : Periodic energy pulse animation
- FLICKER     : Randomized reactor instability effect
- OVERLOAD    : High-energy warning animation
- SHUTDOWN    : Controlled reactor power-down sequence

===============================================================================
[ INSTALLATION & UPLOAD GUIDE ]
===============================================================================
1. Open the "Mini_Nuclear_Reactor_Decorative_5MWORKSHOP.ino" file
   in the Arduino IDE.

2. Connect the Arduino Nano to your computer using USB.

3. Select:
   Tools > Board > Arduino Nano

4. Select the appropriate processor if required:
   Tools > Processor > ATmega328P

5. Select the correct COM Port:
   Tools > Port > COMx

6. Verify the hardware connections and LED current-limiting resistors.

7. Click "Upload".

===============================================================================
[ HARDWARE SAFETY ]
===============================================================================
IMPORTANT:

The Arduino Nano GPIO pins are NOT designed to directly drive a large
parallel LED array.

For the ~30x 1206 LED array, use an appropriate transistor/MOSFET driver
stage and a suitable power supply.

Each LED branch should have proper current limiting.

The 8x individual 3mm LEDs should also be operated within their rated
forward current.

===============================================================================
[ PROJECT FEATURES ]
===============================================================================
- Arduino Nano based controller
- Futuristic nuclear reactor aesthetic
- Blue reactor-core illumination
- Multiple dynamic lighting effects
- Startup and shutdown animations
- Energy pulse simulation
- Reactor instability simulation
- Expandable control architecture
- Suitable for sci-fi, cyberpunk and prop-building projects

===============================================================================
[ CONNECT WITH US ]
===============================================================================
For tutorials, schematics, electronics projects, and Cyberpunk DIY builds,
subscribe to:

YouTube: 5M WORKSHOP
Link: https://www.youtube.com/@5METERWORKSHOP

Copyright (c) 2026 Rez.R (5M Workshop). All rights reserved.
===============================================================================
```

