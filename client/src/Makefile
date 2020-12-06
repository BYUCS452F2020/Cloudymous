# Refer to https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
IDIR =../include
CC = gcc
CFLAGS = -g -I $(IDIR) -lssl -lcrypto -lsqlite3

ODIR=obj

_DEPS = print-menu.h signin-view.h signin-presenter.h register-view.h register-presenter.h ../../shared/requests.h cloudy-crypto.h Server.h AnnonServices.h client-communicator.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = signin-view.o signin-presenter.o register-view.o register-presenter.o main.o cloudy-crypto.o Server.o AnnonServices.o client-communicator.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cloudymous: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~