/*
 Part of Libnodave, a free communication libray for Siemens S7 200/300/400 via
 the MPI adapter 6ES7 972-0CA22-0XAC
 or  MPI adapter 6ES7 972-0CA23-0XAC
 or  TS adapter 6ES7 972-0CA33-0XAC
 or  MPI adapter 6ES7 972-0CA11-0XAC,
 IBH/MHJ-NetLink or CPs 243, 343 and 443
 or VIPA Speed7 with builtin ethernet support.

 (C) Thomas Hergenhahn (thomas.hergenhahn@web.de) 2002..2005

 Libnodave is free software; you can redistribute it and/or modify
 it under the terms of the GNU Library General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 Libnodave is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with Libnodave; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __nodave
#define __nodave

#include "config.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef API_KNOWN
#error Fill in what you need for your OS or API.
#endif

enum daveConnectionType {
    daveSerialConnection = 0,
    daveTcpConnection = 1,
#ifdef HAVE_S7ONLINE
    daveS7OnlineConnection = 2,
#endif // HAVE_S7ONLINE
};

#ifdef OS_LINUX

#include <stdlib.h>
typedef int connection_t;
typedef int timeout_t;
#endif // OS_LINUX

#ifdef OS_WINDOWS
#include <WinSock2.h>	// according to Jochen Kühner, this is needed to compile on Win64
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef int timeout_t;
typedef HANDLE connection_t;

#endif // OS_WINDOWS

#ifdef OS_DOS
#include <stdio.h>
#include <stdlib.h>

typedef int timeout_t;
typedef connection_t int;

#endif // OS_DOS

#ifdef AVR
#include <stdio.h>
#include <stdlib.h>

typedef connection_t int;
typedef long timeout_t;
#endif // AVR


// remember whether this is a socket or a serial connection or a handle or whatever
typedef struct dost {
    connection_t rfd;
    connection_t wfd;
} _daveOSserialType;

/*
    some frequently used ASCII control codes:
*/
enum ASCIICtrlCodes {
    DLE = 0x10,
    ETX = 0x03,
    STX = 0x02,
    SYN = 0x16,
    NAK = 0x15,
    EOT = 0x04, //  for S5
    ACK = 0x06, //  for S5
};
/*
    Protocol types to be used with newInterface:
*/
enum daveProtocol {
    daveProtoMPI = 0,	/* MPI for S7 300/400 */
    daveProtoMPI2 = 1,	/* MPI for S7 300/400, "Andrew's version" without STX */
    daveProtoMPI3 = 2, 	/* MPI for S7 300/400, Step 7 Version, not yet implemented */
    daveProtoMPI4 = 3,	/* MPI for S7 300/400, "Andrew's version" with STX */

    daveProtoPPI = 10,	/* PPI for S7 200 */

    daveProtoAS511 = 20,	/* S5 programming port protocol */

#ifdef HAVE_S7ONLINE
    daveProtoS7online = 50,	/* use s7onlinx.dll for transport */
#endif // HAVE_S7ONLINE

    daveProtoISOTCP = 122,	/* ISO over TCP */
    daveProtoISOTCP243 = 123,	/* ISO over TCP with CP243 */

    daveProtoMPI_IBH = 223,	/* MPI with IBH NetLink MPI to ethernet gateway */
    daveProtoPPI_IBH = 224,	/* PPI with IBH NetLink PPI to ethernet gateway */

    daveProtoNLpro = 230,	/* MPI with NetLink Pro MPI to ethernet gateway */

    daveProtoUserTransport = 255,	/* Libnodave will pass the PDUs of S7 Communication to user */
    /* defined call back functions. */
};

/*
    Communication types
*/
enum daveCommunication {
    davePGCommunication = 1,	/* communication with programming device (PG) (default in Libnodave) */
    daveProgrammerCommunication = 1,	/* communication with programming device (PG) (default in Libnodave) */
    daveOPCommunication = 2,	/* communication with operator panel (OP)) */
    daveS7BasicCommunication = 3,	/* communication with another CPU ? */
};

/*
 *  Speed constants:
*/
enum daveSpeed {
    daveSpeed9k    = 0,
    daveSpeed19k   = 1,
    daveSpeed187k  = 2,
    daveSpeed500k  = 3,
    daveSpeed1500k = 4,
    daveSpeed45k   = 5,
    daveSpeed93k   = 6,
};

/*
    Some S7 communication function codes (yet unused ones may be incorrect).
*/
enum daveFuncCode {
    daveFuncOpenS7Connection = 0xF0,
    daveFuncRead             = 0x04,
    daveFuncWrite            = 0x05,
    daveFuncRequestDownload  = 0x1A,
    daveFuncDownloadBlock    = 0x1B,
    daveFuncDownloadEnded    = 0x1C,
    daveFuncStartUpload      = 0x1D,
    daveFuncUpload           = 0x1E,
    daveFuncEndUpload        = 0x1F,
    daveFuncInsertBlock      = 0x28,
};

/*
    S7 specific constants:
*/
enum daveBlockType {
    daveBlockType_OB   = '8',
    daveBlockType_DB   = 'A',
    daveBlockType_SDB  = 'B',
    daveBlockType_FC   = 'C',
    daveBlockType_SFC  = 'D',
    daveBlockType_FB   = 'E',
    daveBlockType_SFB  = 'F',

    daveS5BlockType_DB   = 0x01,
    daveS5BlockType_SB   = 0x02,
    daveS5BlockType_PB   = 0x04,
    daveS5BlockType_FX   = 0x05,
    daveS5BlockType_FB   = 0x08,
    daveS5BlockType_DX   = 0x0C,
    daveS5BlockType_OB   = 0x10,
};
/*
    Use these constants for parameter "area" in daveReadBytes and daveWriteBytes
*/
enum daveAreaType {
    daveSysInfo  = 0x3, /* System info of 200 family */
    daveSysFlags = 0x5, /* System flags of 200 family */
    daveAnaIn    = 0x6, /* analog inputs of 200 family */
    daveAnaOut   = 0x7, /* analog outputs of 200 family */

    daveP           = 0x80, /* direct peripheral access */
    daveInputs      = 0x81,
    daveOutputs     = 0x82,
    daveFlags       = 0x83,
    daveDB          = 0x84, /* data blocks */
    daveDI          = 0x85, /* instance data blocks */
    daveLocal       = 0x86, /* not tested */
    daveV           = 0x87, /* don't know what it is */
    daveCounter     = 28,   /* S7 counters */
    daveTimer       = 29,   /* S7 timers */
    daveCounter200  = 30,   /* IEC counters (200 family) */
    daveTimer200    = 31,   /* IEC timers (200 family) */
    daveSysDataS5   = 0x86, /* system data area ? */
    daveRawMemoryS5 = 0x00, /* just the raw memory */
};

