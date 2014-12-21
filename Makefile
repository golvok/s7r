.SUFFIXES:
.PHONY: build

build: s7r

LINK_FLAGS = \
	$(shell pkg-config --libs gtk+-3.0) \
	$(shell pkg-config --libs cairo)

INCLUDE_FLAGS = \
	$(shell pkg-config --cflags gtk+-3.0) \
	$(shell pkg-config --cflags cairo) \
	-I .

CFLAGS += -Wall -Wextra -pedantic -Weffc++ -Werror -std=c++11

s7r: main.o draw/window.o sim/particle.o sim/sim.o sim/mover.o util/geometry.o
	g++ $^ -o $@ $(LINK_FLAGS)

%.o: %.c++ %.h
	g++ $(CFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

%.o: %.c++
	g++ $(CFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)
