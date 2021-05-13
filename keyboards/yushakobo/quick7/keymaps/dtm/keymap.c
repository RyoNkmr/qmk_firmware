/* Copyright 2020 yushakobo
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

#ifdef RGB_MATRIX_ENABLE
    led_config_t g_led_config = { {
    // Key Matrix to LED Index
        { 0, 1, 2 },
        { 5, 4, 3 },
        { 6, 7, 8 }
    }, {
    // LED Index to Physical Position
        { 0, 0 },    { 112, 0 },  { 224, 0 },
        { 224, 32 }, { 112, 32 }, { 0, 32 },
        { 0, 64 },   { 112, 64 }, { 224, 64 }
    }, {
    // LED Index to Flag
        4, 4, 4,
        4, 4, 4,
        4, 4, 4
} };
#endif

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FUNC1,
    _LED
};

// Defines the keycodes used by our macros in process_record_user
/*
enum custom_keycodes {
    YUSHAURL = SAFE_RANGE,
    SLACK_TF
};
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        TO(_LED),  TO(_FUNC1),   RGB_TOG,
        KC_DELETE, LCA(KC_UP),   LCTL(KC_R),
        KC_ENTER,  LCA(KC_DOWN), KC_SPACE
    ),
    [_FUNC1] = LAYOUT(
        TO(_LED),      TO(_BASE),      RGB_TOG,
        LCTL(KC_LEFT), LCTL(KC_RIGHT), LCA(KC_UP),
        LCTL(KC_Z),    LCTL(KC_S),     LCA(KC_DOWN)
    ),
    [_LED] = LAYOUT(
        TO(_FUNC1),       TO(_BASE),       RGB_TOG,
#ifdef RGBLIGHT_ENABLE
        RGB_MODE_BREATHE, RGB_MODE_RAINBOW, RGB_MODE_SWIRL,
        RGB_MODE_SNAKE,   RGB_MODE_KNIGHT,  RGB_MODE_XMAS
#endif
#ifdef RGB_MATRIX_ENABLE
        RGB_MODE_BREATHE, RGB_MODE_RAINBOW, RGB_MODE_SWIRL,
        RGB_MODE_FORWARD, RGB_MODE_REVERSE, RGB_MODE_PLAIN
#endif
    )
};


/*
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SLACK_TF:
            if (record->event.pressed) {
                SEND_STRING(":thinking_face:");
            }
            break;
    }
    return true;
}
*/

void encoder_update_user(uint8_t index, bool clockwise) {
  if (IS_LAYER_ON(_BASE)) {
      tap_code(clockwise ? KC_RGHT : KC_LEFT);
      return;
  }
  if (IS_LAYER_ON(_FUNC1)) {
      tap_code(clockwise ? KC_UP : KC_DOWN);
      return;
  }

  if (IS_LAYER_ON(_LED)) {
#ifdef RGBLIGHT_ENABLE
      clockwise ? rgblight_increase_speed() : rgblight_decrease_speed();
#endif
#ifdef RGB_MATRIX_ENABLE
      clockwise ? rgb_matrix_increase_speed() : rgb_matrix_decrease_speed();
#endif
      return;
  }
}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _BASE:
#ifdef RGBLIGHT_ENABLE
      rgblight_mode(RGBLIGHT_MODE_TWINKLE);
      rgblight_sethsv(HSV_GREEN);
#endif
#ifdef RGB_MATRIX_ENABLE
      rgb_matrix_sethsv(HSV_GREEN);
      rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE);
#endif
      break;
    case _FUNC1:
#ifdef RGBLIGHT_ENABLE
      rgblight_mode(RGBLIGHT_MODE_BREATHING);
      rgblight_sethsv(HSV_MAGENTA);
#endif
#ifdef RGB_MATRIX_ENABLE
      rgb_matrix_sethsv(HSV_MAGENTA);
      rgb_matrix_mode(RGB_MATRIX_CYCLE_UP_DOWN);
#endif
      break;
    case _LED:
#ifdef RGBLIGHT_ENABLE
      rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
      rgblight_sethsv(HSV_YELLOW);
#endif
#ifdef RGB_MATRIX_ENABLE
      rgb_matrix_sethsv(HSV_YELLOW);
      rgb_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
#endif
      break;
  }
  return state;
}
