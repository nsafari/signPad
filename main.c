#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include "protocol.h"

void PrintCommState(DCB dcb)
{
    //  Print some of the DCB structure values
    _tprintf( TEXT("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n"), 
              dcb.BaudRate, 
              dcb.ByteSize, 
              dcb.Parity,
              dcb.StopBits );
}

void ReadCharacterFromCom1(HANDLE comPorthandle, char *theCharacter)
{
   DWORD numBytesRead;

   printf ("byte to read: %d.\n", sizeof(theCharacter));

   while( 1 == 1 ){               
       numBytesRead = 0;
       
       while (numBytesRead == 0)
       {
          ReadFile(comPorthandle,           // handle of file to read
                   theCharacter,            // store read data here
                   100,    // number of bytes to read
                   &numBytesRead,           // pointer to number of bytes actually read
                   NULL);
       }
       
       printf ("byte read: %d.\n", numBytesRead);
       printf("Read %s \n", theCharacter);
       

       
       memset(theCharacter, 0, 100*sizeof(*theCharacter));
   }
   return;
}

int main(int argc, char *argv[])
{
    field field1;
    field1.length_type = fix_length;
    
    int values[4] = {1, 2, 3, 4};
 
    AddField(STX, fix_length, 1, values);
    
    BOOL fSuccess;
    TCHAR *pcCommPort = TEXT("\\\\.\\COM5");
    // Open serial port
    HANDLE serialHandle;    
    serialHandle = CreateFile(pcCommPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    // Do some basic settings
    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    
    GetCommState(serialHandle, &serialParams);
    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    fSuccess = SetCommState(serialHandle, &serialParams);

    
    // Set timeouts
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = 500;
    timeout.ReadTotalTimeoutConstant = 500;
    timeout.ReadTotalTimeoutMultiplier = 500;
    timeout.WriteTotalTimeoutConstant = 500;
    timeout.WriteTotalTimeoutMultiplier = 500;
    
    fSuccess = SetCommTimeouts(serialHandle, &timeout);
        
    if (!fSuccess) 
   {
      //  Handle the error.
      printf ("SetCommState failed with error %d.\n", GetLastError());
      return (3);
   }


    
    fSuccess = GetCommState(serialHandle, &serialParams);

   if (!fSuccess) 
   {
      //  Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }


   PrintCommState(serialParams);       //  Output to console
   
    char DataBuffer[] = "Hello";
    DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
    DWORD dwBytesWritten = 0;

    fSuccess = WriteFile(
              serialHandle, 
              DataBuffer,
              dwBytesToWrite,
              &dwBytesWritten, 
              NULL);

   printf ("byte write: %d.\n", dwBytesWritten);

    
    if (!fSuccess) 
   {
      //  Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }
   
      char DataBuffer2[] = "this is a test";
      char *pDataBuffer = DataBuffer2;

       char c;
       
       printf("size of *theCharacter %x \n", DataBuffer2);
       printf("size of *theCharacter %x \n", &DataBuffer2[0]);
       printf("size of *theCharacter %x \n", pDataBuffer);
       
       
       printf("size of theCharacter %d \n", sizeof(DataBuffer2));
       printf("size of char %d \n", sizeof(c));

   ReadCharacterFromCom1(serialHandle, pDataBuffer);
   

    CloseHandle(serialHandle);
    
  system("PAUSE");	
  return 0;
}
