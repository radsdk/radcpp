#include <gtest/gtest.h>
#include <rad/IO/ImageIO.h>

TEST(Core, ImageIO)
{
    rad::ImageU8 image;
    const int w = 256;
    const int h = 256;
    const int c = 4;
    const int tileSize = 32;
    image.Allocate(w, h, c);
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            int tileX = j / tileSize;
            int tileY = i / tileSize;
            if ((tileX + tileY) % 2)
            {
                if (tileX < w / tileSize / 2)
                {
                    image.SetPixel4(i, j, 255, 255, 255, 255);
                }
                else
                {
                    image.SetPixel4(i, j, 179, 179, 179, 255);
                }
            }
            else
            {
                image.SetPixel4(i, j, 0, 0, 0, 255);
            }
        }
    }
    image.WritePNG("checkerboard.png");
    image.Free();
    image.GetInfo("checkerboard.png");
    EXPECT_EQ(image.m_width, w);
    EXPECT_EQ(image.m_height, h);
    EXPECT_EQ(image.m_channelCount, c);
}
