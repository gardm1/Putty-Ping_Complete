#include "dataHandler.h"

void DataHandler_con(DataHandler* dh) {
    dh->emu_amo = EMULATOR_AMOUNT;
    dh->ses_amo = SESSIONS_AMOUNT;
    dh->ses_len = MAX_SESSION_VAR_LEN;

    dh->lusr = (char*)malloc(dh->ses_len);
    dh->lsrv = (char*)malloc(dh->ses_len);

    dh->emulators[0] = _strdup("putty.exe");
    dh->emulators[1] = _strdup("plink.exe");

    for (int i = 0; i < dh->ses_amo; i++) {
        dh->sessions[i] = NULL;
    }
}

void DataHandler_decon(DataHandler* dh) {
    free(dh->lusr);
    free(dh->lsrv);

    for (int i = 0; i < dh->emu_amo; i++) {
        free(dh->emulators[i]);
    }

    for (int i = 0; i < dh->ses_amo; i++) {
        if (dh->sessions[i] != NULL) {
            free(dh->sessions[i]);
        }
    }
}

const char* DataHandler_getSrv(DataHandler* dh) {
    return dh->lsrv;
}

const char* DataHandler_getUsr(DataHandler* dh) {
    return dh->lusr;
}

const char* DataHandler_getEmulator(DataHandler* dh, int ilm) {
    if (ilm >= 0 && ilm < dh->emu_amo) {
        return dh->emulators[ilm];
    }
    return NULL;
}

// Error here --> something does not work
//char** DataHandler_getSessionNames(DataHandler* dh) {
//}

void DataHandler_readTXT(DataHandler* dh, const char* fname) {
    char* filePath = DataHandler_getFilePath(fname);

    FILE* fptr = fopen(filePath, "r");

    free(filePath);

    if (fptr == NULL) {
        printf("Was unable to open file (%ld).\ndh-readTXT\n", GetLastError());
        return;
    }

    fgets(dh->lusr, dh->ses_len, fptr);
    fgets(dh->lsrv, dh->ses_len, fptr);

    fclose(fptr);

    dh->lusr[strcspn(dh->lusr, "\n")] = '\0';
}

void DataHandler_addTXT(DataHandler* dh, const char* fname, const char* srv, const char* usr) {
    char* filePath = DataHandler_getFilePath(fname);
    char* direPath = DataHandler_getDirePath(fname);

    if (_mkdir(direPath) == -1) {
        printf("Directory already exists or failed to create (%ld).\ndh-addTXT\n", GetLastError());
        return;
    }

    FILE* fptr = fopen(filePath, "w");
    free(filePath);
    free(direPath);

    if (fptr == NULL) {
        printf("Was unable to write to file (%ld).\ndh-addTXT\n", GetLastError());
        return;
    }

    fprintf(fptr, "%s\n%s", usr, srv);
    fclose(fptr);
}

void DataHandler_removeTXT(DataHandler* dh, const char* fname) {
    char* direPath = DataHandler_getDirePath(fname);
    char* filePath = DataHandler_getFilePath(fname);

    if (_unlink(filePath) != 0) {
        printf("Was unable to delete file (%ld).\n\"%s\"\ndh-removeTXT\n", GetLastError(), filePath);
        free(filePath);
        free(direPath);
        return;
    }

    if (_rmdir(direPath) != 0) {
        printf("Was unable to delte directory (%ld).\n\"%s\"\ndh-removeTXT\n", GetLastError(), direPath);
        free(filePath);
        free(direPath);
        return;
    }

    free(filePath);
    free(direPath);
    //(void*)DataHandler_getSessionNames(dh);
}

char* DataHandler_getFilePath(const char* fname) {
    size_t len = strlen(BASEDIR) + strlen(fname) + strlen(fname) + 7; // +7 --> \\ + .txt + \0
    char* path = (char*)malloc(len);

    snprintf(path, len, "%s%s\\%s.txt", BASEDIR, fname, fname);  // "%s\\%s"
    return path;
}

char* DataHandler_getDirePath(const char* dname) {
    size_t len = strlen(BASEDIR) + strlen(dname) + 1; // +1 --> \0
    char* path = (char*)malloc(len);

    snprintf(path, len, "%s%s", BASEDIR, dname); // "%s\\%s"
    return path;
}