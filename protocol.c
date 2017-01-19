#include <stdarg.h>
#include <stdio.h>
#include "protocol.h"

log_levels log_level = INFO;

void log(log_levels level, char *msg){    
     if(log_level == level){
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

void AddField(field_type fieldType, int length, int defaultValue, int validValues[]){     
     if(fieldc == 0){
       fields = malloc(sizeof(field));
     }else{           
       fields = realloc(fields, sizeof(field));
     }
     fields[fieldc].index = fieldc;
     fields[fieldc].type = fieldType;
     fields[fieldc].length = length;
     fields[fieldc].default_value = defaultValue;
     fields[fieldc].valid_values = validValues;
     fieldc++;     
     
     log(DBUG, "new field added ");
}

void AddDynamicLengthField(field_type fieldType, field_length_type fieldLengthType, int defaultValue, int validValues[]){
}

/*
* Reutrn a packet from index to index + length of the data
* Example ->
* data : |f|o|o|a|b|
* index : 2 --^ length: 2
* result: |o|a| index: 4
*/
unsigned char * read(int *index, int length, unsigned char *data){
  unsigned char *result = malloc(length * sizeof(unsigned char));
  int i = 0;
  for(i = 0; i < length; i++, *index++){
    *(result + i) = *(data + (*index));
  }   
  return result;
}


char * desialize(){
}


field * Parse(unsigned char *data){
      printf("Parse");
   int count = 0;
   int index = 0;
   
   while(count < fieldc){
      char *field_value;
      field _field = (field) *(fields + count);
      
      //Read field value based on defined length 
      switch(_field.length_type){
         case fix_length:
              field_value = read(&index, _field.length, data);
              index += _field.length;
              break;
         case dynamic_length:
              break;                         
      }
      
      //Deserialize vlaue base on defined type
      switch(_field.type){
         case STX:
         case COM:
              desialize(field_value);
              break;
         case DTA:
              break;                         
      }
   }
}


