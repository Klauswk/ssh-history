
all: prepare move_bin
	+$(MAKE) -C src

.PHONY: clean
clean :
	rm -f target/*.o target/*.exe
	
.PHONY: prepare
prepare :
	mkdir -p target

.PHONY: move_bin
move_bin :
	cp -r bin/ target/bin/ && cp sqlite3.dll target/ 
