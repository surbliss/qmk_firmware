// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com
// SPDX-License-Identifier: GPL-2.0-or-later

#include "action_layer.h"
#include "keycodes.h"
#include "keymap_danish.h"
#include "layer_lock.h"
#include "quantum.h"
#include "quantum_keycodes.h"
#include "stm32f303xc.h"
#include QMK_KEYBOARD_H
// #include "da.h" // Imported by sendstring_danish.h, hence not needed?
#include "sendstring_danish.h"

enum {
    DEF,
    EXT,
    SYM,
    MAC, // Macros
    SYS,
    NUM,
    GAM,
    GNV, // GAME NAV layer
};

// Macros
enum custom_keycodes {
    CODEBLC = SAFE_RANGE,
    LARROW,
    RARROW,
    LDARROW,
    RDARROW,
    LPIPE,
    RPIPE,
    RETSEMI,
    RET_SFT, // Return + Shift if holding
    DBL_CLN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT(
        // TODO: Move æøå so we can put Backspace + Enter on the rightmost column, and Tab + Esc on leftmost
        _______, DK_QUOT, KC_G,    KC_D,    KC_F,    KC_V,                         KC_Q,    KC_L,    KC_U,    KC_O,    DK_MINS, DK_ARNG,
        CW_TOGG, KC_W,    KC_S,    KC_T,    KC_H,    KC_B,                         KC_Z,    KC_N,    KC_E,    KC_A,    KC_Y,    DK_OSTR,
        _______, KC_R,    KC_C,    KC_M,    KC_P,    KC_X,                         DK_COMM, KC_J,    KC_K,    DK_DOT,  KC_I,    DK_AE,
        QK_LLCK, DM_PLY1, DM_PLY2, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                                     KC_LSFT, MO(EXT),    MO(SYM), KC_SPC
    ),
    // Extend layer
    [EXT] = LAYOUT(
        // TODO: Don't think all these 'copy/paste' shortcuts are worth.
        // Instead, find way to add modifier keys to both hands, so pressing these is not so awkward.
        // Am keeping the tmux prefix key tho, for now (can be held down to combine with movement keys)
        // NOTE: Possible idea: Replace ' and - on main layer with Return and Backspace respectively? Maaaybe...
        // Downside: Multiple 'Backspace' presses is annoying ...
        // Could also use spaces to the left/right of respective pinkies (and move æ up on DK_MINS place)
        // TODO: 2: Now the edit-keys on left hand might not be needed, so consider possible replacements
        // NOTE: Top row here is a bunch of accents, grouping them so easier to find, since they are used rarely.
        // eg: ½¨^. TODO: Add more
        CODEBLC, KC_GRV,  KC_RBRC, DK_CIRC, XXXXXXX, XXXXXXX,                      KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_APP,  TO(GAM),
        KC_RALT, KC_MEH,  KC_LGUI, KC_LSFT, KC_LCTL, C(KC_B),                      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, _______,
        _______, KC_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_ENT,  KC_BSPC, KC_DEL,  KC_TAB,  XXXXXXX, _______,
        _______, DM_REC1, DM_REC2, XXXXXXX, XXXXXXX, MO(SYS),                      MO(SYS), XXXXXXX, XXXXXXX, XXXXXXX, DM_RSTP, XXXXXXX,
                                                     XXXXXXX, _______,    _______, QK_AREP
    ),
    [SYM] = LAYOUT(
        XXXXXXX, DK_EXLM, DK_AT,   DK_HASH, DK_DLR,  DK_PERC,                      DK_PLUS, DK_LABK, DK_EQL,  DK_RABK, DK_ASTR, XXXXXXX,
        XXXXXXX, DK_PIPE, DK_LPRN, DK_LCBR, DK_LBRC, DK_AMPR,                      DK_QUES, KC_RCTL, KC_RSFT, KC_RGUI, KC_MEH,  KC_RALT,
        XXXXXXX, DK_GRV,  DK_RPRN, DK_RCBR, DK_RBRC, XXXXXXX,                      DK_CIRC, DK_SLSH, DK_BSLS, DK_TILD, KC_LALT, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     _______,  _______,   _______, XXXXXXX
    ),
    // NOTE: No way to access this layer, for now
    [MAC] = LAYOUT(
        XXXXXXX, CODEBLC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, LARROW,  XXXXXXX, RARROW,  XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, LDARROW, XXXXXXX, RDARROW, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      RETSEMI, LPIPE,   XXXXXXX, RPIPE,   XXXXXXX, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     XXXXXXX, _______,    _______, XXXXXXX
    ),
    [NUM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_7,    KC_8,    KC_9,    S(DK_MINS), TO(GAM),
        KC_RALT, KC_MEH,  KC_LGUI, KC_LSFT, KC_LCTL, XXXXXXX,                      DK_DOT,  KC_4,    KC_5,    KC_6,    KC_0,    XXXXXXX,
        XXXXXXX, KC_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      DK_COMM, KC_1,    KC_2,    KC_3,    KC_SPC,  XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     _______, _______,    _______, _______
    ),
    [SYS] = LAYOUT(
        RM_SPDD, KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,                      XXXXXXX, KC_F7,   KC_F8,   KC_F9,   KC_F12,  XXXXXXX,
        XXXXXXX, XXXXXXX, OS_LGUI, OS_LSFT, OS_LCTL, XXXXXXX,                      XXXXXXX, KC_F4,   KC_F5,   KC_F6,   KC_F11,  XXXXXXX,
        _______, OS_LALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      _______, KC_F1,   KC_F2,   KC_F3,   KC_F10,  XXXXXXX,
        _______, RM_SPDU, RM_PREV, RM_NEXT, RM_VALD, RM_VALU,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     XXXXXXX, _______,    _______, XXXXXXX
    ),
    // Gaming layer, only active if right keyboard is disconnected
    [GAM] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    DK_ARNG,
        KC_KP_1, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,    DK_AE,   DK_OSTR,
        KC_KP_0, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M,    DK_DOT,  DK_COMM, DK_MINS, XXXXXXX,
                                                     KC_LSFT, MO(GNV),    TO(DEF), XXXXXXX
    ),
    [GNV] = LAYOUT(
        XXXXXXX, KC_KP_3, KC_KP_4, KC_UP,   KC_KP_5, KC_KP_6,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_KP_8, KC_KP_2, KC_LEFT, KC_DOWN, KC_RGHT, KC_KP_7,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_KP_9, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                     XXXXXXX, _______,    TO(DEF), XXXXXXX
    ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CODEBLC:
            if (record->event.pressed) {
                // ``` + shift-enter
                // Using 'SEND_STRING("```")' adds spaces between (to compensate for normally being dead keys)
                SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_EQL) SS_TAP(X_EQL) SS_TAP(X_EQL) SS_LSFT("\n") SS_UP(X_LSFT));
            }
            break;
        case LARROW:
            if (record->event.pressed) {
                SEND_STRING(" <- ");
            }
            break;
        case LDARROW:
            if (record->event.pressed) {
                SEND_STRING(" <= ");
            }
            break;
        case LPIPE:
            if (record->event.pressed) {
                SEND_STRING(" <| ");
            }
            break;
        case RARROW:
            if (record->event.pressed) {
                SEND_STRING(" -> ");
            }
            break;
        case RDARROW:
            if (record->event.pressed) {
                SEND_STRING(" => ");
            }
            break;
        case RPIPE:
            if (record->event.pressed) {
                SEND_STRING(" |> ");
            }
            break;
        case RETSEMI:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_END) ";\n");
            }
            break;
        case DBL_CLN:
            if (record->event.pressed) {
                SEND_STRING("::");
            }
            break;
        case RET_SFT:
            if (record->event.pressed) {
                register_mods(MOD_BIT(KC_LSFT));
                tap_code(KC_ENTER);
            } else {
                unregister_mods(MOD_BIT(KC_LSFT));
            }
            return false;
    }
    return true;
};

