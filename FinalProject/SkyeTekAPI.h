/**
 * SkyeTekAPI.h
 * (c) 2004 - 2006 SkyeTek, Inc. All Rights Reserved.
 * 
 * Definition of the SkyeTek C-API.
 */
#ifndef SKYETEK_API_H
#define SKYETEK_API_H

#include "Platform.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SKYETEK_MAX_ID_LENGTH 64
#define SKYETEK_MAX_TAG_LENGTH 64
#define SKYETEK_SERIAL_DEVICE_TYPE _T("Serial")
#define SKYETEK_USB_DEVICE_TYPE _T("USB")
#define SKYETEK_SPI_DEVICE_TYPE _T("SPI")
#define SKYETEK_I2C_DEVICE_TYPE _T("I2C")
#define SKYETEK_TRACK1_MAXIMUM_SIZE 79
#define SKYETEK_TRACK2_MAXIMUM_SIZE 40



/****************************************************
 * ENUMERATIONS AND STRUCTURES 
 ****************************************************/

typedef enum SKYETEK_STATUS
{
    SKYETEK_FAILURE = 0,
    SKYETEK_SUCCESS = 1,
    SKYETEK_INVALID_READER,
    SKYETEK_READER_PROTOCOL_ERROR,
    SKYETEK_READER_IN_BOOT_LOAD_MODE,
    SKYETEK_TAG_NOT_IN_FIELD,
    SKYETEK_NO_TAG_ID_MATCH,
    SKYETEK_READER_IO_ERROR,
    SKYETEK_INVALID_PARAMETER,
    SKYETEK_TIMEOUT,
    SKYETEK_NOT_SUPPORTED,
    SKYETEK_OUT_OF_MEMORY,
    SKYETEK_COLLISION_DETECTED,
    SKYETEK_TAG_DATA_INTEGRITY_CHECK_FAILED,
    SKYETEK_TAG_BLOCKS_LOCKED,
    SKYETEK_NOT_AUTHENTICATED,
    SKYETEK_TAG_DATA_RATE_NOT_SUPPORTED,
    SKYETEK_ENCRYPT_TAG_DATA_FAIL,
    SKYETEK_DECRYPT_TAG_DATA_FAIL,
    SKYETEK_INVALID_KEY_FOR_AUTHENTICATION,
    SKYETEK_NO_APPLICATION_PRESENT,
    SKYETEK_FILE_NOT_FOUND,
    SKYETEK_NO_FILE_SELECTED,
    SKYETEK_INVALID_KEY_NUMBER,
    SKYETEK_INVALID_FILE_TYPE,
    SKYETEK_INVALID_KEY_LENGTH,
    SKYETEK_UNKOWN_ERROR,
    SKYETEK_INVALID_COMMAND,
    SKYETEK_INVALID_CRC,
    SKYETEK_INVALID_MESSAGE_LENGTH,
    SKYETEK_INVALID_ADDRESS,
    SKYETEK_INVALID_FLAGS,
    SKYETEK_INVALID_NUMBER_OF_BLOCKS,
    SKYETEK_NO_ANTENNA_DETECTED,
    SKYETEK_INVALID_TAG_TYPE,
    SKYETEK_INVALID_SIGNATURE_HMAC,
    SKYETEK_INVALID_ASCII_CHAR,
    SKYETEK_INVALID_DATA_LEN,
    SKYETEK_INVALID_ENCODING,
    SKYETEK_INVALID_ARGUMENT,
    SKYETEK_INVALID_SESSION,
    SKYETEK_FIRMWARE_CANCELED,
    SKYETEK_FIRMWARE_BAD_FILE,
    SKYETEK_FIRMWARE_READER_ERROR
} SKYETEK_STATUS;

