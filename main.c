#include <stdio.h>
#include <glib.h>
#include <glib-unix.h>

static GMainLoop *loop = NULL;
static gchar *server = NULL;

static GOptionEntry entries[] =
    {
        {"host", 'h', 0, G_OPTION_ARG_STRING, &server, "Host name to look up", "host"},
        {NULL}};

static void parse_options(int argc, char **argv)
{
    g_autoptr(GOptionContext) context = g_option_context_new("- perform a DNS lookup");
    g_option_context_add_main_entries(context, entries, NULL);

    g_autoptr(GError) error = NULL;
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("option parsing failed: %s\n", error->message);
        exit(EXIT_FAILURE);
    }

    if (!server)
    {
        g_print("No host name given. %s", g_option_context_get_help(context, TRUE, NULL));
        exit(EXIT_FAILURE);
    }
}

static gboolean signal_handler(gpointer user_data)
{
    g_print("Signal received");
    g_main_loop_quit(loop);
    return G_SOURCE_REMOVE;
}

int main(int argc, char **argv)
{
    parse_options(argc, argv);

    g_autoptr(GMainLoop) _loop = loop = g_main_loop_new(NULL, FALSE);

    g_unix_signal_add(SIGTERM, signal_handler, NULL);

    g_print("Running main loop");
    g_main_loop_run(_loop);

    g_print("Terminating");
    return EXIT_SUCCESS;
}
