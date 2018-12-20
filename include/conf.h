#pragma once

#include <BLEHIDDevice.h>

//#define DEVICE_NAME "Henry-KeyLight"

// #define LASER_LIGHT_PIN 32
// #define NEXT_SLIDE_BUTTON_PIN 25
// #define PREVIOUS_SLIDE_BUTTON_PIN 4
// #define LASER_BUTTON_PIN 5

// Sleep after 5 minutes
#define MAX_ACTIVE_TIME 5 * 60 * 1000

/*
 * HOME  - 0x4a
 * END   - 0x4d
 * RIGHT - 0x4f
 * LEFT  - 0x50
 * DOWN  - 0x51
 * UP    - 0x52
 */
// #define KEY_CODE_LEFT_ARROW 0x50
// #define KEY_CODE_RIGHT_ARROW 0x4f

// #define KEY_CODE_PREVIOUS_SLIDE KEY_CODE_LEFT_ARROW
// #define KEY_CODE_NEXT_SLIDE KEY_CODE_RIGHT_ARROW

// const uint8_t _hidReportDescriptorSurfaceDial[] = {
//     // Integrated Radial Controller TLC
//     0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
//     0x09, 0x0e,       // USAGE (System Multi-Axis Controller)
//     0xa1, 0x01,       // COLLECTION (Application)
//     0x85, 1,          //   REPORT_ID (Radial Controller)
//     0x05, 0x0d,       //   USAGE_PAGE (Digitizers)
//     0x09, 0x21,       //   USAGE (Puck)
//     0xa1, 0x00,       //   COLLECTION (Physical)
//     0x05, 0x09,       //     USAGE_PAGE (Buttons)
//     0x09, 0x01,       //     USAGE (Button 1)
//     0x95, 0x01,       //     REPORT_COUNT (1)
//     0x75, 0x01,       //     REPORT_SIZE (1)
//     0x15, 0x00,       //     LOGICAL_MINIMUM (0)
//     0x25, 0x01,       //     LOGICAL_MAXIMUM (1)
//     0x81, 0x02,       //     INPUT (Data,Var,Abs)
//     0x05, 0x01,       //     USAGE_PAGE (Generic Desktop)
//     0x09, 0x37,       //     USAGE (Dial)
//     0x95, 0x01,       //     REPORT_COUNT (1)
//     0x75, 0x0f,       //     REPORT_SIZE (15)
//     0x55, 0x0f,       //     UNIT_EXPONENT (-1)
//     0x65, 0x14,       //     UNIT (Degrees, English Rotation)
//     0x36, 0xf0, 0xf1, //     PHYSICAL_MINIMUM (-3600)
//     0x46, 0x10, 0x0e, //     PHYSICAL_MAXIMUM (3600)
//     0x16, 0xf0, 0xf1, //     LOGICAL_MINIMUM (-3600)
//     0x26, 0x10, 0x0e, //     LOGICAL_MAXIMUM (3600)
//     0x81, 0x06,       //     INPUT (Data,Var,Rel)
//     // 0x09, 0x30,       //     USAGE (X)
//     // 0x75, 0x10,       //     REPORT_SIZE (16)
//     // 0x55, 0x0d,       //     UNIT_EXPONENT (-3)
//     // 0x65, 0x13,       //     UNIT (Inch,EngLinear)
//     // 0x35, 0x00,       //     PHYSICAL_MINIMUM (0)
//     // 0x46, 0xc0, 0x5d, //     PHYSICAL_MAXIMUM (24000)
//     // 0x15, 0x00,       //     LOGICAL_MINIMUM (0)
//     // 0x26, 0xff, 0x7f, //     LOGICAL_MAXIMUM (32767)
//     // 0x81, 0x02,       //     INPUT (Data,Var,Abs)
//     // 0x09, 0x31,       //     USAGE (Y)
//     // 0x46, 0xb0, 0x36, //     PHYSICAL_MAXIMUM (14000)
//     // 0x81, 0x02,       //     INPUT (Data,Var,Abs)
//     // 0x05, 0x0d,       //     USAGE_PAGE (Digitizers)
//     // 0x09, 0x48,       //     USAGE (Width)
//     // 0x36, 0xb8, 0x0b, //     PHYSICAL_MINIMUM (3000)
//     // 0x46, 0xb8, 0x0b, //     PHYSICAL_MAXIMUM (3000)
//     // 0x16, 0xb8, 0x0b, //     LOGICAL_MINIMUM (3000)
//     // 0x26, 0xb8, 0x0b, //     LOGICAL_MAXIMUM (3000)
//     // 0x81, 0x03        //     INPUT (Cnst,Var,Abs)
//     0xc0, //   END_COLLECTION
//     0xc0  // END_COLLECTION
// };

// const uint8_t KEYBOARD_REPORT_MAP[] PROGMEM = {
    USAGE_PAGE(1),
    0x01, // Generic Desktop Ctrls
    USAGE(1),
    0x06, // Keyboard
    COLLECTION(1),
    0x01, // Application
    REPORT_ID(1),
    0x01, // REPORTID
    USAGE_PAGE(1),
    0x07, //   Kbrd/Keypad
    USAGE_MINIMUM(1),
    0xE0,
    USAGE_MAXIMUM(1),
    0xE7,
    LOGICAL_MINIMUM(1),
    0x00,
    LOGICAL_MAXIMUM(1),
    0x01,
    REPORT_SIZE(1),
    0x01, //   1 byte (Modifier)
    REPORT_COUNT(1),
    0x08,
    HIDINPUT(1),
    0x02, //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),
    0x01, //   1 byte (Reserved)
    REPORT_SIZE(1),
    0x08,
    HIDINPUT(1),
    0x01, //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    REPORT_COUNT(1),
    0x05, //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1),
    0x01,
    USAGE_PAGE(1),
    0x08, //   LEDs
    USAGE_MINIMUM(1),
    0x01, //   Num Lock
    USAGE_MAXIMUM(1),
    0x05, //   Kana
    HIDOUTPUT(1),
    0x02, //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),
    0x01, //   3 bits (Padding)
    REPORT_SIZE(1),
    0x03,
    HIDOUTPUT(1),
    0x01, //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
    REPORT_COUNT(1),
    0x06, //   6 bytes (Keys)
    REPORT_SIZE(1),
    0x08,
    LOGICAL_MINIMUM(1),
    0x00,
    LOGICAL_MAXIMUM(1),
    0x65, //   101 keys
    USAGE_PAGE(1),
    0x07, //   Kbrd/Keypad
    USAGE_MINIMUM(1),
    0x00,
    USAGE_MAXIMUM(1),
    0x65,
    HIDINPUT(1),
    0x00, //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
    END_COLLECTION(0)};