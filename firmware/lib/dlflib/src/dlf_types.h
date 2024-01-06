#pragma once
#include <Arduino.h>

#include <vector>

#include "dlf_util.h"

#define HEADER_VERSION 0

typedef uint64_t dlf_tick_t;
typedef uint32_t dlf_time_us_t;
typedef uint16_t dlf_stream_idx_t;

typedef std::vector<DLFDataStream*> dlf_streams_t;

enum dlf_file_err_e : uint8_t {
    NONE,
    QUEUE_FULL,
    HEAP_FULL,
    INIT_ERROR,
};

enum dlf_stream_type_e : uint8_t {
    UNKNOWN,
    POLLED,
    EVENT
};

struct dlf_stream_header_t {
    dlf_stream_type_e type;
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