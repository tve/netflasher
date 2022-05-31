NetFlasher2 - Emit a flash when a bird hits a net
================================================

The NetFlasher2 is a simple circuit to flash an LED when a bird hits a net and is used
in experiments to determine which window coverings are best to deter birds from flying
into them.

The netflasher detects when a bird hits a net using a vibration sensing switch.
It then briefly turns on a bright LED which is placed in the viewfield of a camera
recording the experiment.
This provides an unmistakable "hit" signal.

Instructions
------------

- Use micro-USB cable to power the netflasher (uses <80mA)
- The flashlight lights for 200ms when the vibration sensor is triggered
- After triggering, the flash is locked-out for 10s
- Tiny orange LED: slow (2s) = ready; fast (200ms) = 10s lock-out
- Red LED: vib sw detected (always active, even during lock-out)
- Green LED: display sensitivity (blink 1x/2x/3x/4x), 1=most, 4=least
- Blue sw:
  - first press: display sensitivity & reset lock-out
  - press again within 2s: change sensitivity (1/2/3/4)
- White sw next to microcontroller: program mode (cut power to exit)

Circuit Notes
-------------

### Net motion detector

- Vibration detection switch "SW-18010" https://www.adafruit.com/product/1766
- Sensitivity is adjusted by requiring toggles in multiple 10ms windows: sensitivity 1
  requires just one toggle, 2 requires toggles in 2x 10ms within 100ms of one-another,
  3 requires toggles in 2^2 = 4x 10ms intervals, 4 requires 8x 10ms intervals 

### Sensitivity switch

- Pushbutton to change sensitivity
- On-board sensitivity indicator LED (1, 2, 3, or 4 flashes)

### High power Cree LED

- Amber 5mm LED: C503B-ABS, 4430mcd @20mA, 40mA max continuous
- drive using 2N4401 w/82Ω -> ~40mA

### High power "flash" LED

(Not used due to size concerns)

- Wire-up raw flashlight LED
  - 135mA@2.8V=1/3W
  - 220mA@2.85V=0.6W
- 2N4401: 600mA max
  - resistor emitter-ground: 10Ω->1W flash
- On-board indicator LED
- To disassemble flashlight:
  - pull zoom ring hard and unscrew, remove driver
  - remove btn cap, punch btn through alum housing

Arduino
-------

To use `arduino-cli` install according to first post:
- https://forum.pjrc.com/threads/53548-Arduino-CLI-Alpha-Release-Teensy-Support
Instead of `arduino-cli core install teensy:avr` run:
- `arduino-cli core update-index`
- `arduino-cli core install teensy:arm`
