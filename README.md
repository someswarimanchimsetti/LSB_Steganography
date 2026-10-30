# LSB Image Steganography

## Overview
This project implements Least Significant Bit (LSB) steganography in C. It hides secret text data inside a BMP image and extracts the hidden data from the encoded image.

## Features
- Encode secret text into a BMP image.
- Decode hidden text from an encoded BMP image.
- Store the secret file extension and file size.
- Preserve the BMP image header during encoding.

## Technologies Used
- C Programming
- File Handling
- Bitwise Operations
- LSB Steganography

## Compilation
make

## Encoding
./stego -e beautiful.bmp sample_secret.txt stego.bmp

## Decoding
./stego -d stego.bmp output.txt

## Clean
make clean
