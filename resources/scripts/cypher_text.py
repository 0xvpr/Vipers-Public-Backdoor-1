#!/usr/bin/python3

while True:
    string = input("enter a string: ")
    print(string, "-> ", end="")

    for char in string:
        print(chr(ord(char)+1), end="")
        
    print("\n")