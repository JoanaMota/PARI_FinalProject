/**
 *      @file   myf.c
 *      @brief  Child's and parent's main and necessery functions.
 *
 * Rs232 Comunication in the Child's main.
 * GTK interface in the Parent's main.
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

#include "myf.h"
#include <myutils.h>  



/**
 * @brief  Function that prints the child's messages in RED to distinguish the messages between parent and child.
 *
 * @param  str - The message
 * @return void
 */
void MessageFromChild( char *str)
{
        textcolor(RESET,RED,WHITE);  //from pcolor
        printf("%s",str);
        ResetTextColors();           //from pcolor
}
/**
 * @brief  Function that prints the parents's messages in GREEN to distinguish the messages between parent and child.
 *
 * @param  str - The message
 * @return void
 */
void MessageFromParent( char *str)
{
        textcolor(RESET,GREEN,WHITE);  //from pcolor
        printf("%s",str);
        ResetTextColors();             //from pcolor
}

/**
 * @brief  The Child's Main
 *
 * Opens port to receive serial data from the ESP8266, colects and interprets the distances from the sensor.
 * 
 * @param  void
 * @return -1 - If fails to get shared memmory
 * @return 0 - If succeeded
 */
int ChildMainRs232(void)
{
        int shm_id;
        int  fd;        //Rs232 communication
        float dist;     //to storage the distance with decimal digits;
        char usbPort0[20] = "/dev/ttyUSB0";      //USB port name
        char usbPort1[20] = "/dev/ttyUSB1";      //USB port name

        shm_id = GetSharedMem();
        if(shm_id == -1) return -1;     //failure to get shared memmory

        /* attach to the memory segment to get a pointer to it */
        data = (struct datastruct *)shmat(shm_id, NULL, 0);

        //handling erros
        if(data == (struct datastruct *) (-1))
        {
                perror("shmat");
                exit(1);
        }

        data->usb=0;
        fd=OpenPort(usbPort0, NULL);     //open serial connection on Port 0
        if(fd == -1) //if fails to open serial connection on Port 0

        { 
                printf("Error. Could not open port USB0\n"); 
                fd=OpenPort(usbPort1, NULL);     //open serial connection on Port 1
                if (fd==-1)     //if fails to open serial connection on Port 1
                {
                        printf("Error. Could not open port USB1\n"); 
                        printf("Confirm Port Connection\n");
                        
                        while (fd == -1)
                        {
                                data->usb = 1;
                                fd=OpenPort(usbPort0, NULL);     //open serial connection on Port 0
                                if (fd == -1)
                                {
                                        fd=OpenPort(usbPort1, NULL);     //open serial connection Port 1
                                }
                        }
                        data->usb = 0;
                }

        }       
        
        while (1)
        {
                ReadPortUntilChar(fd);                  //Reads the distance given by the ESP8266 
                dist=atof(distance);
                sprintf(data->Dist, "%.2f", dist);      //Storages the distance in the shared memory
                distance[0]='\0';
        }


        /* detach from the mem segment */
        if( shmdt((void *)data) == -1)
        {
                perror("shmdt");
                exit(1);
        }
        return 0;
}

/**
 * @brief  The Parent's Main
 * 
 * GTK interface
 *
 * @param  void
 * @return shm_id - The shared memmory ID 
 */
int ParentMainGTK(void)
{
        int shm_id;

        shm_id = GetSharedMem();
        if(shm_id == -1) return -1;     //failure

/* attach to the memory segment to get a pointer to it */
        data = (struct datastruct *)shmat(shm_id, NULL, 0);
        if(data == (struct datastruct *) (-1))
        {
                perror("shmat");
                exit(1);
        }

        // -----GTK-----
        gtk_init(NULL, NULL);

/* load the interface after a configuration file*/
        builderG = gtk_builder_new();
        gtk_builder_add_from_file(builderG, "int_grafica.glade", NULL);

/* connect the signals in the interface */
        gtk_builder_connect_signals(builderG, NULL);

/* get main window Widget ID and connect special signals */
        GtkWidget *t = GTK_WIDGET(gtk_builder_get_object(builderG, "window1"));
        if (NULL == t)
        {
                // Print out message if it dosen't find 'window1'
                fprintf(stderr, "Unable to file object with id \"window1\" \n");
        }
        if(t)
        {
                g_signal_connect(G_OBJECT(t), "delete_event", G_CALLBACK(pari_delete_event), NULL);
        }

        // use signal to catch SIGINT  (CTRL-C) - optional
        signal(SIGINT, InterceptCTRL_C);
        
/* inicializes the loop to refrehs the drawing area*/
        p_InitTimer();

/* start the event loop */
        gtk_main();

/* detach from the mem segment since it is leaving */
        if(  shmdt(data) == -1 )
        {
                perror("shmdt");
                exit(1);
        }
        return shm_id;

}

/**
 * @brief  Gets a shared memory and connects to it
 *
 * The function first creates a random key to identify
 * the memory and then connects to it with some specific permissions
 * 
 * @return shm_id - The Shared memory ID. Returns -1 in case of failure.
 */
int GetSharedMem()
{
        key_t key;                              
        char *path ="../src/Rs232GTK/main.c"; 
        int id ='X';
        int shm_id;

        key = ftok(path, id);       /* generate a random key based on a file name and char */
        if(key == -1) { perror("IPC error: ftok"); return -1; }

        /* now connect to (or possibly create) the shared memory segment with permission 644 */
        shm_id = shmget(key, sizeof(struct datastruct), 0644 | IPC_CREAT);
        if(shm_id == -1) { perror("shmget"); return -1; }

        return shm_id;
}

/**
 * @brief  Open Port for connection with ESP8266
 *
 * @param  port_name - Port's identification
 * @param  text - Text that will be written to the file
 * @return fd - The file descriptor
 */
int OpenPort( char *port_name, char* text)
{

	int fd;
	fd=open(port_name, O_RDWR | O_NDELAY ); /*O_RDONLY, O_WRONLY*/
        if (text!=0)
        {
                write( fd, text, strlen(text) );
        }


    return fd;
}

/**
 * @brief  Reads from port until a specific char. 
 * 
 * This functions reads the values received until it finds a paragraph, to storage the intire distance value.
 *
 * @param  fd - The file descriptor
 * @return none
 */
int ReadPortUntilChar(int fd)
{
    char ch;
    int n;
    do
    {
        n=read( fd, &ch, 1);
        if( n == -1 ) continue;   //perror("Err:");
        sprintf(distance,"%s%c",distance,ch);
    } while( ch != '\n');       //Reads until a paragraph is found
    return 0;
}