// Additional definitions for Alternate repeat-key
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool ctrled  = mods & MOD_MASK_CTRL;
    bool shifted = mods & MOD_MASK_SHIFT;
    switch (keycode) {
        case KC_TAB:
            if (shifted && ctrled) return C(KC_TAB);
            if (ctrled) return LCS(KC_TAB);
            if (shifted) return KC_TAB;
            return S(KC_TAB);
        // /// Return after punctuation
        // // Symbols you might want a newline after
        case DK_DOT:
        case DK_COMM:
        case DK_LCBR:
        case DK_RCBR:
        case DK_LPRN:
        case DK_RPRN:
        case DK_LBRC:
        case DK_RBRC:
        case DK_SCLN:
        case DK_COLN:
            return RET_SFT; // Press shift-enter, if held keeps holding shift
        // Keys with bad defaults (so just retunr shift instead)
        case KC_BACKSPACE: // 'Delete' normally
        case KC_ENTER:
            // When quickly going from new-line, make shift instead.
            return KC_LSFT;
        // Helix-stuff
        case KC_X:          // Select line
            return A(KC_X); // Undo line selection
    }
    return KC_TRNS; // Defer to default definitions.
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        // Danish placement of KC_MINS
        case DK_MINS:
        case KC_UNDS:                        // Flip - and _
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;
        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
            return true;
        default:
            return false; // Deactivate Caps Word.
    }
}

// S-' = "
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, DK_QUOT, DK_DQUO);

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {&delete_key_override};

const uint16_t PROGMEM jk_esc[] = {KC_J, KC_K, COMBO_END};

combo_t key_combos[] = {
    COMBO(jk_esc, KC_ESC),
};

// Use EXT-layer as a 'clear mods' button
layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, EXT)) {
        clear_oneshot_mods();
    }
    if (is_layer_locked(NUM)) return state;
    // Less forced delay using this option, compared to setting 'TRI_LAYER_ENABLE'
    return update_tri_layer_state(state, EXT, SYM, NUM);
}

// Runs after all other is done. See zsa/voyager/readme.md for transport-function documentation
// Update: Make bool check first, so this doesn't constantly refire!

static bool was_connected = true;

void housekeeping_task_user(void) {
    bool connected = is_transport_connected();
    if (connected == was_connected) return;
    was_connected = connected;
    set_single_default_layer(connected ? DEF : GAM);
}
