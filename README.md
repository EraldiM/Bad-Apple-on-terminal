This program plays the video clip Bad Apple (link) and renders it in the terminal using ASCII characters.

I’ve always wanted to do this — I like the idea of reproducing it everywhere, and this is my contribution.

How it works:
The program opens all the images stored in the frame folder, reads each pixel, calculates the corresponding brightness, and prints a character representing the portion of the image read.

In detail:
I got the video frames from this repository: https://github.com/Felixoofed/badapple-frames, so thanks to Felixoofed for providing them. However, since I wanted to analyze them as BMP files instead of PNG or JPG, I had to convert them.

I searched for a converter and came across this program: https://github.com/Templarian/image-to-bmp.
Since there are more than 6000 images, I needed to automate the process, so I wrote a Bash script to scan all the files in the folder and convert them to BMP format.

I chose to convert the images using blocks of 12 pixels (width) by 8 pixels (height), since the images are black and white and use only 1 bit per pixel. Each pixel is represented by a single bit.

To convert the images to ASCII, I had to consider the proportions of terminal characters. I noticed that each character in the terminal is not a perfect square — the height is about 1.5 times the width. So I chose a reading ratio that reflects this shape.

I decided to read 12 pixels in height and 8 pixels in width, also because (based on my current knowledge) the minimum amount of data you can read is 1 byte at a time. Even though I believe I could split the read using bit shifts, that would be more complicated to implement — but not impossible.

I also used escape sequences to correctly position the cursor on the terminal.

In order to read the images names i did a ls > names.txt in the frame folder.

Programming this was very fun and challenging, especially since it’s also a graphical project.

If someone is reading this and wants to use it, follow these steps:

    Compile the program using:
    gcc reader.c bmp.c -o reader

    Run the program with:
    ./reader

Limitations:
In theory, you could use this program to display any image composed of 1 bit per pixel and with no padding (see this Wikipedia page for more information on the BMP file format https://en.wikipedia.org/wiki/BMP_file_format).
I will probably improve it to avoid those limitations.

---

Updates: I increased the quality of the "video" using smaller blocks, this time is used 6 x 4 pixel per blocks, now the video is 4 times bigger, having more details.
If you want to use the old version comment the new function draw_image_HD and use draw_image instead. 
I tried to icrease the quality considering, this time a block of 3x2 pixels, and the image is more realistic, but the syncronization seems harder to get, maybe its a 
limitation caused by the terminal used, or can be bypassed in some way. if you want to use a function instead of another just comment/decomment the right function.
The new function I wrote is called draw_image_QHD
