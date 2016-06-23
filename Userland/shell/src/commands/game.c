#include <commands.h>
#include <stdio.h>
#include <console.h>

extern void sysvideo_mode(void);

COMMAND_FUNCTION(game);
COMMAND_HELP(game);

COMMAND_FUNCTION(game) {
	sysvideo_mode();
	//startGame();

	return OK;
}

COMMAND_HELP(game) {
	if(args.argc >= 1) {
		printf("<game> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Runs the snake game. Ignores arguments.\n");
	printf("Usage: 'game'\n");

	return OK;
}