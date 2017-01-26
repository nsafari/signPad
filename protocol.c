#define bytes_to_u16(MSB, LSB) (((unsigned int) ((unsigned char) MSB)) & 255)<<8 | (((unsigned char) LSB)&255)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protocol.h"


log_levels log_level = INFO;

void log(log_levels level, char *msg) {
    if (log_level == level) {
        printf("%s \n", msg);
    }
}

/*
* protocol fields
*/
field *fields;

/*
* Protocol fields count
*/
int static fieldc = 0;

unsigned char *read(int *index, int length, unsigned char *data);

unsigned char *readWithDynamicLength(int *index, int dependentFieldType, unsigned char *data);

field *Parse(unsigned char *data);

int findFieldByType(field_type fieldType);

void SetFieldCount(int count) {
    fields = (field *) malloc(count * sizeof(field));
}

void AddField(field_type fieldType, int length, int defaultValue, int validValues[]) {
    fields[fieldc].index = fieldc;
    fields[fieldc].type = fieldType;
    fields[fieldc].length_type = fix_length;
    fields[fieldc].length = length;
    fields[fieldc].default_value = defaultValue;
    fields[fieldc].valid_values = validValues;
    fieldc++;
    log(DBUG, "a new field added");
}

void
AddDynamicLengthField(field_type fieldType, field_type dependentFieldType, int defaultValue, int validValues[]) {
    fields[fieldc].index = fieldc;
    fields[fieldc].type = fieldType;
    fields[fieldc].length_type = dynamic_length;
    //Length contains the dependent field type
    fields[fieldc].length = dependentFieldType;
    fields[fieldc].default_value = defaultValue;
    fields[fieldc].valid_values = validValues;
    fieldc++;
    log(DBUG, "a new dynamic length field added");
}

/*
 * Search the fields by fieldType and return index of that
 * return -1 if fieldType not found
 */
int findFieldByType(field_type fieldType) {
    int i = 0;
    for (i = 0; i <= fieldc; i++) {
        if (fields[i].type == fieldType) {
            return i;
        }
    }
    return -1;
}

/*
* Return a packet from index to index + length of the data
* Example ->
* data : |f|0|1|a|b|
* index : 2 --^ length: 2
* result: |1|a| index: 4
*/
unsigned char *read(int *index, int length, unsigned char *data) {
    unsigned char *result = malloc(length * sizeof(unsigned char));
    int i = 0;
    for (i; i < length; i++, (*index)++) {
        *(result + i) = *(data + (*index));
    }
    return result;
}

/*
* Return a value of data from index to index + 1
* Example ->
* data : |f|0|1|a|b|
* index : 2 --^
* result: 26
*/
int readInt(int index, unsigned char *data) {
    return bytes_to_u16(data[index], data[index + 1]);
}

unsigned char *readWithDynamicLength(int *index, int dependentFieldType, unsigned char *data) {
    int fieldIndex = findFieldByType((field_type) dependentFieldType);
    return read(index, bytes_to_u16(fields[fieldIndex].value[0], fields[fieldIndex].value[1]), data);
}

void *deserializeInf(cmd *_cmd, field *_field, unsigned char *data) {
    _cmd->id = 1;
}

void *deserializeAnyCmd(cmd *_cmd, field *_field, unsigned char *data) {
    _cmd->data = NULL;
}

void *deserializeClearScreenCmd(cmd *_cmd, field *_field, unsigned char *data) {

}

void *deserialize(cmd *_cmd, field *_field, unsigned char *data) {
    switch (_field->type) {
        case COM:
            _cmd->type = (cmd_type) data[_field->index];
            break;
        case FLG:
            _cmd->flag = (cmd_flag) data[_field->index];
            break;
        case INF:
            deserializeInf(_cmd, _field, data);
            break;
        case DTA:
            switch (_cmd->type) {
                case (cmd_type) 0x80:
                case (cmd_type) 0x81:
                case (cmd_type) 0x82:
                case (cmd_type) 0x83:
                case (cmd_type) 0x84:
                case (cmd_type) 0x85:
                    deserializeAnyCmd(_cmd, _field, data);
                    break;
            }
            break;
    }
}

field *Parse(unsigned char *data) {

    //Should be used internal data
//    unsigned char *_innerData = malloc(sizeof(data));
//    memcpy(_innerData, data, sizeof(data));

    int count = 0;
    int index = 0;

    cmd *_cmd = malloc(sizeof(cmd));

    while (count < fieldc) {
        char *field_value;
        field *_field = fields + count;

        //Read field value based on defined length
        switch ((*_field).length_type) {
            case fix_length:
                //(*_field).value = read(&index, (*_field).length, data);
                (*_field).index = index;
                index += (*_field).length;
                break;
            case dynamic_length:
//                (*_field).value = readWithDynamicLength(&index, (*_field).length, data);
                (*_field).length = readInt(fields[findFieldByType((field_type) (*_field).length)].index, data);
                (*_field).length_type = fix_length;

                (*_field).index = index;
                index += (*_field).length;
                break;
        }

        //validate value base on valid values
        switch ((*_field).type) {
            case STX:
                break;
        }

        deserialize(_cmd, _field, data);

//        printf("field %d: %0.2x \n", (*_field).type, *((*_field).value));
        printf("field %d: Index:%d, Length:%d \n", (*_field).type, (*_field).index, (*_field).length);
        count++;
    }

    printf("command type: %x, flag: %x \n", _cmd->type, _cmd->flag);


}



