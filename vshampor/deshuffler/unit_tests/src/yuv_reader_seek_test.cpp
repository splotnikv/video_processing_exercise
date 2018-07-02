/*
 * yuv_reader_seek_test.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: root
 */

#include "yuv_reader_seek.h"
#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <list>

TEST(YUVReaderSeekTest, ShouldOpenI420YUVFile)
{
    YUVReaderSeek yuv_reader_seek;
    std::string stream_filename("/msdk/MEDIASDK_ROOT/foreman_352x288_300.yuv");
    struct stat buffer;
    if (stat(stream_filename.c_str(), &buffer) != 0)
    {
        std::cout << "Stream " << stream_filename << "not found, which is required for this test case to pass\n";
        FAIL();
    }

    yuv_reader_seek.Init(stream_filename, MFX_FOURCC_I420);
    yuv_reader_seek.Seek(5);
    FAIL();
}
