CFLAGS+=-std=c99 -Wall -Werror
LDLIBS+=-lncurses

# Nom de votre executable
TARGET=splendor
# Liste des fichiers nécessaires à sa réalisation
OBJS=splendor.o ncurses.o

# Create dependencies while compiling
CFLAGS+=-MMD

$(TARGET): $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
clean-deps: clean
	$(RM) *.d

# Injecte les dependences si presentes
-include *.d

# Pour les exemples
examples/print_card: examples/print_card.o examples/ncurses.o
examples/print_splendor: examples/print_splendor.o examples/ncurses.o

.PHONY: clean clean-deps
