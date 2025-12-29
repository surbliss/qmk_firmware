// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "action_layer.h"
#include "keycodes.h"
#include "quantum.h"
#include "stm32f303xc.h"
#include QMK_KEYBOARD_H
#include "da.h"

enum {
    DEF,
    EXT,
    SYM,
    SYS,
    NUM,
    GAM,
    GNV // GAME NAV layer

};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        DM_PLY1, DK_QUOT, KC_G,    KC_D,    KC_F,    KC_V,                         KC_Q,    KC_L,    KC_U,    KC_O,    DK_MINS, DM_PLY2,
        _______, KC_W,    KC_S,    KC_T,    KC_H,    KC_B,                         KC_Z,    KC_N,    KC_E,    KC_A,    KC_Y,    _______,
        _______, KC_R,    KC_C,    KC_M,    KC_P,    KC_X,                         DK_COMM, KC_J,    KC_K,    DK_DOT,  KC_I,    _______, // Only until more elobarate repeat-key
                                                     KC_SPC,  MO(EXT),   MO(SYM), KC_LSFT
    ),
    // Extend layer
    [EXT] = LAYOUT(
        XXXXXXX, DM_REC1, DM_REC2, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DM_RSTP, XXXXXXX,
        XXXXXXX, XXXXXXX, KC_APP,  XXXXXXX, C(KC_F), LCS(KC_V),                    KC_PGUP, KC_HOME, KC_UP,   KC_END,  XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, OS_LGUI, OS_LSFT, OS_LCTL, CW_TOGG,                      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  XXXXXXX,
        XXXXXXX, OS_LALT, C(KC_C), XXXXXXX, XXXXXXX ,MO(SYS),                      KC_ENT,  KC_BSPC, XXXXXXX, KC_TAB,  KC_ESC,  XXXXXXX,
                                                     XXXXXXX, _______,    _______, QK_AREP
    ),
    [SYM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, DK_GRV,  DK_AT,   DK_HASH, DK_DLR,  DK_PERC,                      DK_RABK, DK_PLUS, DK_ASTR, DK_OSTR, DK_MINS, XXXXXXX,
        XXXXXXX, DK_PIPE, DK_LPRN, DK_LCBR, DK_LBRC, DK_AMPR,                      DK_LABK, DK_TILD,  DK_AE,   DK_ARNG, DK_QUES, KC_RSFT, // For capital ÆØÅ
        XXXXXXX, DK_EQL,  DK_RPRN, DK_RCBR, DK_RBRC, DK_CIRC,                      S(DK_COMM), DK_SLSH, DK_BSLS, S(DK_DOT), DK_EXLM, XXXXXXX,
                                                     _______,  _______,    _______, XXXXXXX
    ),
    [NUM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_7,    KC_8,    KC_9,    DK_COMM, XXXXXXX,
        XXXXXXX, XXXXXXX, OS_LGUI, OS_LSFT, OS_LCTL, XXXXXXX,                      XXXXXXX, KC_4,    KC_5,    KC_6,    KC_0,    XXXXXXX,
        XXXXXXX, OS_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_1,    KC_2,    KC_3,    DK_DOT,  XXXXXXX,
                                                     XXXXXXX, _______,    _______, XXXXXXX
    ),
    [SYS] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,                      XXXXXXX, KC_F7,   KC_F8,   KC_F9,   KC_F12,  XXXXXXX,
        XXXXXXX, XXXXXXX, OS_LGUI, OS_LSFT, OS_LCTL, XXXXXXX,                      XXXXXXX, KC_F4,   KC_F5,   KC_F6,   KC_F11,  XXXXXXX,
        XXXXXXX, OS_LALT, XXXXXXX, XXXXXXX, XXXXXXX, _______,                      _______, KC_F1,   KC_F2,   KC_F3,   KC_F10,  XXXXXXX,
                                                     XXXXXXX, _______,    _______, XXXXXXX
    ),
    // Gaming layer, only active if right keyboard is disconnected
    [GAM] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_KP_1, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        OS_LALT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_TAB,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     KC_LSFT, MO(GNV),    XXXXXXX, XXXXXXX
    ),
    [GNV] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_KP_3, KC_KP_4, KC_UP,   KC_KP_5, KC_KP_6,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_KP_2, KC_LEFT, KC_DOWN, KC_RGHT, KC_KP_7,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     XXXXXXX, _______,    XXXXXXX, XXXXXXX
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
        case KC_UNDS: // Flip - and _
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
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


const uint16_t PROGMEM jk_esc[] = {KC_J, KC_K, COMBO_END};
combo_t key_combos[] = {
    COMBO(jk_esc, KC_ESC),
};

// Use EXT-layer as a 'clear mods' button
layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, EXT)) {
        clear_oneshot_mods();
    }
    // Less forced delay using this option, compared to setting 'TRI_LAYER_ENABLE'
    return update_tri_layer_state(state, EXT, SYM, NUM);

}

// Runs after all other is done. See zsa/voyager/readme.md for transport-function documentation

void housekeeping_task_user(void) {
    if (!is_transport_connected()) {
        set_single_default_layer(GAM);
    }
    if (is_transport_connected()) {
        set_single_default_layer(DEF);
    }
}
