.SUFFIXES:
.PHONY: build

build: s8n

LINK_FLAGS = \
	$(shell pkg-config --libs gtk+-3.0) \
	$(shell pkg-config --libs cairo)

INCLUDE_FLAGS = \
	$(shell pkg-config --cflags gtk+-3.0) \
	$(shell pkg-config --cflags cairo)

CFLAGS += -Wall -Wextra -pedantic -Weffc++ -Werror -std=c++11

s8n: main.o draw/window.o sim/particle.o sim/sim.o sim/mover.o
	g++ $^ -o $@ $(LINK_FLAGS)

%.o: %.c++ %.h
	g++ $(CFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

%.o: %.c++
	g++ $(CFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)
