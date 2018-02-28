/*File generated automatically in joana-X541UV by joana on Seg Jan 29 19:08:26 WET 2018*/
#ifdef __cplusplus
extern "C" {
#endif
/* callbacks.c */
void InterceptCTRL_C (int a);
gboolean my_timeout (gpointer user_data);
gboolean my_timeoutFast (gpointer user_data);
void on_Close_clicked (GtkWidget * widget, gpointer user_data);
void on_STARTscan_clicked (GtkWidget * widget, gpointer user_data);
void on_STOPscan_clicked (GtkWidget * widget, gpointer user_data);
void on_drawingarea1_draw (GtkWidget * widget, cairo_t * cr, gpointer user_data);
void on_drawingarea1_motion_notify_event (GtkWidget * widget, GdkEventMotion * event, gpointer user_data);
void on_mouse_button_on (GtkWidget * widget, GdkEventButton * event, gpointer user_data);
gboolean p_ForceRefreshDA (gpointer user_data);
void p_InitTimer ();
void p_RedrawFreeForm (cairo_t * cr);
void p_RedrawText (cairo_t * cr);
void pari_delete_event (GtkWidget * window, GdkEvent * event, gpointer data);
/* main.c */
int main (int argc, char *argv[]);
/* myf.c */
int ChildMainRs232 (void);
int GetSharedMem ();
void MessageFromChild (char *str);
void MessageFromParent (char *str);
int OpenPort (char *port_name, char *text);
int ParentMainGTK (void);
int ReadPortUntilChar (int fd);
#ifdef __cplusplus
}
#endif
