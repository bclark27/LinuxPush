#include <sys/socket.h>
#include <netinet/in.h>

///////////////
//  DEFNIES  //
///////////////

#define SOM_BYTE_LEN    6
#define PKT_SIZE_FIELD  4
#define PKT_TYPE_FIELD  2
#define HEADER_SIZE     (SOM_BYTE_LEN + PKT_SIZE_FIELD + PKT_TYPE_FIELD)
#define MAX_DATA_LEN    2056
#define MAX_BUFF_LEN    (MAX_DATA_LEN + HEADER_SIZE)
#define PORT_CODE(a, b) ((unsigned short)(a) | (((unsigned short)(b)) << 8))

////////////////////////
//  CUSTOME MESSAGES  //
////////////////////////

typedef struct __attribute__((__packed__)) TCP_Packet
{
  unsigned char som[SOM_BYTE_LEN];
  unsigned int fullPacketSize;
  unsigned short type;
  unsigned char data[MAX_DATA_LEN];
} TCP_Packet;

typedef enum TCP_PacketType {
  TCP_PacketType_PUSH,
  TCP_PacketType_PAD,
  TCP_PacketType_BUTTON,
  TCP_PacketType_KNOB,
  TCP_PacketType_SLIDER,
} TCP_PacketType;

////////////////////
//  Ableton Push  //
////////////////////

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

/////////////
//  TYPES  //
/////////////

typedef struct TCPSocketReader
{
  TCP_Packet finishedPacket;
  unsigned int dataLen;

  unsigned int packetWritePos;
  unsigned char gotTotalLen;
  unsigned int somState;

  unsigned int readBuffPos;
  unsigned int readBuffLen;
  unsigned char readBuff[MAX_BUFF_LEN];

} TCPSocketReader;

typedef struct TCPClient
{
  TCPSocketReader socketReader;
  struct sockaddr_in serverAddr;
  int port;
  int c;
} TCPClient;

typedef struct TCPServer
{
  TCPSocketReader socketReader;
  struct sockaddr_in clientAddr; /* client address information          */
  struct sockaddr_in serverAddr; /* server address information          */
  int port;
  int s;                     /* socket for accepting connections    */
  int c;                    /* socket connected to client          */
} TCPServer;

/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

TCPServer * SocketUtils_initTCPServer(unsigned short port);
void SocketUtils_freeTCPServer(TCPServer * tcp);
void SocketUtils_TCPServerSend(TCPServer * tcp, void * data, int len, unsigned short type);
unsigned char *  SocketUtils_TCPServerRecv(TCPServer * tcp, int * len, unsigned short * type);

TCPClient * SocketUtils_initTCPClient(unsigned int port);
void SocketUtils_freeTCPClient(TCPClient * tcp);
void SocketUtils_TCPClientSend(TCPClient * tcp, void * data, int len, unsigned short type);
unsigned char * SocketUtils_TCPClientRecv(TCPClient * tcp, int * len, unsigned short * type);
