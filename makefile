all: scheduler
scheduler: scheduler.c 
	gcc scheduler.c -o scheduler
clean: 
	rm scheduler