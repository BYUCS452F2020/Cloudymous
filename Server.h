#ifndef SERVER_H   /* Include guard */
#define SERVER_H 
#include "AnnonServices.h"
#include "requests.h"

void Init();

int Handle(Request request, char* response);

#endif

