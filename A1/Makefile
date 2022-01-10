CC=g++
SCAN=scanner
MYSCLP=mysclp
TESTDIR=tests

$(MYSCLP) : lex.yy.c parser.tab.h
	$(CC) lex.yy.c -include allheaders.h -L. -Wl,-rpath=. -o $(MYSCLP) -lsclpshared -lfl -ly 

lex.yy.c : $(SCAN).l parser.tab.h storetokens.hh
	lex $(SCAN).l

.PHONY: clean

clean:
	rm -f lex.yy.* $(MYSCLP)

testclean:
	rm -f $(TESTDIR)/*.c.*
