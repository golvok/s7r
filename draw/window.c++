#include "window.h++"

static gboolean draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data) {
	(void)widget;
	(void)data;

	Window* window = static_cast<Window*>(data);

	window->call_draw_function(cr);

	return FALSE;
}

static gboolean clicked_callback(
	GtkWidget* widget, GdkEventButton* event, gpointer user_data) {

	(void)widget;
	(void)event;
	Window* window = static_cast<Window*>(user_data);

	window->call_click_function();
	return TRUE;
}



Window::Window(
	size_t width, size_t height, std::unique_ptr<DrawerObject>&& drawer_)
	:	window(nullptr)
	,	darea(nullptr)
	,	drawer(std::forward<std::unique_ptr<DrawerObject>>(drawer_))
	,	click_function(nullptr)
	,	click_function_data(nullptr) {

	gtk_init (nullptr, nullptr);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

	darea = gtk_drawing_area_new();
	gtk_widget_set_size_request (darea, width, height);

	g_signal_connect (darea, "draw", G_CALLBACK(draw_callback),  this);

	gtk_window_set_position(     GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size( GTK_WINDOW(window), width, height);
	gtk_window_set_title(        GTK_WINDOW(window), "s7r");

	gtk_container_add(GTK_CONTAINER (window), darea);
	gtk_widget_show(darea);
	gtk_widget_show(window);
}

void Window::set_click_function(void* (*click_function_)(void*), void* data) {
	this->click_function = click_function_;
	this->click_function_data = data;
	g_signal_connect(window, "button-press-event", G_CALLBACK(clicked_callback), this);
}

void Window::loop() {
	gtk_main();
}

void Window::requestRedraw() {
	if (GTK_IS_WIDGET(darea)) {
		gtk_widget_queue_draw(darea);
	}
}

void Window::call_click_function() {
	if (click_function != nullptr) {
		this->click_function(click_function_data);
	}
}

void Window::call_draw_function(cairo_t* cr) {
	if (drawer != nullptr) {
		drawer->doDrawing(cr);
	}
}

void DrawerObject::setDrawColour(float r, float g, float b) {
	cairo_set_source_rgb(getContext(), r, g, b);
}

void DrawerObject::setDrawLineWidth(float w) {
	cairo_set_line_width(getContext(), w);
}

void DrawerObject::drawCircle(Point p, float radius) {
	cairo_arc(getContext(), p.x, p.y, radius, 0, 2*G_PI);
	stroke();
}

void DrawerObject::stroke() {
	cairo_stroke(getContext());
}