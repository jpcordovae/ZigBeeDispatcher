CC=g++
CXXFLAGS += -Wall -g -std=c++0x -I/usr/include/libxml2 -I./lua-5.1.4/etc -I./lua-5.1.4/src
LDFLAGS += -lxml2 -lboost_thread -lboost_system

OBJECTS= CDispatcher.o DomBase.o cenddevice.o cevent.o common.o czbenddevice.o czbframe.o czblua.o serialport.o liblua.a serial_manager.o czblog.o

MAIN_SOURCES= main.o

all: dispatcher
          
dispatcher: ${OBJECTS} ${MAIN_SOURCES}
	${CC} $(LDFLAGS) ${OBJECTS} ${MAIN_SOURCES} -lm -o $(@)

clean:
	rm *.o dispatcher *~
