#include "keymap_common.h"

/*
 * SpaceFN
 * http://geekhack.org/index.php?topic=51069.0
 */
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap 0: Default Layer
     * ,-----------------------------------------------------------.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \|
     * |-----------------------------------------------------------|
     * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return  |
     * |-----------------------------------------------------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |
     * |-----------------------------------------------------------|
     * |Ctrl|Gui |Alt |      Space             |Alt |Gui |App |Ctrl|
     * `-----------------------------------------------------------'
     */
    KEYMAP_ANSI(
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC, \
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS, \
        CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,  \
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT, \
        LCTL,LGUI,LALT,          FN0,                     RALT,RGUI,APP, RCTL),

    /* 1: dvorak */
    KEYMAP_ANSI(
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   LBRC,RBRC,BSPC, \
        TAB, QUOT,COMM,DOT, P,   Y,   F,   G,   C,   R,   L,   SLSH,EQL, BSLS, \
        CAPS,A,   O,   E,   U,   I,   D,   H,   T,   N,   S,   MINS,     ENT,  \
        LSFT,SCLN,Q,   J,   K,   X,   B,   M,   W,   V,   Z,             RSFT, \
        LCTL,LGUI,LALT,          FN0,                     RALT, RGUI,APP, RCTL),

    /* Overlay 2: SpaceFN
     * ,-----------------------------------------------------------.
     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|       |
     * |-----------------------------------------------------------|
     * |     |FN1| Up |FN2|   |   |   |   |   |Cal|   |Hom|Ins|FnL  |
     * |-----------------------------------------------------------|
     * |      |Lef|Dow|Rig|   |   |Psc|Slk|Pau|   |Tsk|End|        |
     * |-----------------------------------------------------------|
     * |        |Del|   |Web|Mut|VoU|VoD|   |PgU|PgD|Del|          |
     * |-----------------------------------------------------------|
     * |    |    |    |                        |    |    |    |    |
     * `-----------------------------------------------------------'
     * Fn:  to Fn overlay
     * FnL: to Layout selector overaly
     * FnQ: toggle Esc overlay
     * FnS: toggle Arrow overlay
     */
    KEYMAP_ANSI(
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, TRNS, \
        TRNS,FN1, UP,  FN2,TRNS,TRNS,TRNS,TRNS,TRNS,CALC,TRNS,HOME,INS, FN4,  \
        TRNS,LEFT,DOWN,RGHT,TRNS,TRNS,PSCR,SLCK,PAUS,TRNS,FN3, END,      TRNS, \
        TRNS,DEL, TRNS,WHOM,MUTE,VOLU,VOLD,TRNS,PGUP,PGDN,DEL,           TRNS, \
        TRNS,TRNS,TRNS,          TRNS,                     TRNS,TRNS,TRNS,TRNS),
};

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_TAP_KEY(2, KC_SPACE),
    [1] = ACTION_DEFAULT_LAYER_SET(1),     // set dovorak layout
    [2] = ACTION_DEFAULT_LAYER_SET(0),     // set qwerty layout
};
