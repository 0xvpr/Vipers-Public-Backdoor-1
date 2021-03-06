#!/usr/bin/python3

# This is a simple Caesar cypher encryption
# script that you can use to evade pre-runtime
# detection.

import os
import sys

def caesarCypher():
    """
    Incredibly simple Caesar cypher.
    It simply adds 1 to the ordinal value
    for every character in a user-defined
    string.

    @param: None
    @rtype: None
    """

    while True:
        string = input("enter a string: ")
        if string == 'q':
            print("'q' entered.\n\nTerminating program.")
            sys.exit(1)

        else:
            print(string, "-> ", end="")

            for char in string:
                print(chr(ord(char)+1), end="")
                
            print("\n")

if __name__ == "__main__":
    
    # Sets the cwd to the path of the script
    # for consistency running the script from
    # anywhere.
    path = os.path.dirname(__file__)
    os.chdir(os.path.abspath(path))

    print("Send 'q' or CTRL + C to kill the process\n")

    caesarCypher()
