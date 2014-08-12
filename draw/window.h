#include <gtk/gtk.h>

class Window {
public:
	Window(size_t w, size_t h, GCallback draw_function);
	void set_click_function(GCallback click_function);
	void loop();
private:
	GtkWidget *window;
	GtkWidget *darea;
	GCallback draw_function;
	GCallback click_function;
	Window& operator=(const Window&);
	Window(const Window&);
};
