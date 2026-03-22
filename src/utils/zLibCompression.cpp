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

std::string z_decompress(const std::string &input, std::size_t original_size)
{
    uLongf output_size = original_size;
    std::string output(output_size, '\0');

    const int result = uncompress(
        reinterpret_cast<Bytef *>(&output[0]),
        &output_size,
        reinterpret_cast<const Bytef *>(input.data()),
        input.size());

    if (result != Z_OK)
    {
        throw std::runtime_error("zlib decompression failed");
    }

    output.resize(output_size);
    return output;
}
