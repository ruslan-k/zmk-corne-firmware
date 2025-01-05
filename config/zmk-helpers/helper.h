/*
 * helper.h
 *
 * Convenience macros simplifying ZMK's keymap configuration.
 * See https://github.com/urob/zmk-helpers for documentation.
 */

#pragma once

#define ZMK_HELPER_STRINGIFY(x) #x

// Preprocessor mechanism to overload macros, cf. https://stackoverflow.com/a/27051616/6114651
#define VARGS_(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N
#define VARGS(...) VARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

/* ZMK_BEHAVIOR */

#define ZMK_BEHAVIOR_CORE_caps_word       compatible = "zmk,behavior-caps-word";       #binding-cells = <0>
#define ZMK_BEHAVIOR_CORE_dynamic_macro   compatible = "zmk,behavior-dynamic-macro";   #binding-cells = <1>
#define ZMK_BEHAVIOR_CORE_hold_tap        compatible = "zmk,behavior-hold-tap";        #binding-cells = <2>
#define ZMK_BEHAVIOR_CORE_key_repeat      compatible = "zmk,behavior-key-repeat";      #binding-cells = <0>
#define ZMK_BEHAVIOR_CORE_macro           compatible = "zmk,behavior-macro";           #binding-cells = <0>
#define ZMK_BEHAVIOR_CORE_macro_one_param compatible = "zmk,behavior-macro-one-param"; #binding-cells = <1>
#define ZMK_BEHAVIOR_CORE_macro_two_param compatible = "zmk,behavior-macro-two-param"; #binding-cells = <2>
#define ZMK_BEHAVIOR_CORE_mod_morph       compatible = "zmk,behavior-mod-morph";       #binding-cells = <0>
#define ZMK_BEHAVIOR_CORE_sticky_key      compatible = "zmk,behavior-sticky-key";      #binding-cells = <1>
#define ZMK_BEHAVIOR_CORE_tap_dance       compatible = "zmk,behavior-tap-dance";       #binding-cells = <0>
#define ZMK_BEHAVIOR_CORE_tri_state       compatible = "zmk,behavior-tri-state";       #binding-cells = <0>

#define ZMK_BEHAVIOR(name, type, ...) \
    / { \
        behaviors { \
            name: name { \
                ZMK_BEHAVIOR_CORE_ ## type; \
                __VA_ARGS__ \
            }; \
        }; \
    };

#define ZMK_CAPS_WORD(name, ...) ZMK_BEHAVIOR(name, caps_word, __VA_ARGS__)
#define ZMK_HOLD_TAP(name, ...) ZMK_BEHAVIOR(name, hold_tap, __VA_ARGS__)
#define ZMK_KEY_REPEAT(name, ...) ZMK_BEHAVIOR(name, key_repeat, __VA_ARGS__)
#if ZMK_HELPERS_KEEP_NATIVE != 1
    #undef ZMK_MACRO
    #define ZMK_MACRO(name, ...) ZMK_BEHAVIOR(name, macro, __VA_ARGS__)
#endif
#define ZMK_MACRO_ONE_PARAM(name, ...) ZMK_BEHAVIOR(name, macro_one_param, __VA_ARGS__)
#define ZMK_MACRO_TWO_PARAM(name, ...) ZMK_BEHAVIOR(name, macro_two_param, __VA_ARGS__)
#define ZMK_MOD_MORPH(name, ...) ZMK_BEHAVIOR(name, mod_morph, __VA_ARGS__)
#define ZMK_STICKY_KEY(name, ...) ZMK_BEHAVIOR(name, sticky_key, __VA_ARGS__)
#define ZMK_TAP_DANCE(name, ...) ZMK_BEHAVIOR(name, tap_dance, __VA_ARGS__)
#define ZMK_TRI_STATE(name, ...) ZMK_BEHAVIOR(name, tri_state, __VA_ARGS__)

/* ZMK_LAYER */

