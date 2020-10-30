FakeClient: clean FakeClient.o
	gcc -o FakeClient FakeClient.o Server.o AnnonServices.o -lsqlite3

FakeClient.o : Server.o
	gcc FakeClient.c -o FakeClient.o -c

Server.o: AnnonServices.o
	gcc Server.c -o Server.o -c

AnnonServices.o: 
	gcc AnnonServices.c -o AnnonServices.o -c


	

clean: 
	rm -f FakeClient AnnonServices.o Server.o FakeClient.o
 
 
