#include <stdio.h>
#include <glib.h>
#include <glib-unix.h>

static GMainLoop *loop = NULL;

static gboolean signal_handler(gpointer user_data)
{
    g_print("Signal received");
    g_main_loop_quit(loop);
    return G_SOURCE_REMOVE;
}

int main(int argc, char **argv)
{
    g_print("Hello!\n");

    g_autoptr(GMainLoop) _loop = loop = g_main_loop_new(NULL, FALSE);

    g_unix_signal_add(SIGTERM, signal_handler, NULL);

    g_print("Running main loop");
    g_main_loop_run(_loop);

    g_print("Terminating");
    return EXIT_SUCCESS;
}