/**
    Library specific:
**/
/*
    Result codes. Genarally, 0 means ok,
    >0 are results (also errors) reported by the PLC
    <0 means error reported by library code.
*/
enum daveResultCode {
    daveResOK  = 0,                     /* means all ok */
    daveResNoPeripheralAtAddress    = 1,/* CPU tells there is no peripheral at address */
    daveResMultipleBitsNotSupported = 6,/* CPU tells it does not support to read a bit block with a */
                                        /* length other than 1 bit. */
    daveResItemNotAvailable200  = 3,    /* means a a piece of data is not available in the CPU, e.g. */
                                        /* when trying to read a non existing DB or bit bloc of length<>1 */
                                        /* This code seems to be specific to 200 family. */

    daveResItemNotAvailable  = 10,  /* means a a piece of data is not available in the CPU, e.g. */
                        /* when trying to read a non existing DB */

    daveAddressOutOfRange  = 5,			/* means the data address is beyond the CPUs address range */
    daveWriteDataSizeMismatch  = 7,		/* means the write data size doesn't fit item size */
    daveResCannotEvaluatePDU  = -123,    	/* PDU is not understood by libnodave */
    daveResCPUNoData  = -124,
    daveUnknownError  = -125,
    daveEmptyResultError  = -126,
    daveEmptyResultSetError  = -127,
    daveResUnexpectedFunc  = -128,
    daveResUnknownDataUnitSize  = -129,
    daveResNoBuffer  = -130,
    daveNotAvailableInS5  = -131,
    daveResInvalidLength  = -132,
    daveResInvalidParam  = -133,
    daveResNotYetImplemented  = -134,

    daveResShortPacket = -1024,
    daveResTimeout = -1025,
};
/*
    Error code to message string conversion:
    Call this function to get an explanation for error codes returned by other functions.
*/
EXPORTSPEC const char * CALL_SPEC daveStrerror(int code); // result is char because this is usual for strings

/*
    Copy an internal String into an external string buffer. This is needed to interface
    with Visual Basic. Maybe it is helpful elsewhere, too.
*/
EXPORTSPEC void CALL_SPEC daveStringCopy(char * intString, char * extString); // args are char because this is usual for strings


/*
    Max number of bytes in a single message.
    An upper limit for MPI over serial is:
    8		transport header
    +2*240	max PDU len *2 if every character were a DLE
    +3		DLE,ETX and BCC
    = 491

    Later I saw some programs offering up to 960 bytes in PDU size negotiation

    Max number of bytes in a single message.
    An upper limit for MPI over serial is:
    8		transport header
    +2*960	max PDU len *2 if every character were a DLE
    +3		DLE,ETX and BCC
    = 1931

    For now, we take the rounded max of all this to determine our buffer size. This is ok
    for PC systems, where one k less or more doesn't matter.
*/
enum daveMaxRawLength {
    daveMaxRawLen = 2048
};
/*
    Some definitions for debugging:
*/
enum daveDebugCode {
    daveDebugRawRead        = 0x01, /* Show the single bytes received */
    daveDebugSpecialChars   = 0x02, /* Show when special chars are read */
    daveDebugRawWrite       = 0x04, /* Show the single bytes written */
    daveDebugListReachables = 0x08, /* Show the steps when determine devices in MPI net */
    daveDebugInitAdapter    = 0x10, /* Show the steps when Initilizing the MPI adapter */
    daveDebugConnect        = 0x20, /* Show the steps when connecting a PLC */
    daveDebugPacket         = 0x40,
    daveDebugByte           = 0x80,
    daveDebugCompare        = 0x100,
    daveDebugExchange       = 0x200,
    daveDebugPDU            = 0x400,	/* debug PDU handling */
    daveDebugUpload         = 0x800,	/* debug PDU loading program blocks from PLC */
    daveDebugMPI            = 0x1000,
    daveDebugPrintErrors    = 0x2000,	/* Print error messages */
    daveDebugPassive        = 0x4000,

    daveDebugErrorReporting = 0x8000,
    daveDebugOpen           = 0x10000,  /* print messages in openSocket and setPort */

    daveDebugAll            = 0x1ffff,
};
/*
  IBH-NetLink packet types:
*/
enum daveNetLinkPacketType {
    _davePtEmpty = -2,
    _davePtMPIAck = -3,
    _davePtUnknownMPIFunc = -4,
    _davePtUnknownPDUFunc = -5,
    _davePtReadResponse = 1,
    _davePtWriteResponse = 2,
};

/*
    set and read debug level:
*/
EXPORTSPEC void CALL_SPEC daveSetDebug(int nDebug);
EXPORTSPEC int CALL_SPEC daveGetDebug(void);
/*
    Some data types:
*/
// usigned
typedef uint8_t  uc;
typedef uint16_t us;
typedef uint32_t u32;
// signed
typedef int8_t  i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;

/*
    This is a wrapper for the serial or ethernet interface. This is here to make porting easier.
*/

typedef struct _daveConnection  daveConnection;
typedef struct _daveInterface  daveInterface;

/*
    Helper struct to manage PDUs. This is NOT the part of the packet I would call PDU, but
    a set of pointers that ease access to the "private parts" of a PDU.
*/
typedef struct {
    uc * header;	/* pointer to start of PDU (PDU header) */
    uc * param;		/* pointer to start of parameters inside PDU */
    uc * data;		/* pointer to start of data inside PDU */
    uc * udata;		/* pointer to start of data inside PDU */
    int hlen;		/* header length */
    int plen;		/* parameter length */
    int dlen;		/* data length */
    int udlen;		/* user or result data length */
} PDU;

/*
    Definitions of prototypes for the protocol specific functions. The library "switches"
    protocol by setting pointers to the protol specific implementations.
*/
typedef int (CALL_SPEC *  _initAdapterFunc) (daveInterface *);
typedef int (CALL_SPEC *  _connectPLCFunc) (daveConnection *);
typedef int (CALL_SPEC * _disconnectPLCFunc) (daveConnection *);
typedef int (CALL_SPEC * _disconnectAdapterFunc) (daveInterface *);
typedef int (CALL_SPEC * _exchangeFunc) (daveConnection *, PDU *);
typedef int (CALL_SPEC * _sendMessageFunc) (daveConnection *, PDU *);
typedef int (CALL_SPEC * _getResponseFunc) (daveConnection *);
typedef int (CALL_SPEC * _listReachablePartnersFunc) (daveInterface * di, char * buf); // changed to unsigned char because it is a copy of an uc buffer

/*
    Definitions of prototypes for i/O functions.
*/
typedef int (CALL_SPEC *  _writeFunc) (daveInterface *, char *, int); // changed to char because char is what system read/write expects
typedef int (CALL_SPEC *  _readFunc) (daveInterface *, char *, int);

/*
    This groups an interface together with some information about it's properties
    in the library's context.
*/
struct _daveInterface {
    timeout_t timeout;	/* Timeout in microseconds used in transort. */
    _daveOSserialType fd; /* some handle for the serial interface */
    int localMPI;	/* the adapter's MPI address */

