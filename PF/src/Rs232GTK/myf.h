/**
 *      @file   myf.h
 *      @brief  Header file 
 * 
 * Header file for all the .c files
 * Includes all the libraries, structs and global variables.
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



#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <errno.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cv.h>
#include <highgui.h>
#include <ncurses.h>
#include <curses.h>

struct datastruct
{
	char Dist[1];	/**< Distance */
	int usb;		/**< USB Port information */
}; // Struct with shared memory data


#ifdef _MAIN_C_
	GtkBuilder *builderG;
	int bin_limitG = 128;
	int pid_childG;
	int count_childG=0;
	char distance[100];				/**<distance read by the sensor*/
	float datasave[100000];        	/**<variable to storage the data*/
	float datafilter[2000];			/**<variable to storage the filtered data*/
	int point=0;
	int pointfilt=0;
	int Control=0;
	struct datastruct *data;

#else
	extern GtkBuilder *builderG;
	int count_childG;
	extern int bin_limitG;
	int pid_childG;
	int count_childG;
	char distance[100];
	bool scan;
	bool scan2;
	extern float datasave[100000];        //variable to storage the data
	float datafilter[2000];
	extern int pointfilt;
	extern int point;
	extern int Control;
	extern struct datastruct *data;
#endif


#define SHM_SIZE 128 		/**< 128 bytes for shared memory */
#define PARENTDELAY 50000   /**<parent period in us*/
#define CHILDDELAY 50000    /**<child delay in us*/

#include "prototypes.h"
