# TCP client
Just a basic tcp-based client, made with boost::asio.

# Requirements
-C++17

-Boost/asio

# Description
It's made to work with a tcp server I made https://github.com/ERTUK03/TCP-server. Even though ftp functionality works with earlier mentioned server it doesn't mean
it will work with any ftp server(TL;DR it won't).

FTP works by sending file on main socket so while the transfer lasts no communication is possible(maybe I'll make something more advanced in the future).

# Commands supported
1. getUsers - Shows client a list of server clients

2. getFile -  Sends client the file specified in command. SYNTAX getFile [filename.extension]

3. getFiles - Shows client a list of files in shared folder

4. quit - Disconnects client from server

5. help - Shows client a list of commands
