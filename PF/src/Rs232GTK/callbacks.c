/**
 *      @file   callbacks.c
 *      @brief  Main functions for GTK
 * 
 * GTK callbacks
 *
 *
 *     @author  Joana Mota, joanacarvalhomota@ua.pt
 *
 *   @internal
 *     Created  23-Jan-2018
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2018, Joana Mota
 *
 ***************************************************
 */


#include "myf.h"

/**
* @brief  Callback to process the delete_event (usually, window kill request)
*
* @param  window - The widget that generated the event (not used here)
* @param  event - The event description (not used here)
* @param  data - generic user data (not used here)
* @return void
*/
void pari_delete_event(GtkWidget * window, GdkEvent * event, gpointer data) 
{
        g_print("you should not use the os to force leave\n");
        gtk_main_quit();
}


/**
* @brief  Callback to catch CTRL-C
*/
void InterceptCTRL_C(int a)
{
        g_print("Sair por CTRL-C\n");
        gtk_main_quit();
}

/**
* @brief  Function that writes on the labelDist and labelDetectObj in a loop of 100s to 100ms.
*
* This functions will loop while it returns TRUE.
* It will fill the labelDist with the distance present on the shared memory.
* It will also read the value of the entry in entryDist (which represents the distance 
* from the table to the sensor) and presents it in the labelDetectObj
* if it detects a object or not. 
* It does this by evaluating the diference between the distance presented in the labelDist 
* and the distance from the table.
*
* @param user_data - Custom data with a widget ID to fetch some info
* @return TRUE - If it is to continue the scan
* @return FALSE - If it is to stop the scan
*/
gboolean my_timeout(gpointer user_data)
{
        GtkLabel *lblDist= GTK_LABEL(gtk_builder_get_object(builderG,"labelDist"));
        gtk_label_set_text(lblDist, data->Dist);   //set entry text in "labelDist" widget

        GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builderG,"entryDist"));
        GtkLabel *lblDetectObj= GTK_LABEL(gtk_builder_get_object(builderG,"labelDetectObj")); //Get some specific label ID to change it
        int comp;
        int control;
 
        if(entry) 
        {
                const char* entryval = gtk_entry_get_text(entry); //get the text from "entryDist" widget
                comp=atoi(entryval)-atoi(data->Dist);   
                control=atoi(entryval)/100;     //Control to only evaluate when the input distance has at least 3 decimal digits
                if (control<10 && control>0 && comp<=10)
                {
                        gtk_label_set_text(lblDetectObj, "Object NOT Found");   //set entry text in "labelDetectObj" widget

                }
                else if (control<10 && control>0 )
                {
                        gtk_label_set_text(lblDetectObj, "Object Found");   //set entry text in "labelDetectObj" widget
                }
                else
                {
                        gtk_label_set_text(lblDetectObj, " ");   //set entry text in "labelDetectObj" widget
                }
        }
        return scan;
}

/**
* @brief  Function that fills the arrays datasave and datafilter in a loop of 3s to 3ms .
*
* This functions will loop while it returns TRUE.
* It will storage all the distance values read within the interval of 3ms.
* It will also filter the values for a more smooth presentation in the 
* drawingarea of the GTK interface and storage them in the 
* datafilter array.
*
* @param user_data - Custom data with a widget ID to fetch some info.
* @return TRUE - If it is to continue the scan.
* @return FALSE - If it is to stop the scan or if the datafilter reaches a specific dimension.
*/
gboolean my_timeoutFast(gpointer user_data)
{
        float mean=0;
        datasave[point]= atof(data->Dist);      //storage the distance values

        if (point%10==0)
        {
                if (point==0)
                {
                        datafilter[pointfilt]==datasave[point];
                        pointfilt++;
                }
                else
                {
                        for (int i = 0; i < 100; i++)
                        {
                                mean=mean+datasave[point-i];
                        }
                        //Calculates the mean of 100 values of the datasave 
                        //and storages it in datafilter to smooth the analysis
                        datafilter[pointfilt]=mean/100;
                        pointfilt++;
                }
        }
        point++;
        if (pointfilt==1500)
        {
                return FALSE;   //Stops fuction loop when datafilter reaches a specific dimension
        }

        return scan2;
}

/**
* @brief  Callback to Start Scan
*
* When this callback is triggered it will inicialize two functions 
* that will loop with a respective time delay.
* One function that refreshes the label areas and the other that
* that storages all the data.
*
* @param widget - the one that originated the call
* @param user_data - Custom data with a widget ID to fetch some info
* @return void.
*/
void on_STARTscan_clicked(GtkWidget *widget, gpointer user_data)
{
//Prints a message on the labelWarnings.
        GtkLabel *lblWarningsSTART= GTK_LABEL(gtk_builder_get_object(builderG,"labelWarnings"));
        gtk_label_set_text(lblWarningsSTART, "Click on STOP scan to end Scan!");   //set entry text in "labelWarnings" widget

//Clears the dasave and datafilter to be ready for a new scan.
        memset(datasave, 0, sizeof(datasave));
        point=0;    
        memset(datafilter, 0, sizeof(datafilter));
        pointfilt=0;


        scan=TRUE;      // Sets the functions my_timeout to return TRUE and continue the loop
        scan2=TRUE;     // Sets the functions my_timeoutFast to return TRUE and continue the loop

//loop to refresh the label that presents the distance and the one that presents if a object is or is not found
        gdk_threads_add_timeout(100,my_timeout,NULL);
//loop to storage all the data and all the filtered data
        gdk_threads_add_timeout(3,my_timeoutFast,NULL);

//Changes the sensitivity of the STARTscan anf the STOPscan buttons
        GtkWidget *ButonSTART = GTK_WIDGET(gtk_builder_get_object(builderG,"STARTscan"));
        gtk_widget_set_sensitive (ButonSTART,FALSE);
        GtkWidget *ButonSTOP2 = GTK_WIDGET(gtk_builder_get_object(builderG,"STOPscan"));
        gtk_widget_set_sensitive (ButonSTOP2,TRUE);
}



