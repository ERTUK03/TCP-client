#include "tcp_client.h"

int main()
{
	io_context context;
	for(;;) tcp_client client(context);
	context.run();
}

