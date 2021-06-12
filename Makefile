################################################################################################################################################
###        In this Makefile made by taka, you can use as                                                               	                     ###
###        $ make                     # You can get the executable file which written in TARGET.                                             ###
###        $ make target=hoge         # You can get the executable file which written in hoge.                                               ###
###        $ make clean               # The executable file which written in TARGET will removed.                                            ###
###        $ make clean target=hoge   # The executable file which written in hoge will removed.                                              ###
################################################################################################################################################
TARGET=$(MAKECMDGOALS)
ifeq ($(MAKECMDGOALS),)
	TARGET=read
endif
ifeq ($(MAKECMDGOALS),clean)
	TARGET=read
endif

ifeq ($(TARGET),read)
	SOURCE_MAIN = readmovfile.cpp
	CXXFLAGS = -DREAD_IS_MAIN -I /usr/include/
	LDFLAGS += `pkg-config --cflags --libs opencv` -L /usr/lib
endif


PROGRAM = $(SOURCE_MAIN:%.cpp=%.out)
SUBOBJ = $(SOURCE_SUB:%.cpp=%.o)


read : $(PROGRAM)
all: $(PROGRAM)

%.out: %.o $(SUBOBJ)
	g++ -o $@ $^ $(LDFLAGS) -w
%.o : %.cpp
	g++ -o $@ $< -c $(CXXFLAGS) -w
clean:
	rm -f *.o $(PROGRAM)
