#ifndef __DATAHANDLER_H
#define __DATAHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>


/* Need to defined user variable(s)
* 
* Sessions directory path;
* BASEDIR
* 
* System avalible emulators;
* EMULATOR_AMOUNT
* Datahandler_con --> char* emulators
*/

#define BASEDIR "C:\\Users\\gardm\\.putty\\sessions\\"
#define EMULATOR_AMOUNT 2

#define SESSIONS_AMOUNT 10
#define MAX_SESSION_VAR_LEN 50


// If I manage to make getSessionNames to work I could
// Make it so sessions var could be heap allocated
// It would make it flexiable
// By us counting the size of sessions

typedef struct DataHandler {
    int ses_len;
    int emu_amo;
    int ses_amo;
    char* lusr;
    char* lsrv;
    char* emulators[EMULATOR_AMOUNT];
    char* sessions[SESSIONS_AMOUNT];
} DataHandler;

void DataHandler_con(DataHandler* dh);
void DataHandler_decon(DataHandler* dh);


// -------- May do from go -------- //

const char* DataHandler_getSrv(DataHandler* dh);
const char* DataHandler_getUsr(DataHandler* dh);
const char* DataHandler_getEmulator(DataHandler* dh, int ilm);
//char** DataHandler_getSessionNames(DataHandler* dh);

// -------- 

void DataHandler_readTXT(DataHandler* dh, const char* fname);
void DataHandler_addTXT(DataHandler* dh, const char* fname, const char* srv, const char* usr);
void DataHandler_removeTXT(DataHandler* dh, const char* fname);

char* DataHandler_getFilePath(const char* fname);
char* DataHandler_getDirePath(const char* dname);

// -------- DataHandler end -------- //

#endif // __DATAHANDLER_H

