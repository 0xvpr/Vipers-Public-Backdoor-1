#!/usr/bin/python3

from os import system

compiler = "/bin/x86_64-w64-mingw32-windres"

print("Generating .res files")
system(compiler + " -O coff icon.rc -o ../out/icon.res")
system(compiler + " -O coff version.rc -o ../out/version.res")

print("success fully generated.")