    int users;		/* a counter used when multiple PLCs are accessed via */
            /* the same serial interface and adapter. */
    char * name;	/* just a name that can be used in programs dealing with multiple */
            /* daveInterfaces */
    int protocol;	/* The kind of transport protocol used on this interface. */
    int speed;		/* The MPI or Profibus speed */
    int ackPos;		/* position of some packet number that has to be repeated in ackknowledges */
    int nextConnection;
    _initAdapterFunc initAdapter;		/* pointers to the protocol */
    _connectPLCFunc connectPLC;			/* specific implementations */
    _disconnectPLCFunc disconnectPLC;		/* of these functions */
    _disconnectAdapterFunc disconnectAdapter;
    _exchangeFunc exchange;
    _sendMessageFunc sendMessage;
    _getResponseFunc getResponse;
    _listReachablePartnersFunc listReachablePartners;
    char realName[20];
    _readFunc ifread;
    _writeFunc ifwrite;
    int seqNumber;
};

EXPORTSPEC daveInterface * CALL_SPEC daveNewInterface(_daveOSserialType nfd, const char * nname, int localMPI, int protocol, int speed);
EXPORTSPEC daveInterface * CALL_SPEC davePascalNewInterface(_daveOSserialType* nfd, char * nname, int localMPI, int protocol, int speed);
/*
    This is the packet header used by IBH ethernet NetLink.
*/
typedef struct {
    uc ch1;	// logical connection or channel ?
    uc ch2;	// logical connection or channel ?
    uc len;	// number of bytes counted from the ninth one.
    uc packetNumber;	// a counter, response packets refer to request packets
    us sFlags;		// my guess
    us rFlags;		// my interpretation
} IBHpacket;

/*
    Header for MPI packets on IBH-NetLink:
*/

typedef struct {
    uc src_conn;
    uc dst_conn;
    uc MPI;
    uc localMPI;
    uc len;
    uc func;
    uc packetNumber;
} MPIheader;

typedef struct {
    uc src_conn;
    uc dst_conn;
    uc MPI;
    uc xxx1;
    uc xxx2;
    uc xx22;
    uc len;
    uc func;
    uc packetNumber;
}  MPIheader2;

typedef struct _daveS5AreaInfo {
    int area;
    int DBnumber;
    int address;
    int len;
    struct _daveS5AreaInfo * next;
} daveS5AreaInfo;

typedef struct _daveS5cache {
    int PAE;	// start of inputs
    int PAA;	// start of outputs
    int flags;	// start of flag (marker) memory
    int timers;	// start of timer memory
    int counters;// start of counter memory
    int systemData;// start of system data
    daveS5AreaInfo * first;
} daveS5cache;


typedef struct _daveRoutingData {
    int connectionType;
    int destinationType; 	// destinationIsIP=DestinationIsIP;
    int subnetID1;
    int subnetID2;
    int subnetID3;
    int PLCadrsize;
    uc  PLCadr[4];		// currently, IP is maximum. Maybe there could be MAC adresses for Industrial Ethernet?
} daveRoutingData;

/*
    This holds data for a PLC connection;
*/
struct _daveConnection {
    int AnswLen;	/* length of last message */
    uc * resultPointer;	/* used to retrieve single values from the result byte array */
    int maxPDUlength;
    int MPIAdr;		/* The PLC's address */
    daveInterface * iface; /* pointer to used interface */
    int needAckNumber;	/* message number we need ackknowledge for */
    int PDUnumber; 	/* current PDU number */
    int ibhSrcConn;
    int ibhDstConn;
    uc msgIn[daveMaxRawLen];
    uc msgOut[daveMaxRawLen];
    uc * _resultPointer;
    int PDUstartO;	/* position of PDU in outgoing messages. This is different for different transport methodes. */
    int PDUstartI;	/* position of PDU in incoming messages. This is different for different transport methodes. */
    int rack;		/* rack number for ISO over TCP */
    int slot;		/* slot number for ISO over TCP */
    int connectionNumber;
    int connectionNumber2;
    uc 	messageNumber;  /* current MPI message number */
    uc	packetNumber;	/* packetNumber in transport layer */
    void * hook;	/* used in CPU/CP simulation: pointer to the rest we have to send if message doesn't fit in a single packet */
    daveS5cache * cache; /* used in AS511: We cache addresses of memory areas and datablocks here */
    int TPDUsize; 		// size of TPDU for ISO over TCP
    int partPos;  		// remember position for ISO over TCP fragmentation
    int routing;		// nonzero means routing enabled
    int communicationType;		// (1=PG Communication,2=OP Communication,3=Step7Basic Communication)
    daveRoutingData routingData;
};

EXPORTSPEC void CALL_SPEC daveSetRoutingDestination(daveConnection * dc, int subnet1, int subnet3, int adrsize, uc* plcadr);
/*
        void * Destination,
        int DestinationIsIP,
        int rack, int slot,
        int routing,
        int routingSubnetFirst,
        int routingSubnetSecond,
        int routingRack,
        int routingSlot,
        void * routingDestination,
        int routingDestinationIsIP,
        int ConnectionType,
        int routingConnectionType) {
*/

EXPORTSPEC void CALL_SPEC daveSetCommunicationType(daveConnection * dc, int communicationType);
/*
EXPORTSPEC daveConnection * CALL_SPEC daveNewEonnection(daveInterface * di,
        void * Destination,
        int DestinationIsIP,
        int rack, int slot,
        int routing,
        int routingSubnetFirst,
        int routingSubnetSecond,
        int routingRack,
        int routingSlot,
        void * routingDestination,
        int routingDestinationIsIP,
        int ConnectionType,
        int routingConnectionType) {
*/
/*
    Setup a new connection structure using an initialized
    daveInterface and PLC's MPI address.
*/
EXPORTSPEC daveConnection * CALL_SPEC daveNewConnection(daveInterface * di, int MPI, int rack, int slot);


typedef struct {
    uc type[2];
    unsigned short count;
} daveBlockTypeEntry;

typedef struct {
    unsigned short number;
    uc type[2];
} daveBlockEntry;

typedef struct {
    uc type[2];
    uc x1[2];  /* 00 4A */
    uc w1[2];  /* some word var? */
    char pp[2]; /* allways 'pp' */
    uc x2[4];  /* 00 4A */
    unsigned short number; /* the block's number */
    uc x3[26];  /* ? */
    unsigned short length; /* the block's length */
    uc x4[16];
    uc name[8];
    uc x5[12];
} daveBlockInfo;
/**
    PDU handling:
    PDU is the central structure present in S7 communication.
    It is composed of a 10 or 12 byte header,a parameter block and a data block.
    When reading or writing values, the data field is itself composed of a data
    header followed by payload data
**/
typedef struct {
    uc P;	/* allways 0x32 */
    uc type;	/* Header type, one of 1,2,3 or 7. type 2 and 3 headers are two bytes longer. */
    uc a, b;	/* currently unknown. Maybe it can be used for long numbers? */
    us number;	/* A number. This can be used to make sure a received answer */
        /* corresponds to the request with the same number. */
    us plen;	/* length of parameters which follow this header */
    us dlen;	/* length of data which follow the parameters */
    uc result[2]; /* only present in type 2 and 3 headers. This contains error information. */
} PDUHeader;

