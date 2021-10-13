#define _CRT_SECURE_NO_WARNINGS  // FOR MSVC

#include <iostream>
#include <zip.h>

bool extract_image(zip_t* archive, int nth)
{
    // Try all image file formats: .jpeg, .jpg, .png, .gif one at a time
    zip_file_t* file;

    char name_buf[32];
    const char* image_file_formats[] = { ".jpeg", ".jpg", ".png", ".gif" };
    const int num = sizeof(image_file_formats) / sizeof(image_file_formats[0]);

    for (int i = 0; i < num; i++)
    {
        memset(name_buf, 0, sizeof(name_buf));
        sprintf(name_buf, "word/media/image%d%s", nth, image_file_formats[i]);

        file = zip_fopen(archive, name_buf, 0);
        if (file != NULL)
        {
            zip_stat_t st;
            zip_stat(archive, name_buf, 0, &st);
            memset(name_buf, 0, sizeof(name_buf));
            sprintf(name_buf, "image%d%s", nth, image_file_formats[i]);

            FILE* fp = fopen(name_buf, "wb");

            // read the image file 512 bytes at a time

            constexpr long long BUFFER_SIZE = 512;
            char contents_buf[BUFFER_SIZE];

            long long rep = st.size / BUFFER_SIZE;
            for (int i = 0; i < rep; i++)
            {
                zip_fread(file, contents_buf, BUFFER_SIZE);
                fwrite(contents_buf, 1, BUFFER_SIZE, fp);
            }
            zip_fread(file, contents_buf, st.size - BUFFER_SIZE * rep);
            fwrite(contents_buf, 1, st.size - BUFFER_SIZE * rep, fp);

            fclose(fp);
            zip_fclose(file);

            return true;
        }
    }

    // image was not found. (file == NULL)
    return false;
}

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: docimg <filename>\n\n";
        return 1;
    }

    int error;
    zip_t* document = zip_open(argv[1], 0, &error);

    if (document == NULL)
    {
        std::cout << "Could not find file '" << argv[1] << "'\n\n";
        return 1;
    }

    for (int i = 1; i < 10; i++)
        if (extract_image(document, i) == false)
            break;

    zip_close(document);

    return 0;
}
