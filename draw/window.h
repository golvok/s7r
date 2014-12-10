#ifndef WINDOW_H
#define WINDOW_H

#include <cairo/cairo.h>
#include <gtk/gtk.h>

class Window {
public:
	Window(size_t w, size_t h, void* (*draw_function)(cairo_t*, void*), void* draw_function_data);
	void set_click_function(void* (*click_function)(void*), void* data);
	void loop();

	void call_click_function();
	void call_draw_function(cairo_t* cr);
private:
	GtkWidget* window;
	GtkWidget* darea;
	void* (*draw_function)(cairo_t*, void*);
	void* draw_function_data;
	void* (*click_function)(void*);
	void* click_function_data;
	Window& operator=(const Window&);
	Window(const Window&);
};

#endif /* WINDOW_H */