/*
    same as above, but made up of single bytes only, so that every single byte can be adressed separately
*/
typedef struct {
    uc P;	/* allways 0x32 */
    uc type;	/* Header type, one of 1,2,3 or 7. type 2 and 3 headers are two bytes longer. */
    uc a, b;	/* currently unknown. Maybe it can be used for long numbers? */
    uc numberHi, numberLo;	/* A number. This can be used to make sure a received answer */
        /* corresponds to the request with the same number. */
    uc plenHi, plenLo;	/* length of parameters which follow this header */
    uc dlenHi, dlenLo;	/* length of data which follow the parameters */
    uc result[2]; /* only present in type 2 and 3 headers. This contains error information. */
} PDUHeader2;
/*
    set up the header. Needs valid header pointer in the struct p points to.
*/
EXPORTSPEC void CALL_SPEC _daveInitPDUheader(PDU * p, int type);
/*
    add parameters after header, adjust pointer to data.
    needs valid header
*/
EXPORTSPEC void CALL_SPEC _daveAddParam(PDU * p, uc * param, us len);
/*
    add data after parameters, set dlen
    needs valid header,and valid parameters.
*/
EXPORTSPEC void CALL_SPEC _daveAddData(PDU * p, void * data, int len);
/*
    add values after value header in data, adjust dlen and data count.
    needs valid header,parameters,data,dlen
*/
EXPORTSPEC void CALL_SPEC _daveAddValue(PDU * p, void * data, int len);
/*
    add data in user data. Add a user data header, if not yet present.
*/
EXPORTSPEC void CALL_SPEC _daveAddUserData(PDU * p, uc * da, int len);
/*
    set up pointers to the fields of a received message
*/
EXPORTSPEC int CALL_SPEC _daveSetupReceivedPDU(daveConnection * dc, PDU * p);
/*
    Get the eror code from a PDU, if one.
*/
EXPORTSPEC int CALL_SPEC daveGetPDUerror(PDU * p);
/*
    send PDU to PLC and retrieve the answer
*/
EXPORTSPEC int CALL_SPEC _daveExchange(daveConnection * dc, PDU *p);
/*
    retrieve the answer
*/
EXPORTSPEC int CALL_SPEC daveGetResponse(daveConnection * dc);
/*
    send PDU to PLC
*/
EXPORTSPEC int CALL_SPEC daveSendMessage(daveConnection * dc, PDU * p);

/******

    Utilities:

****/
/*
    Hex dump PDU:
*/
EXPORTSPEC void CALL_SPEC _daveDumpPDU(PDU * p);

/*
    This is an extended memory compare routine. It can handle don't care and stop flags
    in the sample data. A stop flag lets it return success, if there were no mismatches
    up to this point.
*/
EXPORTSPEC int CALL_SPEC _daveMemcmp(us * a, uc *b, size_t len);

/*
    Hex dump. Write the name followed by len bytes written in hex and a newline:
*/
//EXPORTSPEC void CALL_SPEC _daveDump(char * name, uc *b, int len);
EXPORTSPEC void CALL_SPEC _daveDump(const char * name, void *b, int len);

/*
    names for PLC objects:
*/
EXPORTSPEC const char * CALL_SPEC daveBlockName(uc bn);  // char or uc,to decide
EXPORTSPEC const char * CALL_SPEC daveAreaName(uc n); // to decide

/*
    swap functions. These swap function do a swao on little endian machines only:
*/
EXPORTSPEC i16_t CALL_SPEC daveSwapIed_16(i16_t ff);
EXPORTSPEC i32_t CALL_SPEC daveSwapIed_32(i32_t ff);

/**
    Data conversion convenience functions. The older set has been removed.
    Newer conversion routines. As the terms WORD, INT, INTEGER etc have different meanings
    for users of different programming languages and compilers, I choose to provide a new
    set of conversion routines named according to the bit length of the value used. The 'U'
    or 'S' stands for unsigned or signed.
**/
/*
    Get a value from the position b points to. B is typically a pointer to a buffer that has
    been filled with daveReadBytes:
*/
EXPORTSPEC float CALL_SPEC daveGetFloatAt(daveConnection * dc, int pos);
EXPORTSPEC float CALL_SPEC daveGetKGAt(daveConnection * dc, int pos);


EXPORTSPEC float CALL_SPEC toPLCfloat(float ff);
EXPORTSPEC int CALL_SPEC daveToPLCfloat(float ff);
EXPORTSPEC int CALL_SPEC daveToKG(float ff);


EXPORTSPEC int CALL_SPEC daveGetS8from(uc *b);
EXPORTSPEC int CALL_SPEC daveGetU8from(uc *b);
EXPORTSPEC int CALL_SPEC daveGetS16from(uc *b);
EXPORTSPEC int CALL_SPEC daveGetU16from(uc *b);
EXPORTSPEC int CALL_SPEC daveGetS32from(uc *b);
EXPORTSPEC unsigned int CALL_SPEC daveGetU32from(uc *b);
EXPORTSPEC float CALL_SPEC daveGetFloatfrom(uc *b);
EXPORTSPEC float CALL_SPEC daveGetKGfrom(uc *b);
/*
    Get a value from the current position in the last result read on the connection dc.
    This will increment an internal pointer, so the next value is read from the position
    following this value.
*/

