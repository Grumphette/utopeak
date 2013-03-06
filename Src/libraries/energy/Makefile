CC=gcc

OBJECTS = obj
INCLUDES = includes

ENERGY_LIB = energy_msr_snb.so
ENERGY_OBJ = $(OBJECTS)/energy_msr_snb.o
MSR_OBJ = $(OBJECTS)/msr.o

all: $(ENERGY_LIB)


$(ENERGY_LIB): $(MSR_OBJ) $(ENERGY_OBJ)
	$(CC) -o energy_msr_snb.so -shared $(OBJECTS)/energy_msr_snb.o $(OBJECTS)/msr.o -lm -lpthread

$(ENERGY_OBJ): energy_msr_snb.c
	mkdir -p $(OBJECTS)
	$(CC) -o $(OBJECTS)/energy_msr_snb.o -c energy_msr_snb.c -fPIC

$(MSR_OBJ): $(INCLUDES)/msr.c
	mkdir -p $(OBJECTS)
	$(CC) -o $(OBJECTS)/msr.o -c $(INCLUDES)/msr.c -fPIC

clean: 
	rm -rf $(OBJECTS)/*.o *.so

