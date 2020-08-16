#/bin/sh

TOP_PATH = $(shell pwd)

SRCPTH = $(TOP_PATH)
INPTH = $(TOP_PATH)

SDK_PATH = ./

CXX  = g++
INLPTH = -g -Wall -O3 #-fgnu89-inline
INLPTH += -I$(INPTH)/include/      \
		  -I$(INPTH)/include/hisi/  \     		
	   
LPTH :=  -lpthread  -ldl	
OBJ  = $(SDK_PATH)obj  

TARG = mapp
DIRS = $(SRCPTH) audio common network plat record video

FILES = $(foreach dir, $(DIRS),  $(wildcard $(dir)/*.cpp))
OBJS  = $(patsubst %.cpp, %.o, $(FILES))

all:$(TARG)

#链接
$(TARG):$(OBJS)
	$(CXX) -o $(TARG)  $^ $(LPTH)   

#编译
.cpp.o:
	$(CXX) -c $(INLPTH) $< -o $@    
	
clean:
	rm -rf $(OBJS) 
	rm -rf $(TARG)
