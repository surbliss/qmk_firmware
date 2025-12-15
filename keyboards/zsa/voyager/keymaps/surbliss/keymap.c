// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keycodes.h"
#include "stm32f303xc.h"
#include QMK_KEYBOARD_H
#include "da.h"

enum layers {
    DEF,
    EXT,
    SYM,
    NUM,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        _______, DM_PLY1, KC_G,    KC_D,    KC_F,    KC_V,                         KC_Q,    KC_L,    KC_U,    KC_O,    DM_PLY2, _______,
        _______, KC_W,    KC_S,    KC_T,    KC_H,    KC_B,                         KC_Z,    KC_N,    KC_E,    KC_A,    KC_Y,    _______,
        CW_TOGG, KC_R,    KC_C,    KC_M,    KC_P,    KC_X,                         DK_COMM, KC_J,    KC_K,    DK_DOT,  KC_I,    _______,
                                                     MO(EXT), KC_SPC,     QK_REP,  MO(SYM)
    ),
    // Extend layer
    [EXT] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_APP,  XXXXXXX,
        XXXXXXX, CW_TOGG, OS_LGUI, OS_LSFT, OS_LCTL, XXXXXXX,                      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  XXXXXXX,
        XXXXXXX, OS_LALT, XXXXXXX, XXXXXXX, KC_ESC,  XXXXXXX,                      XXXXXXX, KC_BSPC, KC_TAB,  KC_ENT,  KC_ESC,  XXXXXXX,
                                                     _______, _______,    QK_AREP, _______
    ),
    // Symbol layer
    [SYM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, DK_EXLM, DK_AT,   DK_HASH, DK_DLR,  DK_PERC,                      DK_PLUS, DK_MINS, DK_ASTR, DK_SLSH, DK_LABK, XXXXXXX,
        XXXXXXX, DK_LABK, DK_LCBR, DK_LBRC, DK_LPRN, DK_AMPR,                      DK_QUES, OS_RCTL, OS_RSFT, OS_RGUI, DK_RABK, XXXXXXX,
        XXXXXXX, DK_RABK, DK_RCBR, DK_RBRC, DK_RPRN, DK_PIPE,                      DK_GRV,  DK_QUOT, DK_DQUO, DK_BSLS, OS_LALT, XXXXXXX,
                                                     _______, KC_ENT,     _______, _______
    ),
};


// Additional definitions for Alternate repeat-key
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool ctrled = mods & MOD_MASK_CTRL;
    bool shifted = mods & MOD_MASK_SHIFT;
    if (ctrled) {  // Was Ctrl held?
        switch (keycode) {
            // Swapping browser-tabs
            case KC_TAB:
                if (shifted) {
                    return KC_TAB;
                } else {
                    return S(KC_TAB);
                }
        }
    }

    switch (keycode) {
        case DK_DOT: return KC_ENT;
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
