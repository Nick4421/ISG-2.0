# **ISG-2.0**

This is a way to embed files in a video. Taking inspiration from [DvorakDwarf's infinite storage glitch project](https://github.com/DvorakDwarf/Infinite-Storage-Glitch), I decided to make my own version using C as well as some shell scripting. The basic idea is to embed a file in a video, upload that video somewhere, such as YouTube, and essentially use that as a file system. Because YouTube has no limit to how many videos a user can upload, you can theoretically store as many files there as you want (although this is probably against their terms of service, so use with caution).

## **How to Download**

I wrote this using MacOS in a Linux environment, and making it run on other peoples machines the same can be difficult, so if your version doesn't work, here are a few things to try:
- Download the correct version of [ffmpeg here](https://ffmpeg.org/download.html) and replace the existing executable located in ```src/executables```.
- Build your own versions of ```pamscale``` and ```pamenlarge``` from the [Netpbm library here](https://sourceforge.net/projects/netpbm/files/) and replace the existing executables in ```src/executables```.
    - There are instructions on how to build it in the library itself.
    - List of all the [Netpbm programs](https://netpbm.sourceforge.net/doc/directory.html), search for ```pamscale``` and ```pamenlarge``` to see what they actually do and how they work if you are interested.
- Compile your own executables from the source code in ```src/src_code``` and replace the existing executables in ```src/executables```.
    - There is a Makefile in ```src/src_code```, but the two source files ```encode.c``` and ```decode.c``` don't have any dependencies outside of the standard libraries, so you probably don't even need the Makfile when compiling.
- The shell script uses ANSI format in some places to make the terminal output look nicer, so if you don't have this, it might look screwed up, but it won't affect the functionality of the program.

## **How to Use**

Simply run the executable ```run```, and you'll be prompted to encode or decode a file.

### *Encoding a File:*

- You'll be prompted to enter the file path to your desired file, with the current directory being ```ISG-2.0```.
    - If the file path does not lead to a file, you can either enter another file path or go back to the main menu.
- You'll be prompted to enter the name you want the encoded video to have.
    - If you enter a name, it must end in "```.mp4```". If it doesn't, you'll be prompted to re-enter a name.
    - If you just press enter and leave the name blank, it will default to being named "```output.mp4```".
- You'll finally be prompted to enter a path to where the final video will be stored.
    - If the file path leads to an invalid directory, you will be prompted to re-enter a path to a valid directory.
    - If you press enter and leave this blank, the final video will be placed in the current directory, which is ```ISG-2.0```.

### *Decoding a Video:*

Very similar to encoding a file.
- You'll be prompted to enter a path to a video.
    - If it is an invalid file path or the file does not end in "```.mp4```", you can either enter another file path or go back to the main menu.
- You'll be prompted to enter a name for the final decoded file.
    - If you leave this blank and just press enter, the name will default to "```output```".
- You'll finally be prompted to enter a path to where the final file will be stored.
    - If the file path leads to an invalid directory, you will be prompted to re-enter a path to a valid directory.
    - If you press enter and leave this blank, the final file will be placed in the current directory, which is ```ISG-2.0```.

After encoding or decoding a video, you will be returned to the main menu where you can either encode, decode, or quit.

## **How it Works:**

Every piece of digital information can be broken down into a series of bits. We can use this to store information in a series of images using black and white pixels. In the raw pbm image format, a single bit represents a pixel. A 1 is a black pixel and a 0 is a white pixel. Using this information, we can store any file as a series of raw pbm images which can then be turned into a video. Turning the series of images into a video takes up more space than the original file, which may seem counter-intuitive, but we can store this video on websites online that have no limit on number of uploads per user to essentially gain unlimited storage of files.

### *Encoding:*

Encoding a file into a video can be broken down into 3 main steps:
1. Create a series of raw pbm images (learn more about pbm images [here](https://netpbm.sourceforge.net/doc/pbm.html)).
2. Scale each image up by a factor of 2 using ```pamenlarge```.
3. Make a video using each of the images as a single frame using ```ffmpeg```.

*Explanation:*

When ```encode``` runs, it first finds the number of bytes in the desired file. It then gets each byte from the file as a char and copies it into a raw pbm. When one pbm image fills up, a new one is created, and the process continues until all the bytes of the original file have been copied over. Due to video compression, a single pixel getting flipped can mean corruption for the file. To combat this, we scale the image up by a factor of 2 using ```pamenlarge```, so each pixel now becomes 4 pixels. This way when pixels do get flipped, the image does not become corrupted as it has been scaled up. Once each image has been scaled up, it is turned into a video using ```ffmpeg``` where each image is a single frame. Once this step has been completed, the process is complete and the file has successfully been embedded into a video.

### *Decoding:*

Much like encoding a file, decoding a video can be split up into 3 main steps as well:
1. Separate each frame of the video into a raw pbm image file using ```ffmpeg```.
2. Scale each image down by a factor of 0.5 using ```pamscale```.
3. Copy the image raster data into a single file using ```decode```.

*Explanation:*

Decoding a video first starts with turning every frame into it's own raw pbm image using ```ffmpeg```. Since each image has been scaled up by a factor of 2, we use ```pamscale``` to scale each image back to it's original size. In order to eliminate the effects that video compression had on the images, we use the ```-nomix``` flag for ```pamscale```. When scaling down, this option looks at each 4-pixel block and sets the correspoinding pixel in the scaled-down version to the color that shows up the most. This approach seems to work good enough to eliminate the effects of compression. Finally, ```decode``` goes through each images and copies the raster data over to a final file. Once this is done, the video has been successfully decoded.

## **Closing Remarks**

Feel free to post any issues you have with this code, but I probably won't be updating it very much in the future. Use this tool as you wish, but credit would be appreciated.
