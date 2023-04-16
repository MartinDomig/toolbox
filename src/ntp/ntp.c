#include "ntp.h"
#include <arpa/inet.h>

#define NTP_PORT 123
#define TIMEOUT_SECONDS 2

static void ntp_init_packet(ntp_packet_t *packet) {
  memset(packet, 0, sizeof(ntp_packet_t));
  packet->li_vn_mode = 0x1b;
}

static time_t ntp_get_time_from_packet(ntp_packet_t *packet) {
  uint32_t seconds = ntohl(packet->transmit_timestamp.seconds);
  // uint32_t fraction = ntohl(packet->transmit_timestamp.fraction);
  return seconds - 2208988800; // 80 years worth of seconds
}

static void ntp_get_time_from_address_thread(GTask *task,
                                             gpointer source_object,
                                             gpointer task_data,
                                             GCancellable *cancellable);

void ntp_get_time_from_address_aysnc(GInetAddress *address,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data) {
  g_autoptr(GCancellable) cancellable = g_cancellable_new();
  g_autoptr(GTask) task = g_task_new(NULL, cancellable, callback, user_data);
  g_task_set_source_tag(task, ntp_get_time_from_address_aysnc);
  g_task_set_task_data(task, g_object_ref(address), g_object_unref);
  g_task_run_in_thread(task, ntp_get_time_from_address_thread);
}

time_t ntp_get_time_from_address_finish(GAsyncResult *result, GError **error) {
  g_return_val_if_fail(g_task_is_valid(result, NULL), 0);
  g_return_val_if_fail(
      g_async_result_is_tagged(result, ntp_get_time_from_address_aysnc), 0);
  if (g_task_had_error(G_TASK(result))) {
    g_task_propagate_pointer(G_TASK(result), error);
    return 0;
  }
  G_DEBUG_HERE();
  return g_task_propagate_int(G_TASK(result), error);
}

static void ntp_get_time_from_address_thread(GTask *task,
                                             gpointer source_object,
                                             gpointer task_data,
                                             GCancellable *cancellable) {
  GError *error = NULL; // will be propagated to the main thread
  GInetAddress *address = G_INET_ADDRESS(task_data);

  g_autoptr(GSocket) socket =
      g_socket_new(g_inet_address_get_family(address), G_SOCKET_TYPE_DATAGRAM,
                   G_SOCKET_PROTOCOL_UDP, &error);
  if (!socket) {
    g_task_return_error(task, error);
    return;
  }

  g_socket_set_timeout(socket, TIMEOUT_SECONDS);

  g_autoptr(GSocketAddress) socket_address =
      g_inet_socket_address_new(address, NTP_PORT);
  g_socket_connect(socket, socket_address, cancellable, &error);
  if (error) {
    g_task_return_error(task, error);
    return;
  }

  ntp_packet_t packet;
  ntp_init_packet(&packet);
  g_socket_send(socket, (const gchar *)&packet, sizeof(packet), cancellable,
                &error);
  if (error) {
    g_task_return_error(task, error);
    return;
  }

  g_socket_receive(socket, (gchar *)&packet, sizeof(packet), cancellable,
                   &error);
  if (error) {
    g_task_return_error(task, error);
    return;
  }

  time_t time = ntp_get_time_from_packet(&packet);
  g_task_return_int(task, time);
}
