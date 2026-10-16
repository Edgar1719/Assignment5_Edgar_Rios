#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "pixel.h"

void average_colors(std::vector<Pixel> &pixel_list);
void flip_vertically(std::vector<Pixel> &pixel_list);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Incorrect amount of arguments" << std::endl;
        return 1;
    }

    // Open the file provided through the command line
    std::ifstream input_file(argv[1]);

    if(!input_file.is_open())
    {
        std::cout << "Could not open input file" << std::endl;
        return 1;
    }

    std::vector<Pixel> pixel_list;
    std::string line;

    // Read each line until the end of the file
    while(std::getline(input_file, line))
    {
        if(line.empty())
        {
            continue;
        }

        Pixel pixel;

        // Find the positions of the four commas
        size_t comma1 = line.find(',');
        size_t comma2 = line.find(',', comma1 + 1);
        size_t comma3 = line.find(',', comma2 + 1);
        size_t comma4 = line.find(',', comma3 + 1);

        if(comma1 == std::string::npos ||
           comma2 == std::string::npos ||
           comma3 == std::string::npos ||
           comma4 == std::string::npos)
        {
            std::cout << "Invalid line in file" << std::endl;
            return 1;
        }

        // substr extracts the text between commas.
        // stoi converts a string to an integer.
        // stof converts a string to a float.
        pixel.x = std::stoi(line.substr(0, comma1));

        pixel.y = std::stoi(line.substr(
            comma1 + 1, comma2 - comma1 - 1));

        pixel.r = std::stof(line.substr(
            comma2 + 1, comma3 - comma2 - 1));

        pixel.g = std::stof(line.substr(
            comma3 + 1, comma4 - comma3 - 1));

        pixel.b = std::stof(line.substr(comma4 + 1));

        // Store this pixel in the vector
        pixel_list.push_back(pixel);
    }

    input_file.close();

    std::cout << "Pixels read = "
              << pixel_list.size() << std::endl;

    // Verify the expected number of pixels
    if(pixel_list.size() != 512 * 256)
    {
        std::cout << "Incorrect number of pixels" << std::endl;
        return 1;
    }

    // Calculate the average RGB colors
    average_colors(pixel_list);

    // Flip the pixels vertically
    flip_vertically(pixel_list);

    // Create the output file
    std::ofstream output_file("flipped.dat");

    if(!output_file.is_open())
    {
        std::cout << "Could not create output file" << std::endl;
        return 1;
    }

    // Preserve useful precision for float values
    output_file << std::setprecision(9);

    // Write all pixels to the output file
    for(std::size_t i = 0; i < pixel_list.size(); i++)
    {
        output_file << pixel_list[i].x << ","
                    << pixel_list[i].y << ","
                    << pixel_list[i].r << ","
                    << pixel_list[i].g << ","
                    << pixel_list[i].b << std::endl;
    }

    output_file.close();

    std::cout << "Created flipped.dat" << std::endl;

    return 0;
}


void average_colors(std::vector<Pixel> &pixel_list)
{
    double total_r = 0;
    double total_g = 0;
    double total_b = 0;

    int size = pixel_list.size();

    if(size == 0)
    {
        return;
    }

    for(int i = 0; i < size; i++)
    {
        total_r += pixel_list[i].r;
        total_g += pixel_list[i].g;
        total_b += pixel_list[i].b;
    }

    std::cout << "Average red = "
              << total_r / size << std::endl;

    std::cout << "Average green = "
              << total_g / size << std::endl;

    std::cout << "Average blue = "
              << total_b / size << std::endl;
}


void flip_vertically(std::vector<Pixel> &pixel_list)
{
    int height = 256;

    // Create a new vector to hold the flipped pixels
    std::vector<Pixel> flipped_list(pixel_list.size());

    for(std::size_t i = 0; i < pixel_list.size(); i++)
    {
        Pixel pixel = pixel_list[i];

        // Reverse the y coordinate without changing x
        pixel.y = height - 1 - pixel.y;

        // Put the pixel at its new position.
        // The file is ordered by x, then y.
        int index = pixel.x * height + pixel.y;

        flipped_list[index] = pixel;
    }

    // Replace the original vector with the flipped vector
    pixel_list = flipped_list;
}

