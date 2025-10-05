#ifndef FILE_READER_H
#define FILE_READER_H

#include "Common.h"

void SkipLinesUntilCharacter(FILE* const _file, char _charTarget);
sfBool ScanBoolean(FILE* const _file, const char* const _format);
#endif