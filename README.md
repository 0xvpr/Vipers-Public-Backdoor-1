# Viper's Public Backdoor (v1.1)
An Intermediate level backdoor that utilizes HTTP/TCP sockets,  
Caesar encryption, api.smsglobal.com (for SMS), &amp; more.  

### Table of Contents
- [Demonstration](#demonstration)
- [How it works](#how-it-works)
- [Building It Yourself](#building-it-yourself)

### Demonstration
embed YouTube or place gif.

### How it works
**TL;DR**  
The file moves itself, renames itself (as backdoor.exe), hides itself, and re-executes itself  
inside of the victim's <code>AppData</code> folder.

**Detailed explanation**  
The application is a Trojan that moves itself into the users <code>AppData</code> folder and sets its  
attribute to hidden. The file then pings a user-defined host server that holds the malicious  
payload and downloads it. In my case this file was the <code>Nishang Invoke-PowershellTcp.ps1</code>  
You can set this to whatever you want obviously. Once the Payload is downloaded, the  
application then attempts to run it with a reverse shell to another user-defined listening  
server (In this case, defaulted to port 8080).  

If successful, the listening receiver will have a Powershell like environment to do  
whatever the receiver pleases with the victim's pc.

### Building It Yourself
If you compile this code as is, it IS NOT malicious in any way shape or form. In fact, the  
program will run for about 5 minutes before aborting. You will need to look around the source  
code to make things work in your own endeavors.  

Things to look into changing include:
 - User-defined hostname that hosts the payload.
 - User-defined hostname that listens for the activation signal.
 - The actual payload that is hosted itself.

The basis of the build command I used with the GCC compiler was:  
<code>g++ ./src/main.c -lws2_32 -o vipers_public_backdoor.exe</code>