#define ZMK_LAYER(...) CONCAT(ZMK_LAYER_, VARGS(__VA_ARGS__))(__VA_ARGS__)
#define ZMK_LAYER_2(_name, layout) \
    / { \
        keymap { \
            compatible = "zmk,keymap"; \
            layer_ ## _name { \
                display-name = ZMK_HELPER_STRINGIFY(_name); \
                bindings = <layout>; \
            }; \
        }; \
    };
#define ZMK_LAYER_3(_name, layout, sensors) \
    / { \
        keymap { \
            compatible = "zmk,keymap"; \
            layer_ ## _name { \
                display-name = ZMK_HELPER_STRINGIFY(_name); \
                bindings = <layout>; \
                sensor-bindings = <sensors>; \
            }; \
        }; \
    };

/* ZMK_COMBOS */

#define ALL 0xff
#if !defined COMBO_TERM
    #define COMBO_TERM 30
#endif

#define ZMK_COMBO(...) CONCAT(ZMK_COMBO_, VARGS(__VA_ARGS__))(__VA_ARGS__)
#define ZMK_COMBO_3(name, combo_bindings, keypos) \
    ZMK_COMBO_4(name, combo_bindings, keypos, ALL)
#define ZMK_COMBO_4(name, combo_bindings, keypos, combo_layers) \
    ZMK_COMBO_5(name, combo_bindings, keypos, combo_layers, COMBO_TERM)
#define ZMK_COMBO_5(name, combo_bindings, keypos, combo_layers, combo_timeout) \
    ZMK_COMBO_6(name, combo_bindings, keypos, combo_layers, combo_timeout, 0)
#define ZMK_COMBO_6(name, combo_bindings, keypos, combo_layers, combo_timeout, combo_idle) \
    ZMK_COMBO_7(name, combo_bindings, keypos, combo_layers, combo_timeout, combo_idle, )
#define ZMK_COMBO_7(name, combo_bindings, keypos, combo_layers, combo_timeout, combo_idle, combo_vaargs) \
    / { \
        combos { \
            compatible = "zmk,combos"; \
            combo_ ## name { \
                timeout-ms = <combo_timeout>; \
                bindings = <combo_bindings>; \
                key-positions = <keypos>; \
                layers = <combo_layers>; \
                require-prior-idle-ms = <combo_idle>; \
                combo_vaargs \
            }; \
        }; \
    };

/* ZMK_CONDITIONAL_LAYER */

#define ZMK_CONDITIONAL_LAYER(if_layers, then_layer) \
    / { \
        conditional_layers { \
            compatible = "zmk,conditional-layers"; \
            tri_layer { \
                if-layers = <if_layers>; \
                then-layer = <then_layer>; \
            }; \
        }; \
    };

/* ZMK_UNICODE */

#if !defined OS_UNICODE_LEAD
    #if HOST_OS == 2
        #define OS_UNICODE_LEAD &macro_press &kp LALT      // macOS/Windows-Alt-Codes
    #elif HOST_OS == 1
        #define OS_UNICODE_LEAD &macro_tap &kp LS(LC(U))   // Linux
    #else
        #define OS_UNICODE_LEAD &macro_tap &kp RALT &kp U  // Windows + WinCompose (default)
    #endif
#endif
#if !defined OS_UNICODE_TRAIL
    #if HOST_OS == 2
        #define OS_UNICODE_TRAIL &macro_release &kp LALT  // macOS/Windows-Alt-Codes
    #elif HOST_OS == 1
        #define OS_UNICODE_TRAIL &macro_tap &kp SPACE     // Linux
    #else
        #define OS_UNICODE_TRAIL &macro_tap &kp RET       // Windows + WinCompose (default)
    #endif
#endif

#define UC_MACRO(name, unicode_bindings) \
    / { \
        macros { \
            name: name { \
                compatible = "zmk,behavior-macro"; \
                wait-ms = <0>; \
                tap-ms = <0>; \
                #binding-cells = <0>; \
                bindings = <OS_UNICODE_LEAD>, <&macro_tap unicode_bindings>, <OS_UNICODE_TRAIL>; \
            }; \
        }; \
    };