/**
* @brief  Callback to Stop scan
*
* When this callback is triggered it will stop both loops that 
* the callback STARTscan initialized.
*
* @param widget - the one that originated the call
* @param user_data - Custom data with a widget ID to fetch some info
* @return void.
*/
void on_STOPscan_clicked(GtkWidget *widget, gpointer user_data)
{
//Prints a message on the labelWarnings.
        GtkLabel *lblWarningsSTOP= GTK_LABEL(gtk_builder_get_object(builderG,"labelWarnings"));
        gtk_label_set_text(lblWarningsSTOP, "Click on START scan to Start a new Scan!");   //set entry text in "labelDist" widget

        scan=FALSE;     // Sets the functions my_timeout to return FALSE and stop the loop
        scan2=FALSE;    // Sets the functions my_timeoutFast to return FALSE and stop the loop
        Control=0;

//Changes the sensitivity of the STARTscan anf the STOPscan buttons
        GtkWidget *ButonSTOP = GTK_WIDGET(gtk_builder_get_object(builderG,"STOPscan"));
        gtk_widget_set_sensitive (ButonSTOP,FALSE);
        GtkWidget *ButonSTART2 = GTK_WIDGET(gtk_builder_get_object(builderG,"STARTscan"));
        gtk_widget_set_sensitive (ButonSTART2,TRUE);
}


/**
* @brief  Callback to close window
*
* @param widget - the one that originated the call
* @param user_data - Custom data with a widget ID to fetch some info
* @return void.
*/
void on_Close_clicked(GtkWidget *widget, gpointer user_data)
{
        puts("Pedido de destruição de janela--CLOSE");
        gtk_main_quit();  //necessary to to leave GTK main loop
}

/**
* @brief  Callback to draw on the drawing area.
*
* @param widget - the one that originated the call
* @param cr - contains the current state of the rendering device, 
*             including coordinates of yet to be drawn shapes.
* @param user_data - Custom data with a widget ID to fetch some info
* @return void.
*/
void on_drawingareaPerfil_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
        p_RedrawFreeForm(cr);
}

/**
* @brief  Function to paint on the drawingarea.
*
* This function will paint all the datafilter values while it is being filled.
*
* @param widget - the one that originated the call
* @param cr - contains the current state of the rendering device, 
*             including coordinates of yet to be drawn shapes.
* @param user_data - Custom data with a widget ID to fetch some info
* @return void.
*/
void p_RedrawFreeForm(cairo_t * cr)
{
        cairo_set_source_rgb(cr, 1, 0, 0);  //define color
        cairo_set_line_width(cr, 1);        //defines line width 

        float i;
        int ii=0;
        cairo_move_to(cr, 0, datafilter[0]);          //initiates "path"
        for(i = 1; i < 1500; i=i+1)            //goes through all the accumulated points
        {
                cairo_line_to(cr, i, datafilter[ii]); //refreshes the "path"
                ii++;
        }
        cairo_stroke(cr);     //"prints" the entire "path" on the drawing area

}

/**
* @brief  Refreshes the drawing area, evaluates and presents the USB connection.
*
* @param user_data - Custom data with a widget ID to fetch some info
* @return TRUE.
*/
gboolean p_ForceRefreshDA(gpointer user_data)
{
//Refreshes the drawing area.
        GtkWidget *da;
        if( ! user_data)
           da = GTK_WIDGET(gtk_builder_get_object (builderG, "drawingareaPerfil"));
        else
           da=GTK_WIDGET(user_data);

        gtk_widget_queue_draw(da);  //make draw the widget

//Analyzes and write in the label called labelUSB if there is a USB connection or not. 
        GtkLabel *lblUSB= GTK_LABEL(gtk_builder_get_object(builderG,"labelUSB"));
        if (data->usb==1)
        {
                gtk_label_set_text(lblUSB, "Port USB NOT Connected. Confirm Connection!");   //set entry text in "labelUSB" widget
        }
        else
        {
                gtk_label_set_text(lblUSB, "Port USB Connected");   //set entry text in "labelUSB" widget  
        }

        return TRUE;  //continue running
}

/**
* @brief  Inicialize the function that refreshes the drawing area which  
* will loop with a respective time delay.
*
* @param void
* @return void
*/
void p_InitTimer()
{
  //Get widget ID for "drawingarea1"
  GtkWidget *da=GTK_WIDGET(gtk_builder_get_object (builderG, "drawingareaPerfil"));
  //define timeout (timer): interval, callback to execute, and pass the window ID to refresh
  g_timeout_add (3, p_ForceRefreshDA, da);
}
