// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keycodes.h"
#include "stm32f303xc.h"
#include QMK_KEYBOARD_H
#include "da.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        _______, DK_QUOT, KC_G,    KC_D,    KC_F,    KC_V,                         KC_Q,    KC_L,    KC_U,    KC_O,    DK_MINS, _______,
        _______, KC_W,    KC_S,    KC_T,    KC_H,    KC_B,                         KC_Z,    KC_N,    KC_E,    KC_A,    KC_Y,    _______,
        OS_LSFT, KC_R,    KC_C,    KC_M,    KC_P,    KC_X,                         DK_COMM, KC_J,    KC_K,    DK_DOT,  KC_I,    OS_RSFT, // Only until more elobarate repeat-key
                                                     KC_SPC,  TL_LOWR,    TL_UPPR, QK_REP
    ),
    // Extend layer
    [EXT] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_HOME, KC_PGDN, KC_UP,   KC_PGUP, KC_APP,  XXXXXXX,
        XXXXXXX, CW_TOGG, OS_LGUI, OS_LSFT, OS_LCTL, XXXXXXX,                      KC_END,  KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  XXXXXXX,
        XXXXXXX, OS_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_BSPC, KC_TAB,  KC_ESC,  KC_ENT,  XXXXXXX,
                                                     XXXXXXX, _______,    _______, QK_AREP
    ),
    // Symbol layer
    [SYM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, DK_TILD, DK_AT,   DK_HASH, DK_DLR,  DK_PERC,                      DK_SLSH, DK_PLUS, DK_ASTR, DK_OSTR, DK_BSLS, XXXXXXX,
        XXXXXXX, DK_LABK, DK_LPRN, DK_LCBR, DK_LBRC, DK_AMPR,                      DK_QUES, DK_EQL,  DK_AE,   DK_ARNG, DK_EXLM, XXXXXXX,
        XXXXXXX, DK_RABK, DK_RPRN, DK_RCBR, DK_RBRC, DK_PIPE,                      DK_GRV,  DK_QUOT, DK_DQUO, DK_EQL,  DK_MINS, XXXXXXX,
                                                     DK_UNDS, _______,    _______, XXXXXXX
    ),
    [NUM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_7,    KC_8,    KC_9,    XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, OS_LGUI, OS_LSFT, OS_LCTL, XXXXXXX,                      XXXXXXX, KC_4,    KC_5,    KC_6,    XXXXXXX, XXXXXXX,
        XXXXXXX, OS_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_1,    KC_2,    KC_3,    KC_0,    XXXXXXX,
                                                     _______, _______,    _______, _______
    ),
};


// Additional definitions for Alternate repeat-key
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool ctrled = mods & MOD_MASK_CTRL;
    bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
        case KC_TAB:
            if (shifted && ctrled) return C(KC_TAB);
            if (ctrled) return LCS(KC_TAB);
            // Just delete the last space
            return KC_BACKSPACE;
        /// Return after punctuation
        // Also matches shifted, e.g. ; or :
        case DK_DOT:
        case DK_COMM:
        // Braces
        case DK_LCBR:
        case DK_RCBR:
        case DK_LPRN:
        case DK_RPRN:
        case DK_LBRC:
        case DK_RBRC:
             return KC_ENTER;
        case KC_SPACE: return OS_LSFT;
    }
    return KC_TRNS;  // Defer to default definitions.
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        // Danish placement of KC_MINS
        case DK_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

// S-' = "
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, DK_QUOT, DK_DQUO);

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
	&delete_key_override
};



// Use EXT-layer as a 'clear mods' button
layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, EXT)) {
        clear_oneshot_mods();
    }
    return state;
}
