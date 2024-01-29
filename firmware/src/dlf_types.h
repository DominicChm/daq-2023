#pragma once
#include <Arduino.h>
#include <FS.h>

#include <vector>

#include "dlf_util.h"

#define HEADER_VERSION 0

typedef uint64_t dlf_tick_t;
typedef uint32_t dlf_time_us_t;
typedef uint16_t dlf_stream_idx_t;

struct dlf_sample_queue_entry {
    File *f;
    std::vector<uint8_t> *buf;
};

enum dlf_file_state_e : int8_t {
    // Errors are (-)
    FILE_OPEN_ERROR = -7,
    SYNC_CREATE_ERROR = -6,
    STREAM_CREATE_ERROR = -5,
    FLUSHER_CREATE_ERROR = -4,
    QUEUE_FULL = -3,
    HEAP_FULL = -2,
    INIT_ERROR = -1,

    UNINITIALIZED = 0,
    INITIALIZED = 1,
    LOGGING = 2,
    FLUSHING = 3,
    FLUSHED = 4,
};

enum dlf_stream_type_e : uint8_t {
    POLLED,
    EVENT
};

struct dlf_stream_header_t {
    char type_id[32];  // Data type identifier. Used to select a parser. (ie "int", "float", "struct arbitrary")
    char id[32];       // Unique identifier for this specific stream
    char notes[128];   // Anything that needs to be communicated about this data stream.
};

struct dlf_polled_stream_header_t : dlf_stream_header_t {
    dlf_tick_t tick_interval;  // Interval on which this stream is collected.
    dlf_tick_t tick_phase;     // Tick offset defining when this stream starts
};

struct dlf_event_stream_header_t : dlf_stream_header_t {
};

template <typename meta_t, size_t NUM_STREAMS>
struct dlf_header_t {
    uint16_t magic = 0x8414;     // IDs DLF files. Also allows auto-detection of LSB/MSB encoding.
    dlf_time_us_t tick_base_us;  // Base time interval, in us. Limits how fast samples will be stored.

    char application[32];                 // An arbitrary application-specific identifier. Used to select a metadata parser.
    uint32_t meta_size = sizeof(meta_t);  // Metadata size stored in case there is no metadata parser available
    meta_t meta;                          // Metadata. Can be application-specific

    uint16_t num_streams = NUM_STREAMS;
    // Next: Array of dlf_polled_stream_header_t OR dlf_event_stream_header_t
};
struct dlf_run_header_t {
    dlf_time_us_t tick_base_us;  // Base time interval, in us. Limits how fast samples can be stored.
};

struct dlf_logfile_header_t {
    uint16_t magic = 0x8414;  // IDs DLF files. Also allows auto-detection of LSB/MSB encoding.
    dlf_stream_type_e stream_type;
    dlf_tick_t tick_span;  // Total number of ticks this file spans.

    uint16_t num_streams;

    // Next: Array of dlf_polled_stream_header_t OR dlf_event_stream_header_t
};