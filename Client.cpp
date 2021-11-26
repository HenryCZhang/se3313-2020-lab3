
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;
bool flag = true;

// This thread handles the connection to the server
class ClientThread : public Thread
{
private:
	// Reference to our connected socket
	Socket &socket;

	// Data to send to server
	ByteArray data;
	std::string data_str;

public:
	ClientThread(Socket &socket)
		: socket(socket)
	{
	}

	~ClientThread()
	{
	}

	virtual long ThreadMain()
	{
		bool serverNotReady = false;
		try
		{
			int result = socket.Open();
		}
		catch (...)
		{ //check if server is running
			std::cout << "Need to run server first!" << std::endl;
			serverNotReady = true;
			flag = false;
		}
		while (1)
		{
			if (serverNotReady)
			{
				break;
			}
			std::cout << "Please input your data (type 'done' to exit): "<< std::endl;
			std::cout.flush();

			// Get the data
			std::getline(std::cin, data_str);
			data = ByteArray(data_str);

			// Write to the server
			socket.Write(data);
			if (data_str == "done")
			{
				flag = false;
				break;
			}
			// Get the response
			socket.Read(data);
			data_str = data.ToString();
			std::cout << "SERVER RESPONSE: " << data_str << std::endl;
			if (data_str == "")
			{
				flag = false;
				break;
			}
		}
		std::cout << "Ending client" << std::endl;

		return 0;
	}
};

int main(void)
{
	// Welcome the user
	std::cout << "Hi, I'm a client" << std::endl;

	// Create our socket
	Socket socket("127.0.0.1", 1000);
	ClientThread clientThread(socket);
	while (flag)
	{
		sleep(1);
	}
	try
	{
		socket.Close();
		std::cout << "Socket closed" << std::endl;
	}
	catch (...)
	{
		std::cout << "Ending" << std::endl;
	}

	return 0;
}