typedef enum SKYETEK_TAGTYPE
{
    /* STPV3 Tag Types */
    AUTO_DETECT = 0x0000,

    /** ISO 15693 Tags */
    ISO_15693_AUTO_DETECT = 0x0100,
    TI_15693_AUTO_DETECT = 0x0110,
    TAGIT_HF1_STANDARD = 0x0111,
    TAGIT_HF1_PRO = 0x0112,
    TAGIT_HF1_PLUS = 0x0113,
    PHILIPS_15693_AUTO_DETECT = 0x0120,
    ICODE_SLI_SL2 = 0x0121,
    ST_15693_AUTO_DETECT = 0x0130,
    ISO_LRI64 = 0x0131,
    LRI512 = 0x0132,
    LRI2K = 0x0133,
    LRIS2K = 0x0134,
    EM_15693_AUTO_DETECT = 0x0140,
    EM4006 = 0x0141,
    EM4034 = 0x0142,
    EM4035_CRYPTO = 0x0143,
    EM4135 = 0x0144,
    INFINEON_15693_AUTO_DETECT = 0x0150,
    MYD2K = 0x0151,
    MYD2KS = 0x0152,
    MYD10K = 0x0153,
    MYD10KS = 0x0154,
    FUJITSU_AUTO_DETECT = 0x0160,
    MB89R118 = 0x0161,
    TAGSYS_15693 = 0x0170,
    TAGSYS_C370 = 0x0171,

    /** ISO 14443A Tags */
    ISO_14443A_AUTO_DETECT = 0x0200,
    PHILIPS_14443A_AUTO_DETECT = 0x0210,
    ISO_MIFARE_ULTRALIGHT = 0x0211,
    MIFARE_1K = 0x0212,
    MIFARE_4K = 0x0213,
    MIFARE_DESFIRE = 0x0214,
    MIFARE_PROX = 0x0215,
    INNOVISION_14443A_AUTO_DETECT = 0x0220,
    JEWEL = 0x0221,

    /** ISO 14443B Tags */
    ISO_14443B_AUTO_DETECT = 0x0300,
    ATMEL_14443B_AUTO_DETECT = 0x0310,
    CRYPTORF_1K = 0x0311,
    CRYPTORF_2K = 0x0312,
    CRYPTORF_4K = 0x0313,
    CRYPTORF_8K = 0x0314,
    CRYPTORF_16K = 0x0315,
    CRYPTORF_32K = 0x0316,
    CRYPTORF_64K = 0x0317,
    AT88RF001 = 0x0318,
    AT88RF020 = 0x0319,
    SAMSUNG_14443B_AUTO_DETECT = 0x0330,
    S3C89K8 = 0x0331,
    S3C89V5 = 0x0332,
    S3C89V8 = 0x0333,
    S3CC9G4 = 0x0334,
    S3CC9GC = 0x0335,
    S3CC9GW = 0x0336,
    S3CC9W4 = 0x0337,
    S3CC9W9 = 0x0338,
    ST_MICRO_14443B_AUTO_DETECT = 0x0350,
    ST_MICRO_SRIX4K = 0x0351,
    ST_MICRO_SRI176 = 0x0352,
    ST_MICRO_SRI512 = 0x0353,
	  AMEX_CARD = 0x0361,
	  AMEX_FOB = 0x0362,

    /** ISO 18000 Mode1 Tags */
    ISO_18000_3_MODE1_AUTO_DETECT = 0x0400,
    M1_TI_15693_AUTO_DETECT = 0x0410,
    M1_TAGIT_HF1_STANDARD = 0x0411,
    M1_TAGIT_HF1_PRO = 0x0412,
    M1_TAGIT_HF1_PLUS = 0x0413,
    M1_PHILIPS_15693_AUTO_DETECT = 0x0420,
    M1_ICODE_SLI_SL2 = 0x0421,
    M1_ST_15693_AUTO_DETECT = 0x0430,
    M1_LRI64 = 0x0431,
    M1_LRI512 = 0x0432,
    M1_EM_15693_AUTO_DETECT = 0x0440,
    M1_EM4006 = 0x0441,
    M1_EM4034 = 0x0442,
    M1_EM4035_CRYPTO = 0x0443,
    M1_EM4135 = 0x0444,
    M1_INFINEON_15693_AUTO_DETECT = 0x0450,
    M1_MYD2K = 0x0451,
    M1_MYD2KS = 0x0452,
    M1_MYD10K = 0x0453,
    M1_MYD10KS = 0x0454,

    /** Other HF Tags */
    ISO_18000_3_MODE1_EXTENDED_AUTO_DETECT = 0x0500,
    RFU = 0x0510,
    TAGSYS = 0x0511,
    ISO_18000_3_MODE2_AUTO_DETECT = 0x0600,
    INFINEON_AUTO_DETECT = 0x0610,
    INFINEON_PJM_TAG = 0x0611,
    ISO_18092_AUTO_DETECT = 0x0700,
    ISO_21481_AUTO_DETECT = 0x0800,
    HF_PROPRIETARY_RFU = 0x0900,
    TAGIT_HF = 0x0901,
    ICODE1 = 0x0902,
    HF_EPC = 0x0903,
    LTO_PHILIPS = 0x0904,
    LTO_ATMEL = 0x0905,
    FELICIA = 0x0906,
    PICOTAG_2K = 0x0907,
    PICOTAG_16K = 0x0908,
    PICOTAG_2KS = 0x0909,
    PICOTAG_16KS = 0x0910,
    HID_ICLASS = 0x0911,
    GEMWARE_C210 = 0x0912,
    GEMWARE_C220 = 0x0913,
    GEMWARE_C240 = 0x0914,
    SR176 = 0x0915,
    SKYETEK_AFE = 0x0916,

    /** EPC CLASS0 Tags */
    EPC_CLASS0_AUTO_DETECT = 0x8000,
    SYMBOL_CLASS0_AUTO_DETECT = 0x8010,
    MATRICS_CLASS0 = 0x8011,
    MATRICS_CLASS0_PLUS = 0x8012,
    IMPINJ_CLASS0_AUTO_DETECT = 0x8020,
    ZUMA = 0x8021,

    /** EPC C1 G1 Tags */
    EPC_CLASS1_GEN1_AUTO_DETECT = 0x8100,
    ALIEN_C1G1_AUTO_DETECT = 0x8110,
    QUARK = 0x8111,
    OMEGA = 0x8112,
    LEPTON = 0x8113,
    ST_MICRO_C1G1_AUTO_DETECT = 0x8120,
    XRA00 = 0x8121,

    /* EPC Class1 Gen2 */
    ISO_18000_6C_AUTO_DETECT = 0x8200, 
    IMPINJ_C1G2_AUTO_DETECT = 0x8210,
    MONZA = 0x8211,
    PHILIPS_C1G2_AUTO_DETECT = 0x8220,
    UCODE_EPC_G2 = 0x8221,
    MOTOROLA_EPC_G2 = 0x8222,
    ST_C1G2_AUTO_DETECT = 0x8230,
    XRAG2 = 0x8231,
	  ALIEN_HIGGS = 0x8251,
    EM_C1G2_AUTO = 0x8260,
    EM_C1G2_EM4024 = 0x8261,
    EM_C1G2_EM4124 = 0x8262,
    EM_C1G2_EM4224 = 0x8263,
    EM_C1G2_EM4324 = 0x8264,

    /** ISO 180006B Tags */
    ISO_18000_6B_AUTO_DETECT = 0x8300,
    PHILIPS_18000_6B_AUTO_DETECT = 0x8310,
    UCODE_1_19 = 0x8311,
    UCODE_HSL = 0x8312,
    FUJITSU_ISO180006B_AUTO_DETECT = 0x8320,
    FUJITSU_MB97R8010 = 0x8321,
    FUJITSU_MB97R8020 = 0x8322,

    /** ISO 180006A Tags */
    ISO_18000_6A_AUTO_DETECT = 0x8400,
    EM_6A = 0x8401,

    /** EM IPX Tags */
    EM_IPX_AUTO = 0x8500,
    EM4X22_AUTO = 0x8510,
    EM4022 = 0x8511,
    EM4122 = 0x8512,
    EM4222 = 0x8513,
    EM4X44_AUTO = 0x8520,
    EM4044 = 0x8521,
    EM4144 = 0x8522,
    EM4244 = 0x8523,
    EM4344 = 0x8524,
    EM4444 = 0x8525
} SKYETEK_TAGTYPE;

typedef enum SKYETEK_SYSTEM_PARAMETER
{
  /* Common parameters */
  SYS_SERIALNUMBER    = 0x0000, /* 4 bytes */
  SYS_FIRMWARE        = 0x0001, /* 4 bytes */
  SYS_HARDWARE        = 0x0002, /* 4 bytes */
  SYS_PRODUCT         = 0x0003, /* 2 bytes */
  SYS_RID             = 0x0004, /* 4 bytes */
  SYS_READER_NAME     = 0x0005, /* 32 bytes */
  SYS_HOST_INTERFACE  = 0x0006, /* 1 byte */
  SYS_BAUD            = 0x0007, /* 1 byte */
  SYS_PORT_DIRECTION  = 0x0008, /* 1 byte */
  SYS_PORT_VALUE      = 0x0009, /* 1 byte */
  SYS_MUX_CONTROL     = 0x000A, /* 1 byte */
  SYS_BOOTLOAD        = 0x000B, /* 1 byte */
  SYS_OPERATING_MODE  = 0x000C, /* 1 byte */
  SYS_ENCRYPTION      = 0x000D, /* 1 byte */
  SYS_HMAC            = 0x000E, /* 1 byte */
  SYS_PERIPHERAL      = 0x000F, /* 1 byte */
  SYS_TAG_POPULATION  = 0x0010, /* 1 byte */
  SYS_COMMAND_RETRY   = 0x0011, /* 1 byte */
  SYS_POWER_LEVEL     = 0x0012, /* 1 byte */

  /* M2 Specific Parameters */
  SYS_RECV_SENSITIVITY  = 0x0020, /* 1 byte */
  SYS_DESFIRE_FRAMING   = 0x0021, /* 1 byte */  
  SYS_TR_DATA_RATE      = 0x0022, /* 1 byte */
  SYS_RT_DATA_RATE      = 0x0023, /* 1 byte */
  
  /* M9 Specific System Parameters */
  SYS_CURRENT_FREQUENCY     = 0x0030, /* 4 bytes */  
  SYS_START_FREQUENCY       = 0x0031, /* 4 bytes */  
  SYS_STOP_FREQUENCY        = 0x0032, /* 4 bytes */ 
  SYS_RADIO_LOCK			      = 0x0033, /* 4 bytes */
  SYS_HOP_CHANNEL_SPACING   = 0x0034, /* 4 bytes */
  SYS_FREQ_HOP_SEQ          = 0x0035, /* 1 byte */
  SYS_MODULATION_DEPTH      = 0x0036, /* 1 byte */
  SYS_REGULATORY_MODE       = 0x0037, /* 1 byte */
  SYS_LISTEN_BEFORE_TALK    = 0x0038, /* 1 byte */
  SYS_SYNTH_POWER_LEVEL     = 0x003C, /* 1 byte */
  SYS_MARK_DAC_VALUE        = 0x003D, /* 2 bytes */
  SYS_SPACE_DAC_VALUE       = 0x003E, /* 2 bytes */
  SYS_POWER_DETECTOR_VALUE  = 0x0040, /* 2 bytes */
  SYS_RSSI_VALUES           = 0x0044, /* Variable */

  SYS_OPTIMAL_POWER_C1G1    = 0x0045, /* 1 byte */
  SYS_OPTIMAL_POWER_C1G2    = 0x0046, /* 1 byte */
  SYS_OPTIMAL_POWER_180006B = 0x0047, /* 1 byte */
  SYS_TEST_MODE             = 0x0048  /* 1 byte */

} SKYETEK_SYSTEM_PARAMETER;

