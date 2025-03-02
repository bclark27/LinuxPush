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
