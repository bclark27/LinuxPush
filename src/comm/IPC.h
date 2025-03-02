#ifndef IPC_H_
#define IPC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Common.h"


/*
 * ================================
 * Custom Types Section
 * ================================
 */
// Define message types. Extend this list with your own types as needed.
typedef enum {
    MSG_TYPE_RAW = 0,       // Raw binary data with header to define length
    
    MSG_TYPE_ABL_PUSH = 1,
    MSG_TYPE_ABL_PAD = 2,
    MSG_TYPE_ABL_BUTTON = 3,
    MSG_TYPE_ABL_KNOB = 4,
    MSG_TYPE_ABL_SLIDER = 5,

    MSG_TYPE_ABL_CMD_PAD = 6,
    MSG_TYPE_ABL_CMD_BUTTON = 7,
    MSG_TYPE_ABL_CMD_PAD_BUTTON = 8,
    MSG_TYPE_ABL_CMD_TEXT = 9,
    MSG_TYPE_ABL_CMD_SYS = 10,
    
    // Add additional message types here...

} MessageType;

// Optionally, you might want to define the structures that go with your messages.
// For example:
typedef struct {
    int someField;
    float anotherField;
} ExampleMessage;


/*
 * ================================
 * Ableton Push Event Packets
 * ================================
 */

typedef struct AbletonPkt_pushEvent
{
  unsigned char pktType;
  float slider_percent;
  unsigned int slider_value;
  unsigned int data;
  int knob_delta;
  int pad_x;
  int pad_y;

  unsigned char pad_velocity;
  unsigned char pad_state;
  unsigned char knob_state;
  unsigned char knob_id;
  unsigned char btn_id;
  unsigned char btn_state;
  unsigned char slider_state;
  unsigned char event_class;
} AbletonPkt_pushEvent;

typedef struct AbletonPkt_pad
{
  unsigned char pktType;
  unsigned int data;
  int id;
  int padX;
  int padY;
  unsigned char isPress;
  unsigned char isHold;
  unsigned char isRelease;
  unsigned char padVelocity;
} AbletonPkt_pad;

typedef struct AbletonPkt_knob
{
  unsigned char pktType;
  unsigned int data;
  unsigned char id;
  char direction;
  unsigned char isPress;
  unsigned char isTurning;
  unsigned char isRelease;
} AbletonPkt_knob;

typedef struct AbletonPkt_button
{
  unsigned char pktType;
  unsigned int data;
  unsigned char btnId;
  unsigned char isPress;
  unsigned char isRelease;

} AbletonPkt_button;

typedef struct AbletonPkt_slider
{
  unsigned char pktType;
  unsigned int data;
  int value;
  float percent;
  unsigned char isSliding;
  unsigned char isPress;
  unsigned char isRelease;
} AbletonPkt_slider;

// REPLY MESSAGES AND TYPES

typedef enum ColorStates {
  ColorStates_CLEAR = (unsigned char)0xff,
  ColorStates_BLACK = (unsigned char)0,
  ColorStates_DARK_GRAY = (unsigned char)1,
  ColorStates_LIGHT_GRAY = (unsigned char)2,
  ColorStates_WHITE = (unsigned char)3,
  ColorStates_LIGHT_RED = (unsigned char)4,
  ColorStates_RED = (unsigned char)5,
  ColorStates_DARK_RED = (unsigned char)6,
  ColorStates_DIM_RED = (unsigned char)7,
  ColorStates_LIGHT_GREEN = (unsigned char)20,
  ColorStates_GREEN = (unsigned char)21,
  ColorStates_DARK_GREEN = (unsigned char)22,
  ColorStates_DIM_GREEN = (unsigned char)23,

  ColorStates_OutOfBounds = ColorStates_RED,
} ColorStates;

enum BlinkStates {
  BlinkStates_BlinkOff = (unsigned char)0,
  BlinkStates_BlinkShot0 = (unsigned char)1,
  BlinkStates_BlinkShot1 = (unsigned char)2,
  BlinkStates_BlinkShot2 = (unsigned char)3,
  BlinkStates_BlinkShot3 = (unsigned char)4,
  BlinkStates_BlinkShot4 = (unsigned char)5,
  BlinkStates_BlinkPulse0 = (unsigned char)6,
  BlinkStates_BlinkPulse1 = (unsigned char)7,
  BlinkStates_BlinkPulse2 = (unsigned char)8,
  BlinkStates_BlinkPulse3 = (unsigned char)9,
  BlinkStates_BlinkPulse4 = (unsigned char)10,
  BlinkStates_BlinkBlink0 = (unsigned char)11,
  BlinkStates_BlinkBlink1 = (unsigned char)12,
  BlinkStates_BlinkBlink2 = (unsigned char)13,
  BlinkStates_BlinkBlink3 = (unsigned char)14,
  BlinkStates_BlinkBlink4 = (unsigned char)15,

  OUT_OF_BOUNDS_BLINK = BlinkStates_BlinkOff,
};

typedef struct AbletonPkt_Cmd_Pad
{
  unsigned int x;
  unsigned int y;
  
  bool setBlink;
  unsigned char blink;

  bool setColor;
  unsigned char color;

} AbletonPkt_Cmd_Pad;

typedef struct AbletonPkt_Cmd_Button
{
  unsigned char id;
  unsigned char blink;
} AbletonPkt_Cmd_Button;

typedef struct AbletonPkt_Cmd_PadButton
{
  unsigned char id;

  bool setBlink;
  unsigned char blink;

  bool setColor;
  unsigned char color;
} AbletonPkt_Cmd_PadButton;

typedef struct AbletonPkt_Cmd_Text
{
  unsigned int x;
  unsigned int y;
  unsigned int length;
  char text[68];
} AbletonPkt_Cmd_Test;

typedef struct AbletonPkt_Cmd_Sys
{
  bool clear;
  bool update;
} AbletonPkt_Cmd_Sys;









typedef unsigned int MessageSize;



  /*
 * ================================
 * API Functions
 * ================================
 */

/* 
 * Service (server) side functions.
 * A process can host one service (and can also act as a client).
 */

// StartService() begins hosting a TCP service identified by a name.
// Returns TS_SUCCESS or an error code.
int IPC_StartService(const char* myServiceName);

// PostMessage() sends a message to all connected clients.
// The protocol automatically prepends a one-byte message type header.
// Returns TS_SUCCESS or an error code.
// Note: dataLen is the length of the payload (excluding the type byte).
int IPC_PostMessage(MessageType msgType, void* data, unsigned int dataLen);

// CloseService() stops the service and closes all client connections.
// Returns TS_SUCCESS or an error code.
int IPC_CloseService();

/*
 * Client side functions.
 * A client can connect to many services, but only once per service.
 * The callback will be called with the message type, a pointer to the payload,
 * and the payload length.
 */
int IPC_ConnectToService(const char* serviceName, void (*onDataReceivedCallback)(MessageType, void*, MessageSize));
int IPC_CloseConnection(const char* serviceName);

/*
 * Return Codes
 */
#define CC_SUCCESS 0
#define CC_ERR_ALREADY_HOSTING -1
#define CC_ERR_NO_SERVICE -2
#define CC_ERR_CONNECTION_FAIL -3
#define CC_ERR_ALREADY_CONNECTED -4
#define CC_ERR_NOT_CONNECTED -5
#define CC_ERR_INTERNAL -6

#ifdef __cplusplus
}
#endif

#endif // TCP_SERVICE_H
