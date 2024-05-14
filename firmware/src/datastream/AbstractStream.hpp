#pragma once

#include <Arduino.h>
#include <string.h>

#include <chrono>
#include <memory>
#include <vector>
#include <dlf_type_strings.h>

#include "../dlf_cfg.h"
#include "../dlf_types.h"
#include "../dlf_util.h"
#include "../dlf_encodable.h"

inline const char *stream_type_to_string(dlf_stream_type_e t)
{
    switch (t)
    {
    case POLLED:
        return "polled";
    case EVENT:
        return "event";
    default:
        return "PROBLEM";
    }
}

namespace dlf::datastream
{

    using std::chrono::microseconds;

    class AbstractStreamHandle;
    typedef std::unique_ptr<AbstractStreamHandle> stream_handle_t;

    /**
     * Abstract class representing a source of data as well as some information (name, typeID) about it.
     */
    class AbstractStream
    {
    protected:
        AbstractStream(Encodable &dat, String id) : src(dat), id(id)
        {
                Serial.printf("ABSTRACT STREAM CONST ENC %s\n", dat.type_id);
                Serial.printf("ABSTRACT STREAM SRC SET %s\n", src.type_id);

        }

    public:
        const Encodable src;
        const String id;

        /**
         * @brief Creates a new, linked StreamHandle
         * @param tick_interval
         * @param idx
         * @return
         */
        virtual std::unique_ptr<AbstractStreamHandle> handle(microseconds tick_interval, dlf_stream_idx_t idx) = 0;

        virtual dlf_stream_type_e type() = 0;

        inline size_t data_size()
        {
            return src.data_size;
        }

        inline const uint8_t *data_source()
        {
            return src.data;
        }
    };

    typedef std::vector<AbstractStream *> streams_t;

} // namespace dlf::datastream
