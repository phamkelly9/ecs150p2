# Target library
lib := libuthread.a README.html
objs := context.o preempt.o queue.o sem.o uthread.o

CC	:= gcc
CFLAGS := -Wall -Wextra -Werror -MMD
CFLAGS += -g
PANDOC := pandoc

all: $(lib) 

## TODO: Phase 1

# Conditional variables for different 
# behaviors
ifneq ($(V), 1)
Q = @
endif

#Dependency Tracking
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

libuthread.a: ar rcs libuthread.a $(objs)
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -o $@ $^

%.o: %.c private.h uthread.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c priv.h uthread.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o$@ $<

%.o: %.c queue.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o$@ $<

%.o: %.c queue.h sem.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o$@ $<

%.o: %.c priv.h uthread.h queue.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o$@ $<

%.html: %.md
	$(Q)$(PANDOC) -o $@ $<

clean: 
	$(Q)rm -f $(targets) $(objs) $(deps)