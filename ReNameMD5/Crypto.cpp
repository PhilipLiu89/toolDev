//
//  Crypto.cpp
//  Cocos2dxLandlord
//
//  Created by Weidongxian on 14-5-10.
//
//

#include "Crypto.h"
#include "md5.h"
#include <stdio.h>
static unsigned char gs_salt = 0x56;

static bool securityHandler(bool flag, unsigned char *data, size_t data_len, unsigned char *key, size_t key_len) {
    bool ret = false;
    if (data == nullptr || key == nullptr) {
        return ret;
    }
    if (data_len > 0 && key_len > 0) {
        ret = true;
		unsigned char step = 1;
		unsigned char seed = 0;
		unsigned char salt = gs_salt;
		unsigned char delta = 0;
		unsigned char x = 0;
        size_t jx = 0;
        size_t iter_count = 0;
        for (auto ix = 0; ix < data_len; ++ix) {
            if (jx >= key_len) {
                jx = 0;
                step = ++iter_count % 3 + 1;
            }
            seed = key[jx];
            if (flag) {
                //encrypt data
                x = (data[ix] ^ seed) + delta + salt;
                data[ix] = x;
                delta = x;
            }
            else {
                //decrypt data
                x = (data[ix] - delta - salt) ^ seed;
                delta = data[ix];
                data[ix] = x;
            }
            jx += step;
        }
    }
    return ret;
}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = (int)encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}

Crypto::Crypto(void) {
    
}

std::string Crypto::md5(const std::string &str) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    unsigned char digest[16];
    MD5_Update(&ctx, const_cast<char *>(str.c_str()), str.length());
    MD5_Final(digest, &ctx);
    
    char result[64] = {0};
    int count = sizeof(digest) / sizeof(digest[0]);
    for (int ix = 0; ix < count; ix++) {
		sprintf_s(&(result[ix * 2]), sizeof(digest), "%02x", digest[ix]);
    }
    return result;
}

std::string Crypto::encodeBase64(const std::string &str) {
#if 0
    char *encoded_data = nullptr;
    std::string result;
    unsigned int data_len = base64Encode((const unsigned char *)str.c_str(), static_cast<unsigned int>(str.size()), &encoded_data);
    if (encoded_data != nullptr) {
        //result = std::string((const char *)encoded_data, data_len);
        result = encoded_data;
        free(encoded_data);
    }
    return result;
#else
    return base64_encode((const unsigned char *)str.c_str(), (int)str.size());
#endif
}

std::string Crypto::decodeBase64(const std::string &str) {
#if 0
    unsigned char *decoded_data = nullptr;
    std::string result;
    unsigned int data_len = base64Decode((const unsigned char *)str.c_str(), static_cast<unsigned int>(str.size()), &decoded_data);
    if (decoded_data != nullptr) {
        result = std::string((const char *)decoded_data, data_len);
        free(decoded_data);
    }
    return result;
#else
    return base64_decode(str);
#endif
}

bool Crypto::encrypt(unsigned char *data, size_t data_len, unsigned char *key, size_t key_len) {
    return securityHandler(true, data, data_len, key, key_len);
}

bool Crypto::decrypt(unsigned char *data, size_t data_len, unsigned char *key, size_t key_len) {
    return securityHandler(false, data, data_len, key, key_len);
}
