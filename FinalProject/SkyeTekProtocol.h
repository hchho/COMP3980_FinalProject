/**
 * SkyeTekProtocol.h
 * (c) 2006 SkyeTek, Inc. All Rights Reserved.
 * 
 * Definition of the SkyeTek Protocol.
 */
#ifndef SKYETEK_PROTOCOL_H
#define SKYETEK_PROTOCOL_H

#include "SkyeTekAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * System Parameter Information for both STPV2 and STPV3
 ********************************************************************************/

/**
 * Gets the number of system parameters supported.
 * @return Number of system parameters supported
 */
SKYETEK_API unsigned int 
SkyeTek_GetSysParmIdCount();

/**
 * Gets the string name for the given system parameter index.
 * @param index System parameter index
 * @return Name of the system parameter
 */
SKYETEK_API TCHAR *
SkyeTek_GetSysParmIdName(
    unsigned int    index
    );

/**
 * Gets the parameter for the given system parameter index.
 * @param index System parameter index
 * @return System parameter
 */
SKYETEK_API SKYETEK_SYSTEM_PARAMETER 
SkyeTek_GetSysParmId(
  unsigned int  index
  );

/**
 * Gets the system parameter for the given name.
 * @param name system parameter name
 * @return system parameter
 */
SKYETEK_API SKYETEK_SYSTEM_PARAMETER 
SkyeTek_GetSysParmIdFromName(
    TCHAR    *name
    );

/**
 * Gets the system parameter name for the given type.
 * @param name system parameter
 * @return system parameter name
 */
SKYETEK_API TCHAR *
SkyeTek_GetSysParmIdNameFromParameter(
    SKYETEK_SYSTEM_PARAMETER  parameter
    );

/********************************************************************************
 * Tag Type Information for both STPV2 and STPV3
 ********************************************************************************/

/**
 * Gets the number of tag types supported.
 * @return Number of tag types supported
 */
SKYETEK_API unsigned int 
SkyeTek_GetTagTypeCount();

/**
 * Gets the string name for the given tag type index.
 * @param index Tag type index
 * @return Name of the tag type
 */
SKYETEK_API TCHAR *
SkyeTek_GetTagTypeName(
    unsigned int    index
    );

/**
 * Gets the tag type for the given tag type index.
 * @param index Tag type index
 * @return Name of the tag type
 */
SKYETEK_API SKYETEK_TAGTYPE 
SkyeTek_GetTagType(
    unsigned int    index
    );

/**
 * Gets the tag type for the given name.
 * @param name Tag type name
 * @return Tag type
 */
SKYETEK_API SKYETEK_TAGTYPE 
SkyeTek_GetTagTypeFromName(
    TCHAR    *name
    );

/**
 * Gets the tag type name for the given type.
 * @param name Tag type
 * @return Tag type name
 */
SKYETEK_API TCHAR *
SkyeTek_GetTagTypeNameFromType(
    SKYETEK_TAGTYPE     type
    );

/********************************************************************************
 * Raw Device 
 ********************************************************************************/

SKYETEK_API SKYETEK_STATUS
SkyeTek_OpenDevice(
  LPSKYETEK_DEVICE    lpDevice
  );

SKYETEK_API int
SkyeTek_ReadDevice(
  LPSKYETEK_DEVICE    lpDevice,
  unsigned char       *data,
  unsigned int        size,
  unsigned int        timeout
  );

SKYETEK_API int
SkyeTek_WriteDevice(
  LPSKYETEK_DEVICE    lpDevice,
  unsigned char       *data,
  unsigned int        size,
  unsigned int        timeout
  );

SKYETEK_API void
SkyeTek_FlushDevice(
  LPSKYETEK_DEVICE    lpDevice
  );

SKYETEK_API SKYETEK_STATUS
SkyeTek_CloseDevice(
  LPSKYETEK_DEVICE    lpDevice
  );




/********************************************************************************
 * STP Version 3 
 ********************************************************************************/

/* Max Sizes */
#define STPV3_MAX_ASCII_REQUEST_SIZE	4168
#define STPV3_MAX_RESPONSE_SIZE 8192
#define STPV3_MAX_ASCII_RESPONSE_SIZE (2*STPV3_MAX_RESPONSE_SIZE + 24)

