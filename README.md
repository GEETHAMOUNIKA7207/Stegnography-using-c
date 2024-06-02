# Stegnography-using-c
Description:

    Steganography is the practice of concealing a message within another medium, such as an image, video, or audio file, such that only the sender and intended recipient know of the existence of the message. Implementing steganography in C can involve manipulating the bits of an image to embed the hidden message. One common technique is Least Significant Bit (LSB) steganography, where the least significant bits of the image pixels are modified to store the hidden message.
    In this project mostly BMP (Bitmap) files are commonly used for steganography because they are uncompressed and easier to manipulate at the byte level. 

Key Features of a Steganography

Image Encoding: Convert the message into binary format for embedding.
Image Decoding: Extract and convert the binary data back into readable text.
Least Significant Bit (LSB) Modification: Embed each bit of the message into the least significant bit of the image’s pixel data.
Bit Manipulation: Ensure accurate modification and retrieval of LSBs without significant distortion of the image.
Command-Line Interface (CLI): Provide a simple CLI for users to specify input and output files, and the message to be hidden.
Error Handling: Alert if the message size exceeds the image’s capacity to prevent data loss or corruption.Identify and handle errors during the embedding    and extraction process to ensure the hidden message is intact.
Cross-Platform Compatibility: Ensure the program can run on different operating systems (e.g., Windows, Linux, macOS).

Prerequisites: 

    A C compiler (GCC recommended) BMP image files for encoding and decoding

How it works :

    The process begins by reading the image file. Here, in this project we have done two steps Encoding and Decoding
Encoding involves converting the message to binary, reading the carrier image and secret text, embedding the binary message into the lsb of the image's pixel data. 
Decoding involves reading the steganographic image, extracting the least significant bits to retrieve the binary message, and converting the binary data back to readable text.

Acknowledgements:

    Inspiration from various steganography tutorials and resources. BMP image handling based on public domain BMP libraries.
