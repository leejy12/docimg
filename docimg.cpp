#define _CRT_SECURE_NO_WARNINGS // FOR MSVC

#include <iostream>
#include <format>
#include <zip.h>

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: docimg <filename>\n";
        return 1;
    }

    int error;
    zip_t* document = zip_open(argv[1], 0, &error);

    if (document == NULL)
    {
        std::cout << "Could not find file: " << argv[1] << '\n';
        return 1;
    }
    std::cout << "Successfully opened file: " << argv[1] << '\n';

    constexpr static long long BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];

    const auto entries = zip_get_num_entries(document, 0);

    for (int i = 0; i < entries; i++)
    {
        std::string fileName(zip_get_name(document, i, 0));

        if (fileName.starts_with("word/media/image"))
        {
            // format of imageName is "image<n>.ext"
            std::string imageName = fileName.substr(11);

            zip_file_t* file = zip_fopen(document, fileName.c_str(), 0);
            zip_stat_t st;
            zip_stat(document, fileName.c_str(), 0, &st);

            FILE* fp = fopen(imageName.c_str(), "wb");

            // read the image file 512 bytes at a time
            const long long rep = st.size / BUFFER_SIZE;

            for (int i = 0; i < rep; i++)
            {
                zip_fread(file, buffer, BUFFER_SIZE);
                fwrite(buffer, 1, BUFFER_SIZE, fp);
            }

            zip_fread(file, buffer, st.size - BUFFER_SIZE * rep);
            fwrite(buffer, 1, st.size - BUFFER_SIZE * rep, fp);

            std::cout << std::format("Extracted {} \t{:>6} bytes.\n", imageName, st.size);

            fclose(fp);
            zip_fclose(file);
        }
    }

    zip_close(document);

    return 0;
}
