#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>                //Used for srand((unsigned) time(NULL)) command
//#include <process.h>             //Used for system("cls") command
 
#define spade 35                 //Used to print spade symbol
#define club 36                 //Used to print club symbol
#define diamond 37               //Used to print diamond symbol
#define heart 38                 //Used to print heart symbol
#define RESULTS "Blackjack.txt"  //File name is Blackjack
#define WIN_SUM 65535 
 
char * visitor_name ;
int login_flag=0;
char * motto=0;

void handler(){
	puts("[-] Time out ... \n");
	exit(1);
}

void init() {
    unsigned int v2 = 0;
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);
	setvbuf(stderr,0,2,0);
    signal(14,handler);	
	alarm(200);
    srand(time(0));
    while (v2 <= 0x10000 )
        v2 = rand() & 0xFFFFF000;
    visitor_name =  mmap((void *)(signed int)v2, 0x1000uLL, 3, 34, -1, 0LL);
    if ( visitor_name == -1 )
    {
        puts("mmap error");
      exit(0);
    }    
    v2 = 0;
    while ((v2 <= 0x10000 )||(visitor_name == v2))
        v2 = rand() & 0xFFFFF000;
    motto =  mmap((void *)(signed int)v2, 0x1000uLL, 3, 34, -1, 0LL);
    if ( motto == -1 )
    {
        puts("mmap error");
      exit(0);
    }      
    strcpy(visitor_name,"anonym");
}

int read_n(char *src,int len){
  unsigned int i;
  for ( i = 0; i < len; i++ )
  {
    if ( read(0, (void *)(i + src), 1) != 1 )
      exit(-1);
    if ( *(src+i) == 10 )
    {
      *(src+i) = 0;
      return i;
    }
  }
  return i; 
}


void menu1(){
    puts("=====MENU======");
    puts("1. set name");
    puts("2. set motto");
    puts("3. show motto");
	puts("4. Exit");
    puts("===============");
    //printf(">");
}
void maybe_real_set_name(){
	scanf("%6s",visitor_name);//%5$hn%3$p
	puts("OK, I know your name :");
	printf(visitor_name);
}
void set_name(){
	maybe_real_set_name();
}
void set_motto(){
    int i = 0;
	char buffer[0x140] ;
	i = read_n(buffer,0x140);
	memcpy(motto,buffer,i);

}




int login(){
    menu1();
	if (login_flag != 0)
		return -1;
	char * fmt = "%d";
	int opt;
	while(1){
		printf(">");
		if(scanf(fmt,&opt)<=0){
			exit(0);
		}
		switch(opt){
			case 1:
				if((login_flag & 0x01)==0){
					login_flag = login_flag|0x01;
					if (!strncmp("anonym",visitor_name,6))
                    {   
                        printf("name:");
					    set_name();
                    }
				}
				break;
			case 2:
				if((login_flag &0x10) == 0){
					login_flag = login_flag|0x10;
					printf("motto:");
					set_motto();
				}
				break;
			case 3:
				if((login_flag &0x100) == 0){
					login_flag = login_flag|0x100;
					if(motto!=0)
					puts(motto);
				}
				break;	
			case 4:
				return 0;
				break;
			default:
				puts("invalid options\n");

		}
	}
}

int main(){
    asm(
        "shr $0x16,%rsp\n"
        "shl $0x16,%rsp\n"
        "xor $0x188,%rsp\n"
        "push %rbp\n"
        "mov %rsp,%rbp\n"
        "sub $0x20,%rsp\n"
    );    
    init();
    login();
    exit(0);

}