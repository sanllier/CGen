all: cgen

OBJECTS = obj/cgen.o obj/cindivid.o \
          obj/cprobvec.o obj/pugixml.o \
          obj/mtrand.o
DFLAG =
CFLAG = -std=c++11 -Iinclude/ -O3
CC    = icpc

cgen: $(OBJECTS)
	@mkdir -p lib
	@echo "\033[30;1;41m "lib" dir created \033[0m"
	@ar rvs lib/CGen.a $(OBJECTS)
	@echo "\033[30;1;41m CGen builded successfully! \033[0m"
	@echo "\033[30;1;41m --> lib/CGen.a \033[0m\n"

obj/cgen.o: src/cgen.cpp
	@mkdir -p obj
	@$(CC) -c $(DFLAG) $(CFLAG) -openmp $^ -o $@
	@echo "\033[30;1;46m $@ - done \033[0m\n"

obj/cindivid.o: src/cindivid.cpp
	@mkdir -p obj
	@$(CC) -c $(DFLAG) $(CFLAG) $^ -o $@
	@echo "\033[30;1;46m $@ - done \033[0m\n"

obj/cprobvec.o: src/cprobvec.cpp
	@mkdir -p obj
	@$(CC) -c $(DFLAG) $(CFLAG) $^ -o $@
	@echo "\033[30;1;46m $@ - done \033[0m\n"

obj/pugixml.o: src/pugixml.cpp
	@mkdir -p obj
	@$(CC) -c $(CFLAG) $^ -o $@
	@echo "\033[30;1;46m $@ - done \033[0m\n"

obj/mtrand.o: src/mtrand.cpp
	@mkdir -p obj
	@$(CC) -c $(CFLAG) $^ -o $@
	@echo "\033[30;1;46m $@ - done \033[0m\n"

clean:
	rm -r -f lib
	rm -r -f obj