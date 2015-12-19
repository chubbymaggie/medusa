#ifndef __MEDUSA_UTIL_HPP__
#define __MEDUSA_UTIL_HPP__

#include <medusa/namespace.hpp>
#include <medusa/export.hpp>
#include <medusa/types.hpp>
#include <medusa/log.hpp>

#include <string>


MEDUSA_NAMESPACE_BEGIN

// ref: http://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
std::string Medusa_EXPORT Base64Encode(void const *pRawData, u32 Size);
std::string Medusa_EXPORT Base64Encode(std::string const &rRawData);

std::string Medusa_EXPORT Base64Decode(std::string const &rBase64Data);

std::string Medusa_EXPORT Sha1(void const *pData, size_t Length);
Id          Medusa_EXPORT Sha1(std::string const &Name);

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_UTIL_HPP__