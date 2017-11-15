#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



int babbler(lua_State *L) {

	return 0;
}

int tokenizer() {

	return 0;
}

int shingler() {

	return 0;
}

int makeTable() {

	return 0;
}

char *readFile(char *filename) {
	// get the size of the file
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

	return buffer;
}

int main(int argc, char *argv[]) {
	//command line arg handling
	int n = 0;
	char filename;
	if (argc == 3) {
		n = *argv[2] - '0';
	} else {
		n = 3;
	}

	// setup
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//lua_pushinteger(L, n);
	
	// register the c functions
	lua_register(L, "cbabbler", babbler);
	//lua_register(L, "creadFile", readFile);
	lua_register(L, "ctokenizer", tokenizer);
	lua_register(L, "cshingler", shingler);
	lua_register(L, "cmakeTable", makeTable);

	// load the lua file
	if (luaL_dofile(L, "babbler.lua")) {
		printf("Error in dofile\n");
		return 1;
	}

	// call babble
	lua_getglobal(L, "driver");
	lua_pushstring(L, argv[1]);
	lua_pushnumber(L, n);
	lua_pcall(L, 2, 0, 0);

	// shut down
	lua_close(L);
	return 0;
}