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

#include "../ringbuffer.h"


class RingBufferTest: public ::testing::Test
{
protected:
	void TearDown() override {
	}
};

/* Name:                RingBufferTest.RingBufferInit
 *
 * Tested functions:    RingBuffer()
 *
 * Description:         Test the RingBuffer constructor
 */
TEST_F(RingBufferTest, RingBufferCtorDefault)
{
	RingBuffer<char> ringBuffer1;
	// TODO
}

TEST_F(RingBufferTest, RingBufferCtorOverwritable)
{
	RingBuffer<char> ringBuffer1(3, true);
	// TODO
}

TEST_F(RingBufferTest, RingBufferCtorNotOveritable)
{
	RingBuffer<char> ringBuffer1(3, false);
	// TODO
}