#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int BUFFER_SIZE = 1000000;


int babbler(lua_State *L) {

	return 0;
}

int tokenizer(lua_State *L) {

	const char *text = lua_tostring(L, 1);
	int offset = lua_tonumber(L, 2);

	char token[BUFFER_SIZE];

	int j = 0;

	while ( text[offset] != NULL && !isspace(text[offset]) )
	{
		if( isalnum(text[offset]) )
		{
			token[j] = text[offset];
			token[j] = tolower(token[j]);
			j += 1;
		}

		offset += 1;
	}

	//printf("%c \n", text[offset]);

	//printf( "%s \n", token );

	lua_pushstring(L, token);
	lua_pushnumber(L, offset);

	for (int i = 0; i < j; i++)
	{
		token[i] = 0;
	}

	return 2;
}

void stringRemoveNonAlphaNum(char *str)
{
    unsigned long i = 0;
    unsigned long j = 0;
    char c;

    while ((c = str[i++]) != '\0')
    {
        if (isalnum(c))
        {
            str[j++] = c;
        }
    }
    str[j] = '\0';

    //printf(str);
	//    return str;
}

int readFile(lua_State *L) {
	// get the size of the file

	char *filename = lua_tostring(L, 1);

	struct stat info;
	int status = stat(filename, &info);
	if (status < 0) {
		perror("stat error");
		exit(1);
	}

	// get a buffer of the appropriate size
	int size = (int) info.st_size;
	char *buffer = malloc(size + 1);
	if (buffer == NULL) {
		fprintf(stderr, "malloc error\n");
		exit(1);
	}

	// open the file
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("open error");
		exit(1);
	}

	// read the entire file
	char *ptr = buffer;
	int left = size;
	while (left > 0) {
		int chunk = read(fd, ptr, left);
		if (chunk < 0) {
			perror("read error");
			exit(1);
		} else if (chunk == 0) {
			fprintf(stderr, "ran out of data\n");
			exit(1);
		}
		left -= chunk;
		ptr += chunk;
	}

	// terminate the string with a null
	*ptr = 0;

	// close the file
	status = close(fd);
	if (status < 0) {
		perror("close error");
		exit(1);
	}

	lua_pushstring(L, buffer);
	return 1;
}

int main(int argc, char *argv[]) {
	//command line arg handling
	int n = 0;
	int amount = 0;
	char filename;

	if (argc == 4) {
		n = *argv[3] - '0';
	} else {
		n = 3;
	}

	amount = atoi(argv[2]);

	//printf("%d", amount);


	// setup
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	
	// register the c functions
	lua_register(L, "cbabbler", babbler);
	lua_register(L, "creadFile", readFile);
	lua_register(L, "ctokenizer", tokenizer);

	// load the lua file
	if (luaL_dofile(L, "babbler.lua")) {
		printf("Error in dofile\n");
		return 1;
	}

	// call babble
	lua_getglobal(L, "driver");
	lua_pushstring(L, argv[1]);
	lua_pushnumber(L, amount);
	lua_pushnumber(L, n);
	lua_pcall(L, 3, 0, 0);

	// shut down
	lua_close(L);
	return 0;
}