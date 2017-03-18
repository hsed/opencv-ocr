# opencv-ocr
A simple OCR using Opencv + Tesseract. Using code from https://github.com/opencv/opencv_contrib/blob/master/modules/text/samples/end_to_end_recognition.cpp.

## Usage Instructions
- Have a window opened and get its title using Spy++, direct method or any other software.
- Start this program, as you type in your window, the program will detect words and output on console as well as highlight on output screen.

## Setup Instructions (Windows)
- Install Tesseract (make sure you select 'add to path' in installer.
- Get OpenCV binaries from https://github.com/cesardelgadof/OpenCVBinaries (works with VS2015/2017).
- Create new empty cpp proj, add source file to project.
- Add opencv include dir in `Project Properties > C/C++ > Additional Include Directories`.
- Add opencv lib dir in `Project Properties > Linker > General > Additional Library Directories`.
- Add opencv lib files:
  - Goto opencv lib folder.
  - Open cmd, type `dir /b *d.lib > libs.txt` (this is for debug mode libs).
  - Open libs.txt and copy/paste to `Project Properties > Linker > Input > Additional Dependencies`.
- Run in debug mode! (make sure you have a window opened with the title in `windowString`).


## Footnote
1. For testing, notepad or any simple text editor with reasonably big font works best.
1. It's easier to use prebuilt libraries, building on windows almost always causes issues.
1. To build with tesseract APIs (as required here) you need extra libs from tesseract.