/* Request */
typedef struct STPV3_REQUEST
{
    unsigned int    flags;          /* 2 bytes */
    unsigned int    cmd;            /* 2 bytes */
    unsigned char   rid[4];         /* 4 bytes */
    unsigned int    tagType;        /* 2 bytes */
    unsigned short  tidLength;      /* 1 byte */
    unsigned char   tid[16];        /* 16 bytes maximum */
    unsigned short  afi;		    /* 1 byte */
    unsigned short  session;	    /* 1 byte */
    unsigned char   address[2];     /* 2 bytes */
    unsigned int    numBlocks;      /* 2 bytes */
    unsigned int    dataLength;     /* 2 bytes */
    unsigned char   data[2048];     /* 2048 bytes maximum */
    unsigned char   msg[STPV3_MAX_ASCII_REQUEST_SIZE];
    unsigned int    msgLength;
    unsigned char   isASCII;
    unsigned char   anyResponse;
} STPV3_REQUEST, *LPSTPV3_REQUEST;

/* Response */
typedef struct STPV3_RESPONSE
{
    unsigned int    code;           /* 2 bytes */
    unsigned char   rid[4];         /* 4 bytes */
    unsigned int    tagType;        /* 2 bytes */
    unsigned int    dataLength;     /* 2 bytes */
    unsigned char   data[STPV3_MAX_RESPONSE_SIZE];
    unsigned char   msg[STPV3_MAX_ASCII_RESPONSE_SIZE];
    unsigned int    msgLength;
    unsigned int    isASCII;
} STPV3_RESPONSE, *LPSTPV3_RESPONSE;

#define STPV3_FORMAT_NOTHING  0x00
#define STPV3_FORMAT_ADDRESS  0x01
#define STPV3_FORMAT_BLOCKS   0x02
#define STPV3_FORMAT_DATA     0x04

/**
 * This builds the request and fills the request msg array.
 * @param req Pointer to the request structure
 * @return Results of the build 
 */
SKYETEK_API SKYETEK_STATUS 
STPV3_BuildRequest( 
    LPSTPV3_REQUEST  req
    );

/**
 * This writes the request to the given device. 
 * This calls STPV3_buildRequest to build it.
 * @param device The device to write the request to
 * @param req Pointer to the request structure
 * @param isASCII Set to 0 for binary and 1 for ASCII
 * @param timeout Timeout in milliseconds for the write operation
 * @return Results of building or writing the request 
 */
SKYETEK_API SKYETEK_STATUS 
STPV3_WriteRequest( 
    LPSKYETEK_DEVICE     device, 
    LPSTPV3_REQUEST      req,
    unsigned int         timeout
    );

/**
 * Reads the response from the device.
 * @param device The device to read from
 * @param req The request is used to interpret the response
 * @param resp The response structure to be filled in
 * @param isASCII Set to 0 for binary and 1 for ASCII
 * @param timeout Timeout in milliseconds for the read operation
 * @return SkyeTek API status value
 */
SKYETEK_API SKYETEK_STATUS 
STPV3_ReadResponse(
    LPSKYETEK_DEVICE     device, 
    LPSTPV3_REQUEST      req, 
    LPSTPV3_RESPONSE     resp,
    unsigned int         timeout
    );

/** 
 * This returns whether or not the command access an address
 * and/or data.
 * @param cmd The command to lookup
 * @return STPV3_FORMAT
 */
SKYETEK_API unsigned char 
STPV3_IsAddressOrDataCommand( 
    unsigned int    cmd 
    );

/** 
 * This returns the string of the friendly name for
 * the command passed in.
 * @param cmd The command to lookup
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV3_LookupCommand( 
    unsigned int    cmd 
    );

/** 
 * This returns the command code for friendly name
 * passed in.
 * @param cmdName The command to lookup
 * @return The command code or zero
 */
SKYETEK_API unsigned int 
STPV3_LookupCommandCode( 
    TCHAR   *cmdName 
    );

/** 
 * Returns the number of comamnds.
 * @return The count of commands
 */
SKYETEK_API int 
STPV3_GetCommandCount();

/** 
 * This returns the string of the friendly name for
 * the command at the index given.
 * @param ix Index of the command name to return
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV3_GetCommandNameAt( 
    int     ix 
    );

/** 
 * This returns the string of the friendly name for
 * the response code passed in.
 * @param resp The response code to lookup
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV3_LookupResponse( 
    unsigned int    resp 
    );

/** 
 * This returns the response code for friendly name
 * passed in.
 * @param respName The response to lookup
 * @return The command code or zero
 */
SKYETEK_API unsigned int 
STPV3_LookupResponseCode( 
    TCHAR   *respName 
    );

/** 
 * Returns the number of response codes.
 * @return The count of response codes
 */
SKYETEK_API int 
STPV3_GetResponsesCount();

/** 
 * This returns the string of the friendly name for
 * the response code at the index given.
 * @param ix Index of the response code name to return
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV3_GetResponseNameAt( 
    int     ix 
    );







/********************************************************************************
 * STP Version 2 
 ********************************************************************************/

