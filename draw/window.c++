#include "window.h"

Window::Window(
	size_t width, size_t height, GCallback draw_function_
	):	window(NULL)
	,	darea(NULL)
	,	draw_function(draw_function_)
	,	click_function(NULL) {

	gtk_init (NULL, NULL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	darea = gtk_drawing_area_new();
	gtk_widget_set_size_request (darea, width, height);

	g_signal_connect (darea, "draw", G_CALLBACK(draw_function_),  NULL);

	gtk_window_set_position(     GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size( GTK_WINDOW(window), width, height); 
	gtk_window_set_title(        GTK_WINDOW(window), "Lines");

	gtk_container_add(GTK_CONTAINER (window), darea);
	gtk_widget_show(darea);
	gtk_widget_show(window);
}

void Window::set_click_function(GCallback click_function_) {
	this->click_function = click_function_;
	g_signal_connect(window, "button-press-event", G_CALLBACK(click_function_), NULL);
}

void Window::loop() {
	gtk_main ();
}