EXPORTSPEC int CALL_SPEC daveGetS8(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetU8(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetS16(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetU16(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetS32(daveConnection * dc);
EXPORTSPEC unsigned int CALL_SPEC daveGetU32(daveConnection * dc);
EXPORTSPEC float CALL_SPEC daveGetFloat(daveConnection * dc);
EXPORTSPEC float CALL_SPEC daveGetKG(daveConnection * dc);
/*
    Get a value from a given position in the last result read on the connection dc.
*/
EXPORTSPEC int CALL_SPEC daveGetS8At(daveConnection * dc, int pos);
EXPORTSPEC int CALL_SPEC daveGetU8At(daveConnection * dc, int pos);
EXPORTSPEC int CALL_SPEC daveGetS16At(daveConnection * dc, int pos);
EXPORTSPEC int CALL_SPEC daveGetU16At(daveConnection * dc, int pos);
EXPORTSPEC int CALL_SPEC daveGetS32At(daveConnection * dc, int pos);
EXPORTSPEC unsigned int CALL_SPEC daveGetU32At(daveConnection * dc, int pos);
/*
    put one byte into buffer b:
*/
EXPORTSPEC uc * CALL_SPEC davePut8(uc *b, int v);
EXPORTSPEC uc * CALL_SPEC davePut16(uc *b, int v);
EXPORTSPEC uc * CALL_SPEC davePut32(uc *b, int v);
EXPORTSPEC uc * CALL_SPEC davePutFloat(uc *b, float v);
EXPORTSPEC uc * CALL_SPEC davePutKG(uc *b, float v);
EXPORTSPEC void CALL_SPEC davePut8At(uc *b, int pos, int v);
EXPORTSPEC void CALL_SPEC davePut16At(uc *b, int pos, int v);
EXPORTSPEC void CALL_SPEC davePut32At(uc *b, int pos, int v);
EXPORTSPEC void CALL_SPEC davePutFloatAt(uc *b, int pos, float v);
EXPORTSPEC void CALL_SPEC davePutKGAt(uc *b, int pos, float v);
/**
    Timer and Counter conversion functions:
**/
/*
    get time in seconds from current read position:
*/
EXPORTSPEC float CALL_SPEC daveGetSeconds(daveConnection * dc);
/*
    get time in seconds from random position:
*/
EXPORTSPEC float CALL_SPEC daveGetSecondsAt(daveConnection * dc, int pos);
/*
    get counter value from current read position:
*/
EXPORTSPEC int CALL_SPEC daveGetCounterValue(daveConnection * dc);
/*
    get counter value from random read position:
*/
EXPORTSPEC int CALL_SPEC daveGetCounterValueAt(daveConnection * dc, int pos);

/*
    Functions to load blocks from PLC:
*/
EXPORTSPEC void CALL_SPEC _daveConstructUpload(PDU *p, char blockType, int blockNr); // char or uc,to decide

EXPORTSPEC void CALL_SPEC _daveConstructDoUpload(PDU * p, int uploadID);

EXPORTSPEC void CALL_SPEC _daveConstructEndUpload(PDU * p, int uploadID);
/*
    Get the PLC's order code as ASCIIZ. Buf must provide space for
    21 characters at least.
*/

#define daveOrderCodeSize 21
EXPORTSPEC int CALL_SPEC daveGetOrderCode(daveConnection * dc, char * buf); // char, users buffer, or to decide

/*
    connect to a PLC. returns 0 on success.
*/
EXPORTSPEC int CALL_SPEC daveConnectPLC(daveConnection * dc);

/*
    Read len bytes from the PLC. Start determines the first byte.
    Area denotes whether the data comes from FLAGS, DATA BLOCKS,
    INPUTS or OUTPUTS, etc.
    DB is the number of the data block to be used. Set it to zero
    for other area types.
    Buffer is a pointer to a memory block provided by the calling
    program. If the pointer is not NULL, the result data will be copied thereto.
    Hence it must be big enough to take up the result.
    In any case, you can also retrieve the result data using the get<type> macros
    on the connection pointer.

    RESTRICTION:There is no check for max. message len or automatic splitting into
        multiple messages. Use daveReadManyBytes() in case the data you want
        to read doesn't fit into a single PDU.

*/
EXPORTSPEC int CALL_SPEC daveReadBytes(daveConnection * dc, int area, int DB, int start, int len, void * buffer);

/*
    Read len bytes from the PLC. Start determines the first byte.
    In contrast to daveReadBytes(), this function can read blocks
    that are too long for a single transaction. To achieve this,
    the data is fetched with multiple subsequent read requests to
    the CPU.
    Area denotes whether the data comes from FLAGS, DATA BLOCKS,
    INPUTS or OUTPUTS, etc.
    DB is the number of the data block to be used. Set it to zero
    for other area types.
    Buffer is a pointer to a memory block provided by the calling
    program. It may not be NULL, the result data will be copied thereto.
    Hence it must be big enough to take up the result.
    You CANNOT read result bytes from the internal buffer of the
    daveConnection. This buffer is overwritten in each read request.
*/
EXPORTSPEC int CALL_SPEC daveReadManyBytes(daveConnection * dc, int area, int DBnum, int start, int len, void * buffer);

/*
    Write len bytes from buffer to the PLC.
    Start determines the first byte.
    Area denotes whether the data goes to FLAGS, DATA BLOCKS,
    INPUTS or OUTPUTS, etc.
    DB is the number of the data block to be used. Set it to zero
    for other area types.
    RESTRICTION: There is no check for max. message len or automatic splitting into
         multiple messages. Use daveReadManyBytes() in case the data you want
         to read doesn't fit into a single PDU.

*/
EXPORTSPEC int CALL_SPEC daveWriteBytes(daveConnection * dc, int area, int DB, int start, int len, void * buffer);

/*
    Write len bytes to the PLC. Start determines the first byte.
    In contrast to daveWriteBytes(), this function can write blocks
    that are too long for a single transaction. To achieve this, the
    the data is transported with multiple subsequent write requests to
    the CPU.
    Area denotes whether the data comes from FLAGS, DATA BLOCKS,
    INPUTS or OUTPUTS, etc.
    DB is the number of the data block to be used. Set it to zero
    for other area types.
    Buffer is a pointer to a memory block provided by the calling
    program. It may not be NULL.
*/
EXPORTSPEC int CALL_SPEC daveWriteManyBytes(daveConnection * dc, int area, int DB, int start, int len, void * buffer);
/*
    Bit manipulation:
*/
EXPORTSPEC int CALL_SPEC daveReadBits(daveConnection * dc, int area, int DB, int start, int len, void * buffer);
EXPORTSPEC int CALL_SPEC daveWriteBits(daveConnection * dc, int area, int DB, int start, int len, void * buffer);
EXPORTSPEC int CALL_SPEC daveSetBit(daveConnection * dc, int area, int DB, int byteAdr, int bitAdr);
EXPORTSPEC int CALL_SPEC daveClrBit(daveConnection * dc, int area, int DB, int byteAdr, int bitAdr);

/*
    PLC diagnostic and inventory functions:
*/
EXPORTSPEC int CALL_SPEC daveBuildAndSendPDU(daveConnection * dc, PDU*p2, uc *pa, int psize, uc *ud, int usize);
EXPORTSPEC int CALL_SPEC daveReadSZL(daveConnection * dc, int ID, int index, void * buf, int buflen);
EXPORTSPEC int CALL_SPEC daveListBlocksOfType(daveConnection * dc, uc type, daveBlockEntry * buf);
EXPORTSPEC int CALL_SPEC daveListBlocks(daveConnection * dc, daveBlockTypeEntry * buf);
EXPORTSPEC int CALL_SPEC daveGetBlockInfo(daveConnection * dc, daveBlockInfo *dbi, uc type, int number);
/*
    PLC program read functions:
*/
EXPORTSPEC int CALL_SPEC initUpload(daveConnection * dc, char blockType, int blockNr, int * uploadID); // char or uc,to decide
EXPORTSPEC int CALL_SPEC doUpload(daveConnection*dc, int * more, uc**buffer, int*len, int uploadID);
EXPORTSPEC int CALL_SPEC endUpload(daveConnection*dc, int uploadID);
/*
    PLC run/stop control functions:
*/
EXPORTSPEC int CALL_SPEC daveStop(daveConnection*dc);
EXPORTSPEC int CALL_SPEC daveStart(daveConnection*dc);
/*
    PLC special commands
*/
EXPORTSPEC int CALL_SPEC daveCopyRAMtoROM(daveConnection*dc);

EXPORTSPEC int CALL_SPEC daveForce200(daveConnection * dc, int area, int start, int val);
/*
    Multiple variable support:
*/
typedef struct {
    int error;
    int length;
    uc * bytes;
} daveResult;

typedef struct {
    int numResults;
    daveResult * results;
} daveResultSet;


/* use this to initialize a multivariable read: */
EXPORTSPEC void CALL_SPEC davePrepareReadRequest(daveConnection * dc, PDU *p);
/* Adds a new variable to a prepared request: */
EXPORTSPEC void CALL_SPEC daveAddVarToReadRequest(PDU *p, int area, int DBnum, int start, int bytes);
/* Executes the complete request. */
EXPORTSPEC int CALL_SPEC daveExecReadRequest(daveConnection * dc, PDU *p, daveResultSet * rl);
/* Lets the functions daveGet<data type> work on the n-th result: */
EXPORTSPEC int CALL_SPEC daveUseResult(daveConnection * dc, daveResultSet * rl, int n);
/* Frees the memory occupied by the result structure */
EXPORTSPEC void CALL_SPEC daveFreeResults(daveResultSet * rl);
/* Adds a new bit variable to a prepared request: */
EXPORTSPEC void CALL_SPEC daveAddBitVarToReadRequest(PDU *p, int area, int DBnum, int start, int byteCount);

/* use this to initialize a multivariable write: */
EXPORTSPEC void CALL_SPEC davePrepareWriteRequest(daveConnection * dc, PDU *p);
/* Add a preformed variable aderess to a read request: */
EXPORTSPEC void CALL_SPEC daveAddToReadRequest(PDU *p, int area, int DBnum, int start, int byteCount, int isBit);
/* Adds a new variable to a prepared request: */
EXPORTSPEC void CALL_SPEC daveAddVarToWriteRequest(PDU *p, int area, int DBnum, int start, int bytes, void * buffer);
/* Adds a new bit variable to a prepared write request: */
EXPORTSPEC void CALL_SPEC daveAddBitVarToWriteRequest(PDU *p, int area, int DBnum, int start, int byteCount, void * buffer);
EXPORTSPEC int CALL_SPEC _daveTestResultData(PDU * p);
EXPORTSPEC int CALL_SPEC _daveTestReadResult(PDU * p);
EXPORTSPEC int CALL_SPEC _daveTestPGReadResult(PDU * p);

/* Executes the complete request. */
EXPORTSPEC int CALL_SPEC daveExecWriteRequest(daveConnection * dc, PDU *p, daveResultSet * rl);
EXPORTSPEC int CALL_SPEC _daveTestWriteResult(PDU * p);

EXPORTSPEC int CALL_SPEC daveInitAdapter(daveInterface * di);
EXPORTSPEC int CALL_SPEC daveConnectPLC(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveDisconnectPLC(daveConnection * dc);

EXPORTSPEC int CALL_SPEC daveDisconnectAdapter(daveInterface * di);
EXPORTSPEC int CALL_SPEC daveListReachablePartners(daveInterface * di, char * buf);

EXPORTSPEC int CALL_SPEC _daveReturnOkDummy(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveReturnOkDummy2(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveListReachablePartnersDummy(daveInterface * di, char * buf);

EXPORTSPEC int CALL_SPEC _daveNegPDUlengthRequest(daveConnection * dc, PDU *p);

/* MPI specific functions */

#define daveMPIReachable 0x30
#define daveMPIActive 0x30
#define daveMPIPassive 0x00
#define daveMPIunused 0x10
#define davePartnerListSize 126

EXPORTSPEC int CALL_SPEC _daveListReachablePartnersMPI(daveInterface * di, char * buf);
EXPORTSPEC int CALL_SPEC _daveInitAdapterMPI1(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveInitAdapterMPI2(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveConnectPLCMPI1(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveConnectPLCMPI2(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectPLCMPI(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectAdapterMPI(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveExchangeMPI(daveConnection * dc, PDU * p1);

EXPORTSPEC int CALL_SPEC _daveListReachablePartnersMPI3(daveInterface * di, char * buf);
EXPORTSPEC int CALL_SPEC _daveInitAdapterMPI3(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveConnectPLCMPI3(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectPLCMPI3(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectAdapterMPI3(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveExchangeMPI3(daveConnection * dc, PDU * p1);
EXPORTSPEC int CALL_SPEC _daveIncMessageNumber(daveConnection * dc);

/* ISO over TCP specific functions */
EXPORTSPEC int CALL_SPEC _daveExchangeTCP(daveConnection * dc, PDU * p1);
EXPORTSPEC int CALL_SPEC _daveConnectPLCTCP(daveConnection * dc);
/*
    make internal PPI functions available for experimental use:
*/
EXPORTSPEC int CALL_SPEC _daveExchangePPI(daveConnection * dc, PDU * p1);
EXPORTSPEC void CALL_SPEC _daveSendLength(daveInterface * di, int len);
EXPORTSPEC void CALL_SPEC _daveSendRequestData(daveConnection * dc, int alt);
EXPORTSPEC void CALL_SPEC _daveSendIt(daveInterface * di, uc * b, int size);
EXPORTSPEC int CALL_SPEC _daveGetResponsePPI(daveConnection *dc);
EXPORTSPEC int CALL_SPEC _daveReadChars(daveInterface * di, uc *b, timeout_t tmo, int max);
EXPORTSPEC int CALL_SPEC _daveReadChars2(daveInterface * di, uc *b, int max);
EXPORTSPEC int CALL_SPEC _daveConnectPLCPPI(daveConnection * dc);

/*
    make internal MPI functions available for experimental use:
*/
EXPORTSPEC int CALL_SPEC _daveReadMPI(daveInterface * di, uc *b);
EXPORTSPEC void CALL_SPEC _daveSendSingle(daveInterface * di, uc c);
EXPORTSPEC int CALL_SPEC _daveSendAck(daveConnection * dc, int nr);
EXPORTSPEC int CALL_SPEC _daveGetAck(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveSendDialog2(daveConnection * dc, int size);
EXPORTSPEC int CALL_SPEC _daveSendWithPrefix(daveConnection * dc, uc * b, int size);
EXPORTSPEC int CALL_SPEC _daveSendWithPrefix2(daveConnection * dc, int size);
EXPORTSPEC int CALL_SPEC _daveSendWithCRC(daveInterface * di, uc *b, int size);
EXPORTSPEC int CALL_SPEC _daveReadSingle(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveReadOne(daveInterface * di, uc *b);
EXPORTSPEC int CALL_SPEC _daveReadMPI2(daveInterface * di, uc *b);
EXPORTSPEC int CALL_SPEC _daveGetResponseMPI(daveConnection *dc);
EXPORTSPEC int CALL_SPEC _daveSendMessageMPI(daveConnection * dc, PDU * p);

/*
    make internal ISO_TCP functions available for experimental use:
*/
/*
    Read one complete packet. The bytes 3 and 4 contain length information.
*/
EXPORTSPEC int CALL_SPEC _daveReadISOPacket(daveInterface * di, uc *b);
EXPORTSPEC int CALL_SPEC _daveGetResponseISO_TCP(daveConnection *dc);


typedef uc * (*userReadFunc) (int, int, int, int, int *);
typedef void(*userWriteFunc) (int, int, int, int, int *, uc *);
extern userReadFunc readCallBack;
extern userWriteFunc writeCallBack;

EXPORTSPEC void CALL_SPEC _daveConstructReadResponse(PDU * p);
EXPORTSPEC void CALL_SPEC _daveConstructWriteResponse(PDU * p);
EXPORTSPEC void CALL_SPEC _daveConstructBadReadResponse(PDU * p);
EXPORTSPEC void CALL_SPEC _daveHandleRead(PDU * p1, PDU * p2);
EXPORTSPEC void CALL_SPEC _daveHandleWrite(PDU * p1, PDU * p2);
/*
    make internal IBH functions available for experimental use:
*/
EXPORTSPEC int CALL_SPEC _daveReadIBHPacket(daveInterface * di, uc *b);
EXPORTSPEC int CALL_SPEC _daveWriteIBH(daveInterface * di, uc * buffer, int len);
EXPORTSPEC int CALL_SPEC _davePackPDU(daveConnection * dc, PDU *p);
EXPORTSPEC void CALL_SPEC _daveSendMPIAck_IBH(daveConnection*dc);
EXPORTSPEC void CALL_SPEC _daveSendIBHNetAck(daveConnection * dc);
EXPORTSPEC int CALL_SPEC __daveAnalyze(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveExchangeIBH(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveSendMessageMPI_IBH(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveGetResponseMPI_IBH(daveConnection *dc);
EXPORTSPEC int CALL_SPEC _daveInitStepIBH(daveInterface * iface, uc * chal, int cl, us* resp, int rl, uc*b);

EXPORTSPEC int CALL_SPEC _daveConnectPLC_IBH(daveConnection*dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectPLC_IBH(daveConnection*dc);
EXPORTSPEC void CALL_SPEC _daveSendMPIAck2(daveConnection *dc);
EXPORTSPEC int CALL_SPEC _davePackPDU_PPI(daveConnection * dc, PDU *p);
EXPORTSPEC void CALL_SPEC _daveSendIBHNetAckPPI(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveListReachablePartnersMPI_IBH(daveInterface *di, char * buf);
EXPORTSPEC int CALL_SPEC __daveAnalyzePPI(daveConnection * dc, uc sa);
EXPORTSPEC int CALL_SPEC _daveExchangePPI_IBH(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveGetResponsePPI_IBH(daveConnection *dc);

/**
    C# interoperability:
**/
EXPORTSPEC void CALL_SPEC daveSetTimeout(daveInterface * di, int tmo);
EXPORTSPEC int CALL_SPEC daveGetTimeout(daveInterface * di);
EXPORTSPEC char * CALL_SPEC daveGetName(daveInterface * di);

EXPORTSPEC int CALL_SPEC daveGetMPIAdr(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetAnswLen(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetMaxPDULen(daveConnection * dc);
EXPORTSPEC daveResultSet * CALL_SPEC daveNewResultSet();
EXPORTSPEC void CALL_SPEC daveFree(void * dc);
EXPORTSPEC PDU * CALL_SPEC daveNewPDU();
EXPORTSPEC int CALL_SPEC daveGetErrorOfResult(daveResultSet *, int number);

/*
    Special function do disconnect arbitrary connections on IBH-Link:
*/
EXPORTSPEC int CALL_SPEC daveForceDisconnectIBH(daveInterface * di, int src, int dest, int mpi);
/*
    Special function do reset an IBH-Link:
*/
EXPORTSPEC int CALL_SPEC daveResetIBH(daveInterface * di);
/*
    Program Block from PLC:
*/
EXPORTSPEC int CALL_SPEC daveGetProgramBlock(daveConnection * dc, int blockType, int number, char* buffer, int * length);
/**
    PLC realtime clock handling:
*/
/*
    read out clock:
*/
EXPORTSPEC int CALL_SPEC daveReadPLCTime(daveConnection * dc);
/*
    set clock to a value given by user:
*/
EXPORTSPEC int CALL_SPEC daveSetPLCTime(daveConnection * dc, uc * ts);
/*
    set clock to PC system clock:
*/
EXPORTSPEC int CALL_SPEC daveSetPLCTimeToSystime(daveConnection * dc);
/*
    BCD conversions:
*/
EXPORTSPEC uc CALL_SPEC daveToBCD(uc i);
EXPORTSPEC uc CALL_SPEC daveFromBCD(uc i);
/*
    S5:
*/
EXPORTSPEC int CALL_SPEC _daveFakeExchangeAS511(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveExchangeAS511(daveConnection * dc, uc * b, int len, int maxLen, int trN);
EXPORTSPEC int CALL_SPEC _daveSendMessageAS511(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveConnectPLCAS511(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectPLCAS511(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveIsS5BlockArea(uc area);
EXPORTSPEC int CALL_SPEC _daveReadS5BlockAddress(daveConnection * dc, uc area, uc BlockN, daveS5AreaInfo * ai);
EXPORTSPEC int CALL_SPEC _daveReadS5ImageAddress(daveConnection * dc, uc area, daveS5AreaInfo * ai);
EXPORTSPEC int CALL_SPEC _daveIsS5ImageArea(uc area);
EXPORTSPEC int CALL_SPEC _daveIsS5DBBlockArea(uc area);
EXPORTSPEC int CALL_SPEC daveReadS5Bytes(daveConnection * dc, uc area, uc BlockN, int offset, int count);
EXPORTSPEC int CALL_SPEC daveWriteS5Bytes(daveConnection * dc, uc area, uc BlockN, int offset, int count, void * buf);
EXPORTSPEC int CALL_SPEC daveStopS5(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveStartS5(daveConnection * dc);
EXPORTSPEC int CALL_SPEC daveGetS5ProgramBlock(daveConnection * dc, int blockType, int number, char* buffer, int * length);

/*
    MPI version 3:
*/
EXPORTSPEC int CALL_SPEC _daveSendWithPrefix31(daveConnection * dc, uc *b, int size);
EXPORTSPEC int CALL_SPEC _daveGetResponseMPI3(daveConnection *dc);
EXPORTSPEC int CALL_SPEC _daveSendMessageMPI3(daveConnection * dc, PDU * p);

#ifdef HAVE_S7ONLINE
/*
    using S7 dlls for transporrt:
*/

#pragma pack(1)

#ifndef OS_WINDOWS  //We can use this under windows only, but avoid error messages
#define HANDLE int
#endif

/*
    Prototypes for the functions in S7onlinx.dll:
    They are guessed.
*/
typedef int (CALL_SPEC * _openFunc) (const uc *);
typedef int (CALL_SPEC * _closeFunc) (int);
typedef int (CALL_SPEC * _sendFunc) (int, us, uc *);
typedef int (CALL_SPEC * _receiveFunc) (int, us, int *, us, uc *);
//typedef int (CALL_SPEC * _SetHWndMsgFunc) (int, int, ULONG);
//typedef int (CALL_SPEC * _SetHWndFunc) (int, HANDLE);
typedef int (CALL_SPEC * _get_errnoFunc) (void);

/*
    And pointers to the functions. We load them using GetProcAddress() on their names because:
    1. We have no .lib file for s7onlinx.
    2. We don't want to link with a particular version.
    3. Libnodave shall remain useable without Siemens .dlls. So it shall not try to access them
       unless the user chooses the daveProtoS7online transport.
*/
extern _openFunc SCP_open;
extern _closeFunc SCP_close;
extern _sendFunc SCP_send;
extern _receiveFunc SCP_receive;
//_SetHWndMsgFunc SetSinecHWndMsg;
//_SetHWndFunc SetSinecHWnd;
extern _get_errnoFunc SCP_get_errno;
/*
    A block of data exchanged between S7onlinx.dll and a program using it. Most fields seem to
    be allways 0. Meaningful names are guessed.
*/

typedef struct {
    us		unknown[2];
    uc		headerlength;
    us		user;
    uc		allways2;
    uc		priority;
    uc		unknown3[3];
    uc		field6;
    uc		field7;
    us		field8;
    us		validDataLength;
    uc          unknown11;
    us          payloadLength;
    us		payloadStart;
    uc		unknown2[12];
    uc		field10;
    us		id3;
    short 	application_block_service;
    uc		unknown4[2];
    uc          field13;
    uc          field11;
    uc          field12;
    uc		unknown6[35];
    uc          payload[520];
} S7OexchangeBlock;

EXPORTSPEC int CALL_SPEC _daveCP_send(int fd, int len, uc * reqBlock);
EXPORTSPEC int CALL_SPEC _daveSCP_send(int fd, uc * reqBlock);
EXPORTSPEC int CALL_SPEC _daveConnectPLCS7online(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveSendMessageS7online(daveConnection * dc, PDU *p);
EXPORTSPEC int CALL_SPEC _daveDisconnectPLCS7online(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveGetResponseS7online(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveExchangeS7online(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveListReachablePartnersS7online(daveInterface * di, char * buf);
EXPORTSPEC int CALL_SPEC _daveDisconnectAdapterS7online(daveInterface * di);
#endif

EXPORTSPEC int CALL_SPEC stdwrite(daveInterface * di, char * buffer, int length);
EXPORTSPEC int CALL_SPEC stdread(daveInterface * di, char * buffer, int length);

EXPORTSPEC int CALL_SPEC _daveInitAdapterNLpro(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveInitStepNLpro(daveInterface * iface, int nr, uc* fix, int len, char*caller, uc * buffer);
EXPORTSPEC int CALL_SPEC _daveConnectPLCNLpro(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveSendMessageNLpro(daveConnection *dc, PDU *p);
EXPORTSPEC int CALL_SPEC _daveGetResponseNLpro(daveConnection *dc);
EXPORTSPEC int CALL_SPEC _daveExchangeNLpro(daveConnection * dc, PDU * p);
EXPORTSPEC int CALL_SPEC _daveSendDialogNLpro(daveConnection * dc, int size);
EXPORTSPEC int CALL_SPEC _daveSendWithPrefixNLpro(daveConnection * dc, uc * b, int size);
EXPORTSPEC int CALL_SPEC _daveSendWithPrefix2NLpro(daveConnection * dc, int size);
EXPORTSPEC int CALL_SPEC _daveDisconnectPLCNLpro(daveConnection * dc);
EXPORTSPEC int CALL_SPEC _daveDisconnectAdapterNLpro(daveInterface * di);
EXPORTSPEC int CALL_SPEC _daveListReachablePartnersNLpro(daveInterface * di, char * buf);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _nodave */


/*
    Changes:
    07/19/04  added the definition of daveExchange().
    09/09/04  applied patch for variable Profibus speed from Andrew Rostovtsew.
    09/09/04  applied patch from Bryan D. Payne to make this compile under Cygwin and/or newer gcc.
    12/09/04  added daveReadBits(), daveWriteBits()
    12/09/04  added some more comments.
    12/09/04  changed declaration of _daveMemcmp to use typed pointers.
    01/15/04  generic getResponse, more internal functions, use a single dummy to replace
          initAdapterDummy,
    01/26/05  replaced _daveConstructReadRequest by the sequence prepareReadRequest, addVarToReadRequest
    01/26/05  added multiple write
    02/02/05  added readIBHpacket
    02/06/05  replaced _daveConstructBitWriteRequest by the sequence prepareWriteRequest, addBitVarToWriteRequest
    02/08/05  removed inline functions.
    03/23/05  added _daveGetResponsePPI_IBH().
    03/24/05  added function codes for download.
    03/28/05  added some comments.
    04/05/05  reworked error reporting.
    04/06/05  renamed swap functions. When I began libnodave on little endian i386 and Linux, I used
          used Linux bswap functions. Then users (and later me) tried other systems without
          a bswap. I also cannot use inline functions in Pascal. So I made my own bswaps. Then
          I, made the core of my own swaps dependent of LITTLEENDIAN conditional to support also
          bigendien systems. Now I want to rename them from bswap to something else to avoid
          confusion for LINUX/UNIX users. The new names are swapIed_16 and swapIed_32. This
          shall say swap "if endianness differs". While I could have used similar functions
          from the network API (htons etc.) on Unix and Win32, they may not be present on
          e.g. microcontrollers.
          I highly recommend to use these functions even when writing software for big endian
          systems, where they effectively do nothing, as it will make your software portable.
    04/08/05  removed deprecated conversion functions.
    04/09/05  removed daveDebug. Use setDebug and getDebug instead. Some programming environments
          do not allow access to global variables in a shared library.
    04/09/05  removed CYGWIN defines. As there were no more differences against LINUX, it should
          work with LINUX defines.
    04/09/05  reordered fields in daveInterface to put fields used in normal test programs at the
          beginning. This allows to make a simplified version in nodavesimple as short as
          possible.
    05/09/05  renamed more functions to daveXXX.
    05/11/05  added some functions for the convenience of usage with .net or mono. The goal is
          that the application doesn't have to use members of data structures defined herein
          directly. This avoids "unsafe" pointer expressions in .net/MONO. It should also ease
          porting to VB or other languages for which it could be difficult to define byte by
          byte equivalents of these structures.
    07/31/05  added message string copying for Visual Basic.
    08/28/05  added some functions to read and set PLC realtime clock.
    09/02/05  added the pointer "hook" to daveConnection. This can be used by applications to pass
          data between function calls using the dc.
    09/11/05  added read/write functions for long blocks of data.
    09/17/05  incorporation of S5 functions
    09/18/05  implemented conversions from and to S5 KG format (old, propeiatary floating point format).
Version 0.8.5:
    05/17/13  added include winsock2 for 46 bit compatibility.
    05/17/13  added conType field to daveOSserialType

    2018-02-14 * API detection moved into config.h
    2018-02-16 * const defines replaced with enums
               + new define HAVE_S7ONLINE - more in config.h
    2018-05-10 * const defines replaced with enums
               * replace typedefs for unsigned integers with <stdint.h>
               + new typedefs for signed integers
*/