#define UC_MODMORPH(name, uc_binding, shifted_uc_binding) \
    / { \
        behaviors { \
            name: name { \
                compatible = "zmk,behavior-mod-morph"; \
                #binding-cells = <0>; \
                bindings = <uc_binding>, <shifted_uc_binding>; \
                mods = <(MOD_LSFT|MOD_RSFT)>; \
            }; \
        }; \
    };

#define ZMK_UNICODE_SINGLE(name, L0, L1, L2, L3) \
    UC_MACRO(name ## _lower, &kp L0 &kp L1 &kp L2 &kp L3) \
    UC_MODMORPH(name, &name ## _lower, &none)

#define ZMK_UNICODE_PAIR(name, L0, L1, L2, L3, U0, U1, U2, U3) \
    UC_MACRO(name ## _lower, &kp L0 &kp L1 &kp L2 &kp L3) \
    UC_MACRO(name ## _upper, &kp U0 &kp U1 &kp U2 &kp U3) \
    UC_MODMORPH(name, &name ## _lower, &name ## _upper)

/* ZMK_APPLY_MATRIX_TRANSFORM */

#define ZMK_APPLY_MATRIX_TRANSFORM(transform) \
    / { \
        chosen { zmk,matrix_transform = &transform; }; \
    };

ZMK_UNICODE_PAIR(ru_a    ,    N0 , N4 , N3 , N0 ,    N0 , N4 , N1 , N0) // а/А
ZMK_UNICODE_PAIR(ru_b    ,    N0 , N4 , N3 , N1 ,    N0 , N4 , N1 , N1) // б/Б
ZMK_UNICODE_PAIR(ru_v    ,    N0 , N4 , N3 , N2 ,    N0 , N4 , N1 , N2) // в/В
ZMK_UNICODE_PAIR(ru_g    ,    N0 , N4 , N3 , N3 ,    N0 , N4 , N1 , N3) // г/Г
ZMK_UNICODE_PAIR(ru_d    ,    N0 , N4 , N3 , N4 ,    N0 , N4 , N1 , N4) // д/Д
ZMK_UNICODE_PAIR(ru_ye   ,    N0 , N4 , N3 , N5 ,    N0 , N4 , N1 , N5) // е/Е
ZMK_UNICODE_PAIR(ru_yo   ,    N0 , N4 , N5 , N1 ,    N0 , N4 , N0 , N1) // ё/Ё
ZMK_UNICODE_PAIR(ru_zh   ,    N0 , N4 , N3 , N6 ,    N0 , N4 , N1 , N6) // ж/Ж
ZMK_UNICODE_PAIR(ru_z    ,    N0 , N4 , N3 , N7 ,    N0 , N4 , N1 , N7) // з/З
ZMK_UNICODE_PAIR(ru_i    ,    N0 , N4 , N3 , N8 ,    N0 , N4 , N1 , N8) // и/И
ZMK_UNICODE_PAIR(ru_y    ,    N0 , N4 , N3 , N9 ,    N0 , N4 , N1 , N9) // й/Й
ZMK_UNICODE_PAIR(ru_k    ,    N0 , N4 , N3 ,  A ,    N0 , N4 , N1 ,  A) // к/К
ZMK_UNICODE_PAIR(ru_l    ,    N0 , N4 , N3 ,  B ,    N0 , N4 , N1 ,  B) // л/Л
ZMK_UNICODE_PAIR(ru_m    ,    N0 , N4 , N3 ,  C ,    N0 , N4 , N1 ,  C) // м/М
ZMK_UNICODE_PAIR(ru_n    ,    N0 , N4 , N3 ,  D ,    N0 , N4 , N1 ,  D) // н/Н
ZMK_UNICODE_PAIR(ru_o    ,    N0 , N4 , N3 ,  E ,    N0 , N4 , N1 ,  E) // о/О
ZMK_UNICODE_PAIR(ru_p    ,    N0 , N4 , N3 ,  F ,    N0 , N4 , N1 ,  F) // п/П
ZMK_UNICODE_PAIR(ru_r    ,    N0 , N4 , N4 , N0 ,    N0 , N4 , N2 , N0) // р/Р
ZMK_UNICODE_PAIR(ru_s    ,    N0 , N4 , N4 , N1 ,    N0 , N4 , N2 , N1) // с/С
ZMK_UNICODE_PAIR(ru_t    ,    N0 , N4 , N4 , N2 ,    N0 , N4 , N2 , N2) // т/Т
ZMK_UNICODE_PAIR(ru_u    ,    N0 , N4 , N4 , N3 ,    N0 , N4 , N2 , N3) // у/У
ZMK_UNICODE_PAIR(ru_f    ,    N0 , N4 , N4 , N4 ,    N0 , N4 , N2 , N4) // ф/Ф
ZMK_UNICODE_PAIR(ru_kh   ,    N0 , N4 , N4 , N5 ,    N0 , N4 , N2 , N5) // х/Х
ZMK_UNICODE_PAIR(ru_ts   ,    N0 , N4 , N4 , N6 ,    N0 , N4 , N2 , N6) // ц/Ц
ZMK_UNICODE_PAIR(ru_ch   ,    N0 , N4 , N4 , N7 ,    N0 , N4 , N2 , N7) // ч/Ч
ZMK_UNICODE_PAIR(ru_sh   ,    N0 , N4 , N4 , N8 ,    N0 , N4 , N2 , N8) // ш/Ш
ZMK_UNICODE_PAIR(ru_shch ,    N0 , N4 , N4 , N9 ,    N0 , N4 , N2 , N9) // щ/Щ
ZMK_UNICODE_PAIR(ru_hard ,    N0 , N4 , N4 ,  A ,    N0 , N4 , N2 ,  A) // ъ/Ъ
ZMK_UNICODE_PAIR(ru_bi   ,    N0 , N4 , N4 ,  B ,    N0 , N4 , N2 ,  B) // ы/Ы
ZMK_UNICODE_PAIR(ru_soft ,    N0 , N4 , N4 ,  C ,    N0 , N4 , N2 ,  C) // ь/Ь
ZMK_UNICODE_PAIR(ru_e    ,    N0 , N4 , N4 ,  D ,    N0 , N4 , N2 ,  D) // э/Э
ZMK_UNICODE_PAIR(ru_yu   ,    N0 , N4 , N4 ,  E ,    N0 , N4 , N2 ,  E) // ю/Ю
ZMK_UNICODE_PAIR(ru_ya   ,    N0 , N4 , N4 ,  F ,    N0 , N4 , N2 ,  F) // я/Я

// Special Letters
ZMK_UNICODE_PAIR(ru_yeyo ,    N0 , N4 , N3 ,  N5,    N0 , N4 , N5 , N1) // е/ё
ZMK_UNICODE_PAIR(ru_tone ,    N0 , N4 , N4 ,  C ,    N0 , N4 , N4 ,  A) // ь/ъ

#define MAKE_MOD_TAP(NAME, BINDING1, BINDING2) \
    ZMK_HOLD_TAP(NAME, \
        flavor = "hold-preferred"; \
        tapping-term-ms = <400>; \
        bindings = <BINDING1>, <BINDING2>; \
    )

MAKE_MOD_TAP(mod_ru_f, &kp, &ru_f)
MAKE_MOD_TAP(mod_ru_bi, &kp, &ru_bi)
MAKE_MOD_TAP(mod_ru_v, &kp, &ru_v)
MAKE_MOD_TAP(mod_ru_a, &kp, &ru_a)
MAKE_MOD_TAP(mod_ru_zh, &kp, &ru_zh)
MAKE_MOD_TAP(mod_ru_d, &kp, &ru_d)
MAKE_MOD_TAP(mod_ru_l, &kp, &ru_l)
MAKE_MOD_TAP(mod_ru_o, &kp, &ru_o)
