
TARGET:= testing
C_SRCS:= testing.c
C_OBJS:= testing.o
CFLAGS:= -Wall -fdiagnostics-color=auto
.PHONY: all clean run

all:
	$(CC) $(CFLAGS) `pkg-config --cflags glfw3` -o $(TARGET) $(C_SRCS) `pkg-config --libs glfw3`

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(C_OBJS)

run: all
	./$(TARGET)
