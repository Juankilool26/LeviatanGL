##################################
#MACROS#
##################################

# $(1) : Compiler
# $(2) : Object file to generate
# $(3) : Source file
# $(4) : Additional dependencies
# $(5) : Compiler flags

define COMPILE
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)

endef

# $(1) : Source file
# $(1) : src/tinyPTC/xbcde.cpp
# $(1) : obj/tinyPTC/xbcde.o

define C2O #Transformamos .c en .o y lo meto en el subdirectorio
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef

define C2H #Transformamos .c en .o y lo meto en el subdirectorio
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

#-fsanitize=address

#################################
#CONFIG#
#################################

APP		:= leviatanGL.a
CFLAGS  := -Wall -pedantic
CCFLAGS	:= $(CFLAGS) -std=c++17
CC		:= g++
C		:= gcc
AR		:= ar
ARFLAGS := -crs
RANLIB 	:= ranlib
MKDIR   := mkdir -p
SRC		:= src
OBJ		:= obj
LIBDIR  := lib
INCDIRS	:= -I$(SRC) -I$(LIBDIR)
RM		:= rm

ifdef DEBUG
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif

ALLCPPS		:= $(shell find $(SRC) -type f -iname *.cpp) #buscar todos los .cpp
ALLCS		:= $(shell find $(SRC) -type f -iname *.c) #buscar todos los .c
ALLOBJ		:= $(foreach F,$(ALLCPPS) $(ALLCS), $(call C2O, $(F))) #converitr los .c en .o
SUBDIRS		:= $(shell find $(SRC) -type d)
OBJSUBDIRS	:=$(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

.PHONY: info libs libs-clean libs-cleanall clean cleanall

#generar libreria
$(APP) : $(OBJSUBDIRS) $(ALLOBJ) #para que se genere la aplicacion primero hay que generear los directorios y despues los .o #ponemos os flags de libreria donde linkamos 
	$(AR) $(ARFLAGS) $(APP) $(ALLOBJ)
	$(RANLIB) $(APP)

#generate rules for all objects ( las reglas son las que deciden si un fichero se vuelve a generar o no)
$(foreach F, $(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS) $(INCDIRS))))
$(foreach F, $(ALLCS),$(eval $(call COMPILE,$(C),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CFLAGS) $(INCDIRS))))

#$(OBJ)%.o : $(SRC)%.c #Cualquier cosa .o depende de cualquier cosa .c
#	$(C) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CFLAGS)


#$(OBJ)/%.o : $(SRC)/%.cpp #Cualquier cosa .o depende de cualquier cosa .c
#	$(CC) -o $(patsubst $(SRC)%,$(OBJ)%,$@) -c $^ $(CCFLAGS)

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCS))
	$(info $(ALLOBJ))


$(OBJSUBDIRS): #Solo tendría que crear lo que falte no todos cuando falte 1
	$(MKDIR) $(OBJSUBDIRS)


clean:
	$(RM) -r "./$(OBJ)"

cleanall : clean
	$(RM) "./$(APP)"



## LIB rules
libs: 
	$(MAKE) -C $(LIBDIR)

libs-clean:
	$(RM) "./$(APP)"

libs-cleanall:
	$(MAKE) -C $(LIBDIR) cleanall
