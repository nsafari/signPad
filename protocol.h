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

/*
* Add static length field
*/
void AddField(field_type fieldType, int length, int defaultValue, int validValues[]);

/*
* Add dynamic length field
*/
void AddDynamicLengthField(field_type fieldType, field_length_type lengthType, int defaultValue, int validValues[]);

/*
* Parse the incoming data based on the defined protocol
*/
field * Parse(unsigned char *data);

/*
*
*/
void Compose();
 
#endif
