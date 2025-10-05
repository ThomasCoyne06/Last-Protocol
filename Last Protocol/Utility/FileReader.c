#include "FileReader.h"

void SkipLinesUntilCharacter(FILE* const _file, char _charTarget)
{
	char line[100];
	while (fgets(line, sizeof(line), _file))
	{
		char* cursor = strchr(line, _charTarget);
		if (cursor == NULL)
		{
			continue; //This line does not contain an _signTarget, we ignore it
		}
		else
		{
			int offset = strlen(line) + 1; //The previous condition has counted an extra line, it is necessary 
			fseek(_file, -offset, SEEK_CUR); //to recover and deduct by the number of characters from the same line
			break; //This line does contain an _signTarget, we stop while
		}
	}
}

sfBool ScanBoolean(FILE* const _file, const char* const _format)
{
	char boolean[6] = { '\0' };
	char text[50] = { '\0' };
	strcpy_s(text, sizeof(text), _format);
	strcat_s(text, sizeof(text), " = %5s\n");
	fscanf_s(_file, text, boolean, (unsigned)sizeof(boolean));
	return strcmp(boolean, "true") == 0;
}
