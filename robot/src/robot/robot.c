#include "simpletools.h"                      // incluir simpletools para controladores de la placa
#include "abdrive.h"                          // Inckuir abdrive para encoders y servos
#include "ping.h"                             // Incluir ping para sensor

//--------------------------------------------------
//DEFINIMOS EL STACK
//--------------------------------------------------
#define STACK_MAX 30              //tamano del stack
struct Stack {
    int     data[STACK_MAX];
    int     size;
};
typedef struct Stack STACK;
STACK pila;
void push(int);
int  pop(void);

//--------------------------------------------------
//DECLARAMOS VARIABLES
//--------------------------------------------------
int derecha;
int izquierda;
int modo=0;
int adelante;
int valor=0;
int izq[2] = {-28,26};
int der[2] = {25,-25};
int vuelta[2] = {56,-51};
int err = 5;
int ade = 90;
int pau = 50;
int rapi = 40;
int main()                                    
{
	pila.size = -1;
	while (1){
		pause(pau);
		derecha=ping_cm(3);
		pause(pau);
		izquierda=ping_cm(10);
		pause(pau);
		adelante=ping_cm(7);
		pause(pau);
		if(modo==0){
			drive_speed(rapi,rapi);
			if(adelante<=10){
				drive_speed(10,10);
				drive_goto(-10,-10);
				pause(pau);
				drive_goto(vuelta[0],vuelta[1]);
				modo=1;          
			}else if((derecha>25)||izquierda>25)){
				drive_goto(err,err);
				pause(pau);
				derecha=ping_cm(3);
				pause(pau);
				izquierda=ping_cm(10);
				pause(pau);
				if((derecha>25)&&(izquierda>25)){
					push(4);
					drive_goto(der[0],der[1]);
				}else if(derecha>25){
					push(1);
					drive_goto(der[0],der[1]);
				}else if(izquierda>25){
					push(2);
					drive_goto(izq[0],izq[1]);
				}
				pause(pau);
				drive_goto(ade,ade);
			}
		}else if(modo==1){
			drive_speed(rapi,rapi);
			if((derecha>25)||(izquierda>25)){
				drive_goto(err,err);
				pause(pau);
				derecha=ping_cm(3);
				pause(pau);
				izquierda=ping_cm(10);
				pause(pau);
				valor = pop();
				if((derecha>25)&&(izquierda>25)){
					if(valor==1){
						drive_goto(der[0],der[1]);
						push(3);
						modo=0;
					}else if(valor==2){
						drive_goto(izq[0],izq[1]);
						push(3);
						modo=0;
					}else if(valor==4){
						drive_goto(der[0],der[1]);
						push(5);
						modo=0;
					}else if(valor==5){
						drive_goto(der[0],der[1]);
						push(6);
						modo=0;
					}else if(valor==6){
						drive_goto(der[0],der[1]);
					}
				}else if(valor==1){
					drive_goto(izq[0],izq[1]);
				}else if(valor==2){
					drive_goto(der[0],der[1]);
				}else if(valor==4){
					push(2);
					modo=0;
				}
				pause(pau);
				drive_goto(ade,ade);
			}			
		}
	}
}  
    
//----------------------------------------------
//METODOS DEL STACK
//----------------------------------------------

//guarda un valor en el stack
void push (int num) {
    if (pila.size == (STACK_MAX - 1)) {
        printf ("Stack is Full\n");
        return;
    }
    else {
        //printf ("Enter the element to be pushed\n");
        //scanf ("%d", &num);
        pila.size = pila.size + 1;
        pila.data[pila.size] = num;
        printf("%d guardado\n",&num);
    }
    return;
}

//regresa un valor del stack
int pop (){
    int num;
    if (pila.size == - 1) {
        printf ("Stack is Empty\n");
        return (pila.size);
    }
    else {
        num = pila.data[pila.size];
        printf ("poped element is = %d\n", num);
        pila.size = pila.size - 1;
    }
    return(num);
}
