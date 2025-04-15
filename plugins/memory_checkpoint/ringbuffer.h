/*******************************************************************************
 * Copyright (C) 2025 Commissariat a l'energie atomique et aux energies alternatives (CEA)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of CEA nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include <queue>

namespace {

struct Test;    // used for unit tests

template<typename T>
class RingBuffer : private std::queue<T> {
    friend struct Test;
    using Base = std::queue<T>;
    using capacity_type = uint8_t;

public:
    RingBuffer() : RingBuffer(1, true) {
    }

    RingBuffer(const capacity_type capacity, const bool overwrite) 
    : capacity(capacity), overwrite(overwrite) {
    }
    
    void put(const T& el) {
        if(size()==capacity) {
            if(overwrite) {
                Base::pop();
            } else {
                throw std::overflow_error("Ring buffer is full.");
            }
        }
        Base::push(el);
    }

    T front() {
        return Base::front();
    }

    T back() {
        return Base::back();
    }

    T pop() {
        // TODO
    }

    T peek() {
        // TODO
    }

    void clear() noexcept {
        // TODO
    }

    capacity_type max_size() const noexcept {
        return capacity;
    }

    auto size() const {
        return std::queue<T>::size();
    }


private:
    const capacity_type capacity;
    const bool overwrite;
    // typename std::forward_list<T>::iterator it;
};

}

