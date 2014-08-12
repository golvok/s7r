#include "draw/window.h"

#include <cairo/cairo.h>
#include <gtk/gtk.h>

static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

static gboolean draw_window(GtkWidget *widget, cairo_t* cr, gpointer data) {
	return FALSE;
}

int main () {
	Window win(800, 600, G_CALLBACK(draw_window));
	win.set_click_function( G_CALLBACK(clicked));
	win.loop();
	return 0;
}


static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
	return TRUE;
}
