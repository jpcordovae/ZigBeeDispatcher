CC=g++
CXXFLAGS+=-W -Wall -g -std=c++0x -I./db/include
LDFLAGS+=-lxml2 -lboost_thread -lboost_system -L/usr/lib64/mysql -L./db/lib -lmysqlcppconn

OBJECTS=czblog.o czbdb.o 
MAIN_SOURCES=main.cpp

all: dispatcher

dispatcher: ${OBJECTS} ${MAIN_SOURCES}
	${CC} ${CXXFLAGS} $(LDFLAGS) ${OBJECTS} ${MAIN_SOURCES} -lm -o $(@)

czbdb.o:
	${CC} -c -fPIC czbdb.cpp -I./db/include -o $@ 

#serialport.o:
#	g++ serialport. -c -o $@

#configuration
#czbconfiguration.o:
#	g++ -I/usr/include/libxml2 czbconfiguration.cpp -c -o $@

clean:
	rm *.o dispatcher

