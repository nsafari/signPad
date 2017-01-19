#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include "protocol.h"

void PrintCommState(DCB dcb) {
    //  Print some of the DCB structure values
    _tprintf(TEXT("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n"),
             dcb.BaudRate,
             dcb.ByteSize,
             dcb.Parity,
             dcb.StopBits);
}

void ReadCharacterFromCom1(HANDLE comPorthandle, unsigned char *theCharacter) {

    DWORD numBytesRead;

    printf("Read bytes -> %x \n", *theCharacter);

    while (1 == 1) {
        numBytesRead = 0;

        while (numBytesRead == 0) {
            ReadFile(comPorthandle,           // handle of file to read
                     theCharacter,            // store read data here
                     100,    // number of bytes to read
                     &numBytesRead,           // pointer to number of bytes actually read
                     NULL);
        }


        printf("byte read: %d.\n", numBytesRead);
        printf("Read bytes -> %x \n", *theCharacter);



//       memset(theCharacter, 0, 100*sizeof(*theCharacter));
    }
    return;
}


int main(int argc, char *argv[]) {

    log_level = DBUG;

    SetFieldCount(5);

    AddField(STX, 1, 0x55, 0x00);
    AddField(COM, 1, 0X00, NULL);
    AddField(FLG, 1, 0x00, NULL);
    AddField(SER, 2, 0x00, 0x00);
    AddField(LEN, 2, 0x00, 0x00);

    unsigned char data[] = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0xf6, 0xa2};
    Parse(data);

    BOOL fSuccess;
    TCHAR *pcCommPort = TEXT("\\\\.\\COM5");
    // Open serial port
    HANDLE serialHandle;
    serialHandle = CreateFile(pcCommPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    // Do some basic settings
    DCB serialParams = {0};
    serialParams.DCBlength = sizeof(serialParams);

    GetCommState(serialHandle, &serialParams);
    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    fSuccess = SetCommState(serialHandle, &serialParams);


    // Set timeouts
    COMMTIMEOUTS timeout = {0};
    timeout.ReadIntervalTimeout = 50;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.ReadTotalTimeoutMultiplier = 50;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 50;

    fSuccess = SetCommTimeouts(serialHandle, &timeout);

    if (!fSuccess) {
        //  Handle the error.
        printf("SetCommState failed with error %d.\n", GetLastError());
        return (3);
    }


    fSuccess = GetCommState(serialHandle, &serialParams);

    if (!fSuccess) {
        //  Handle the error.
        printf("GetCommState failed with error %d.\n", GetLastError());
        return (2);
    }


    PrintCommState(serialParams);       //  Output to console

    char DataBuffer[] = "Hello";
    DWORD dwBytesToWrite = (DWORD) strlen(DataBuffer);
    DWORD dwBytesWritten = 0;

    fSuccess = WriteFile(
            serialHandle,
            DataBuffer,
            dwBytesToWrite,
            &dwBytesWritten,
            NULL);

    printf("byte write: %d.\n", dwBytesWritten);


    if (!fSuccess) {
        //  Handle the error.
        printf("GetCommState failed with error %d.\n", GetLastError());
        return (2);
    }

    unsigned char byte = 0xaa;
    unsigned char *bytes = &byte;

    ReadCharacterFromCom1(serialHandle, bytes);


    CloseHandle(serialHandle);

    system("PAUSE");
    return 0;
}