typedef enum SKYETEK_FILE_TYPE
{
    STANDARD_DATA_FILE = 0x00,
    BACKUP_DATA_FILE = 0x01,
    VALUE_FILE = 0x02,
    LINEAR_RECORD_FILE = 0x03,
    CYCLICAL_RECORD_FILE = 0x04
} SKYETEK_FILE_TYPE;

typedef enum SKYETEK_COMM_SETTINGS
{
    PLAIN = 0x00,
    PLAIN_WITH_MACING = 0x01,
    ENCRYPTED = 0x03
} SKYETEK_COMM_SETTINGS;

typedef enum SKYETEK_ACCESS
{
    KEY0 = 0,
    KEY1,
    KEY2,
    KEY3,
    KEY4,
    KEY5,
    KEY6,
    KEY7,
    KEY8,
    KEY9,
    KEY10,
    KEY11,
    KEY12,
    KEY13,
    ALLOW_ALL,
    DENY_ALL
} SKYETEK_ACCESS;

typedef enum SKYETEK_INTERFACE
{
    I14443D4 = 1, 
    I7816D3, 
    I7816D12
} SKYETEK_INTERFACE;

typedef enum SKYETEK_TRANSPORT
{
    T7816D4 = 1
} SKYETEK_TRANSPORT;

typedef enum SKYETEK_BLOCK
{
    I_BLOCK = 2, 
    R_BLOCK = 162, 
    S_BLOCK = 194
} SKYETEK_BLOCK;


typedef enum SKYETEK_HMAC_ALGORITHM
{
  SHA1 = 1,
  SHA224 = 2, 
  SHA256 = 3, 
  SHA384 = 4,
  SHA512 = 5, 
  MD5 = 6
} SKYETEK_HMAC_ALGORITHM, *LPSKYETEK_HMAC_ALGORITHM;

typedef enum SKYETEK_CIPHER_ALGORITHM
{
  NOCIPHER = 0,
  TDEA56 = 18, 
  TDEA112 = 34, 
  TDEA168 = 66,
  AES128 = 49, 
  AES192 = 81, 
  AES256 = 97
} SKYETEK_CIPHER_ALGORITHM, *LPSKYETEK_CIPHER_ALGORITHM;

typedef enum SKYETEK_PAYMENT_TYPE
{
  EXPRESSPAY = 1,
  PAYPASS = 2,
  DISCOVER = 3
} SKYETEK_PAYMENT_TYPE;

typedef enum SKYETEK_TRANSACTION
{
  OFFLINE = 1,
  ONLINE = 2,
  REJECT = 3
} SKYETEK_TRANSACTION;

typedef enum SKYETEK_PARITY
{
	MARK,
	NONE,
	ODD,
	EVEN,
	SPACE
} SKYETEK_PARITY;

typedef enum SKYETEK_STOPBITS
{
	ONE,
	ONE_POINT_FIVE,
	TWO
} SKYETEK_STOPBITS;

typedef struct SKYETEK_PROTOCOL 
{
  unsigned short version;
  void *internal;
} SKYETEK_PROTOCOL, *LPSKYETEK_PROTOCOL;

typedef struct SKYETEK_ID 
{
    unsigned char   *id;
    unsigned int    length;
} SKYETEK_ID, *LPSKYETEK_ID;

typedef struct SKYETEK_DEVICE 
{
  TCHAR                  friendly[64];
  TCHAR                  type[64];
  TCHAR                  address[256];
  unsigned char         asynchronous;
  unsigned int          major;
  SKYETEK_DEVICE_FILE   readFD;
  SKYETEK_DEVICE_FILE   writeFD;
  void                  *user;
  void                  *internal;
} SKYETEK_DEVICE, *LPSKYETEK_DEVICE;

typedef struct SERIAL_SETTINGS
{
  int               baudRate;
  int               dataBits;
  SKYETEK_PARITY    parity;
  SKYETEK_STOPBITS  stopBits;
} SKYETEK_SERIAL_SETTINGS, *LPSKYETEK_SERIAL_SETTINGS;

typedef struct SKYETEK_READER
{
  LPSKYETEK_ID              id;
  TCHAR                      model[128];
  TCHAR                      serialNumber[128];
  TCHAR                      firmware[128];
  TCHAR                      manufacturer[128];
  TCHAR                      rid[128];
  TCHAR                      readerName[128];
  TCHAR                      friendly[256];
  unsigned char             isBootload;
  unsigned char             sendRID;
  LPSKYETEK_PROTOCOL        lpProtocol;
  LPSKYETEK_DEVICE          lpDevice;
  void                      *user;
  void                      *internal;
} SKYETEK_READER, *LPSKYETEK_READER;

typedef struct SKYETEK_TAG 
{
  SKYETEK_TAGTYPE     type;
  LPSKYETEK_ID        id;
  unsigned short      afi;
  unsigned short      session;
  unsigned char       rf;
  TCHAR                friendly[128];
  void                *user;
  void                *internal;
} SKYETEK_TAG, *LPSKYETEK_TAG;

typedef struct SKYETEK_DATA
{
    unsigned char *data;
    unsigned int  size;
} SKYETEK_DATA, *LPSKYETEK_DATA;

typedef TCHAR * LPSKYETEK_STRING;

typedef struct SKYETEK_ADDRESS
{
    unsigned int start;
    unsigned int blocks;
} SKYETEK_ADDRESS, *LPSKYETEK_ADDRESS;

typedef struct SKYETEK_MEMORY
{
    unsigned int          startBlock;
    unsigned int          maxBlock;
    unsigned int          bytesPerBlock;
    LPSKYETEK_DATA        lpOtherInfo;
} SKYETEK_MEMORY, *LPSKYETEK_MEMORY;

typedef struct SKYETEK_FILE_SETTINGS
{
    SKYETEK_FILE_TYPE       type;
    SKYETEK_COMM_SETTINGS   comm;
    SKYETEK_ACCESS          readAccess;
    SKYETEK_ACCESS          writeAccess;
    SKYETEK_ACCESS          readWriteAccess;
    SKYETEK_ACCESS          changeAccess;
} SKYETEK_FILE_SETTINGS, *LPSKYETEK_FILE_SETTINGS;

typedef struct SKYETEK_DATA_FILE_SETTINGS
{
    SKYETEK_FILE_TYPE       type;
    SKYETEK_COMM_SETTINGS   comm;
    SKYETEK_ACCESS          readAccess;
    SKYETEK_ACCESS          writeAccess;
    SKYETEK_ACCESS          readWriteAccess;
    SKYETEK_ACCESS          changeAccess;
    unsigned int            fileSize;
} SKYETEK_DATA_FILE_SETTINGS, *LPSKYETEK_DATA_FILE_SETTINGS;

typedef struct SKYETEK_VALUE_FILE_SETTINGS
{
    SKYETEK_FILE_TYPE       type;
    SKYETEK_COMM_SETTINGS   comm;
    SKYETEK_ACCESS          readAccess;
    SKYETEK_ACCESS          writeAccess;
    SKYETEK_ACCESS          readWriteAccess;
    SKYETEK_ACCESS          changeAccess;
    unsigned int            lowerLimit;
    unsigned int            upperLimit;
    unsigned int            value;
    unsigned char           limitedCreditEnabled;
} SKYETEK_VALUE_FILE_SETTINGS, *LPSKYETEK_VALUE_FILE_SETTINGS;

