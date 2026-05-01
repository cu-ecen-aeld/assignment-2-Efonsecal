#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

int recordError(char * errorMessage);

int main(int argc, char ** argv) {

    char * openFileErrMsg = "File couldn't be opened";
    char * writeFileErrMsg = "Error writing to file";
    char * closeFileErrMsg = "Error closing the file";
    openlog("[File Writer application]", 0, LOG_USER);

    if (argc < 3) {
        char errormsg [50];
        sprintf(errormsg, "Expected 2 arguments but got: %d", (argc - 1));
        syslog(LOG_ERR, "%s", errormsg);
        errno = EINVAL;
        perror(errormsg);
        return 1;
    }
    char * path = *(argv + 1);
    char * text = *(argv + 2);

    // Access file
    FILE * fileptr = fopen(path, "w+");
    
    if (fileptr == NULL) return recordError(openFileErrMsg);

    // Write to file
    syslog(LOG_DEBUG, "Writing \"%s\" to \"%s\"", text, path);

    size_t textLen = strlen(text);
    size_t writtenBytes = fwrite(text, 1, strlen(text), fileptr);

    if (writtenBytes < textLen) return recordError(writeFileErrMsg);

    // Close file
    if (fclose(fileptr) != 0) return recordError(closeFileErrMsg);

    return 0;
}

int recordError(char * errorMessage) {
    syslog(LOG_ERR, "%s\n", errorMessage);
    perror(errorMessage);
    return 1;
}