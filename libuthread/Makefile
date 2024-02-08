# Target library
lib := libuthread.a
objs := context.o preempt.o queue.o sem.o uthread.o

AR := ar rcs
CC	:= gcc
CFLAGS := -Wall -Wextra -Werror -MMD
CFLAGS += -g

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

$(lib): $(objs)
	@echo "CC $@"
	$(Q)$(AR) $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	$(Q)rm -f $(targets) $(objs) $(deps)