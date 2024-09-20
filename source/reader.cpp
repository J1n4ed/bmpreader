#include "../headers/reader.h"

Bitmap::Bitmap(std::string * filename)
{
    filename_ = *filename;
    read();
}

Bitmap::~Bitmap()
{
    data_.clear();
}

void Bitmap::read()
{        
    file_.open(filename_, std::ios::binary|std::ios::in);

    if (file_)
    {
        std::cout << "\n\n --- READING FILE CONTENT --- \n";

        file_.read((char*)&header_, sizeof(bm_header));
            
        if (header_.file_type != 0x4D42)
        {
            std::cerr << "\nWrong file type!" << std::endl;
        }
        else
        {
            file_.read((char*)&info_, sizeof(bm_info));

            if (info_.bit_count == 32)
            {
                if (info_.size >= (sizeof(bm_info) + sizeof(bm_color)))
                {
                    file_.read((char*)&color_, sizeof(bm_color));
                    check_color_header();
                }
                else
                {
                    std::cerr << "\nNo bitmask info found!" << std::endl;
                }
            }

            file_.seekg(header_.offset_data, file_.beg);
               
            if (info_.bit_count == 32)
            {
                info_.size = sizeof(bm_info) + sizeof(bm_color);
                header_.offset_data = sizeof(bm_header) + sizeof(bm_info) + sizeof(bm_color);
                whiteMask_ = 0xFFFFFFFF;
                bits_ = 4;
            }
            else if (info_.bit_count == 24)
            {
                info_.size = sizeof(bm_header);
                header_.offset_data = sizeof(bm_header) + sizeof(bm_info);
                whiteMask_ = 0x00FFFFFF;                    
                bits_ = 3;
            }
            else
            {
                std::cerr << "\nUnsopported bitcount!" << std::endl;
            }

            header_.file_size = header_.offset_data;                

            if (info_.height < 0)
            {
                default_direction = false;
            }
                
            data_.resize(info_.width * info_.height);

            for (auto i = 0; i < info_.width * info_.height; ++i)
            {
                uint32_t pix{0};
                file_.read((char*)&pix, bits_);
                data_[i] = pix == whiteMask_ ? 0 : 1;
            }

            header_.file_size += data_.size();                           
        }            
    }
    else
    {
        std::cerr << "\nFile couldn't be opened!" << std::endl;
    }        
}

void Bitmap::paint()
{
    std::cout << "\nFile height: "  << info_.height;
    std::cout << "\nFile width: "   << info_.width;
    std::cout << "\nFile bitset: "  << info_.bit_count;
    std::cout << "\nData size: "    << data_.size();

    std::cout << "\n\n";

    unsigned char tmp;        

    std::cout << "\nwhiteMask " << whiteMask_;
    std::cout << "\ndata[0] "   << data_[0];
    std::cout << "\ndata[1] "   << data_[1];
    std::cout << "\ndata[2] "   << data_[2];            
        
    // если рисуется с нижнего левого угла
    if (default_direction)
    {

        for (int i = info_.height - 1; i > 0; --i)
        {
            for (int j = 0; j < info_.width; ++j)
            {                
                char c = data_[i * info_.width + j] == 0 ? ' ' : '#';                    
                std::cout << c;  
            }

            std::cout << '\n';
        }
    }

    // если рисуется с верхнего левого угла
    else
    {
        int counter = 0;           

        for (auto elem : data_)
        {
            if (elem == 0)
            {
                std::cout << ' ';
            }
            else
            {
                std::cout << '#';
            }

            if(counter != 0)
            {
                if (counter % info_.width == 0)
                {
                    std::cout << '\n';
                    counter = 0;
                }
            }           
                
            ++counter;
                
        }
    }        
}

/*
uint32_t Bitmap::make_stride_aligned(uint32_t align_stride) 
{
    uint32_t new_stride = row_stride;
       
    while (new_stride % align_stride != 0) 
    {
            new_stride++;
    }
        
    return new_stride;
}
*/

void Bitmap::check_color_header()
{
    bm_color expected_header;
    if (expected_header.red_mask    != color_.red_mask    ||
        expected_header.blue_mask   != color_.blue_mask   ||
        expected_header.green_mask  != color_.green_mask  ||
        expected_header.alpha_mask  != color_.alpha_mask
    )
    {
        std::cerr << "\nUnexpected color format!" << std::endl;
    }

    if (expected_header.color_space_type != color_.color_space_type)
    {
        std::cerr << "\nUnexpected color space type!" << std::endl;
    }
}