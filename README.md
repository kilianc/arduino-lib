# Arduino / Atmega* multi purpose library

## Index

* [\<arduino-lib.h\>](#arduino-libh)
* [\<simple_button.h\>](#simple_buttonh)
  * [`simple_button_t`](#simple_button_t)
  * [`simple_button_set`](#simple_button_set)
  * [`simple_button_read`](#simple_button_read)
* [\<simple_timer.h\>](#simple_timerh)
  * [`___SIMPLE_TIMER_SLOTS___`](#___SIMPLE_TIMER_SLOTS___)
  * [`set_repeat`](#set_repeat)
  * [`set_interval`](#set_interval)
  * [`set_timeout`](#set_timeout)
  * [`update_timers`](#update_timers)
  * [`clear_timer`](#clear_timer)
* [\<oscillate.h\>](#oscillateh)
  * [`oscillate`](#oscillate)
* [\<utils.h\>](#utilsh)
  * [`free_ram`](#free_ram)
  * [`seconds_to_digits`](#seconds_to_digits)
* [\<SerialLCD.h\>](#SerialLCDh)
  * [`SerialLCD`](#SerialLCD)
  * [`begin`](#begin)
  * [`toggleSplash`](#toggleSplash)
  * [`clearDisplay`](#clearDisplay)
  * [`debugLevel`](#debugLevel)
  * [`backLight`](#backLight)
  * [`eraseBlock`](#eraseBlock)
  * [`reset`](#reset)
  * [`baudRate`](#baudRate)
  * [`setPixel`](#setPixel)
  * [`filledBox`](#filledBox)
  * [`reverseMode`](#reverseMode)
  * [`bitblt`](#bitblt)
  * [`placeCursor`](#placeCursor)
  * [`writeString`](#writeString)

# Installation

Download the library from [here](https://github.com/kilianc/arduino-lib/archive/master.zip) or
using git `git clone https://github.com/kilianc/arduino-lib`. On OSX/Linux create a symbolic link
to your Arduino folder:

    $ ln -s /path/to/arduino-lib/lib /path/to/Arduino/libraries/arduinolib

It's important that you use `arduinolib` and not `arduino-lib` as folder name because Arduino IDE
is a little too picky about libraries' name.

If you are on Windows *(and I feel terribly sorry for you btw)* you can always just copy/paste
the library in the right place.

# \<arduino-lib.h\>
All-in header file. Include this in your sketch and you are ready to use the whole library.

# \<simple_button.h\>
C style lightweight button library. Defines two functions, [`simple_button_set`](#simple_button_set)
[`simple_button_read`](#simple_button_read) and a type, [`simple_button_t`](#simple_button_t).

It's pretty straight forward to use, declare a [`simple_button_t`](#simple_button_t) variable and
after configuring it with [`simple_button_set`](#simple_button_set) you are ready to update its
status with [`simple_button_read`](#simple_button_read) inside your `loop` function.

It supports both **callbacks** and **conditional block** events style.

## Quick look

```c
#include <simple_button.h>

simple_button_t btn1;
simple_button_t btn2;

void setup() {
  simple_button_set(12, &btn1, HIGH, 1000, 200);
  simple_button_set(11, &btn2, HIGH, 2000, 100);

  btn1.rising_edge_cb = on_rising_edge;
  btn1.falling_edge_cb = on_falling_edge;
  btn1.click = on_click;
  btn1.hold_cb = on_hold;

  btn2.rising_edge_cb = on_rising_edge;
  btn2.falling_edge_cb = on_falling_edge;
  btn2.click = on_click;
  btn2.hold_cb = on_hold;
}

void on_rising_edge(simple_button_t *btn) {
  Serial.println("on_rising_edge " + String(btn == &btn1 ? "btn1" : "btn2"));
}

void on_falling_edge(simple_button_t *btn) {
  Serial.println("on_falling_edge " + String(btn == &btn1 ? "btn1" : "btn2"));
}

void on_click(simple_button_t *btn) {
  Serial.println("on_click " + String(btn == &btn1 ? "btn1" : "btn2"));
}

void on_hold(simple_button_t *btn) {
  Serial.println("on_hold " + String(btn == &btn1 ? "btn1" : "btn2"));
}

void print_status(simple_button_t btn, String btn_name) {
  // event flag
  Serial.print(btn_name + ".click " + btn.click);
  Serial.print(btn_name + ".rising_edge " + btn.rising_edge);
  Serial.print(btn_name + ".falling_edge " + btn.falling_edge);
  Serial.print(btn_name + ".hold " + btn.hold);
  // status flag
  Serial.print(btn_name + ".down " + btn.down);
  Serial.print(btn_name + ".up " + btn.up);
  Serial.print(btn_name + ".read " + btn.read);
}

void loop() {
  simple_button_read(&btn1);   // updates btn1 state and calls callbacks
  print_status(btn1, "btn1");  // prints flags and state for the button

  simple_button_read(&btn2);
  print_status(btn2, "btn2");

  // if you don't like callbacks just use flags
  if (btn1.click) {
    Serial.println("btn1.click no callback"); // capture changes from high to low
  }

  if (btn2.rising_edge || btn2.hold) {
    Serial.println("btn2 being hold!"); // capture the first press and every hold event
  }

  delay(50); //adjust this accordingly to your debounce time
}
```

## `simple_button_t`

```c
typedef struct simple_button_struct simple_button_t;
```

The type used by the library to store the button status across digital reads based on `simple_button_struct`.
Below the struct available data:

### Status (read only)
You should **never** change these values manually. They are supposed to be manipulated only
by [`simple_button_set`](#simple_button_set) and [`simple_button_read`](#simple_button_read) functions.

* **pin** `unsigned int`: Digital pin number. This is the [Arduino pin map index](http://arduino.cc/en/Hacking/PinMapping168).
* **read** `bool`: Digital I/O state from the microcontroller.
* **down** `bool`: Logical button state, 1 if the button **is pressed**.
* **up** `bool`: Logical button state, 1 if the button **is not pressed**.
* **hold_count** `unsigned int`: Counts how many hold events have been triggered, resets on `falling_edge`.

### Events flags (read only)
These flags are set to `0` or `1` after every [`simple_button_read`](#simple_button_read)
if the corrispondent event has been triggered. Every flag is set to `1` only once per event,
that means that you can use a conditional block to act consequently after an event.

* **rising_edge** `bool`: Triggers when the logical state changes from `LOW` to `HIGH` (button is pressed).
* **hold** `bool`: Triggers every `hold_frequency` ms after `hold_treshold` ms while holding the button down.
* **falling_edge** `bool`: Triggers when the logical state changes from `HIGH` to `LOW` (button is released).
* **click** `bool`: Triggers when the logical state changes quickly from `LOW` to `HIGH`.

### Events Callbacks (read write)
Callbacks are pointers to user defined functions that will be called passing the button target of the event
as actual parameter. If the function pointer is NULL the callback is ignored. Callbacks are called
after every [`simple_button_read`](#simple_button_read) if required. [Event flags](#events-flag-read-only) are still set.

* **rising_edge_cb** `void (*)(simple_button_t *)`: Called once when the `rising_edge` flag is `1`.
* **hold_cb** `void (*)(simple_button_t *)`: Called once when the `hold` flag is `1`.
* **falling_edge_cb** `void (*)(simple_button_t *)`: Called once when `the falling_edge` flag is `1`.
* **click_cb** `void (*)(simple_button_t *)`: Called once when the `click` flag is `1`.`

## `simple_button_set`

```c
void simple_button_set(unsigned int pin,
                       simple_button_t *button,
                       unsigned int mode,
                       unsigned long hold_treshold,
                       unsigned long hold_frequency);
```

* **pin** `unsigned int`:	 Digital pin number. This is the [Arduino pin map index](http://arduino.cc/en/Hacking/PinMapping168).
* **button** `simple_button_t`: Pointer to the [**`simple_button_t`**](#simple_button_t) variable to configure.
* **mode** `unsigned int: Digital logic mode for the button; can be `HIGH` or `LOW`.
* **hold_treshold** `unsigned long`: Amount of milliseconds to wait before triggering the first [**`hold`**](#events-flags-read-only) event.
* **hold_frequency** `unsigned long`: Amount of seconds between each [**`hold`**](#events-flags-read-only) event after `hold_treshold`.

Configures the microcontroller digital port and the [`simple_button_t`](#simple_button_t) passed reference.
If you pass `HIGH` as `mode` it will activate the [*Atmega** internal pullup resistor*](http://arduino.cc/en/Tutorial/DigitalPins)
and invert the button logic from *active high* to *active low*.

The two formal parameters `hold_treshold` and `hold_frequency` represent the number
of millisecond that the library should wait before triggering the [**hold**](#events-flags-read-only) event
the first time *(treshold)* and all consequent times *(frequency)*.

## `simple_button_read`

```c
int simple_button_read(simple_button_t *button);
```
* **button** `simple_button_t *`: Pointer to the event target aka the button variable associated with the event.

This function reads the digital input from the microcontroller and updates the status of `button` accordingly.
If required the function calls [events callbacks](#events-callbacks-read-write) and sets [event flags](#events-flags-read-only).
You should call this function for each `simple_button_t` variable at the very top of your loop function, before any logic code.

### Example

```c++
void loop() {
  simple_button_read(&btn1); // updates btn1 state and calls callbacks
  //... now you can use flags and read the status on btn1.
}
```

# \<simple_timer.h\>
C style timer library, *stupid simple*.
Every other library out there is either huge of memory consuming or more luckely buggy.
Even the [Timer](http://playground.arduino.cc/Code/Timer) doesn't support concurrency properly.

This library uses arduino `millis()` function internally, so it will not work if you plan to play
with [timers interrupts](http://arduino.cc/en/Reference/Interrupts) manually, as `millis()` `delay()` wouldn't work either.

## `___SIMPLE_TIMER_SLOTS___`

```c
#ifndef ___SIMPLE_TIMER_SLOTS___
#define ___SIMPLE_TIMER_SLOTS___ 5
#endif
```

Represents the maximum allowed concurrency of running timers. This tells the library to allocate enough space
to monitor and update n `___SIMPLE_TIMER_SLOTS___` overlapping timers. If you plan to use just one timer at time
you can low this number down and save some memory (50 bytes/timer).

*Maximum concurrency is 65536 default is 5*.

### Example

```c
#define ___SIMPLE_TIMER_SLOTS___ 1
#include <arduino-lib.h>
```

## `set_repeat`

```c
int set_repeat(unsigned int interval, void (*callback)(int tick_count, int arg), unsigned int repeat, ...)
```

* **interval** `unsigned int`: Number of milliseconds to wait before each call to `callback`.
* **callback** `void (*)(int tick_count, int arg)`: Function to be called repeatedly every `interval` ms `repeat` times.
* **repeat** `unsigned int`:  Times the callback will be called.
* **...** `int`:  Optional parameter forwarded to the callback.

This is the core function of the library, all other functions will refer to this.
The function creates a new timer that will call `callback` every `interval` ms for `repeat` times
passing the last optional parameter as actual parameter `arg` of `callback`. If you omit the optional
parameter `callback` will be called with `arg=0`. `tick_count` represents the number of calls
happened to `callback` so far.

Returns the **unique id** of the timer to be used with [`clear_timer`](#clear_timer) to stop it.

## `set_interval`

```c
int set_interval(unsigned int interval, void (*callback)(int tick_count, int arg))
```

* **interval** `unsigned int`: Number of milliseconds to wait before each call to `callback`.
* **callback** `void (*)(int tick_count, int arg)`: Function to be called repeatedly every `interval` ms `repeat` times.

Similarly to [`set_repeat`](#set_repeat) calls repeatedly `callback` every `interval` ms **infinitely**.

Returns the **unique id** of the timer to be used with [`clear_timer`](#clear_timer) to stop it.

## `set_timeout`

```c
int set_timeout(unsigned int interval, void (*callback)(int tick_count, int arg))
```

* **interval** `unsigned int`: Number of milliseconds to wait before each call to `callback`.
* **callback** `void (*)(int tick_count, int arg)`: Function to be called repeatedly every `interval` ms `repeat` times.

Similarly to [`set_repeat`](#set_repeat) calls repeatedly `callback` every `interval` ms **just once**.

Returns the **unique id** of the timer to be used with [`clear_timer`](#clear_timer) to stop it.

## `update_timers`

```c
void update_timers()
```

Updates timers state and calls the callbacks if necessary. It needs to be called within your
`loop` function repeatedly or your timers will never trigger.

## `clear_timer`

```c
void clear_timer(unsigned int id)
``

* **id** `unsigned int`: The unique id of the timer you want to clear.

Stops and clears the timer associated with `id`. The timer will be discarded,
no furter calls to `callback` will happen and the timer slot will be freed.

Multiple calls with the same `id` don't trigger any error, same behavior for already expired
timers or non existent timer ids. It deals with it silently.

# \<oscillate.h\>

## `oscillate`

```c
int oscillate(int pin, unsigned long interval, int start_value, char times)
```

* **pin** `unsigned int`: Digital pin number. This is the [Arduino pin map index](http://arduino.cc/en/Hacking/PinMapping168).
* **interval** `unsigned long`: Number of milliseconds to wait before each `digitalWrite`.
* **start_value** `int`: The value to be instantly written to `pin`, reversed after each `interval` ms.
* **times** `char`: Number of times the `pin` value will oscillate. Use 0 for infinity.

Oscillates the value of the digital `pin` between `HIGH` and `LOW` N `times` starting from
`start_value`. `start_value` will be written instantly to the `pin`.

Every `interval` ms the value of `pin` will be reversed.

Returns the **unique id** of the internal timer, you can use the `id` with [`clear_timer`](#clear_timer) to stop it.

Multiple calls on the same MC pin number are safe, they will result in clearing the old timer and starting a new one.

*Since it uses a timer internally (this is just a wrapper around [`set_repeat`](#set_repeat)),
you have to call [`update_timers`](#update_timers) as you would do with a timer,
otherwise the oscillation will not happen*.

### Example

```c
int osc_id, int osc_id2;

void setup() {
  // will set pin 13 from LOW to HIGH and from HIGH to LOW 3 times
  osc_id = = oscillate(13, 200, LOW, 3);

  // will not run
  osc_id2 = oscillate(13, 200, LOW, 3);
}

void loop() {
  update_timers();
  clear_timer(osc_id2);
}
```

# \<utils.h\>

Collection of uncategorized function.

## `free_ram`

```c
int free_ram()
```
Supposedly returns the amount of free RAM in bytes. Still a WIP.

## `seconds_to_digits`

```c
void seconds_to_digits(int seconds, char digits[])
```

Given an amount of `seconds` it will populate the `digit` array with
a zero padded minute/seconds string "mm:ss".

I used this in my [atmega328-timer-relay](https://github.com/kilianc/atmega328-timer-relay) project.

# \<SerialLCD.h\>



## `SerialLCD`

```c

```

## `begin`

```c

```

## `toggleSplash`

```c

```

## `clearDisplay`

```c

```

## `debugLevel`

```c

```

## `backLight`

```c

```

## `eraseBlock`

```c

```

## `reset`

```c

```

## `baudRate`

```c

```

## `setPixel`

```c

```

## `filledBox`

```c

```

## `reverseMode`

```c

```

## `bitblt`

```c

```

## `placeCursor`

```c

```

## `writeString`

```c

```


## License

_This software is released under the MIT license cited below_.

    Copyright (c) 2010 Kilian Ciuffolo, me@nailik.org. All Rights Reserved.

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the 'Software'), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
