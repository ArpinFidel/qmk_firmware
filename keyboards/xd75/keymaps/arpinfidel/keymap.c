/* Copyright 2017 Wunder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "quantum.h"
#include "backlight.h"
#include "backlight_driver_common.h"
#include "print.h"

// Layer shorthand
#define _QW 0
#define _FN 1

#define MOD_LED 0
#define KC_LED 1
#define CAPS_LED 2
#define FN1_LED 3
#define FN2_LED 4


uint8_t ARPIN_STATE = 0;

#define BIT(x, b) (x&(1<<b))
#define BITOR(x, a, b) (BIT(x,a)|BIT(x,b))
// b >= a
#define BITAND(x, a, b) (BIT(x,a)&(BIT(x,b)>>(b-a)))
#define BITTRUE(x, b) (x|=(1<<b))
#define BITFALSE(x, b) (x&=~(1<<b))
#define BITTOGGLE(x, b) (x^=(1<<b))

enum my_kc {
    A_BL_MD = SAFE_RANGE,
    A_BL_KC,
    A_OS_LS = OSM(MOD_LSFT),
    A_OS_RS = OSM(MOD_RSFT)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | ESC    | 1      | 2      | 3      | 4      | 5      | -      | `      | =      | 6      | 7      | 8      | 9      | 0      | `      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | W      | E      | R      | T      | [      | ]      | \      | Y      | U      | I      | O      | P      | '      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | CAP LK | A      | S      | D      | F      | G      | UP     | DEL    | DOWN   | H      | J      | K      | L      | ;      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | C      | V      | B      | HOME   | PG UP  | END    | N      | M      | ,      | .      | /      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | LCTRL  | HOME   | LGUI   | LALT   | SPACE  | SPACE  | FN     | PG DN  | FN     | BSPC   | DEL    | RALT   | RGUI   | END    | RCTRL  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_QW] = { /* QWERTY */
	{ KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_MINS, KC_GRV , KC_EQL , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_GRV   },
	{ KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_LBRC, KC_RBRC, KC_BSLS, KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_QUOT  },
	{ KC_CAPS, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_UP  , KC_DEL , KC_DOWN, KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_ENT   },
	{ A_OS_LS, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_HOME, KC_PGUP, KC_END , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, A_OS_RS  },
	{ KC_LCTL, KC_HOME, KC_LGUI, KC_LALT, KC_SPC , KC_SPC , TT(_FN), KC_PGDN, TT(_FN), KC_BSPC, KC_DEL , KC_RALT, KC_RGUI, KC_END , KC_RCTL  },},

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | F1     | F2     | F3     | F4     | F5     | F6     |        |        |        | F7     | F8     | F9     | F10    | F11    | F12    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | MS W U | MS W R | MS W R | MS UP  | MS 1   | MS 2   | RGB HD |        | RGB HI | [      | ]      | UP     |        | =      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | MS W D |        | MS L   | MS DN  | MS R   | MS 3   | RGB SD |        | RGB SI | -      | LEFT   | DOWN   | RIGHT  |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        | RGB VD | BL TG  | RGB VI |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | FN     | RGB TG |        |        |        |        | RESET  |        | MS 1   | MS 2   | RGB RMD| RGB MD | FN     |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = { /* FUNCTION */
	{ KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   RGB_HUD, RESET  , RGB_HUI, KC_F7,   KC_F8,   KC_F9,   KC_F10  , KC_F11 , KC_F12   },
	{ KC_WH_U, KC_WH_L, KC_WH_R, KC_MS_U, KC_BTN1, KC_BTN2, RGB_SAD, _______, RGB_SAI, KC_LBRC, KC_RBRC, KC_UP  , _______ , KC_EQL , KC_BSLS  },
	{ KC_WH_D, _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN3, RGB_VAD, _______, RGB_VAI, KC_MINS, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______  },
	{ _______, _______, _______, _______, _______, _______, RGB_RMOD,A_BL_MD, RGB_MOD, _______, _______, _______, _______ , _______, _______  },
	{ _______, TT(_FN), RGB_TOG, _______, _______, _______, BL_DEC , A_BL_KC, BL_INC , KC_BTN1, KC_BTN2, _______, _______ , TT(_FN), _______  },}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
        case A_BL_MD:
            if (record->event.pressed) {
                BITTOGGLE(ARPIN_STATE, MOD_LED);
			}
			return false;
        case A_BL_KC:
        if (record->event.pressed) {
            BITTOGGLE(ARPIN_STATE, KC_LED);
        }
        return false;
		default:
			return true;
	}
}

