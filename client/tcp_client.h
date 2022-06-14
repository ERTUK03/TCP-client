#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sstream>
#include <fstream>

class tcp_client
{
public:
	boost::asio::ip::tcp::socket socket;
	boost::system::error_code ec;
	char buffer[16 * 1024] = {};
	std::string message;
	bool flag = false;

	void receive()
	{
		message = "";
		size_t len = socket.read_some(boost::asio::buffer(buffer));
		for (int i = 0; i < len; i++) message += buffer[i];
		std::cout << message << "\n";
		if (message == "Failed attempt to connect" || message == "Disconnected")
		{
			flag = true;
			throw "ERROR";
		}
	}

	void send()
	{
		message = "";
		std::cout << ">";
		std::getline(std::cin, message);
		std::cin.clear();
		if (message.size() == 0)
		{
			send();
			return;
		}
		socket.write_some(boost::asio::buffer(message.data(), message.size()));
		std::vector<std::string> words;
		std::string word;
		std::stringstream ss(message);
		while (ss >> word)	words.push_back(word);
		if (words.size() > 0)
		{
			if (words[0] == "getFile" && words.size() == 2)
			{
				std::string filename;
				std::cout << "Set filename: ";
				std::getline(std::cin, filename);
				std::cin.clear();
				std::ofstream file(filename, std::ios::out | std::ios::binary);
				std::string receivedMessage;
				for (;;)
				{
					receivedMessage = "";
					size_t len = socket.read_some(boost::asio::buffer(buffer));
					for (int i = 1; i < len; i++)
					{
						receivedMessage += buffer[i];
					}
					if (receivedMessage == "An error occured while opening the file")
					{
						std::cout << receivedMessage << "\n";
						send();
						return;
					}
					file << receivedMessage;
					if (buffer[0] == 'Y') break;
				}
				std::cout << "Operation completed\n";
				file.close();
				send();
			}
		}
	}

	tcp_client(boost::asio::io_context& context) : socket{ context }
	{
		std::string adr;
		std::cout << "Enter address you wish to connect to: ";
		std::getline(std::cin, adr);
		boost::asio::ip::tcp::endpoint endpoint;
		try
		{
			endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(adr), 1010);
		}
		catch (...)
		{
			std::cout << "Invalid address";
			return;
		}
		socket.connect(endpoint, ec);
		if (ec)
		{
			std::cout << "Couldn't connect";
			return;
		}
		for (;;)
		{
			try
			{
				receive();
				send();
			}
			catch (...)
			{
				if (!flag) std::cout << "Error occured. Exiting";
				return;
			}
		}
	}
};

