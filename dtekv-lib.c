#include "dtekv-lib.h"
#define JTAG_UART ((volatile unsigned int*) 0x04000040)
#define JTAG_CTRL ((volatile unsigned int*) 0x04000044)

void printc(char s){
    while (((*JTAG_CTRL)&0xffff0000) == 0);
    *JTAG_UART = s;
}
void print(char *s){
  while (*s != '\0') { printc(*s); s++; }
}
void print_dec(unsigned int x){
  unsigned divident = 1000000000; char first = 0;
  do {
    int dv = x / divident;
    if (dv != 0) first = 1;
    if (first != 0) printc(48+dv);
    x -= dv*divident; divident /= 10;
  } while (divident != 0);
  if (first == 0) printc(48);
}
void print_hex32 ( unsigned int x){
  printc('0'); printc('x');
  for (int i = 7; i >= 0; i--) {
    char hd = (char) ((x >> (i*4)) & 0xf);
    if (hd < 10) hd += '0'; else hd += ('A' - 10);
    printc(hd);
  }
}
void handle_exception ( unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3, unsigned arg4, unsigned arg5, unsigned mcause, unsigned syscall_num ){
  switch (mcause){
    case 0: print("\n[EXCEPTION] Instruction address misalignment. "); break;
    case 2: print("\n[EXCEPTION] Illegal instruction. "); break;
    case 11:
      if (syscall_num == 4) print((char*) arg0);
      if (syscall_num == 11) printc(arg0);
      return ;
    default: print("\n[EXCEPTION] Unknown error. "); break;
  }
  print("Exception Address: "); print_hex32(arg0); printc('\n');
  while (1);
}
int nextprime( int inval ){
   int perhapsprime = 0, testfactor, found;
   if (inval < 3 ){
     if(inval <= 0) return 1;
     if(inval == 1) return 2;
     if(inval == 2) return 3;
   } else {
     perhapsprime = ( inval + 1 ) | 1 ;
   }
   for( found = 0; found != 1; perhapsprime += 2 ){
     for( testfactor = 3; testfactor <= (perhapsprime >> 1) + 1; testfactor += 1 ){
       found = 1;
       if( (perhapsprime % testfactor) == 0 ){ found = 0; goto check_next; }
     }
     check_next:;
     if( found == 1 ){ return perhapsprime; }
   }
   return perhapsprime;
}
