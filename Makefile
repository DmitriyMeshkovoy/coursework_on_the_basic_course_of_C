all: temperature_app

temperature_app: main.c temp_functions.c temp_functions.h
	gcc -Wall -Wextra -std=c99 -g main.c temp_functions.c -o temperature_app

clean:
	rm -f temperature_app

run: temperature_app
	./temperature_app

rebuild: clean all