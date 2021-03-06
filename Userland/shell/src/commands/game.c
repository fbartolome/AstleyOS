#include <commands.h>
#include <stdio.h>
#include <console.h>
#include <libDraw.h>
#include <snake.h>
#include <snake_input.h>
#include <ipc.h>
#include <songs.h>
#include <strings.h>


extern void sysvideo_mode(void);
extern void syspaint_pixel(int x, int y, int blue_c, int green_c, int red_c);
void paintPixel(int x, int y, int blue_c, int green_c, int red_c);
extern void sys_sound(int frequency, double time);
extern int sys_new_process(const char* name, uint64_t func, uint64_t argc, void* argv);
extern void sys_send_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message);
extern read_msg sys_read_message(uint64_t pid);
extern int sys_waitpid(int pid);
void playRick(void);

COMMAND_FUNCTION(game);
COMMAND_HELP(game);

COMMAND_FUNCTION(game) {
	sysvideo_mode();
	//playRick();
	uint64_t game_pid = sys_new_process("snake", (uint64_t) &startGame, 0, 0);
	uint64_t control_pid = sys_new_process("snake_input", (uint64_t) &snake_input, 0, 0);
	
	//le mando el pid del juego al proceso que controla las teclas
	sys_send_message(game_pid, control_pid, 1, (void*)game_pid);
	
	if(argc > 0){
		if(strcmp("mute",argv[0]) == 0){
			sys_waitpid(game_pid);
			return OK;
		}
	}
	sys_new_process("game_music", (uint64_t) &playRick, 0, 0);
	sys_waitpid(game_pid);
	return OK;
}

COMMAND_HELP(game) {
	if(argc >= 1) {
		printf("<game> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Runs the snake game while listening to music God Rick Astley's Never Gonna Give You Up.\nTo play mute, first argument should be mute\n");
	printf("Usage: 'game'\n");

	return OK;
}

void paintPixel(int x, int y, int blue_c, int green_c, int red_c){
	syspaint_pixel(x,y,blue_c,green_c,red_c);	
}

void playRick(){
	while(1){
		play_song(RICK);
	}
}
