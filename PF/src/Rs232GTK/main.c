/**
 *      @file   main.c
 *      @brief  Main Program
 *
 * Fork of the main program into the child main and the parent main.
 * 
 *     @author  Joana Mota, joanacarvalhomota@ua.pt
 *
 *   @internal
 *     Created  24-Jan-2018
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2018, Joana Mota
 *
 ***************************************************
 */

#define _MAIN_C_


#include "myf.h"

int main(int argc, char *argv[])
{

#if 1
        int pid = fork();
        int s_id;

        if(pid == -1) { printf("Could not fork(). Exiting\n"); return -1; }

        if(pid == 0)            /* The child */
        {
                MessageFromChild("I am the child\n");
                ChildMainRs232();
                
        }
        else                    /* The parent */
        {
                MessageFromParent("I am the parent\n");
                s_id = ParentMainGTK();                     //Get s_id from return value to know the id to destroy the shared mem
                if(s_id > 0) shmctl(s_id, IPC_RMID, NULL);  //Allow elimination of shared memory
                MessageFromParent("PARENT ended\n");
                memset(datasave, 0, sizeof(datasave));
                kill(pid,SIGTERM);      //Kill the Child when GTK window is closed
                MessageFromChild("CHILD ended\n");
        }

        return 0;
#endif

}