typedef struct SKYETEK_RECORD_FILE_SETTINGS
{
    SKYETEK_FILE_TYPE       type;
    SKYETEK_COMM_SETTINGS   comm;
    SKYETEK_ACCESS          readAccess;
    SKYETEK_ACCESS          writeAccess;
    SKYETEK_ACCESS          readWriteAccess;
    SKYETEK_ACCESS          changeAccess;
    unsigned int            recordSize;
    unsigned int            maxRecords;
    unsigned int            numRecords;
} SKYETEK_RECORD_FILE_SETTINGS, *LPSKYETEK_RECORD_FILE_SETTINGS;

typedef struct SKYETEK_KEY
{
  SKYETEK_ADDRESS       address;
  unsigned char         number;
  unsigned char         version;
  LPSKYETEK_DATA        lpData;
  TCHAR                 name[128];
} SKYETEK_KEY, *LPSKYETEK_KEY;

typedef struct SKYETEK_APP_KEY_SETTINGS
{
    SKYETEK_ACCESS  accessRights;
    unsigned char   isConfigFrozen;
    unsigned char   isAuthReqCreateDelFile;
    unsigned char   isAuthReqDirAccess;
    unsigned char   isAllowAppMasterKeyChange;
    unsigned int    numKeys;
} SKYETEK_APP_KEY_SETTINGS, *LPSKYETEK_APP_KEY_SETTINGS;

typedef struct SKYETEK_MASTER_KEY_SETTINGS
{
    unsigned char   isConfigFrozen;
    unsigned char   isAuthReqCreateDelApp;
    unsigned char   isAuthReqAppDirAccess;
    unsigned char   isAllowPiccMasterKeyChange;
    unsigned int    numKeys;
} SKYETEK_MASTER_KEY_SETTINGS, *LPSKYETEK_MASTER_KEY_SETTINGS;

typedef struct SKYETEK_PAYMENT_SYSTEM
{
  SKYETEK_PAYMENT_TYPE   systemType;
  unsigned char          id[128];
  unsigned               idLength;
  char                   label[128];
  unsigned short         interchangeProfile;
  unsigned char          elementaryFiles[8192];
  unsigned               elementaryFilesLength;
  unsigned char          track1[SKYETEK_TRACK1_MAXIMUM_SIZE];
  unsigned               track1Length;
  unsigned char          track2[SKYETEK_TRACK2_MAXIMUM_SIZE];
  unsigned               track2Length;
} SKYETEK_PAYMENT_SYSTEM, *LPSKYETEK_PAYMENT_SYSTEM;

typedef struct SKYETEK_TRACK1
{
	char                   primaryAccountNumber[19+1];
	char                   surname[26+1];
	char                   firstName[26+1];
	char                   middleName[26+1];
	char                   title[26+1];
	char                   expiration[4+1];
	char                   serviceCode[3+1];
} SKYETEK_TRACK1, *LPSKYETEK_TRACK1;


/****************************************************
 * CALLBACKS 
 ****************************************************/

/**
 * Tag select callback used by inventory and loop modes
 * @param lpTag Tag selected
 * @param user User data
 * @return 0 to stop inventory/loop, 1 to continue
 */ 
typedef unsigned char 
(*SKYETEK_TAG_SELECT_CALLBACK)(
    LPSKYETEK_TAG    lpTag, 
    void            *user
    );

/**
 * Firmware upload callback. Called everytime a block is successfully written.
 * @param percentComplete Percent of upload completed
 * @param version Version of the bootloader (read from file)
 * @param user User data
 * @return Zero if the user canceled, one to continue
 */
typedef int 
(*SKYETEK_FIRMWARE_UPLOAD_CALLBACK)(
    unsigned int    percentComplete, 
    unsigned int    version,
    void            *user
    );

/**
 * Payment scan mode callback. This is called by the API every time
 * a payment line is received by the reader when it is in payment
 * scan mode. This is passed to the SkyeTek_ScanPayment() call.
 * @param line The line that is returned.
 * @param user User data passed in to SkyeTek_ScanPayment()
 * @param return 0 to stop the scan or 1 to continue
 */
typedef unsigned char 
(*SKYETEK_PAYMENT_SCAN_CALLBACK)(
    char    *line,
    void    *user
    );

/**
 * Debug output callback. Called by API to report debugging messages.
 * @param msg Message to write to debugger
 */
typedef void 
(*SKYETEK_DEBUG_CALLBACK)(
    TCHAR *msg
    );


/****************************************************
 * DEVICE FUNCTIONS
 ****************************************************/

/**
 * This discovers all of the devices connected to the host that might
 * have readers attached to them.
 * @param lpDevices Pointer to array to popluate.  This function will allocate memory.
 * @return Number of devices found (size of devices array) 
 */
SKYETEK_API unsigned int 
SkyeTek_DiscoverDevices(
    LPSKYETEK_DEVICE		**lpDevices
    );

/**
 * Frees the devices allocated during SkyeTek_DiscoverDevices().
 * @param lpDevices Devices to free
 * @param count Number of devices
 */
SKYETEK_API void 
SkyeTek_FreeDevices(
    LPSKYETEK_DEVICE   *lpDevices,
    unsigned int       count
    );

/**
 * Creates a device on the device address..
 * @param address Device address
 * @param lpDevice Pointer to reader pointer to fill.  This function will allocate memory.
 * @return Status 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateDevice(
	TCHAR                *address, 
	LPSKYETEK_DEVICE    *lpDevice
	);

/**
 * Frees the device allocated during SkyeTek_CreateDevice().
 * @param lpDevice Device to free
 */
SKYETEK_API void 
SkyeTek_FreeDevice(
    LPSKYETEK_DEVICE   lpDevice
    );

/** 
 * This sets an additional timeout to be added to all
 * API calls made on this device. Some devices are slower
 * than others. If the API calls report SKYETEK_TIMEOUT
 * this may be called to increase the timeout.
 * @param lpDevice Device to set additional timeout on
 * @param timeout Timeout in milliseconds to add
 * @return Status
 */
SKYETEK_API SKYETEK_STATUS
SkyeTek_SetAdditionalTimeout(
  LPSKYETEK_DEVICE    lpDevice,
  unsigned int        timeout
  );

/**
 * Open the device for direct reading (non-Windows platforms only).
 * @param lpDevice Device to free
 */
SKYETEK_API FILE* 
SkyeTek_OpenDeviceStream(
    LPSKYETEK_DEVICE   lpDevice
    );


/********************************************************************************
 * SERIAL DEVICE FUNCTIONS
 ********************************************************************************/

/** 
 * Sets the serial device communication options.
 * @param device The device
 * @param settings The settings
 * @return Status
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SetSerialOptions(
  LPSKYETEK_DEVICE            device, 
  LPSKYETEK_SERIAL_SETTINGS   lpSettings
  );

/** 
 * Gets the serial device communication options.
 * @param device The device
 * @param settings The settings
 * @return Status
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetSerialOptions(
  LPSKYETEK_DEVICE            device, 
  LPSKYETEK_SERIAL_SETTINGS   lpSettings
  );


/****************************************************
 * READER FUNCTIONS
 ****************************************************/

/**
 * This discovers all of the readers to be found on the devices that
 * are passed in.
 * @param lpDevices Point to an array of devices
 * @param deviceCount Count of devices in the array
 * @param lpReaders Pointer to array to popluate.  This function will allocate memory.
 * @return Number of readers found (size of readers array) 
 */
SKYETEK_API unsigned int 
SkyeTek_DiscoverReaders(
    LPSKYETEK_DEVICE     *lpDevices, 
    unsigned int         deviceCount, 
    LPSKYETEK_READER     **lpReaders
    );

/**
 * Frees the readers.
 * @param lpReaders Readers to free
 * @param count Number of readers
 */
