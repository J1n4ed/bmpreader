// bmp reader class

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#pragma pack(push, 1)

struct bm_header
{
    uint16_t file_type{0x4D42};
    uint32_t file_size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset_data{0};
};

struct bm_info
{
    uint32_t size{0};
    int32_t width{0};
    int32_t height{0};
    
    uint16_t planes{1};
    uint16_t bit_count{0};
    uint32_t compression{0};
    uint32_t size_image{0};

    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};

    uint32_t colors_used{0};
    uint32_t colors_important{0};
};

struct bm_color
{
    uint32_t red_mask           {0x00ff0000};
    uint32_t green_mask         {0x0000ff00};
    uint32_t blue_mask          {0x000000ff};
    uint32_t alpha_mask         {0xff000000};
    uint32_t color_space_type   {0x73524742};
    uint32_t unused[16]{0};
};

#pragma pack(pop)

class Bitmap
{
    public:

    Bitmap() = delete;

    Bitmap(std::string * filename);

    ~Bitmap();

    void read();

    void paint();

    private:

    bm_header header_;
    bm_info info_;
    bm_color color_;
    std::vector<uint32_t> data_;
    bool default_direction = true;
    uint32_t whiteMask_;
    int bits_;

    std::fstream file_;
    std::string filename_;

    // uint32_t row_stride = 0;    

    void check_color_header();

    // uint32_t make_stride_aligned(uint32_t align_stride);
};