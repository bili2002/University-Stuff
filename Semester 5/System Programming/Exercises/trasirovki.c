#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void tras_13a() {
    int  pid,  var = 1, status;
    if  (( pid = fork()) == 0 )  {var+=2; 
    printf ("\n Var = %d ", var);} 
    else { wait(&status);  var++ ;
    printf ("\n Var = %d ", var); 
    exit(0);}
    --var;
    printf ("\n Var = %d ", var);
}


void tras_12() {
 int fdr,fdw,n_byt,i=0,status;
 char symbols[40], c;
 if ( fork())
   { wait ( & status);
     if ( open ( "file_second", O_RDONLY) != -1)
       write(1,"\n",1);
        execlp("grep", "grep","234","file_second",0);
        execlp("wc", "wc","-l","file_second",0);
    }
else { if (( fdr=open("file_first.txt",O_RDONLY))== -1)
      {printf("\n Cannot open \n"); exit(1); }
       if (( fdw=open("file_second",O_CREAT|O_TRUNC|O_RDWR,0666))== -1)
      {printf("\n Cannot open \n"); exit(1); }
     n_byt=read( fdr, symbols,22);
     write( fdw,"12345\n",6);
    c=symbols[i++];
    if ( c>='0' && c <='9')
      {  while (symbols[i++] != '\n' && i<n_byt)
                write(fdw, "12345",5);
       write(fdw,"\n",1);
       write(1,"AAA\n",4);
      }
    else {write(1, symbols,n_byt); 
          write(fdw, symbols,8);
           write(1,"\n",1);
         }
       write(fdw,"AAA\n",4);
       write(1,symbols,12);
       close(fdr); close(fdw);
    }
}

#define  COM  "ps"
void tras_13b() {
int  i=2, status;
if ( fork() ) {wait(&status); --i;
                    printf( " Stoinostta na i = %d;", i);}
else  { i++; 
         if ( execlp ( COM,  COM, 0) == -1) { i=i+2;
                     printf( " Stoinostta na i = %d;", i);}
          }
printf( " Stoinostta na i = %d;", --i);
}

void tras_15a() {
int  j  = 7 ;

if  ( fork() == 0 )  {
      j++ ; exit(0);
      if  ( execlp ("who","who",0 ) == -1  )   j = 7;
      else   printf ("\nStoinostta na j = %d ", j); 
}
else  {  --j ;   printf ("\nStoinostta na j = %d ", j )  ;   }

}

void tras_15b(int argc, char *argv[]) {
int fd, k, status;
       fd = open ( argv[1], O_RDWR) ;
           if ( fork() )
           { wait(&status);
              dup(fd);
               write ( 1, "who\n", 4);
               execlp ( "who", "who","-u",0); 
              write (fd, " exam1\n",5);
             }
         else { close(1); dup(fd);
                 for ( k=0; k<=3; k++)
                 write (1, "exam2\n",6); 
                 }
        execlp ( "pwd", "pwd",0);
}

void tras_15c(int argc, char *argv[]) {
  int fdr, fdw, n_byt, i = 0 , status;
	char symbols [ 40 ], c ;	
	if ( fork( ) )
	           {  wait ( &status);
	                if ( open ("file_new", O_RDONLY ) != -1 )
		        { write ( 1, "\n", 1 );
		         execlp ("wc", "wc", "-c", "file_new",0 );
                               execlp ("head", "head", "-5c", "file_new",0 );
                              }
	             }
            else {
		    if  (  ( fdr = open ("fileD.txt",O_RDONLY ) ) == -1 )
		     { printf ("\n Cannot open  \n" );  exit (1);  }
if (( fdw=open ("file_new", O_CREAT|O_TRUNC|O_RDWR,0666 )) == -1 )
		     { printf ("\n Cannot creat  \n" );  exit (1);  }
		     n_byt = read ( fdr , symbols, 40 );
		     c = symbols[ i++];
		     if  ( c >= '0' && c <= '9' )
		      {	while  ( symbols [ i ++] != '\n' && i < n_byt )
                                             write ( fdw, "$", 1 );
                        write ( fdw, "\n", 1 );
                        write ( 1, "\n", 1 );
		       }
		else { write( 1,  symbols, n_byt );  write ( 1, "\n", 1 ); 
                        }
                        write ( fdw, "$\n", 2 );
                        write ( 1, symbols, 12);
                        close ( fdr); close (fdw);
	          }

}

void tras_15d(int argc, char *argv[]) {
int  a = 1000;
if  ( fork()  )  {  a/=2; 
                      printf ("\nStoinostta na а = %d ", a ) ; }
else  {  if ( fork() )
            { a*=2;
              printf ("\nStoinostta na а = %d \n", a ) ; 
               if  (  execlp("ls","ls", "-l", 0 ) == -1 )  {a = a + 2; 
                     printf ("\nStoinostta na a = %d ", a);   }
             }
           else 
            {    a+=2;
                  printf ("\nStoinostta na а = %d ", a ) ; }
             }
a++;
printf ("\nStoinostta na a = %d ", a);
}

int main(int args, char **argv){
  tras_15d(args, argv);
}
