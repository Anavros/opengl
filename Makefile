
TARGET:= testing
C_SRCS:= testing.c
C_OBJS:= testing.o
CFLAGS:= -Wall -fdiagnostics-color=auto `pkg-config --cflags glfw3`
LGLFW3:= `pkg-config --libs glfw3`
.PHONY: all clean run

all: $(C_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(C_OBJS) $(LGLFW3)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(C_OBJS)

run: all
	./$(TARGET)
