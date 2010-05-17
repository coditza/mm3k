PROJECT=mm3k
PROJECTDIR=../../
OUTDIR=$(PROJECTDIR)bin/linux/
TMPDIR=$(PROJECTDIR)temp/linux/

SRCDIR=$(PROJECTDIR)src/

OBJECTS=$(patsubst $(SRCDIR)%.cpp,$(TMPDIR)%.o,$(wildcard $(SRCDIR)*.cpp))

WXCXXFLAGS=
WXLFLAGS=

CC = g++
CFLAGS = 

dirs:
	@echo "Making directories"
	mkdir -p $(OUTDIR)
	mkdir -p $(TMPDIR)

res: 	dirs
	@echo "Copying resources"
	cp $(PROJECTDIR)res/*.bmp $(OUTDIR)	

clean:
	@echo "Cleaning the house"
	rm -rf $(OUTDIR)
	rm -rf $(TMPDIR)

$(OBJECTS): dirs
	$(CC) -c -o $@ $(CFLAGS) $(WXCXXFLAGS) $(SRCDIR)$(patsubst %.o,%.cpp,$(@F)) 

$(PROJECT): $(OBJECTS) res
	$(CC) -o $(OUTDIR)$(PROJECT) $(WXLFLAGS) $(OBJECTS)
	@echo "--- Build done ---"
	@echo "Executable generated in $(OUTDIR). Make sure to run it from that directory or it won't find the images."

