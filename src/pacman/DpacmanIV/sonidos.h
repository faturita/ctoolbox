/*
Programa: sonidos.h
Objetivo: Funciones con manejo de sonido PC-SPEAKER
Autor: Hernan Montani

*/



#include <sys/ioctl.h>
#include </usr/include/linux/kd.h>


/*FUNCIONES PARA MANEJO DE SONIDO*/
void sonido(int frec,int dura)
    {
    ioctl(0,KIOCSOUND,(int)(1190000/frec));
    usleep(dura*500);
    ioctl(0,KIOCSOUND,40); //deberia ir 0 aca pero no funciona!!!
    }

void musicagolpe()
    {
    sonido(293, 300);
    sonido(311, 300);
    sonido(329, 300);
    sonido(522, 500);
    sonido(329, 300);
    sonido(522, 400);
    sonido(329, 300);
    sonido(522, 900);
    usleep(300*500);
    sonido(522, 300);
    sonido(586, 300);
    sonido(622, 250);
    sonido(658, 250);
    sonido(522, 250);
    sonido(586, 300);
    sonido(658, 550);
    usleep(30*500);
    sonido(522, 300);
    sonido(586, 500);
    usleep(50*500);
    sonido(522, 400);
    usleep(150*500);
    sonido(392, 500);
    usleep(100*500);
    sonido(261, 500);
    }


void musicafunebre()
    {
    sonido(293,800);
    usleep(100*500);
    sonido(293,500);
    usleep(100*500);
    sonido(293,300);
    usleep(100*500);
    sonido(293,500); 
    usleep(100*500);
    sonido(349,500);
    sonido(329,300);
    sonido(329,500);
    sonido(293,300);
    sonido(293,500);
    sonido(277,300);
    sonido(293,800);
    }

/*FIN DE FUNCIONES DE MANEJO DE SONIDO*/    