/* Max Sizes */
#define STPV2_MAX_ASCII_REQUEST_SIZE	1052
#define STPV2_MAX_ASCII_RESPONSE_SIZE 1050

/* Request */
typedef struct STPV2_REQUEST
{
    unsigned char   flags;          /* 1 byte */
    unsigned char   cmd;            /* 1 byte */
    unsigned char   rid;            /* 1 byte */
    unsigned char   tagType;        /* 1 byte */
    unsigned char   tid[8];         /* 8 bytes max */
    unsigned short  tidLength;      /* Not sent */
    unsigned char   afiSession;     /* 1 byte */
    unsigned char   address;        /* 1 byte */
    unsigned char   numBlocks;      /* 1 byte */
    unsigned int    dataLength;     /* Not included in message */
    unsigned char   data[1024];     /* 1024 bytes max */
    unsigned char   msg[STPV2_MAX_ASCII_REQUEST_SIZE];
    unsigned int    msgLength;
    unsigned char   isASCII;
    unsigned char   anyResponse;
} STPV2_REQUEST, *LPSTPV2_REQUEST;

/* Response */
typedef struct STPV2_RESPONSE
{
    unsigned char   code;           /* 1 byte */
    unsigned char   rid;            /* 1 byte */
    unsigned char   tagType;        /* 1 byte */
    unsigned char   data[1024];     /* 1024 bytes max */
    unsigned int    dataLength;		  /* Not used */
    unsigned char   msg[STPV2_MAX_ASCII_RESPONSE_SIZE];
    unsigned int    msgLength;
} STPV2_RESPONSE, *LPSTPV2_RESPONSE;

/**
 * This builds the request and fills the request msg array.
 * @param req Pointer to the request structure
 * @return Results of the build 
 */
SKYETEK_API SKYETEK_STATUS 
STPV2_BuildRequest( 
    LPSTPV2_REQUEST  req
    );

/**
 * This writes the request to the given device. 
 * This calls STPV3_buildRequest to build it.
 * @param device The device to write the request to
 * @param req Pointer to the request structure
 * @param isASCII Set to 0 for binary and 1 for ASCII
 * @param timeout Timeout in milliseconds for the write operation
 * @return Results of building or writing the request 
 */
SKYETEK_API SKYETEK_STATUS 
STPV2_WriteRequest( 
    LPSKYETEK_DEVICE     device, 
    LPSTPV2_REQUEST      req,
    unsigned int         timeout
    );

/**
 * Reads the response from the device.
 * @param device The device to read from
 * @param req The request is used to interpret the response
 * @param resp The response structure to be filled in
 * @param isASCII Set to 0 for binary and 1 for ASCII
 * @param timeout Timeout in milliseconds for the read operation
 * @return SkyeTek API status value
 */
SKYETEK_API SKYETEK_STATUS 
STPV2_ReadResponse(
    LPSKYETEK_DEVICE     device, 
    LPSTPV2_REQUEST      req, 
    LPSTPV2_RESPONSE     resp,
    unsigned int         timeout
    );

/** 
 * This returns the string of the friendly name for
 * the command passed in.
 * @param cmd The command to lookup
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV2_LookupCommand( 
    unsigned char  cmd 
    );

/** 
 * This returns the command code for friendly name
 * passed in.
 * @param cmdName The command to lookup
 * @return The command code or zero
 */
SKYETEK_API unsigned char 
STPV2_LookupCommandCode( 
    TCHAR   *cmdName 
    );

/** 
 * Returns the number of comamnds.
 * @return The count of commands
 */
SKYETEK_API int 
STPV2_GetCommandCount();

/** 
 * This returns the string of the friendly name for
 * the command at the index given.
 * @param ix Index of the command name to return
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV2_GetCommandNameAt( 
    int     ix 
    );

/** 
 * This returns the string of the friendly name for
 * the response code passed in.
 * @param resp The response code to lookup
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV2_LookupResponse( 
    unsigned char      resp 
    );

/** 
 * This returns the response code for friendly name
 * passed in.
 * @param respName The response to lookup
 * @return The command code or zero
 */
SKYETEK_API unsigned char
STPV2_LookupResponseCode( 
    TCHAR       *respName 
    );

/** 
 * Returns the number of response codes.
 * @return The count of response codes
 */
SKYETEK_API int 
STPV2_GetResponsesCount();

/** 
 * This returns the string of the friendly name for
 * the response code at the index given.
 * @param ix Index of the response code name to return
 * @return The string containing the friendly name
 */
SKYETEK_API TCHAR *
STPV2_GetResponseNameAt( 
    int     ix 
    );


#ifdef __cplusplus
}
#endif

#endif
