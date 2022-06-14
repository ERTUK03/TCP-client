#include "tcp_client.h"

int main()
{
	boost::asio::io_context context;
	tcp_client client(context);
	context.run();
}

