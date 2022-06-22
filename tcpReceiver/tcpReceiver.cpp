#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>

#include <unistd.h>
#include <syslog.h>
#include <dirent.h>
#include <netinet/in.h>

using namespace std;

int socketDesc;

char *getTime()
{
    time_t now = time(0);
    char *date_time = ctime(&now);
    return date_time;
}

int SaveToFile(char const *text, char *fileName)
{

    fstream fileStream(fileName, fstream::app);
    if (fileStream.fail())
    {
        throw string("No such file");
    }
    fileStream << endl
               << getTime() << endl
               << text;
    fileStream.close();
    return 0;
}

int Bind(string port = "69999")
{
    int socketDesc;
    sockaddr_in addrSpec;
    socketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDesc == -1)
        return -1;

    addrSpec.sin_family = AF_INET;
    addrSpec.sin_addr.s_addr = INADDR_ANY;
    addrSpec.sin_port = htons(stoi(port));

    int bindError = bind(socketDesc, (struct sockaddr *)&addrSpec, sizeof(addrSpec));
    if (bindError == -1)
        return -1;

    return socketDesc;
}

int Listen(int socketDesc, char *filePath)
{
    int listen_return = listen(socketDesc, 3), newSocket, adrrSize = sizeof(sockaddr);
    char rcvBuff[1024] = {0};
    sockaddr address;
    if (listen_return == -1)
        return -1;
    while (true && socketDesc != -1)
    {
        newSocket = accept(socketDesc, (struct sockaddr *)&address, (socklen_t *)&adrrSize);
        int rcvCount = read(newSocket, rcvBuff, 1024);
        cout << rcvCount << " : " << rcvBuff << endl;
        SaveToFile(rcvBuff, filePath);
    }

    return true;
}
void StopReceiver(int signum)
{
    if (signum == SIGTERM || signum == SIGHUP)
    {
        // SaveToFile((to_string(signum)).c_str(), "text.txt");
        close(socketDesc);
        socketDesc = -1;
        exit(0);
    }
}
int StartService(string port, char *filePath = "text.txt")
{
    signal(SIGTERM, StopReceiver);
    signal(SIGHUP, StopReceiver);
    try
    {
        if ((socketDesc = Bind(port)) == -1)
        {
            cout << "Error binding port" << endl;
            return -1;
        }
        while (true)
        {

            try
            {
                Listen(socketDesc, filePath);
            }
            catch (exception &err)
            {
            }
        }
    }
    catch (exception &err)
    {
        std::cout << "Exception was caught:" << err.what() << "\nExiting.\n";
    }
    return 0;
}
void Daemon()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
}
int main(int argc, char *argv[])
{
    /*
ps -xj | grep main
fuser 69999/tcp
kill -15 7546
    */
    cout << "Start" << endl;
    if (argc < 2)
    {
        cout << "Usage: daemon [1]mode:-d for daemon -e for executable -s to stop [2]server_port [3]File path" << endl;
        // return 0;
    }
    string port = "626262";
    if (argc >= 3)
        port = string(argv[2]);
    if (argc > 1 && argv[1] == string("-d"))
    {
        cout << "Daemon mode" << endl;
        Daemon();
    }
    argc == 4 ? StartService(port, argv[3]) : StartService(port);

    return 0;
}