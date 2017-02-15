/**
 * The MIT License (MIT)
 * Copyright (c) 2016-2017 Tuetuopay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include "WindowBackend.h"

#if defined(WINDOW_BACKEND_GLFW3)

#define HID_KEY_UNKNOWN                 GLFW_KEY_UNKNOWN
#define HID_KEY_SPACE                   GLFW_KEY_SPACE
#define HID_KEY_APOSTROPHE              GLFW_KEY_APOSTROPHE
#define HID_KEY_COMMA                   GLFW_KEY_COMMA
#define HID_KEY_MINUS                   GLFW_KEY_MINUS
#define HID_KEY_PERIOD                  GLFW_KEY_PERIOD
#define HID_KEY_SLASH                   GLFW_KEY_SLASH
#define HID_KEY_0                       GLFW_KEY_0
#define HID_KEY_1                       GLFW_KEY_1
#define HID_KEY_2                       GLFW_KEY_2
#define HID_KEY_3                       GLFW_KEY_3
#define HID_KEY_4                       GLFW_KEY_4
#define HID_KEY_5                       GLFW_KEY_5
#define HID_KEY_6                       GLFW_KEY_6
#define HID_KEY_7                       GLFW_KEY_7
#define HID_KEY_8                       GLFW_KEY_8
#define HID_KEY_9                       GLFW_KEY_9
#define HID_KEY_SEMICOLON               GLFW_KEY_SEMICOLON
#define HID_KEY_EQUAL                   GLFW_KEY_EQUAL
#define HID_KEY_A                       GLFW_KEY_A
#define HID_KEY_B                       GLFW_KEY_B
#define HID_KEY_C                       GLFW_KEY_C
#define HID_KEY_D                       GLFW_KEY_D
#define HID_KEY_E                       GLFW_KEY_E
#define HID_KEY_F                       GLFW_KEY_F
#define HID_KEY_G                       GLFW_KEY_G
#define HID_KEY_H                       GLFW_KEY_H
#define HID_KEY_I                       GLFW_KEY_I
#define HID_KEY_J                       GLFW_KEY_J
#define HID_KEY_K                       GLFW_KEY_K
#define HID_KEY_L                       GLFW_KEY_L
#define HID_KEY_M                       GLFW_KEY_M
#define HID_KEY_N                       GLFW_KEY_N
#define HID_KEY_O                       GLFW_KEY_O
#define HID_KEY_P                       GLFW_KEY_P
#define HID_KEY_Q                       GLFW_KEY_Q
#define HID_KEY_R                       GLFW_KEY_R
#define HID_KEY_S                       GLFW_KEY_S
#define HID_KEY_T                       GLFW_KEY_T
#define HID_KEY_U                       GLFW_KEY_U
#define HID_KEY_V                       GLFW_KEY_V
#define HID_KEY_W                       GLFW_KEY_W
#define HID_KEY_X                       GLFW_KEY_X
#define HID_KEY_Y                       GLFW_KEY_Y
#define HID_KEY_Z                       GLFW_KEY_Z
#define HID_KEY_LEFT_BRACKET            GLFW_KEY_LEFT_BRACKET
#define HID_KEY_BACKSLASH               GLFW_KEY_BACKSLASH
#define HID_KEY_RIGHT_BRACKET           GLFW_KEY_RIGHT_BRACKET
#define HID_KEY_GRAVE_ACCENT            GLFW_KEY_GRAVE_ACCENT
#define HID_KEY_WORLD_1                 GLFW_KEY_WORLD_1
#define HID_KEY_WORLD_2                 GLFW_KEY_WORLD_2
#define HID_KEY_ESCAPE                  GLFW_KEY_ESCAPE
#define HID_KEY_ENTER                   GLFW_KEY_ENTER
#define HID_KEY_TAB                     GLFW_KEY_TAB
#define HID_KEY_BACKSPACE               GLFW_KEY_BACKSPACE
#define HID_KEY_INSERT                  GLFW_KEY_INSERT
#define HID_KEY_DELETE                  GLFW_KEY_DELETE
#define HID_KEY_RIGHT                   GLFW_KEY_RIGHT
#define HID_KEY_LEFT                    GLFW_KEY_LEFT
#define HID_KEY_DOWN                    GLFW_KEY_DOWN
#define HID_KEY_UP                      GLFW_KEY_UP
#define HID_KEY_PAGE_UP                 GLFW_KEY_PAGE_UP
#define HID_KEY_PAGE_DOWN               GLFW_KEY_PAGE_DOWN
#define HID_KEY_HOME                    GLFW_KEY_HOME
#define HID_KEY_END                     GLFW_KEY_END
#define HID_KEY_CAPS_LOCK               GLFW_KEY_CAPS_LOCK
#define HID_KEY_SCROLL_LOCK             GLFW_KEY_SCROLL_LOCK
#define HID_KEY_NUM_LOCK                GLFW_KEY_NUM_LOCK
#define HID_KEY_PRINT_SCREEN            GLFW_KEY_PRINT_SCREEN
#define HID_KEY_PAUSE                   GLFW_KEY_PAUSE
#define HID_KEY_F1                      GLFW_KEY_F1
#define HID_KEY_F2                      GLFW_KEY_F2
#define HID_KEY_F3                      GLFW_KEY_F3
#define HID_KEY_F4                      GLFW_KEY_F4
#define HID_KEY_F5                      GLFW_KEY_F5
#define HID_KEY_F6                      GLFW_KEY_F6
#define HID_KEY_F7                      GLFW_KEY_F7
#define HID_KEY_F8                      GLFW_KEY_F8
#define HID_KEY_F9                      GLFW_KEY_F9
#define HID_KEY_F10                     GLFW_KEY_F10
#define HID_KEY_F11                     GLFW_KEY_F11
#define HID_KEY_F12                     GLFW_KEY_F12
#define HID_KEY_F13                     GLFW_KEY_F13
#define HID_KEY_F14                     GLFW_KEY_F14
#define HID_KEY_F15                     GLFW_KEY_F15
#define HID_KEY_F16                     GLFW_KEY_F16
#define HID_KEY_F17                     GLFW_KEY_F17
#define HID_KEY_F18                     GLFW_KEY_F18
#define HID_KEY_F19                     GLFW_KEY_F19
#define HID_KEY_F20                     GLFW_KEY_F20
#define HID_KEY_F21                     GLFW_KEY_F21
#define HID_KEY_F22                     GLFW_KEY_F22
#define HID_KEY_F23                     GLFW_KEY_F23
#define HID_KEY_F24                     GLFW_KEY_F24
#define HID_KEY_F25                     GLFW_KEY_F25
#define HID_KEY_KP_0                    GLFW_KEY_KP_0
#define HID_KEY_KP_1                    GLFW_KEY_KP_1
#define HID_KEY_KP_2                    GLFW_KEY_KP_2
#define HID_KEY_KP_3                    GLFW_KEY_KP_3
#define HID_KEY_KP_4                    GLFW_KEY_KP_4
#define HID_KEY_KP_5                    GLFW_KEY_KP_5
#define HID_KEY_KP_6                    GLFW_KEY_KP_6
#define HID_KEY_KP_7                    GLFW_KEY_KP_7
#define HID_KEY_KP_8                    GLFW_KEY_KP_8
#define HID_KEY_KP_9                    GLFW_KEY_KP_9
#define HID_KEY_KP_DECIMAL              GLFW_KEY_KP_DECIMAL
#define HID_KEY_KP_DIVIDE               GLFW_KEY_KP_DIVIDE
#define HID_KEY_KP_MULTIPLY             GLFW_KEY_KP_MULTIPLY
#define HID_KEY_KP_SUBTRACT             GLFW_KEY_KP_SUBTRACT
#define HID_KEY_KP_ADD                  GLFW_KEY_KP_ADD
#define HID_KEY_KP_ENTER                GLFW_KEY_KP_ENTER
#define HID_KEY_KP_EQUAL                GLFW_KEY_KP_EQUAL
#define HID_KEY_LEFT_SHIFT              GLFW_KEY_LEFT_SHIFT
#define HID_KEY_LEFT_CONTROL            GLFW_KEY_LEFT_CONTROL
#define HID_KEY_LEFT_ALT                GLFW_KEY_LEFT_ALT
#define HID_KEY_LEFT_SUPER              GLFW_KEY_LEFT_SUPER
#define HID_KEY_RIGHT_SHIFT             GLFW_KEY_RIGHT_SHIFT
#define HID_KEY_RIGHT_CONTROL           GLFW_KEY_RIGHT_CONTROL
#define HID_KEY_RIGHT_ALT               GLFW_KEY_RIGHT_ALT
#define HID_KEY_RIGHT_SUPER             GLFW_KEY_RIGHT_SUPER
#define HID_KEY_MENU                    GLFW_KEY_MENU
#define HID_KEY_LAST                    GLFW_KEY_LAST

#define HID_PRESS                       GLFW_PRESS
#define HID_RELEASE                     GLFW_RELEASE
#define HID_REPEAT                      GLFW_REPEAT

#elif defined(WINDOW_BACKEND_SDL2)

#endif

