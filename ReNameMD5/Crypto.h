//
//  Crypto.h
//  Cocos2dxLandlord
//
//  Created by Weidongxian on 14-5-10.
//
//

#ifndef __Cocos2dxLandlord__Crypto__
#define __Cocos2dxLandlord__Crypto__
#include "stdafx.h"
#include <iostream>
#include <string>

class Crypto {
public:
    static std::string md5(const std::string &str);
    static std::string encodeBase64(const std::string &str);
    static std::string decodeBase64(const std::string &str);
	static bool encrypt(unsigned char *data, size_t data_len, unsigned char *key, size_t key_len);
	static bool decrypt(unsigned char *data, size_t data_len, unsigned char *key, size_t key_len);
private:
    Crypto(void);
};

#endif /* defined(__Cocos2dxLandlord__Crypto__) */
