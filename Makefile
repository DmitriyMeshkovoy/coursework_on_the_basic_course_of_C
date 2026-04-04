all: temperature_app

temperature_app: main.c temp_api.c temp_api.h
	gcc -Wall -Wextra -std=c99 -g main.c temp_api.c -o temperature_app

clean:
	rm -f temperature_app

run: temperature_app
	./temperature_app

rebuild: clean all