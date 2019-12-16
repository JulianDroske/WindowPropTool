#include "stdio.h"
#include "malloc.h"
#include "windows.h"

int LWA_COLORKEY = 0x1;
int LWA_ALPHA = 0x2;

int mouse=0;
char* title=NULL;
char* prop=NULL;
int sleep=0;

/* props */
int top=HWND_NOTOPMOST;
int transparent=-1;


void printUsageExit(){
	printf("Usage:\tjurt [sleep] [mouse|title=*] <prop=top|notop,transparent=[0~255]>\n");
	exit(1);
}

int strl(char* a){
	int i=0;
	while(a[i]!='\0') ++i;
	return i;
}

void printA(char* str){
	if(str==NULL){
		printf("String NULL.\n");
		return;
	}
	int l = strl(str);
	printf("String ANSI:Hex : \n\t");
	for(int i=0;i<l;++i){
		printf("%c:%x\t",str[i],str[i]);
	}
	printf("\nString End.\n");
}

int strnp(char*a,char*b,int l){
	if(strl(a)>l || strl(b)>l) return 0;
	for(int i=0;i<l;++i){
		if(a[i] != b[i]) return 0;
	}
	return 1;
}

int strp(char* a,char* b){
	int l = strl(a);
	if(l != strl(b)) return 0;
	for(int i=0;i<l;++i){
		if(a[i] != b[i]) return 0;
	}
	return 1;
}

void strncp(char* tar,char* str,int l){
	for(int i=0;i<l;++i){
		tar[i] = str[i];
	}
	tar[l] = '\0';
}

void clear(char* a,int l){
	for(int i=0;i<l;++i){
		a[i] = '\0';
	}
}

int countChar(char* a,char ch){
	int n = strl(a);
	int c=0;
	for(int i=0;i<n;++i){
		if(a[i] == ch) ++c;
	}
	return c;
}

/**
	*@return
		int[0] = int;
		int[1] = char**;
	*/
int* split(char* a,char sp){
	int n = strl(a);
	int c = countChar(a,sp);
	if(c==0){
		char** tar = NULL;
		tar = (char**)malloc(sizeof(char*));
		tar[0] = (char*) malloc(strl(a)*sizeof(char));
		strncp(tar[0],a,strl(a));
		int retn[2] = {c,tar};
		return retn;
	}
	static char** tar = NULL;
	tar = (char**)malloc((c+1)*sizeof(char*));
	tar[0] = (char*) malloc(255*sizeof(char));
	clear(tar[0],255);
	int swt=0;
	for(int i=0,pos=0;i<n;++i){
		if(pos>254){
			tar[255] = '\0';
			break;
		}
		if(a[i]==sp){
			tar[++swt] = (char*) malloc(255*sizeof(char));
			clear(tar[swt],255);
			pos=0;
			continue;
		}
		tar[swt][pos] = a[i];
		pos++;
	}
	int retn[2] = {c,tar};
	return retn;
}

int splitFirst(char* a,char sp){
	int n = strl(a);
	int c = countChar(a,sp);
	if(c==0){
		char** tar = NULL;
		tar = (char**)malloc(sizeof(char*));
		tar[0] = (char*) malloc(strl(a)*sizeof(char));
		strncp(tar[0],a,strl(a));
		int retn[2] = {c,tar};
		return retn;
	}
	c=1;
	static char** tar = NULL;
	tar = (char**)malloc((c+1)*sizeof(char*));
	tar[0] = (char*) malloc(255*sizeof(char));
	clear(tar[0],255);
	int swt=0;
	for(int i=0,pos=0;i<n;++i){
		if(pos>254){
			tar[255] = '\0';
			break;
		}
		if(a[i]==sp && (!swt)){
			tar[++swt] = (char*) malloc(255*sizeof(char));
			clear(tar[swt],255);
			pos=0;
			continue;
		}
		tar[swt][pos] = a[i];
		pos++;
	}
	int retn[2] = {c,tar};
	return retn;
}

char* getName(char* a){
	return ((char**)(split(a,'=')[1]))[0];
}

