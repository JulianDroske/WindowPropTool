# WindowPropTool
A tool for setting the properties of a window

---

---

# Compiling Commandline For GCC (MinGW)
gcc -std=c99 tool.c -o jurt.exe

---

# Usage:
jurt [sleep] [mouse|title=*] <prop=top|notop,transparent=[0~255]>
<br/>
"prop" & "mouse | title" option must be decleared

---

sleep: sleep for 3 secs
<br/>
mouse: get handle from cursor
<br/>
title=VAR: get handle from a "title-matched-VAR" window

---

top: set window always the top
<br/>
notop: remove "alwaysTop" prop
<br/>
transparent=VAR: set transparent (VAR value from 0 to 255)
