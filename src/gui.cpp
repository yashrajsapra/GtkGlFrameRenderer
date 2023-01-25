#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <gtk/gtk.h>

#include "background.h"
#include "matrix.h"
#include "model.h"
#include "program.h"
#include "util.h"
#include "view.h"

// Hold init data for GTK signals:
struct signal {
	const gchar	*signal;
	GCallback	 handler;
	GdkEventMask	 mask;
};

static gboolean panning = FALSE;

static void
on_resize (GtkGLArea *area, gint width, gint height)
{
	view_set_window(width, height);
	background_set_window(width, height);
}

static gboolean
on_render (GtkGLArea *glarea, GdkGLContext *context)
{
	// Clear canvas:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw background:
	background_draw();

	// Draw model:
	// model_draw();
	draw_frames();

	// Don't propagate signal:
	return TRUE;
}

static void
on_realize (GtkGLArea *glarea)
{
	// Make current:
	gtk_gl_area_make_current(glarea);

	// Print version info:
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	std::cout << "Renderer:" << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;

	// Enable depth buffer:
	gtk_gl_area_set_has_depth_buffer(glarea, TRUE);

	// Init programs:
	programs_init();

	// Init background:
	// background_init();

	// Init model:
	model_init();

	// Get frame clock:
	GdkGLContext *glcontext = gtk_gl_area_get_context(glarea);
	GdkWindow *glwindow = gdk_gl_context_get_window(glcontext);
	GdkFrameClock *frame_clock = gdk_window_get_frame_clock(glwindow);

	// Connect update signal:
	g_signal_connect_swapped
		( frame_clock
		, "update"
		, G_CALLBACK(gtk_gl_area_queue_render)
		, glarea
		) ;

	// Start updating:
	gdk_frame_clock_begin_updating(frame_clock);
}

static gboolean
on_button_press (GtkWidget *widget, GdkEventButton *event)
{
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	if (event->button == 1)
		if (panning == FALSE) {
			panning = TRUE;
			model_pan_start(event->x, allocation.height - event->y);
		}

	return FALSE;
}

static gboolean
on_button_release (GtkWidget *widget, GdkEventButton *event)
{
	if (event->button == 1)
		panning = FALSE;

	return FALSE;
}

static gboolean
on_motion_notify (GtkWidget *widget, GdkEventMotion *event)
{
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	if (panning == TRUE)
		model_pan_move(event->x, allocation.height - event->y);

	return FALSE;
}

static gboolean
on_scroll (GtkWidget* widget, GdkEventScroll *event)
{
	switch (event->direction)
	{
	case GDK_SCROLL_UP:
		view_z_decrease();
		break;

	case GDK_SCROLL_DOWN:
		view_z_increase();
		break;

	default:
		break;
	}

	return FALSE;
}

static void
connect_signals (GtkWidget *widget, struct signal *signals, size_t members)
{
	FOREACH_NELEM (signals, members, s) {
		gtk_widget_add_events(widget, s->mask);
		g_signal_connect(widget, s->signal, s->handler, NULL);
	}
}

static void
connect_window_signals (GtkWidget *window)
{
	struct signal signals[] = {
		//yash change
		{ "destroy",			G_CALLBACK(gtk_main_quit),	(GdkEventMask)0			},
	};

	connect_signals(window, signals, NELEM(signals));
}

static void
connect_glarea_signals (GtkWidget *glarea)
{
	struct signal signals[] = {
		//yash change
		{ "realize",			G_CALLBACK(on_realize),		(GdkEventMask)0			},
		{ "render",			G_CALLBACK(on_render),		(GdkEventMask)0			},
		{ "resize",			G_CALLBACK(on_resize),		(GdkEventMask)0			},
		{ "scroll-event",		G_CALLBACK(on_scroll),		GDK_SCROLL_MASK		},
		{ "button-press-event",		G_CALLBACK(on_button_press),	GDK_BUTTON_PRESS_MASK	},
		{ "button-release-event",	G_CALLBACK(on_button_release),	GDK_BUTTON_RELEASE_MASK	},
		{ "motion-notify-event",	G_CALLBACK(on_motion_notify),	GDK_BUTTON1_MOTION_MASK	},
	};

	connect_signals(glarea, signals, NELEM(signals));
}

bool
gui_init (int *argc, char ***argv)
{
	// Initialize GTK:
	if (!gtk_init_check(argc, argv)) {
		fputs("Could not initialize GTK", stderr);
		return false;
	}

	return true;
}

bool
gui_run (void)
{

	//yash change typecast to gtkwidget
	GtkWidget *glarea, *mainFixed;
	// Create toplevel window, add GtkGLArea:
	GtkBuilder *m_builder = gtk_builder_new();
	m_builder = gtk_builder_new();
	gtk_builder_add_from_file(m_builder, "atlui.glade", NULL);

	GtkWidget *window = (GtkWidget *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size ((GtkWindow*)window, 1920, 1080);
	glarea = (GtkWidget *)gtk_builder_get_object(m_builder, "glareadraw");
	// GtkWidget *glarea = gtk_gl_area_new();
	mainFixed = (GtkWidget *)gtk_builder_get_object(m_builder, "mainWidget");
	gtk_container_add(GTK_CONTAINER(window), mainFixed);

	// Connect GTK signals:
	connect_window_signals(window);
	connect_glarea_signals(glarea);

	gtk_widget_show_all(window);

	// Enter GTK event loop:
	gtk_main();

	return true;
}
