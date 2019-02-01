# iff2raw converter

A commandline tool for converting IFF images to RAW + palette file.

## Building

```
$ git clone https://github.com/nattfalk/iff2raw.git
$ cd iff2raw
$ make
```

## Usage

Run `iff2raw -h` for help.

### Example
```
iff2raw -i image.iff -o outputname -r 1 -c 4 -p 1
```
Results in one RAW imagefile, `outputname.raw`, and a palette-file, `outputname.s`.


### Parameters
`-i <infile>`
Name of input iff file

`-o <outfile>`
Output filename (without extension)

`-c <mode>`
Color mode during save. 4 (default) = 4 bits per channel, 8 = 8 bits per channel

`-p <mode>`
Palette mode during save. 0 (default) = binary, 1 = sourcecode/asm 

`-r <mode>`
Scanline mode during save. 0 (default) = Non-interleaved, 1 = Interleaved

## Contact

Maintainer: [Michael Nattfalk](mailto:michaelnattfalk@gmail.com)
