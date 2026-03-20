#include "../../include/utils/sha256.hpp"

#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <string>

std::string sha256(const std::string &text)
{
    // Deprecated, but works for now
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;

    SHA256_Init(&ctx);
    SHA256_Update(&ctx, text.data(), text.size());
    SHA256_Final(hash, &ctx);

    std::ostringstream out;
    out << std::hex << std::setfill('0');

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        // make two-character wide representations
        out << std::setw(2) << static_cast<int>(hash[i]);
    }

    return out.str();
}