char* getVar(char* a){
	int *x = splitFirst(a,'=');
	if(x[0]==0) return NULL;
	return ((char**)x[1])[1];
}

int getVarInt(char* a){
	char* var = getVar(a);
	if(var==NULL) return 255;
	int l = strl(var);
	int num=0;
	for(int i=0;i<l;++i){
		if(var[i]<48 || var[i]>57) return 255;
		num = num*10+var[i]-48;
	}
	return num;
}

void initProp(){
	int* x = split(prop,',');
	int n = x[0]+1;
	char** strs = x[1];
	for(int i=0;i<n;++i){
		// TODO: No Effect
		if(strp(strs[i],"top")) top=HWND_TOPMOST;
		if(strp(strs[i],"notop")) top=HWND_NOTOPMOST;
		if(strp(getName(strs[i]),"transparent")) transparent = getVarInt(strs[i]);
	}
}

void operate(){
	if(prop==NULL){
		printf("Error: No Prop Set.\n");
		return 1;
	}
	
	initProp();
	
	if(sleep){
		printf("Sleep Command Detected, Sleeping For 5 Secs\n");
		for(int i=0;i<5;++i){
			printf(".");
			fflush(stdout);
			Sleep(1000);
		}
		printf("\n");
	}
	
	HWND handle = NULL;
	
	if(mouse){
		// get handle from cursor
		POINT p = {0,0};
		if(GetCursorPos(&p)){
			handle = WindowFromPoint(p);
		}else{
			printf("Error: Cannot Get Handle From Cursor.\n");
			exit(1);
		}
	}
	
	if(title != NULL){
		if(handle){
			char* wintitle = (char*) malloc(127*sizeof(char));
			clear(wintitle,127);
			GetWindowTextA(handle,wintitle,sizeof(wintitle));
			if(strp(wintitle,title)){
				printf("Same Window Detected.\n");
			}else{
				printf("Error: Different Windows Detected.\n");
				return 1;
			}
		}else{
			handle = FindWindow(NULL,title);
		}
	}
	
	if(!handle){
		printf("Error: Cannot Get Handle.\n");
		return 1;
	}
	

	// Main Set
	if(transparent!=-1){
		HMODULE hInst = LoadLibrary("user32.dll");
		if(hInst==NULL){
			printf("Error: User32.dll Not Found.\n");
			exit(1);
		}
	
		int sty = (int)GetWindowLong(handle, GWL_EXSTYLE);
		if(!(sty&WS_EX_LAYERED)) SetWindowLong(handle, GWL_EXSTYLE, sty | WS_EX_LAYERED);
		
		typedef BOOL (WINAPI*SETALPHA)(HWND, COLORREF, BYTE, DWORD);
		SETALPHA SetAlpha = (SETALPHA)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(SetAlpha)
			// SetLayeredWindowAttributes(handle, RGB(255, 255, 255),transparent, LWA_ALPHA);
			SetAlpha(handle, RGB(255, 255, 255),transparent, LWA_ALPHA);
		FreeLibrary(hInst);
	}
	// SetWindowPos(handle,top,SWP_NOMOVE,SWP_NOMOVE,SWP_NOSIZE,SWP_NOSIZE,SWP_NOMOVE|SWP_NOSIZE);
	SetWindowPos(handle,top,1,1,1,1,SWP_NOMOVE|SWP_NOSIZE);
}

int main(int argc,char** argv){
	if(argc==1) printUsageExit();
	for(int i=1;i<argc;++i){
		// char** pro = parseProp(argv[i]);
		char** pro = NULL;
		int* m = splitFirst(argv[i],'=');
		int c = m[0];
		pro = m[1];
		char* name = pro[0];
		char* var = NULL;
		if(c>0) var = pro[1];
		if(strp(name,"sleep")) sleep=1;
		if(strp(name,"mouse")) mouse=1;
		if(strp(name,"title")) title = var;
		if(strp(name,"prop")) prop = var;
	}
	// printf("Debug: mouse=%d,title=%s,prop=%s\n",mouse,title,prop);
	operate();
	return 0;
}
