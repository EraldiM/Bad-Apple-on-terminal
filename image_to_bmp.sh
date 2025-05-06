#!bin/bash

for f in *.jpg; do
    npx image-to-bmp /home/eraldi/Downloads/frames/$f 1
done
