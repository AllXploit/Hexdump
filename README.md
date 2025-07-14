# Hexdump

A lightweight hexdump utility written in C.
A lightweight hexdump utility written in C.  
Perfect for reverse engineering warm-ups, binary analysis or simply poking at mysterious `.bin` files.

## Features

- Hex + ASCII display (offset, hex bytes, readable chars)
- Options:
    - `-n <count>` : number of bytes to display
    - `-s <skip>` : skip N bytes at start
    - `-w <width>` : bytes per line (default: 16)
    - `--help`     : display usage information and exit
- Clean output, padding-aware formatting
- Works on any binary file (including `/dev/*` and firmware dumps)

## Usage

```bash
./hexdump myfile.bin -n 128 -s 64 -w 32
```
or simply
```bash
./hexdump myfile.bin
```

## Output
```bash
00000040: 7a 65 72 6f  62 79 74 65  73 20 61 72  65 20 73 65  zerobytes are se
00000050: 65 6e 20 6f  6e 6c 79 20  62 79 20 65  79 65 73 2e  en only by eyes.
```

## Building

**With `make` (recommended)**

```bash
make
```

Or manually
```bash
gcc -Wall -O2 -o hexdump hexdump.c
```

**Clean build**
```bash
make clean
```

## Test

```bash
./hexdump test.bin
```

## Why?

Because `hexdump` and `xxd` are great but, sometimes, you just want to write your own and flex a bit.

## License

MIT: do whatever you want with it.
But if you make money with this, consider buying your cat a throne.
