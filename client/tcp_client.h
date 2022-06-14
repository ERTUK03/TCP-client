#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sstream>
#include <fstream>
#include <cstdio>

using namespace std;
using namespace boost::asio;

class tcp_client
{
public:
	ip::tcp::socket socket;
	boost::system::error_code ec;
	char buffer[4 * 1024] = {};
	string message;
	bool flag = false;

	void receive();
	void send();
	tcp_client(io_context& context);
};

