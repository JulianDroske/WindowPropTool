# WindowPropTool
To set some properties of a window

---

---

```
# Compilation for GCC (MinGW)
gcc -std=c99 tool.c -o wpt.exe
```

---

```
# Usage:
wpt [sleep] <mouse|title=*> <prop=top|notop,transparent=[0~255]>
```

---

```
sleep: sleep for 3 secs
mouse: get handle from cursor
title=VAR: get handle from a window titled the same as VAR
```

---
```
top: set window always at the top
notop: remove "alwaysTop" prop
transparent=VAR: set transparent (VAR value from 0 to 255)
```