SKYETEK_API void 
SkyeTek_FreeReaders(
    LPSKYETEK_READER   *lpReaders,
    unsigned int       count
    );

/**
 * Creates a reader on the device given..
 * @param lpDevice Device to create reader on
 * @param lpReader Pointer to reader pointer to fill.  This function will allocate memory.
 * @return Status 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateReader(
    LPSKYETEK_DEVICE		lpDevice, 
    LPSKYETEK_READER		*lpReader
    );

/**
 * Frees the reader.
 * @param lpReader Reader to free
 */
SKYETEK_API void 
SkyeTek_FreeReader(
    LPSKYETEK_READER   lpReader
    );

/** 
 * Exercises the reader in select mode. 
 * @param lpReader Reader to execute this command on.
 * @param tagType Select only a specific tag type. 
 * @param callback Function to call when a tag is found.  The return of this
 * function determines when this call completes if in loop mode (0 to stop, 1 to continue)
 * @param inv true(1) indicates the reader should run in inventory/anti-collision mode
 * @param loop Run reader in loop mode, reader will continually scan for
 * tags in its field. 
 * @param user User data to pass to callback along with tag
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SelectTags(
    LPSKYETEK_READER            lpReader, 
    SKYETEK_TAGTYPE             tagType, 
    SKYETEK_TAG_SELECT_CALLBACK callback, 
    unsigned char               inv, 
    unsigned char               loop, 
    void                        *user
    );

/** 
 * Gets the list of tags that the reader has detected. 
 * @param lpReader Reader to execute this command on.
 * @param tagType Select only a specific tag type. 
 * @param lpTags Pointer to pointer to be allocated and filled with tags
 * @param count Pointer to count to be filled with number of tags
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetTags(
    LPSKYETEK_READER   lpReader, 
    SKYETEK_TAGTYPE    tagType, 
    LPSKYETEK_TAG      **lpTags, 
    unsigned short     *count
    );

/** 
 * Gets the list of tags that the reader has detected
 * that match the TID mask provided. The mask is one byte
 * long and any tags that with IDs with their first byte
 * matching the mask will be returned. This is only supported
 * in the M9 reader.
 * @param lpReader Reader to execute this command on.
 * @param tagType Select only a specific tag type. 
 * @param lpTagIdMask One byte mask for TID matching 
 * @param lpTags Pointer to pointer to be allocated and filled with tags
 * @param count Pointer to count to be filled with number of tags
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetTagsWithMask(
    LPSKYETEK_READER   lpReader, 
    SKYETEK_TAGTYPE    tagType, 
	LPSKYETEK_ID	   lpTagIdMask,
    LPSKYETEK_TAG      **lpTags, 
    unsigned short     *count
    );

/**
 * Frees the tags returned from SkyeTek_GetTags().
 * @param lpReader Reader on which tags discovered
 * @param lpTags Tags to free
 * @param count Number of tags
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_FreeTags(
    LPSKYETEK_READER  lpReader,
    LPSKYETEK_TAG     *lpTags,
    unsigned short    count
    );


/** 
 * Stores the key on the reader.
 * @param lpReader Reader to execute this command on.
 * @param type Tag type of SkyeTek AFE for SkyeTek Crypto API
 * @param lpKey The key to store
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_StoreKey(
    LPSKYETEK_READER     lpReader,
    SKYETEK_TAGTYPE      type,
    LPSKYETEK_KEY        lpKey
    );

/** 
 * Loads the key for use in authentication and encryption.
 * @param lpReader Reader to execute this command on.
 * @param lpKey The key; only the key number is required
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_LoadKey(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_KEY        lpKey
    );

/**
 * Loads the default values into the reader.
 * @param lpReader Reader to execute command on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_LoadDefaults(
    LPSKYETEK_READER     lpReader
    );

/**
 * Resets the device
 * @param lpReader Reader to execute command on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ResetDevice(
    LPSKYETEK_READER     lpReader
    );

/**
 * Causes the device to bootload
 * @param lpReader Reader to execute command on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_Bootload(
    LPSKYETEK_READER     lpReader
    );

/**
 * Retrieves systerm parameter value from reader.
 * @param lpReader Reader to execute command on
 * @param parameter Parameter to retrieve
 * @param lpData Data to receive; buffer is allocated
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetSystemParameter(
    LPSKYETEK_READER             lpReader, 
    SKYETEK_SYSTEM_PARAMETER     parameter,
    LPSKYETEK_DATA               *lpData
    );

/**
 * Sets reader system parameter.
 * @param lpReader Reader to execute command on
 * @param parameter Parameter to set
 * @param lpData Value of parameter
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SetSystemParameter(
    LPSKYETEK_READER             lpReader, 
    SKYETEK_SYSTEM_PARAMETER     parameter, 
    LPSKYETEK_DATA               lpData
    );

/**
 * Retrieves the default systerm parameter value from reader.
 * @param lpReader Reader to execute command on
 * @param parameter Parameter to retrieve
 * @param lpData Value of parameter; Buffer is allocated.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetDefaultSystemParameter(
    LPSKYETEK_READER             lpReader, 
    SKYETEK_SYSTEM_PARAMETER     parameter, 
    LPSKYETEK_DATA               *lpData
    );

/**
 * Sets reader default system parameter.
 * @param lpReader Reader to execute command on
 * @param parameter Parameter to set
 * @param lpData Value of parameter
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SetDefaultSystemParameter(
    LPSKYETEK_READER             lpReader, 
    SKYETEK_SYSTEM_PARAMETER     parameter, 
    LPSKYETEK_DATA               lpData
    );

/**
 * Authenticates the reader.
 * @param lpReader Reader to execute command on
 * @param lpKey Key to authenticate with
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_AuthenticateReader(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_KEY        lpKey
    );

/**
 * Enable debug on reader.
 * @param lpReader Reader to execute command on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_EnableDebug(
    LPSKYETEK_READER     lpReader
    );

/**
 * Disable debug on reader.
 * @param lpReader Reader to execute command on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DisableDebug(
    LPSKYETEK_READER     lpReader
    );

/**
 * Get debug messages from reader.
 * @param lpReader Reader to execute command on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetDebugMessages(LPSKYETEK_READER     lpReader,
                         LPSKYETEK_DATA      *lpData);

/**
 * Upload firmware to reader. 
 * @param lpReader Reader to execute this command on.
 * @param file Path to the SkyeTek Hex File
 * @param defaultsOnly Set to 1 to have it load the default EEPROM settings only
 *        and not load the firmware code
 * @param callback Function to call with status updates
 * @param user User data passed to callback
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_UploadFirmware(
    LPSKYETEK_READER                      lpReader, 
    TCHAR                                  *file, 
    unsigned char                         defaultsOnly,
    SKYETEK_FIRMWARE_UPLOAD_CALLBACK      callback, 
    void                                  *user
    );




/****************************************************
 * TAG FUNCTIONS
 ****************************************************/

/**
 * Creates a tag. This is useful for creating a tag to pass in
 * to SkyeTek_SelectTag(). 
 * @param type The tag type to create
 * @param lpId The ID of the tag or NULL if not known
 * @param lpTag Pointer to tag pointer to fill. This function will allocate memory.
 * @return Status 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateTag(
    SKYETEK_TAGTYPE     type,
    LPSKYETEK_ID        lpId,
    LPSKYETEK_TAG       *lpTag
    );

/**
 * Creates a new tag duplicate of the tag passed in
 * @param lpTag Pointer to tag to duplicate
 * @return New tag or NULL 
 */
SKYETEK_API LPSKYETEK_TAG 
SkyeTek_DuplicateTag(
    LPSKYETEK_TAG       lpTag
    );

/**
 * Frees the tag.
 * @param lpTag Tag to free
 */