bool led_update_user(led_t led_state) {
    // CAPS_ON = !led_state.caps_lock && BL_MOD_ON;
    if (led_state.caps_lock) BITTRUE(ARPIN_STATE, CAPS_LED);
    else BITFALSE(ARPIN_STATE, CAPS_LED);
    return true;
}

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
	uint8_t layer = biton32(state);

    // turns on gp100 (top left led) for odd layers
    if (layer & (1<<0)) BITTRUE(ARPIN_STATE, FN1_LED);
    else BITFALSE(ARPIN_STATE, FN1_LED);
    // turns on gp103 (top mid led) for layers 2, 6, ...
    if (layer & (1<<1)) BITTRUE(ARPIN_STATE, FN2_LED);
    else BITFALSE(ARPIN_STATE, FN2_LED);

	return state;
}

void backlight_pins_init(void);
void backlight_pins_on(void);
void backlight_pins_off(void);

void breathing_task(void);

static const pin_t backlight_pins[] = BACKLIGHT_PINS;

static inline void backlight_on(pin_t backlight_pin) {
#if BACKLIGHT_ON_STATE == 0
    writePinLow(backlight_pin);
#else
    writePinHigh(backlight_pin);
#endif
}

static inline void backlight_off(pin_t backlight_pin) {
#if BACKLIGHT_ON_STATE == 0
    writePinHigh(backlight_pin);
#else
    writePinLow(backlight_pin);
#endif
}

void backlight_pins_init(void) {
    pin_t backlight_pin;
    // Setup backlight pin as output and output to off state.
    for (uint8_t i = 0; i < BACKLIGHT_LED_COUNT; i++) {
        backlight_pin = backlight_pins[i];
        setPinOutput(backlight_pin);
        backlight_off(backlight_pin);
    }
}

void backlight_pins_on(void) {
    pin_t backlight_pin;
    bool on;
    for (uint8_t i = 0; i < BACKLIGHT_LED_COUNT; i++) {
        backlight_pin = backlight_pins[i];
        switch (backlight_pin) {
            case B2:
            on = BITAND(ARPIN_STATE, MOD_LED, CAPS_LED);
            break;
            case F4:
            if (BITAND(ARPIN_STATE, MOD_LED, FN1_LED)) gp100_led_on();
            else gp100_led_off();
            on = false;
            break;
            case F7:
            if (BITAND(ARPIN_STATE, MOD_LED, FN1_LED)) gp103_led_on();
            else gp103_led_off();
            on = false;
            break;
            default:
            on = BIT(ARPIN_STATE, KC_LED);
            // dprintf("%d %d %d\n", BIT(ARPIN_STATE, KC_LED), BIT(ARPIN_STATE, CAPS_LED), BITAND(ARPIN_STATE, MOD_LED, CAPS_LED));
        }
        if (on) backlight_on(backlight_pin);
    }
}

void backlight_pins_off(void) {
    pin_t backlight_pin;
    for (uint8_t i = 0; i < BACKLIGHT_LED_COUNT; i++) {
        backlight_pin = backlight_pins[i];
        backlight_off(backlight_pin);
    }

}


#ifdef BACKLIGHT_BREATHING
#    error "Backlight breathing is not available for software PWM. Please disable."
#endif

static uint16_t s_duty_pattern = 0;

// clang-format off

/** \brief PWM duty patterns
 *
 * We scale the current backlight level to an index within this array. This allows
 * backlight_task to focus on just switching LEDs on/off, and we can predict the duty pattern
 */
static const uint16_t backlight_duty_table[] = {
    0b0000000000000000,
    0b1000000000000000,
    0b1000000010000000,
    0b1000001000010000,
    0b1000100010001000,
    0b1001001001001000,
    0b1010101010101010,
    0b1110111011101110,
    0b1111111111111111,
};
#define backlight_duty_table_size (sizeof(backlight_duty_table) / sizeof(backlight_duty_table[0]))

// clang-format on

static uint8_t scale_backlight(uint8_t v) { return v * (backlight_duty_table_size - 1) / BACKLIGHT_LEVELS; }

void backlight_init_ports(void) { backlight_pins_init(); }

void backlight_set(uint8_t level) { s_duty_pattern = backlight_duty_table[scale_backlight(level)]; }

void backlight_task(void) {
    static uint8_t backlight_tick = 0;

    if (s_duty_pattern & ((uint16_t)1 << backlight_tick)) {
        backlight_pins_on();
    } else {
        backlight_pins_off();
    }
    backlight_tick = (backlight_tick + 1) % 16;
}
