# docimg
Extract image files from Microsoft Word documents!

## Build
This project depends on [libzip](https://libzip.org/). You will need to link the library yourself.

On Linux, it's very simple to build.
```bash
$ sudo apt install libzip-dev
$ g++ main.cpp -lzip
```
On Windows, I recommend using [vcpkg](https://github.com/microsoft/vcpkg) and Visual Studio.

I am planning to add CMake support to this project.

## How It Works

.docx files are just zip files, and images are stored in the `word/media` directory. This program just opens and reads image files in that directory and writes them somewhere else.
