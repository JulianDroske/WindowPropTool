# WindowPropTool
A tool for setting the properties of a window

---

---

# Compiling Commandline For GCC (MinGW)
gcc -std=c99 tool.c -o jurt.exe

---

# Usage:
jurt [sleep] [mouse|title=*] <prop=top|notop,transparent=[0~255]>
"prop" & "mouse | title" option must be decleared

---

sleep: sleep for 3 secs
mouse: get handle from cursor
title=VAR: get handle from a "title-matched-VAR" window

---

top: set window always the top
notop: remove "alwaysTop" prop
transparent=VAR: set transparent (VAR value from 0 to 255)
