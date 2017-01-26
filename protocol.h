#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

typedef enum {
     INFO = 0,
     DBUG = 1,
     EROR = 2
} log_levels;

extern log_levels log_level; 
 
int getfavoritenumber(void);

typedef enum{
        STX = 0,
        COM = 1,
        FLG = 2,
        INF = 3,
        SER = 4,
        LEN = 5,
        DTA = 6,
        CRC = 7,
        ETX = 8
} field_type;

typedef enum{
    CLR_SCR = 0x80,
    SHO_TXT = 0x81,
    GET_SNG = 0x82,
    GOT_SNG = 0x83,
    SND_SNG = 0x84,
    SVE_SNG = 0X85
} cmd_type;

typedef enum{
    EXC_CMD = 0x80,
    EXC_SUC = 0x81,
    CMD_IGN = 0x82,
    STR_FRM_ERR = 0x83,
    END_FRM_ERR = 0x84,
    DTA_LNG_ERR = 0X85,
    CRC_ERR = 0X86
} cmd_flag;

typedef enum {
        fix_length = 0,
        dynamic_length = 1
} field_length_type;

typedef struct {
        int index;
        field_type type;
        field_length_type length_type;
        int length;
        int default_value;
        unsigned char *value;
        int *valid_values;
} field;

typedef struct{
    unsigned int id;
    cmd_type type;
    cmd_flag flag;
    void *data;
} cmd;

/*
 * Allocate memory with size of (count * sizeof(field))
 */
void SetFieldCount(int count);

/*
* Add static length field
*/
void AddField(field_type fieldType, int length, int defaultValue, int validValues[]);

/*
* Add dynamic length field
*/
void AddDynamicLengthField(field_type fieldType, field_type dependentFieldType, int defaultValue, int validValues[]);

/*
* Parse the incoming data based on the defined protocol
*/
field * Parse(unsigned char *data);

/*
*
*/
void Compose();
 
#endif
