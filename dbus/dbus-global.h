#ifndef DBUS_GLOBAL_H
#define DBUS_GLOBAL_H

#include "dbus.h"
#include "dbus-dataslot.h"
#include "dbus-mempool.h"

/** Avoid caching too many messages */
#define MAX_MESSAGE_CACHE_SIZE    5

typedef struct {
  /* Member in dbus-bus.c */
  dbus_int32_t bus_data_slot;

  /* Member in dbus-connection.c */
  DBusDataSlotAllocator slot_allocator_conn;

  /* Member in dbus-message.c */
  DBusMessage *message_cache[MAX_MESSAGE_CACHE_SIZE];
  int message_cache_count;
  dbus_bool_t message_cache_shutdown_registered;

  /* Member in dbus-pending.c */
  dbus_int32_t notify_user_data_slot;
  DBusDataSlotAllocator slot_allocator_pending;

  /* Member in dbus_thread.c */
  int thread_init_generation;
  DBusRMutex *global_locks[_DBUS_N_GLOBAL_LOCKS];

  /* Member in dbus_mempool.c */
  DBusMemPool *list_pool;

  /* Member in dbus_memory.c and dbus-internals.c */
  int _dbus_current_generation;
} dbus_global_t;

dbus_global_t* dbus_global_get(void);

/** The slot we have reserved to store BusData.
 * Protected by _DBUS_LOCK_connection_slots.
 */
#define bus_data_slot dbus_global_get()->bus_data_slot

#define slot_allocator_conn dbus_global_get()->slot_allocator_conn

/* Protected by _DBUS_LOCK (message_cache) */
#define message_cache dbus_global_get()->message_cache
#define message_cache_count dbus_global_get()->message_cache_count
#define message_cache_shutdown_registered dbus_global_get()->message_cache_shutdown_registered

/* protected by _DBUS_LOCK_pending_call_slots */
#define notify_user_data_slot dbus_global_get()->notify_user_data_slot

#define slot_allocator_pending dbus_global_get()->slot_allocator_pending

/* Protected by _dbus_threads_lock_platform_specific() */
#define thread_init_generation dbus_global_get()->thread_init_generation

/* Protected by _dbus_threads_lock_platform_specific() */
#define global_locks dbus_global_get()->global_locks

/* Protected by _DBUS_LOCK (list) */
#define list_pool dbus_global_get()->list_pool

/**
 * _dbus_current_generation is used to track each
 * time that dbus_shutdown() is called, so we can
 * reinit things after it's been called. It is simply
 * incremented each time we shut down.
 */
#define _dbus_current_generation dbus_global_get()->_dbus_current_generation

#endif // DBUS_GLOBAL_H