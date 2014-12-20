#ifndef WINDOW_H
#define WINDOW_H

#include <cairo/cairo.h>
#include <gtk/gtk.h>
#include <memory>

class DrawerObject {
public:
	DrawerObject() : context(nullptr) {}
	virtual ~DrawerObject() {}

	cairo_t* getContext() { return context; }
	void doDrawing(cairo_t* c) {
		context = c;
		draw();
		context = nullptr;
	}
	virtual void draw() = 0;
private:
	DrawerObject(const DrawerObject&);
	DrawerObject& operator=(const DrawerObject&);
	cairo_t* context;
};

class Window {
public:
	Window(size_t w, size_t h, std::unique_ptr<DrawerObject>&& drawer);
	void set_click_function(void* (*click_function)(void*), void* data);
	void loop();

	void call_click_function();
	void call_draw_function(cairo_t* cr);
private:
	GtkWidget* window;
	GtkWidget* darea;
	std::unique_ptr<DrawerObject> drawer;
	void* (*click_function)(void*);
	void* click_function_data;

	Window& operator=(const Window&);
	Window(const Window&);
};

#endif /* WINDOW_H */
