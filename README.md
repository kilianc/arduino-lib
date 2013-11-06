# Arduino / Atmega* multi purpose library

## Index

* [\<simple_button.h\>](#simple-button)
* [\<simple_timer.h\>](#simple-timer)
* [\<oscillate.h\>](#oscillate)
* [\<utils.h\>](#utils)
* [\<SerialLCD.h\>](#SerialLCD)

# Simple Button \<simple_button.h\>
C style lightweight button library. Defines two functions, [`simple_button_set`]() [`simple_button_read`]() and a type, [`simple_button_t`]().

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

## `button_set`

```c
void simple_button_set(unsigned int pin,
                button_t *button,
                unsigned int mode,
                unsigned long hold_treshold,
                unsigned long hold_frequency);
```

* **pin** *(unsigned int)*:	 Digital pin number. This is the **Arduino pin map index** not the MC pin number.
* **button** (**button_t*): Pointer to the [**`simple_button_t`**]() variable to configure.
* **mode** (*unsigned int*): Digital logic mode for the button; can be `HIGH` or `LOW`.
* **hold_treshold** (*unsigned long*): Amount of milliseconds to wait before triggering the first [**`hold`**]() event.
* **hold_frequency** (*unsigned long*): Amount of seconds between each [**`hold`**]() event after `hold_treshold`.

Configures the microcontroller digital port and the `button_t` passed reference.
If you pass `HIGH` as [`mode`]() it will activate the [*Atmega** internal pullup resistor*](http://arduino.cc/en/Tutorial/DigitalPins)
and invert the button logic from *active high* to *active low*.

The two formal parameters [`hold_treshold`]() and [`hold_frequency`]() represent the number
of millisecond that the library should wait before triggering the [**hold**](#hold_event) event
the first time *(treshold)* and all consuquent times *(frequency)*.

## `simple_button_read`

```c
int simple_button_read(button_t *button);
```

## `simple_button_t`

```c
typedef struct simple_button_struct simple_button_t;
```

The type used by the library to store the button status across digital reads based on `simple_button_struct`.

### Read only
* **pin** `unsigned int`:
* **read** `bool`:
* **down** `bool`:
* **up** `bool`:
* **click** `bool`:
* **rising_edge** `bool`:
* **falling_edge** `bool`:
* **hold** `bool`:
* **hold_count** `unsigned int`:

### Callbacks

click_cb void (*)(simple_button_t *button)
rising_edge_cb void (*)(simple_button_t *button)
falling_edge_cb void (*)(simple_button_t *button)
hold_cb void (*)(simple_button_t *button)

## Events flags

* click
* rising_edge
* falling_edge
* hold