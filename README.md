This program plays the video clip Bad Apple (https://www.youtube.com/watch?v=FtutLA63Cp8&pp=ygUJYmFkIGFwcGxl) and renders it in the terminal using ASCII characters.

I’ve always wanted to do this — I like the idea of reproducing it everywhere, and this is my contribution.

How it works:
The program opens all the images stored in the frame folder, reads each pixel, calculates the corresponding brightness, and prints a character representing the portion of the image read.

In detail:
I got the video frames from this repository: https://github.com/Felixoofed/badapple-frames, so thanks to Felixoofed for providing them. However, since I wanted to analyze them as BMP files instead of PNG or JPG, I had to convert them.

I searched for a converter and came across this program: https://github.com/Templarian/image-to-bmp.
Since there are more than 6000 images, I needed to automate the process, so I wrote a Bash script to scan all the files in the folder and convert them to BMP format.

I chose to convert the images using blocks of 12 pixels (height) by 8 pixels (width), since the images are black and white and use only 1 bit to represent the colors per pixel. Each pixel is represented by a single bit.

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
In theory, you could use this program to display any image composed of 1 bit per pixel and with no padding (see this Wikipedia page for more information on the BMP file format).
I will probably improve it to avoid those limitations.
