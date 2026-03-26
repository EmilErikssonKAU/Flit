#include "../../include/utils/zLibCompression.hpp"

#include <string>
#include <stdexcept>
#include <zlib.h>

std::string z_compress(const std::string &input)
{
    uLongf output_size = compressBound(input.size());
    std::string output(output_size, '\0');

    const int result = compress2(
        reinterpret_cast<Bytef *>(&output[0]),
        &output_size,
        reinterpret_cast<const Bytef *>(input.data()),
        input.size(),
        Z_BEST_COMPRESSION);

    if (result != Z_OK)
    {
        throw std::runtime_error("zlib compression failed");
    }

    output.resize(output_size);
    return output;
}

std::string z_decompress(const std::string &input)
{
    z_stream stream{};
    stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input.data()));
    stream.avail_in = static_cast<uInt>(input.size());

    if (inflateInit(&stream) != Z_OK)
    {
        throw std::runtime_error("inflateInit failed");
    }

    std::string output;
    char buffer[4096];

    int result = Z_OK;
    do
    {
        stream.next_out = reinterpret_cast<Bytef *>(buffer);
        stream.avail_out = sizeof(buffer);

        result = inflate(&stream, Z_NO_FLUSH);

        if (result != Z_OK && result != Z_STREAM_END)
        {
            inflateEnd(&stream);
            throw std::runtime_error("inflate failed");
        }

        const std::size_t produced = sizeof(buffer) - stream.avail_out;
        output.append(buffer, produced);

    } while (result != Z_STREAM_END);

    inflateEnd(&stream);
    return output;
}