/* Copyright 2021 Aaron Isele

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "rgb_matrix_map.h"

enum custom_keycodes {
    ECS_1 = SAFE_RANGE,
	BOT_1
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
	
	
	
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_INS,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_DEL,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F13,  _______,          RGB_TOG,
        _______, RGB_M_P, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          ECS_1,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET,            BOT_1,
        _______, _______, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, KC_CALC, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),
	
};
// clang-format on

// Rotary Knob Stuff
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) {
    switch(get_highest_layer(layer_state)){
		case 1: // layer 1: hue control
			if (clockwise) {
				rgb_matrix_increase_hue();
			}
			else {
				rgb_matrix_decrease_hue();
			}
			break;
			
        default: // layer 0: volume control
            if (clockwise) {
                tap_code16(KC_VOLU);
            } 
			else {
                tap_code16(KC_VOLD);
            }
            break;
		}
    }
    return true;
}
#endif // ENCODER_ENABLE

// RGB Stuff
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	
	if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
		for (uint8_t i=0; i<sizeof(LED_CAPS_LOCK)/sizeof(LED_CAPS_LOCK[0]); i++) {
			rgb_matrix_set_color(LED_CAPS_LOCK[i], RGB_RED);
		}
	}
	
	switch(get_highest_layer(layer_state)) {
		case 1: // layer 1: light up function keys
			for (uint8_t i=0; i<sizeof(LED_LAYER_ONE)/sizeof(LED_LAYER_ONE[0]); i++) {
				rgb_matrix_set_color(LED_LAYER_ONE[i], RGB_WHITE);
			}
			break;
		default: //layer 0
			break;
	}
}

// Macro stuff
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case ECS_1:
        if (record->event.pressed) {
			SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(300) "cmd" SS_DELAY(100) SS_TAP(X_ENT) SS_DELAY(500));
            SEND_STRING(SS_DELAY(500) "wsl -d docker-desktop" SS_TAP(X_ENT) SS_DELAY(50));
			SEND_STRING("sysctl -w vm.max_map_count=262144" SS_TAP(X_ENT) SS_DELAY(50));
			SEND_STRING("exit" SS_TAP(X_ENT) SS_DELAY(50));
			SEND_STRING("exit" SS_TAP(X_ENT) SS_DELAY(50));
        } else {

        }
        break;
		
	case BOT_1:
		if (record->event.pressed) {
			SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(300) "git-bash" SS_DELAY(100) SS_TAP(X_ENT) SS_DELAY(1000));
			SEND_STRING("location of A-A-Bot goes here");
		} else {
			
		}
		break;
	};
	return true;
}