SKYETEK_API void 
SkyeTek_FreeTag(
    LPSKYETEK_TAG   lpTag
    );

/** 
 * Selects a single tag in the readers field.  If no tag is
 * supplied then the first tag found in the reader's field
 * is selected and passed by to the caller.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to select
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SelectTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag
    );

/** 
 * Reads the specified number of blocks of data from a tag.  Tag must be in selected state.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to read
 * @param lpAddr Address to read data from.
 * @param lpData Buffer to place data read in. Buffer is allocated.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReadTagData(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ADDRESS    lpAddr,
    unsigned char        encrypt,
    unsigned char        hmac,
    LPSKYETEK_DATA       *lpData
    );

/** 
 * Write the specified data to the start block specified in the tag
 * an Erase command is done by issuing a write command with numBlocks = 0 
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to read  
 * @param lpAddr Address to write.
 * @param encrypt Flag to indicate encryption or not
 * @param hmac Flag to indicate adding HMAC or not
 * @param lpData Buffer to write. 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_WriteTagData(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ADDRESS    lpAddr, 
    unsigned char        encrypt,
    unsigned char        hmac,
    LPSKYETEK_DATA       lpData
    );

/** 
 * Reads the specified number of blocks of configuration data from a tag.  Tag must be in selected state.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to read
 * @param lpAddr Address to read data from.
 * @param lpData Buffer to place data read in. Buffer is allocated.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReadTagConfig(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ADDRESS    lpAddr,
    LPSKYETEK_DATA       *lpData
    );

/** 
 * Write the specified configuration data to the start block specified in the tag
 * an Erase command is done by issuing a write command with numBlocks = 0 
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to read  
 * @param lpAddr Address to write.
 * @param encrypt Flag to indicate encryption or not
 * @param hmac Flag to indicate adding HMAC or not
 * @param lpData Buffer to write. 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_WriteTagConfig(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ADDRESS    lpAddr, 
    LPSKYETEK_DATA       lpData
    );

/**
 * Locks the specified blocks
 * @param lpTag Tag to read  
 * @param lpAddr Address to lock.
 * @param data Lock data (appropriate for some tags)
 */
SKYETEK_API SKYETEK_STATUS SkyeTek_LockTagBlock(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ADDRESS    lpAddr,
    LPSKYETEK_DATA       lpData
    );

/** 
 * Activates tags of the given type.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag with type to activate
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ActivateTagType(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag
    );

/** 
 * Deactivates tags of the given type.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag with type to deactivate
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DeactivateTagType(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag
    );

/**
 * Sets the tag to the reader's data rate.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to set data rate for
 * @param rate Data rate value to set
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SetTagBitRate(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    unsigned char        rate
    );

/**
 * Gets tag info. It puts the starting block, maximum
 * number of blocks and bytes per block in the memory structure.
 * It also fills in additional tag information, if available.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to get info for
 * @param lpMemory Structure of memory specifications
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetTagInfo(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_MEMORY     lpMemory
    );

/**
 * Gets the lock state on the given address.
 * @param lpReader Reader to execute command on
 * @param tag Optional tag to get lock status on (or first tag found)
 * @param lpAddr Address to get lock status for
 * @param status Status of lock, either 0 for not locked, 1 for locked
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetLockStatus(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ADDRESS    lpAddr,
    unsigned char        *status
    );

/**
 * Issues kill command to tag rendering the tag un-selectable.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to kill
 * @param key Optional password or key to send with kill command
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_KillTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_KEY        lpKey
    );

/**
 * Revives the tag so it is selectable again
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to revive
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReviveTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag
    );

/**
 * Erases all blocks on the tag.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to erase
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_EraseTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag
    );

/**
 * Formats the tag for writing.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to format
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_FormatTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag
    );

/**
 * Deselects the tag.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to deselect
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DeselectTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag
    );

/**
 * Authenticates the tag using the key or password set by a previous
 * command.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to format 
 * @param lpKey The key
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_AuthenticateTag(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_KEY        lpKey
    );

/**
 * Sends the tag password.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to format 
 * @param lpData Password
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SendTagPassword(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_DATA       lpData
    );

/**
 * Gets the application IDs and puts them in the array.
 * It allocates memory for the array and fills in the count
 * with the number of IDs in the array.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to get the IDs from
 * @param lpIds Pointer to array to filled in
 * @param count Pointer to count to filled in
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetApplicationIDs(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ID         **lpIds,
    unsigned int         *count
    );

/**
 * Selects the application with the given application ID.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to select the application on
 * @param lpId The ID of the application
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SelectApplication(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ID         lpId
    );

/**
 * Creates a new application with the given application ID.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the application on
 * @param lpId The ID of the application
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateApplication(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ID         lpId,
    LPSKYETEK_APP_KEY_SETTINGS  lpSettings
    );

/**
 * Deletes an application with the given application ID.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to delete the application from
 * @param lpId The ID of the application
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DeleteApplication(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ID         lpId
    );

/**
 * Gets the file IDs and puts them in the array.
 * It allocates memory for the array and fills in the count
 * with the number of IDs in the array.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to get the IDs from
 * @param lpId ID of the application
 * @param lpFiles Pointer to array of file IDs to filled in
 * @param count Pointer to count to filled in
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetFileIDs(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_ID         **lpFiles,
    unsigned int         *count
    );

/**
 * Selects the file with the given file ID.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to select the file on
 * @param id The ID of the file
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SelectFile(
    LPSKYETEK_READER    lpReader,
    LPSKYETEK_TAG       lpTag,
    LPSKYETEK_ID        lpFile
    );

/**
 * Creates a new file with the given file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings File settings
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateDataFile(
    LPSKYETEK_READER             lpReader,
    LPSKYETEK_TAG                lpTag,
    LPSKYETEK_ID                 lpFile,
    LPSKYETEK_DATA_FILE_SETTINGS lpSettings
    );

/**
 * Creates a new file with the given file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings File settings
 */

SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateValueFile(
    LPSKYETEK_READER                 lpReader,
    LPSKYETEK_TAG                    lpTag,
    LPSKYETEK_ID                     lpFile,
    LPSKYETEK_VALUE_FILE_SETTINGS    lpSettings
    );

