# Output executable name (REQUIRED: myshell)
TARGET = myshell
CC     = gcc
# Removed -O2 so you can debug variables clearly
CFLAGS = -g -Wall -Wextra -std=c11
LDFLAGS=
LDLIBS =

# EXPLICITLY list your files here.
# As you add parse.c or execute.c, add them to this list.
SRCS = main.c

# Automagic setup (Don't touch below)
OBJS := $(SRCS:%.c=%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS) myshell.log