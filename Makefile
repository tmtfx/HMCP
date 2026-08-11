# Nome dell'eseguibile finale
NAME = HaikuMCPClient

# Compilatore e flag di compilazione
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Percorsi degli Header (-I)
# Include sia 'headers' (per risolvere <os/ai/...>) sia le sottocartelle
INCLUDES = -I/boot/system/develop/headers -I/boot/system/develop/headers/os/ai -I/boot/home/config/non-packaged/headers/os/ai

# Librerie da linkare (-l) e relativi percorsi (-L)
# Nota: -lbe è fondamentale su Haiku per Application/Interface/Layout Kit
LIBS = -lbe -lai_api
LIBPATHS = -L. -L/boot/home/config/non-packaged/lib

# Elenco dei file sorgente e degli oggetti
SRCS = hmcp.cpp MainWindow.cpp
OBJS = $(SRCS:.cpp=.o)

# Regola principale (Default)
all: $(NAME)

# Regola per fare il link dell'eseguibile
$(NAME): $(OBJS)
	@echo "[LINK] $@"
	@$(CXX) $(OBJS) $(LIBPATHS) $(LIBS) -o $(NAME)
	@# Imposta il flag di Haiku per identificare l'eseguibile come applicazione con GUI
	@xres -o $(NAME)
	@mimeset -f $(NAME)

# Regola generica per compilare i file .cpp in .o
%.o: %.cpp
	@echo "[CXX] $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Pulizia dei file generati
clean:
	@echo "[CLEAN]"
	@rm -f $(OBJS) $(NAME)

# Evita conflitti con file omonimi nella directory
.PHONY: all clean
