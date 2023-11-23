#pragma once

#include "token.h"

class Stream {
public:
    virtual ~Stream()=default;
    virtual void read()=0;
    virtual size_t size()=0;
    virtual void clear()=0;         // clear for sub calculations
    virtual Stream& operator>>(Token*& token)=0;
    bool eof() const { return _eof; }
    void seekb() { it=0; }
    void seekp(size_t pos) { it=pos; }
    size_t pos() const { return it; }
protected:
    Stream() {}
    bool _eof {true};
    size_t it {0};
};