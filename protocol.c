#include "protocol.h"
/*
* protocol fields
*/
field *fields;

/*
* Protocol fields count
*/
int fieldc;

void AddField(field_type fieldType, int length, int defaultValue, int validValues[]){
}

void AddDynamicLengthField(field_type fieldType, field_length_type fieldLengthType, int defaultValue, int validValues[]){
}

char * read(int index, int length, char *data){
     
}

char * desialize(){
}


field * parse(char data[]){
   int count = 0;
   int index = 0;
   
   while(count < fieldc){
      char *field_value;
      field _field = (field) *(fields + count);
      
      //Read field value based on defined length 
      switch(_field.length_type){
         case fix_length:
              field_value = read(index, _field.length, data);
              index += _field.length;
              break;
         case dynamic_length:
              break;                         
      }
      
      //Deserialize vlaue base on defined type
      switch(_field.type){
         case STX:
         case COM:
              desialize();
              break;
         case DTA:
              break;                         
      }
   }
}


