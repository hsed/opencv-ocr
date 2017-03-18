# opencv-ocr
A simple OCR using Opencv + Tesseract.

## Instructions (Windows)
- Install Tesseract (make sure you select 'add to path' in installer.
- Get OpenCV binaries from https://github.com/cesardelgadof/OpenCVBinaries (works with VS2015/2017)
- Create new empty cpp proj, add source file to project
- Add opencv include dir in `Project Properties > C/C++ > Additional Include Directories`.
- Add opencv lib dir in `Project Properties > Linker > General > Additional Library Directories`.
- Add opencv lib dir in `Project Properties > Linker > General > Additional Library Directories`.
- Add opencv lib files:
  - Goto opencv lib folder.
  - Open cmd, type `dir /b *d.lib > libs.txt` (this is for debug mode libs).
  - Open libs.txt and copy/paste to `Project Properties > Linker > Input > Additional Dependencies`.
- Run in debug mode! (make sure you have a window with the title in `windowString`.

## Footnote
Note: It's easier to use prebuilt libraries, building on windows almost always causes issues.
Note 2: Building especially for tesseract you need extra libs from tesseract.
