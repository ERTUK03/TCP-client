#include "tcp_client.h"

void tcp_client::receive()
{
	message = "";
	size_t len = socket.read_some(boost::asio::buffer(buffer));
	for (int i = 0; i < len; i++) message += buffer[i];
	cout << message << "\n";
	if (message == "Failed attempt to connect" || message == "Disconnected")
	{
		flag = true;
		throw "ERROR";
	}
}

void tcp_client::send()
{
	message = "";
	cout << ">";
	getline(cin, message);
	cin.clear();
	if (message.size() == 0)
	{
		send();
		return;
	}
	socket.write_some(boost::asio::buffer(message.data(), message.size()));
	vector<string> words;
	string word;
	stringstream ss(message);
	while (ss >> word)	words.push_back(word);
	if (words.size() > 0)
	{
		if (words[0] == "getFile" && words.size() == 2)
		{
			string filename;
			cout << "Set filename: ";
			getline(cin, filename);
			cin.clear();
			ofstream file(filename, ios::out | ios::binary);
			string receivedMessage;
			for (;;)
			{
				receivedMessage = "";
				size_t len = socket.read_some(boost::asio::buffer(buffer));
				for (int i = 1; i < len; i++) receivedMessage += buffer[i];
				if (receivedMessage == "An error occured while opening the file")
				{
					cout << receivedMessage << "\n";
					send();
					return;
				}
				file << receivedMessage;
				if (buffer[0] == 'Y') break;
			}
			cout << "Operation completed\n";
			file.close();
			send();
		}
	}
}

tcp_client::tcp_client(io_context& context) : socket{ context }
{
	string adr;
	string port;
	cout << "Enter address you wish to connect to: ";
	getline(cin, adr);
	cout << "Enter port: ";
	getline(cin, port);
	ip::tcp::endpoint endpoint;
	try
	{
		endpoint = ip::tcp::endpoint(ip::make_address(adr), stoi(port));
	}
	catch (...)
	{
		cout << "Invalid address\n";
		return;
	}
	socket.connect(endpoint, ec);
	if (ec)
	{
		cout << "Couldn't connect\n";
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
			if (!flag) cout << "Error occured. Exiting";
			return;
		}
	}
}
