#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_WIDTH 16

/**
 * Struct to store parsed command-line options.
 */
typedef struct
{
    const char *filename;
    long max_bytes;      // Max number of bytes to read (-1 for unlimited): -n
    long skip_bytes;     // Bytes to skip from the beginning: -s
    int width;           // Number of bytes per line: -w
} options_t;

/**
 * Parse command-line arguments into the options structure.
 * Returns 0 on success, non-zero on failure.
 */
int parse_args(int argc, char *argv[], options_t *opts)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename> [-n count] [-s skip] [-w width]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            printf("Usage: %s <filename> [-n count] [-s skip] [-w width]\n", argv[0]);
            printf("\nOptions:\n");
            printf("  -n <count>    Number of bytes to display (default: all)\n");
            printf("  -s <skip>     Number of bytes to skip at the beginning (default: 0)\n");
            printf("  -w <width>    Number of bytes per line (default: 16)\n");
            printf("  --help        Show this help message and exit\n");
            return 2; // help shown
        }
    }

    opts->filename = argv[1];
    opts->max_bytes = -1;
    opts->skip_bytes = 0;
    opts->width = DEFAULT_WIDTH;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
        {
            opts->max_bytes = atol(argv[++i]);
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
        {
            opts->skip_bytes = atol(argv[++i]);
        }
        else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc)
        {
            opts->width = atoi(argv[++i]);
        }
        else
        {
            fprintf(stderr, "Unknown or incomplete option: %s\n", argv[i]);
            return 1;
        }
    }

    return 0;
}

/**
 * Print a single line of hexdump output.
 * Shows offset, hex bytes, and ASCII representation.
 */
void print_line(unsigned char *buffer, long offset, size_t len, int width)
{
    printf("%08lx: ", offset);

    // Hex bytes
    for (int i = 0; i < width; i++)
    {
        if (i < len)
        {
            printf("%02x ", buffer[i]);
        }
        else
        {
            printf("   ");
        }

        // Add extra space in the middle
        if (i == (width / 2 - 1))
        {
            printf(" ");
        }
    }

    printf(" ");

    // ASCII characters
    for (int i = 0; i < len; i++)
    {
        printf("%c", isprint(buffer[i]) ? buffer[i] : '.');
    }

    printf("\n");
}

/**
 * Main entry point.
 * Opens the file, reads it chunk by chunk and prints formatted output.
 */
int main(int argc, char *argv[])
{
    options_t opts;

    int result = parse_args(argc, argv, &opts);
    if (result != 0)
    {
        return result == 2 ? 0 : 1; // 2 = help shown, not an error
    }

    FILE *file = fopen(opts.filename, "rb");
    if (!file)
    {
        perror("Error opening file");
        return 1;
    }

    if (fseek(file, opts.skip_bytes, SEEK_SET) != 0)
    {
        perror("Error seeking file");
        fclose(file);
        return 1;
    }

    unsigned char *buffer = malloc(opts.width);
    if (!buffer)
    {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    long offset = opts.skip_bytes;
    size_t read;

    while ((read = fread(buffer, 1, opts.width, file)) > 0 && (opts.max_bytes < 0 || offset - opts.skip_bytes < opts.max_bytes))
    {
        print_line(buffer, offset, read, opts.width);
        offset += read;
    }

    free(buffer);
    fclose(file);

    return 0;
}
