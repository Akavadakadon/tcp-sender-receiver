#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>

using namespace std;
#include <unistd.h>
#include <netdb.h>

vector<string> PrepAgrs(int argc, char *argv[])
{
    vector<string> args;
    args = vector<string>(argv, argv + argc);
    return args;
}

auto File(string fileName)
{
    struct result
    {
        string file;
        int size;
    };
    string file;
    fstream fileStream(fileName);
    if (fileStream.fail())
    {
        throw string("No such file");
    }
    fileStream.seekg(0, ios::end);
    int size = fileStream.tellg();
    if (size == 0)
    {
        throw string("File is empty");
    }
    fileStream.seekg(0, ios::beg);
    stringstream strStream;
    strStream << fileStream.rdbuf();
    file = string(strStream.str());
    fileStream.close();
    return result{file, size};
}
int Connect(string ip, string port)
{
    // port = "626262";
    //Соединение
    addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    auto result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
    if (result != 0)
        throw(string("Error getting destitation node. error ") + to_string(result));

    int sock = 0, socket_error;
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
        throw(string("Error opening socket"));
    socket_error = connect(sock, res->ai_addr, res->ai_addrlen);
    if (socket_error != 0)
        throw(string("Error opening connection"));
    return sock;
}
int Send(int socketDesc, string file)
{
    int n = send(socketDesc, file.c_str(), strlen(file.c_str()), 0);
    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage: sendfile [1]server_ip [2]server_port [3]file_path" << endl;
        // return 0;
    }
    if (argc != 4)
    {
        argv = new char *[4];
        argv[0] = "asda";
        argv[1] = "127.0.0.1";
        argv[2] = "626262";
        argv[3] = "text.txt";
        argc = 4;
    }
    std::vector<std::string> args = PrepAgrs(argc, argv);
    string ip = args[1], port = args[2], file;
    int size;
    try
    {
        auto res = File(args[3]);
        file = res.file;
        size = res.size;
    }
    catch (string e)
    {
        cout << e << endl;
        return 0;
    }

    int socketDesc;
    try
    {
        socketDesc = Connect(ip, port);
    }
    catch (string e)
    {
        cout << e << endl;
        return 0;
    }

    if (Send(socketDesc, file) == -1)
        cout << "Sending error" << endl;

    close(socketDesc);
    return 0;
}