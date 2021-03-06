#!/usr/bin/python3

import os
import sys

from os import system

compiler = "/bin/x86_64-w64-mingw32-windres"
icon_src = "../resources/src/icon.rc"
icon_dst = "../resources/out/icon.res"
vers_src = "../resources/src/version.rc"
vers_dst = "../resources/out/version.res"

def compileResources(icon_src, icon_dst, vers_src, vers_dst):
    """
    Compiles the icon.rc and version.rc files
    so that the executable will have version
    information and an icon.

    @param: icon_src, icon_dst, vers_src, vers_dst
    @rtype: None
    """

    exit_status = 0
    exit_status = system(compiler + " -Ocoff " + icon_src + " -o " + icon_dst)
    if exit_status != 0:
        print(f"Error code: {exit_status}\n\nFailed to compile icon.rc")
        sys.exit(1)

    exit_status = system(compiler + " -Ocoff " + vers_src + " -o " + vers_dst)
    if exit_status != 0:
        print(f"Error code: {exit_status}\n\nFailed to compile version.rc")
        sys.exit(1)
    

if __name__ == "__main__":

    # Sets the cwd to the path of the script
    # for consistency running the script from
    # anywhere.
    path = os.path.dirname(__file__)
    os.chdir(os.path.abspath(path))

    print("Generating resource files..")

    compileResources(icon_src, icon_dst, vers_src, vers_dst)

    print("Resources files Successfully generated.")
