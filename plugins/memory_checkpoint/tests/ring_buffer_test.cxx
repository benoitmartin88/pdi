/*******************************************************************************
 * Copyright (C) 2015-2019 Commissariat a l'energie atomique et aux energies alternatives (CEA)
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

#include <gtest/gtest.h>
#include <string>

#include "../ringbuffer.h"

namespace {

struct Test {
	Test(const bool overwrite) : ringBuffer(3, overwrite) {
		std::cout << "Test::Test()" << std::endl;
	}
	std::string getString() {
		return std::string(ringBuffer.c.cbegin(), ringBuffer.c.cend());
	}

	RingBuffer<char> ringBuffer;
};
}


using RingBufferAccessor = Test;


class RingBufferTest: public ::testing::Test
{
protected:
	void TearDown() override {
	}
};


TEST_F(RingBufferTest, CtorDefault)
{
	RingBuffer<char> ringBuffer;
	ASSERT_EQ(1, ringBuffer.max_size());
	ASSERT_EQ(0, ringBuffer.size());
}

TEST_F(RingBufferTest, CtorOverwritable)
{
	RingBuffer<char> ringBuffer(3, true);
	ASSERT_EQ(3, ringBuffer.max_size());
	ASSERT_EQ(0, ringBuffer.size());
}

TEST_F(RingBufferTest, CtorNotOveritable)
{
	RingBuffer<char> ringBuffer(3, false);
	ASSERT_EQ(3, ringBuffer.max_size());
	ASSERT_EQ(0, ringBuffer.size());
}

TEST_F(RingBufferTest, InsertOverwritable)
{
	RingBufferAccessor ringBufferAccessor(true);

	ASSERT_EQ(3, ringBufferAccessor.ringBuffer.max_size());
	ASSERT_EQ(0, ringBufferAccessor.ringBuffer.size());

	ringBufferAccessor.ringBuffer.put('a');
	ASSERT_EQ(1, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("a", ringBufferAccessor.getString());

	ringBufferAccessor.ringBuffer.put('b');
	ASSERT_EQ(2, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('b', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("ab", ringBufferAccessor.getString());

	ringBufferAccessor.ringBuffer.put('c');
	ASSERT_EQ(3, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('c', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("abc", ringBufferAccessor.getString());

	ringBufferAccessor.ringBuffer.put('d');
	ASSERT_EQ(3, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('b', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('d', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("bcd", ringBufferAccessor.getString());

	ringBufferAccessor.ringBuffer.put('e');
	ASSERT_EQ(3, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('c', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('e', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("cde", ringBufferAccessor.getString());
}

TEST_F(RingBufferTest, InsertNotOverwritable)
{
	RingBufferAccessor ringBufferAccessor(false);

	ASSERT_EQ(3, ringBufferAccessor.ringBuffer.max_size());
	ASSERT_EQ(0, ringBufferAccessor.ringBuffer.size());

	ringBufferAccessor.ringBuffer.put('a');
	ASSERT_EQ(1, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("a", ringBufferAccessor.getString());

	ringBufferAccessor.ringBuffer.put('b');
	ASSERT_EQ(2, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('b', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("ab", ringBufferAccessor.getString());

	ringBufferAccessor.ringBuffer.put('c');
	ASSERT_EQ(3, ringBufferAccessor.ringBuffer.size());
	ASSERT_EQ('a', ringBufferAccessor.ringBuffer.front());
	ASSERT_EQ('c', ringBufferAccessor.ringBuffer.back());
	ASSERT_EQ("abc", ringBufferAccessor.getString());

	EXPECT_THROW({
        ringBufferAccessor.ringBuffer.put('d');
    }, std::overflow_error);
}