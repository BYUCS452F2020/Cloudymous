#ifndef SERVER_H   /* Include guard */
#define SERVER_H 
#include "AnnonServices.h"
#include "../shared/requests.h"
#include "../shared/responses.h"

void Init();

int Handle(Request request, Response* response);

#endif

