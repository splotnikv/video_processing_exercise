/*
 * yuv_reader_seek_test.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: root
 */

#include "yuv_reader_seek_i420.h"
#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <list>
#include <algorithm>
#include <memory.h>

TEST(YUVReaderSeekTest, ShouldOpenI420YUVFile)
{
    YUVReaderSeekI420 yuv_reader_seek;
    std::string stream_filename("/msdk/MEDIASDK_STREAMS/YUV/foreman_352x288_300.yuv");
    struct stat buffer;
    if (stat(stream_filename.c_str(), &buffer) != 0)
    {
        std::cout << "Stream " << stream_filename << " not found, which is required for this test case to pass\n";
        FAIL();
    }

    ASSERT_EQ(MFX_ERR_NONE, yuv_reader_seek.Init(stream_filename, 352, 288));
}

TEST(YUVReaderSeekTest, ShouldSeekInOpenFile)
{
    YUVReaderSeekI420 yuv_reader_seek;
    std::string stream_filename("/msdk/MEDIASDK_STREAMS/YUV/foreman_352x288_300.yuv");
    struct stat buffer;
    if (stat(stream_filename.c_str(), &buffer) != 0)
    {
        std::cout << "Stream " << stream_filename << " not found, which is required for this test case to pass\n";
        FAIL();
    }

    CSmplYUVReader yuv_reader_ref;
    std::list<msdk_string> input(1, stream_filename);
    ASSERT_EQ(MFX_ERR_NONE, yuv_reader_seek.Init(stream_filename, 352, 288));
    ASSERT_EQ(MFX_ERR_NONE, yuv_reader_ref.Init(input, MFX_FOURCC_I420));
    mfxU32 seek_frame_num = 5;

    mfxFrameSurface1 ref_surf = { 0 }, seek_surf = { 0 };
    ref_surf.Info.Width = 352;
    ref_surf.Info.Height = 288;
    ref_surf.Info.FourCC = MFX_FOURCC_NV12;
    ref_surf.Data.Pitch = 352;
    seek_surf.Data.Pitch = 352;

    mfxU32 luma_size = ref_surf.Info.Width * ref_surf.Info.Height;
    mfxU32 chroma_size = 2 * ref_surf.Info.Width * ref_surf.Info.Height / 4;

    seek_surf.Info = ref_surf.Info;

    ref_surf.Data.Y = (mfxU8*) malloc(luma_size);
    ref_surf.Data.UV = (mfxU8*) malloc(chroma_size);

    seek_surf.Data.Y = (mfxU8*) malloc(luma_size);
    seek_surf.Data.UV = (mfxU8*) malloc(chroma_size);

    for (mfxU32 i = 0; i < seek_frame_num; ++i)
    {
        ASSERT_EQ(MFX_ERR_NONE,yuv_reader_ref.LoadNextFrame(&ref_surf));
    }

    ASSERT_EQ(MFX_ERR_NONE,yuv_reader_ref.LoadNextFrame(&ref_surf));
    yuv_reader_seek.Seek(seek_frame_num);
    ASSERT_EQ(MFX_ERR_NONE, yuv_reader_seek.LoadNextFrame(&seek_surf));

    EXPECT_TRUE(std::equal(ref_surf.Data.Y, ref_surf.Data.Y + luma_size, seek_surf.Data.Y));
    EXPECT_TRUE(std::equal(ref_surf.Data.UV, ref_surf.Data.UV + chroma_size, seek_surf.Data.UV));

    free(ref_surf.Data.Y);
    free(ref_surf.Data.UV);
    free(seek_surf.Data.Y);
    free(seek_surf.Data.UV);
}
