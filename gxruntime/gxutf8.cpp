#include "gxutf8.h"

int UTF8::measureCodepoint(char chr) {
    if ((chr & 0x80) == 0x00) {
        //first bit is 0: treat as ASCII
        return 1;
    }
    //first bit is 1, number of consecutive 1 bits at the start is length of codepoint
    int len = 0;
    while (((chr >> (7 - len)) & 0x01) == 0x01) {
        len++;
    }
    return len;
}
int UTF8::decodeCharacter(const char* buf, int index) {
    int codepointLen = measureCodepoint(buf[index]);
    if (codepointLen == 1) {
        return buf[index];
    } else {
        //decode first byte by skipping all bits that indicate the length of the codepoint
        int newChar = buf[index] & (0x7f >> codepointLen);
        for (int j = 1; j < codepointLen; j++) {
            //decode all of the following bytes, fixed 6 bits per byte
            newChar = (newChar << 6) | (buf[index + j] & 0x3f);
        }
        return newChar;
    }
}

int UTF8::length(const std::string& str) {
    int utf8Len = 0;
    for (int i=0;i<str.size();) {
        utf8Len++;
        i += measureCodepoint(str[i]);
    }
    return utf8Len;
}

int UTF8::find(const std::string& str, const std::string& sstr, int from) {
    int utf8Index = 0;
    int bytesFrom = -1;
    for (int i=0;i<str.size();) {
        if (from == utf8Index)
        {
            bytesFrom = i;
            break;
        }
        utf8Index++;
        i += measureCodepoint(str[i]);
    }
    if (bytesFrom < 0) { return -1; }
    int bytesResult = str.find(sstr,bytesFrom);
    int result = -1;
    for (int i=bytesFrom;i<str.size();) {
        if (bytesResult == i)
        {
            result = utf8Index;
            break;
        }
        utf8Index++;
        i += measureCodepoint(str[i]);
    }
    return result;
}

std::string UTF8::substr(const std::string& str, int start, int length) {
    int utf8Index = 0;
    int bytesStart = str.size();
    int bytesLength = str.size();
    for (int i=0;i<str.size();) {
        if (start == utf8Index)
        {
            bytesStart = i;
        }
        if ((start+length) == utf8Index)
        {
            bytesLength = i - bytesStart;
            break;
        }
        utf8Index++;
        i += measureCodepoint(str[i]);
    }
    return str.substr(bytesStart, bytesLength);
}

int UTF8::encodeCharacter(int chr, char* result) {
    //fits in standard ASCII, just return the char as-is
    if ((chr & 0x7f) == chr) {
        if (result != nullptr) { result[0] = chr; }
        return 1;
    }

    int len = 1;

    //determine most of the bytes after the first one
    while ((chr & (~0x3f)) != 0x00) {
        if (result != nullptr) { result[len - 1] = 0x80 | (chr & 0x3f); }
        chr >>= 6;
        len++;
    }

    //determine the remaining byte(s): if the number of free bits in
    //the first byte isn't enough to fit the remaining bits,
    //add another byte
    char firstByte = 0x00;
    for (int i = 0; i < len; i++) {
        firstByte |= (0x1 << (7-i));
    }

    if (((firstByte | (0x1 << (7-len))) & chr) == 0x00) {
        //it fits!
        firstByte = firstByte | chr;
        if (result != nullptr) { result[len - 1] = firstByte; }
    } else {
        //it doesn't fit: add another byte
        if (result != nullptr) { result[len - 1] = 0x80 | (chr & 0x3f); }
        chr >>= 6;
        firstByte = (firstByte | (0x1 << (7 - len))) | chr;
        len++;
        if (result != nullptr) { result[len - 1] = firstByte; }
    }

    if (result != nullptr) {
        //flip the result
        for (int i = 0; i < len/2; i++) {
            char b = result[i];
            result[i] = result[len - 1 - i];
            result[len - 1 - i] = b;
        }
    }

    return len;
}

void UTF8::popBack(std::string& str) {
    for (int i=str.size()-1;i>=0;i--) {
        char chr = str[i];
        str.pop_back();
        if (((chr&0x80) == 0) || ((chr&0xc0) == 0xc0)) {
            break;
        }
    }
}

std::wstring UTF8::convertToUtf16(const std::string& str) {
    std::wstring result = L"";

    for (int i=0;i<str.size();) {
        result.push_back(decodeCharacter(str.c_str(),i));
        i+=measureCodepoint(str[i]);
    }

    return result;
}
