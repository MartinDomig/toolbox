#include <stdio.h>
#include <glib.h>
#include <glib-unix.h>
#include <gio/gio.h>

#include "ntp.h"

static void resolved_callback(GObject *source, GAsyncResult *result, gpointer data);
static void time_received_callback(GObject *source, GAsyncResult *result, gpointer data);

static GResolver *resolver = NULL;
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
    g_message("Signal received");
    g_main_loop_quit(loop);
    return G_SOURCE_REMOVE;
}

int main(int argc, char **argv)
{
    parse_options(argc, argv);

    g_autoptr(GMainLoop) _loop = loop = g_main_loop_new(NULL, FALSE);

    g_unix_signal_add(SIGTERM, signal_handler, NULL);

    g_info("resolving %s...", server);
    g_autoptr(GResolver) _resolver = resolver = g_resolver_get_default();
    g_resolver_lookup_by_name_async(_resolver, server, NULL, resolved_callback, NULL);

    g_info("Running main loop");
    g_main_loop_run(_loop);

    g_info("Terminating");
    return EXIT_SUCCESS;
}

static void resolved_callback(GObject *source, GAsyncResult *result, gpointer data)
{
    g_autoptr(GError) error = NULL;
    GList *addresses = g_resolver_lookup_by_name_finish(resolver, result, &error);

    for (GList *a = addresses; a; a = a->next)
    {
        GInetAddress *address = G_INET_ADDRESS(a->data);
        ntp_get_time_from_address_aysnc(address, time_received_callback, NULL);
    }

    g_resolver_free_addresses(addresses);
}

static void time_received_callback(GObject *source, GAsyncResult *result, gpointer data)
{
    g_autoptr(GError) error = NULL;
    time_t time = ntp_get_time_from_address_finish(result, &error);
    if (error)
    {
        g_warning("Error getting time from NTP server: %s", error->message);
        return;
    }

    g_message("Got time from NTP server: %s", ctime(&time));
}