/**
 * Creates a new file with the given file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings File settings
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreateRecordFile(
    LPSKYETEK_READER                 lpReader,
    LPSKYETEK_TAG                    lpTag,
    LPSKYETEK_ID                     lpFile,
    LPSKYETEK_RECORD_FILE_SETTINGS   lpSettings
    );

/**
 * Gets the common file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings Settings to fill in 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetCommonFileSettings(
    LPSKYETEK_READER             lpReader,
    LPSKYETEK_TAG                lpTag,
    LPSKYETEK_ID                 lpFile,
    LPSKYETEK_FILE_SETTINGS      lpSettings
    );

/**
 * Gets the data file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings Settings to fill in 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetDataFileSettings(
    LPSKYETEK_READER                 lpReader,
    LPSKYETEK_TAG                    lpTag,
    LPSKYETEK_ID                     lpFile,
    LPSKYETEK_DATA_FILE_SETTINGS     lpSettings
    );

/**
 * Gets the file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings Settings to fill in 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetValueFileSettings(
    LPSKYETEK_READER                 lpReader,
    LPSKYETEK_TAG                    lpTag,
    LPSKYETEK_ID                     lpFile,
    LPSKYETEK_VALUE_FILE_SETTINGS    lpSettings
    );

/**
 * Gets the record file settings.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to create the file on
 * @param lpFile The ID of the file
 * @param lpSettings Settings to fill in 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetRecordFileSettings(
    LPSKYETEK_READER                 lpReader,
    LPSKYETEK_TAG                    lpTag,
    LPSKYETEK_ID                     lpFile,
    LPSKYETEK_RECORD_FILE_SETTINGS   lpSettings
    );

/**
 * Changes the settings on the given file.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag with the file
 * @param lpFile The ID of the file
 * @param lpSettings File settings
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ChangeFileSettings(
    LPSKYETEK_READER             lpReader,
    LPSKYETEK_TAG                lpTag,
    LPSKYETEK_ID                 lpFile,
    LPSKYETEK_FILE_SETTINGS      lpSettings
    );

/** 
 * Reads the file from the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to get file settings from
 * @param lpFile File to read off tag
 * @param lpAddr Address is the offset into the file to read
 * @param lpData Buffer to place file data in; it is allocated.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReadFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    LPSKYETEK_ADDRESS    lpAddr,
    LPSKYETEK_DATA       *lpData
    );
    	
/** 
 * Writes data to the given file on the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to write file data to 
 * @param lpFile File to write data to 
 * @param lpAddr Address is the offset into the file to read
 * @param lpData Data to write.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_WriteFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    LPSKYETEK_ADDRESS    lpAddr,
    LPSKYETEK_DATA       lpData
    );

/** 
 * Deletes the given file from the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to delete file from
 * @param lpFile File to delete
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DeleteFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile
    );

/** 
 * Clears the file.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to clear file on
 * @param lpFile File to clear
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ClearFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile
    );

/** 
 * Credit the value file by the amount given.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag with file
 * @param lpFile File to credit
 * @param amount The amount to credit
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CreditValueFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    unsigned int         amount
    );

/** 
 * Debit the value file by the amount given.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag with file
 * @param lpFile File to debit
 * @param amount The amount to debit
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DebitValueFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    unsigned int         amount
    );

/** 
 * Limited credit value file by the amount given.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag with file
 * @param lpFile File
 * @param amount The amount
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_LimitedCreditValueFile(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    unsigned int         amount
    );

/** 
 * Gets the value of the file.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag with file
 * @param lpFile File
 * @param value Pointer to the value
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetValue(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    unsigned int         *value
    );

/** 
 * Reads the record from the file on the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to get file settings from
 * @param lpFile File to read off tag
 * @param lpAddr Address is the offset into the record to read
 * @param lpData Buffer to place file data in; it is allocated.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReadRecords(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    LPSKYETEK_ADDRESS    lpAddr,
    LPSKYETEK_DATA       *lpData
    );
    	
/** 
 * Writes data to the given file on the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to write file data to 
 * @param lpFile File to write data to 
 * @param lpAddr Address is the offset into the record to write
 * @param lpData Data to write to the record
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_WriteRecord(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_ID         lpFile,
    LPSKYETEK_ADDRESS    lpAddr,
    LPSKYETEK_DATA       lpData
    );

/** 
 * Commits the outstanding transaction. All fill calls are
 * transactional and require either a commit or an abort 
 * to complete.
 * @param lpReader Reader.
 * @param lpTag Tag 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CommitTransaction(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag
    );

/** 
 * Aborts the outstanding transaction. All fill calls are
 * transactional and require either a commit or an abort 
 * to complete.
 * @param lpReader Reader.
 * @param lpTag Tag 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_AbortTransaction(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag
    );

/** 
 * Enables Electronic Article Surveillance (EAS) on the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to enable EAS on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_EnableEAS(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag
    );

/** 
 * Disables Electronic Article Surveillance (EAS) on the tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to disable EAS on
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_DisableEAS(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag
    );

/** 
 * Scans EAS for current state.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to scan for EAS
 * @param state Current state of EAS, 0 = disabled, 1 = enabled
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ScanEAS(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag,
    unsigned char        *state
    );

/** 
 * Reads the AFI value.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to read AFI value from
 * @param lpData Buffer to store AFI data in; it is allocated.
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReadAFI(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_DATA       *lpData
    );
    	
/** 
 * Writes AFI value.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to write AFI data to 
 * @param lpData AFI value data
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_WriteAFI(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_DATA       lpData
    );

/** 
 * Reads the DSFID value.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to read DSFID from
 * @param lpData Buffer to store DSFID; it is allocated
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ReadDSFID(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_DATA       *lpData
    );
    	
/** 
 * Writes DSFID value.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag to write DSFID data to 
 * @param lpData DSFID value data
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_WriteDSFID(
    LPSKYETEK_READER     lpReader, 
    LPSKYETEK_TAG        lpTag, 
    LPSKYETEK_DATA       lpData
    );

/** 
 * Gets the key version number for the given key.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag for this command
 * @param lpKey The key to get the version for; key number is
 *            required, version is filled in
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetKeyVersion(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_KEY        lpKey
    );

/** 
 * Changes the key for the given key number.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag for this command
 * @param lpNewKey New key
 * @param lpCurrentKey Current key (optional)
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ChangeKey(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    LPSKYETEK_KEY        lpNewKey,
    LPSKYETEK_KEY        lpCurrentKey
    );

/** 
 * Gets the key settings for the application. Note that
 * the application must be selected first.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag for this command
 * @param lpSettings Key settings to be filled in
 * @return Status
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetApplicationKeySettings(
    LPSKYETEK_READER            lpReader,
    LPSKYETEK_TAG               lpTag,
    LPSKYETEK_APP_KEY_SETTINGS  lpSettings
    );

/** 
 * Gets the key master settings for the reader/tag.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag for this command
 * @param lpSettings Key settings to be filled in
 * @return Status
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_GetMasterKeySettings(
    LPSKYETEK_READER                lpReader,
    LPSKYETEK_TAG                   lpTag,
    LPSKYETEK_MASTER_KEY_SETTINGS   lpSettings
    );

/** 
 * Changes the application key settings.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag for this command
 * @param lpSettings Key settings
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ChangeApplicationKeySettings(
    LPSKYETEK_READER             lpReader,
    LPSKYETEK_TAG                lpTag,
    LPSKYETEK_APP_KEY_SETTINGS   lpSettings
    );

/** 
 * Changes the master key settings.
 * @param lpReader Reader to execute this command on.
 * @param lpTag Tag for this command
 * @param lpSettings Key settings
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ChangeMasterKeySettings(
    LPSKYETEK_READER                 lpReader,
    LPSKYETEK_TAG                    lpTag,
    LPSKYETEK_MASTER_KEY_SETTINGS    lpSettings
    );

/**
 * Initializes the tag's memory for use with HMAC and 
 * encryption of memory reads and writes. In the case of 
 * Mifare 1k/4k tags it is necessary to authenticate to 
 * the tag before calling this command.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to format 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_InitializeSecureMemoryTag(
    LPSKYETEK_READER          lpReader,
    LPSKYETEK_TAG             lpTag,
    SKYETEK_HMAC_ALGORITHM    hmac,
    LPSKYETEK_KEY             lpKeyHMAC,
    SKYETEK_CIPHER_ALGORITHM  cipher,
    LPSKYETEK_KEY             lpKeyCipher,
		int                       useKeyDerivationFunction
    );

/**
 * Sets up the secure memory configuration for the tag.
 * @param lpReader Reader to execute command on
 * @param lpTag Tag to format 
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_SetupSecureMemoryTag(
    LPSKYETEK_READER          lpReader,
    LPSKYETEK_TAG             lpTag,
    LPSKYETEK_KEY             lpKeyHMAC,
    LPSKYETEK_KEY             lpKeyCipher,
		int                       useKeyDerivationFunction
    );


/**
 * Sends data over the air interface
 * @param lpReader Reader to execute command on
 * @param lpTag Tag for command
 * @param interface Interface to send data on
 * @param block Block
 * @param lpSendData Data to send
 * @param lpRecvData Data to receive
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_InterfaceSend(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    SKYETEK_INTERFACE    inteface,
    SKYETEK_BLOCK        block,
    LPSKYETEK_DATA       lpSendData,
    LPSKYETEK_DATA       *lpRecvData
    );

/**
 * Sends data over the transport
 * @param lpReader Reader to execute command on
 * @param lpTag Tag for command
 * @param transport Transport to send data on
 * @param block Block
 * @param lpCla Data to send
 * @param lpIns Data to send
 * @param lpP1p2 Data to send
 * @param lpData Data to send
 * @param le Integer to send
 * @param lpRecvData Data to receive
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_TransportSend(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    SKYETEK_TRANSPORT    transport,
    SKYETEK_BLOCK        block,
    LPSKYETEK_DATA       lpCla,
    LPSKYETEK_DATA       lpIns,
    LPSKYETEK_DATA       lpP1p2,
    LPSKYETEK_DATA       lpData, 
    unsigned int         le,
    LPSKYETEK_DATA       *lpRecvData
    );

/**
 * Initiate Payment Process
 * @param lpReader Reader to execute command on
 * @param lpTag Tag for command
 * @param lpPaymentSystem Returned payment system details
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_InitiatePayment(
    LPSKYETEK_READER         lpReader,
    LPSKYETEK_TAG            lpTag,
    SKYETEK_PAYMENT_SYSTEM  *lpPaymentSystem
    );

/**
 * Compute payment cryptogram
 * @param lpReader Reader to execute command on
 * @param lpTag Tag for command
 * @param transaction Requested transaction type
 * @param lpPaymentSystem Returned payment system details
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ComputePayment(
    LPSKYETEK_READER     lpReader,
    LPSKYETEK_TAG        lpTag,
    SKYETEK_TRANSACTION  transaction,
    SKYETEK_PAYMENT_SYSTEM   *lpPaymentSystem
    );

/**
 * Sends the command to the reader to enter payment
 * scan mode. The reader will exit its normal
 * SkyeTek Protocol mode and write out lines of
 * payment information, including Track 1 and Track 2 data.
 * This function returns immediately. Applications using this
 * function (versus the SkyeTek_ScanPayments) will need to
 * read from the device handle using SkyeTek_ReadDevice.
 * @param lpReader Reader to execute command on
 * @return Status
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_EnterPaymentScanMode(
    LPSKYETEK_READER         lpReader
    );

/**
 * Sends the command to the reader to enter payment
 * scan mode and then waits for the response from the
 * reader. The reader will exit its normal
 * SkyeTek Protocol mode and write out lines of
 * payment information, including Track 1 and Track 2 data.
 * This function calls the callback function with each line
 * returned from the reader until the payment scan is finished.
 * Once finished, this function will return.
 * @param lpReader Reader to execute command on
 * @param callback Callback function to call for each line read
 * @param user User data to pass with each callback call
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ScanPayments(
    LPSKYETEK_READER              lpReader,
    SKYETEK_PAYMENT_SCAN_CALLBACK callback,
    void                          *user
    );

/**
 * Parse Payment Track 1 Data
 * @param rawTrack1 Raw track1 data bytes
 * @param lpTrack1 Parse track1 data structure
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_ParsePaymentTrack1(
    unsigned char* rawTrack1,
		int rawTrack1length,
		LPSKYETEK_TRACK1 lpTrack1
    );


/********************************************************************************
 * MEMORY FUNCTIONS
 ********************************************************************************/

