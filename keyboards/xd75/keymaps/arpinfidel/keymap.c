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

// Layer shorthand
#define _QW 0
#define _FN 1

bool BL_MOD_ON = true;

enum my_kc {
	A_BL_TG = SAFE_RANGE,
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
	{ KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   RGB_HUD, _______, RGB_HUI, KC_F7,   KC_F8,   KC_F9,   KC_F10  , KC_F11 , KC_F12   },
	{ KC_WH_U, KC_WH_L, KC_WH_R, KC_MS_U, KC_BTN1, KC_BTN2, RGB_SAD, _______, RGB_SAI, KC_LBRC, KC_RBRC, KC_UP  , _______ , KC_EQL , KC_BSLS  },
	{ KC_WH_D, _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN3, RGB_VAD, _______, RGB_VAI, KC_MINS, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______  },
	{ _______, _______, _______, _______, _______, _______, RGB_RMOD,A_BL_TG, RGB_MOD, _______, _______, _______, _______ , _______, _______  },
	{ _______, TT(_FN), RGB_TOG, _______, _______, _______, RESET  , _______, RESET  , KC_BTN1, KC_BTN2, _______, _______ , TT(_FN), _______  },}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case A_BL_TG:
			if (record->event.pressed) {
				// toggle keycaps leds (f5 pin)
				PORTF ^= (1 << 5);
                BL_MOD_ON = !BL_MOD_ON;
			}
			return false;

		default:
			return true;
	}
}

void led_set_user(uint8_t usb_led) {
	if (usb_led & (1<<USB_LED_CAPS_LOCK)) {
		capslock_led_on();
	} else {
		capslock_led_off();
	}
}

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
    if (!BL_MOD_ON) return state;
	uint8_t layer = biton32(state);

	gp100_led_off();
	gp103_led_off();

	// turns on gp100 (top left led) for odd layers
	if (layer & (1<<0)) gp100_led_on();
	// turns on gp103 (top mid led) for layers 2, 6, ...
	if (layer & (1<<1)) gp103_led_on();

	return state;
}
