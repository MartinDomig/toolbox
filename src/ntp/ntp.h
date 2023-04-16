#pragma once

#include <gio/gio.h>
#include <glib.h>
#include <stdint.h>

typedef struct {
  uint32_t seconds;
  uint32_t fraction;
} ntp_timestamp_t;

typedef struct {
  uint8_t li_vn_mode;
  uint8_t stratum;
  uint8_t poll;
  uint8_t precision;
  uint32_t root_delay;
  uint32_t root_dispersion;
  uint32_t reference_identifier;
  ntp_timestamp_t reference_timestamp;
  ntp_timestamp_t originate_timestamp;
  ntp_timestamp_t receive_timestamp;
  ntp_timestamp_t transmit_timestamp;
} ntp_packet_t;

void ntp_get_time_from_address_aysnc(GInetAddress *address,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data);
time_t ntp_get_time_from_address_finish(GAsyncResult *result, GError **error);