/**
 * Allocates a data buffer.
 * @param size Size of buffer to allocate.
 */
SKYETEK_API LPSKYETEK_DATA 
SkyeTek_AllocateData(
    int size
    );

/**
 * Frees a data buffer.
 * @param lpData Data to free
 */
SKYETEK_API void 
SkyeTek_FreeData(
    LPSKYETEK_DATA     lpData
    );

/**
 * Allocates an ID.
 * @param size Size of ID to allocate.
 */
SKYETEK_API LPSKYETEK_ID 
SkyeTek_AllocateID(
    int length
    );

/**
 * Frees a ID.
 * @param lpId ID to free
 */
SKYETEK_API void 
SkyeTek_FreeID(
    LPSKYETEK_ID     lpId
    );

/**
 * Allocates an string buffer. 
 * @param ascii Data buffer to allocate
 */
SKYETEK_API LPSKYETEK_STRING
SkyeTek_AllocateString(
    unsigned int size
    );

/**
 * Frees an string buffer.
 * @param ascii string buffer to free
 */
SKYETEK_API void 
SkyeTek_FreeString(
    LPSKYETEK_STRING str
    );

/**
 * Gets the string equivalent of the data buffer.
 * The caller should call SkyeTek_FreeString() when done
 * with the result.
 * @param lpData Data buffer to convert
 */
SKYETEK_API LPSKYETEK_STRING 
SkyeTek_GetStringFromData(
    LPSKYETEK_DATA     lpData
    );

/**
 * Gets the data buffer equivalent the string.
 * The caller should call SkyeTek_FreeData() when done
 * with the result.
 * @param lpStr string to convert
 */
SKYETEK_API LPSKYETEK_DATA 
SkyeTek_GetDataFromString(
    LPSKYETEK_STRING     lpStr
    );

/**
 * Gets the string equivalent the data buffer.
 * The caller should call SkyeTek_FreeString() when done
 * with the result.
 * @param lpId Data buffer to convert
 */
SKYETEK_API LPSKYETEK_STRING
SkyeTek_GetStringFromID(
    LPSKYETEK_ID     lpId
    );

/**
 * Gets an ID from the ASCII string.
 * The caller should call SkyeTek_FreeID() when done
 * with the result.
 * @param lpStr string to convert
 */
SKYETEK_API LPSKYETEK_ID 
SkyeTek_GetIDFromString(
    LPSKYETEK_STRING     lpStr
    );

/**
 * Copied data from lpDataFrom to lpDataTo.
 * @param lpDataFrom Data to copy
 * @param lpDataTo Data to copy to
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CopyData(
    LPSKYETEK_DATA     lpDataTo,
    LPSKYETEK_DATA     lpDataFrom
    );

/**
 * Copied data from the buffer to lpData.
 * @param lpData Data to copy to
 * @param buffer Data to copy from
 * @param size Size of the buffer to copy
 */
SKYETEK_API SKYETEK_STATUS 
SkyeTek_CopyBuffer(
    LPSKYETEK_DATA     lpData,
    unsigned char      *buffer,
    unsigned int       size
    );


/****************************************************
 * DEBUGGING FUNCTIONS
 ****************************************************/

/**
 * Gets an error string for the given status code.
 * @param status Status code
 * @return string version
 */
SKYETEK_API TCHAR *
SkyeTek_GetStatusMessage(
    SKYETEK_STATUS  status
    );

/**
 * Sets the debugger. If callback is NULL, the debugger is disabled.
 * @param callback Callback to call to report debugging messages
 */
SKYETEK_API void 
SkyeTek_SetDebugger(
    SKYETEK_DEBUG_CALLBACK  callback
    );


/**
 * Backward compatibility.
 */
#define SkyeTek_AllocateASCII SkyeTek_AllocateString
#define SkyeTek_FreeASCII SkyeTek_FreeString
#define SkyeTek_GetASCIIFromData SkyeTek_GetStringFromData
#define SkyeTek_GetDataFromASCII SkyeTek_GetDataFromString
#define SkyeTek_GetASCIIFromID SkyeTek_GetStringFromID
#define SkyeTek_GetIDFromASCII SkyeTek_GetIDFromString
#define LPSKYETEK_ASCII LPSKYETEK_STRING


#ifdef __cplusplus
}
#endif

#endif
