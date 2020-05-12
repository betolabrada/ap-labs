#include "logger.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define WHITESPACE 64
#define EQUALS 65
#define INVALID 66

static const unsigned char d[] = {
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 62, 66, 66, 66, 63, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 66, 66, 66, 65, 66, 66, 66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 66, 66, 66, 66, 66, 66, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 66, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
    66, 66, 66, 66, 66, 66};

#define BUFSIZ 2000000000 // 2gb max

int progress = 0;

sigHandler(int sig)
{
    infof("progress %d %%", progress);
}

int base64encode(const void *data_buf, size_t dataLength, char *result, size_t resultSize)
{
    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t *data = (const uint8_t *)data_buf;
    size_t resultIndex = 0;
    size_t x;
    uint32_t n = 0;
    int padCount = dataLength % 3;
    uint8_t n0, n1, n2, n3;

    /* increment over the length of the string, three characters at a time */
    for (x = 0; x < dataLength; x += 3)
    {
        progress = (x * 100 / dataLength);
        /* these three 8-bit (ASCII) characters become one 24-bit number */
        n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if ((x + 1) < dataLength)
            n += ((uint32_t)data[x + 1]) << 8; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if ((x + 2) < dataLength)
            n += data[x + 2];

        /* this 24-bit number gets separated into four 6-bit numbers */
        n0 = (uint8_t)(n >> 18) & 63;
        n1 = (uint8_t)(n >> 12) & 63;
        n2 = (uint8_t)(n >> 6) & 63;
        n3 = (uint8_t)n & 63;

        /*
        * if we have one byte available, then its encoding is spread
        * out over two characters
        */
        if (resultIndex >= resultSize)
            return 1; /* indicate failure: buffer too small */
        result[resultIndex++] = base64chars[n0];
        if (resultIndex >= resultSize)
            return 1; /* indicate failure: buffer too small */
        result[resultIndex++] = base64chars[n1];

        /*
        * if we have only two bytes available, then their encoding is
        * spread out over three chars
        */
        if ((x + 1) < dataLength)
        {
            if (resultIndex >= resultSize)
                return 1; /* indicate failure: buffer too small */
            result[resultIndex++] = base64chars[n2];
        }

        /*
        * if we have all three bytes available, then their encoding is spread
        * out over four characters
        */
        if ((x + 2) < dataLength)
        {
            if (resultIndex >= resultSize)
                return 1; /* indicate failure: buffer too small */
            result[resultIndex++] = base64chars[n3];
        }
    }

    /*
        * create and add padding that is required if we did not have a multiple of 3
        * number of characters available
        */
    if (padCount > 0)
    {
        for (; padCount < 3; padCount++)
        {
            if (resultIndex >= resultSize)
                return 1; /* indicate failure: buffer too small */
            result[resultIndex++] = '=';
        }
    }
    if (resultIndex >= resultSize)
        return 1; /* indicate failure: buffer too small */
    result[resultIndex] = 0;
    return 0; /* indicate success */
}

int base64decode(char *in, size_t inLen, unsigned char *out, size_t *outLen)
{
    char *end = in + inLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;

    while (in < end)
    {
        progress = (len * 100 / inLen);

        unsigned char c = d[*in++];

        switch (c)
        {
        case WHITESPACE:
            continue; /* skip whitespace */
        case INVALID:
            return 1; /* invalid input, return error */
        case EQUALS:  /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            /* If the buffer is full, split it into bytes */
            if (iter == 4)
            {
                if ((len += 3) > *outLen)
                    return 1; /* buffer overflow */
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0;
                iter = 0;
            }
        }
    }

    if (iter == 3)
    {
        if ((len += 2) > *outLen)
            return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2)
    {
        if (++len > *outLen)
            return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len; /* modify to reflect the actual output size */
    return 0;
}

int main(int argc, char const *argv[])
{

    char *buf;
    char *res;

    if (argc < 3)
    {
        errorf("Usage: ./base64 --action <textfile>");
        exit(1);
    }

    if (signal(SIGINT, sigHandler) == SIG_ERR)
    {
        errorf("error signal SIGINT");
        return -1;
    }

    if (signal(SIGUSR1, sigHandler) == SIG_ERR)
    {
        errorf("error signal SIGUSR1");
        return -1;
    }

    infof("opening %s", argv[2]);
    int fd = open(argv[2], O_RDONLY);
    if (fd == -1)
    {
        errorf("error opening file %s", argv[2]);
        exit(1);
    }

    infof("reading %s", argv[2]);
    buf = (char *)malloc(BUFSIZ * sizeof(char));
    long n = read(fd, buf, BUFSIZ);
    // infof("filesize: %d", n);
    if (n == -1)
    {
        errorf("error reading file");
        exit(1);
    }

    res = (char *)malloc(BUFSIZ * sizeof(char));

    if (strcmp(argv[1], "--encode") == 0)
    {

        base64encode(buf, strlen(buf), res, BUFSIZ);

        int fd_new = open("output.txt", O_WRONLY | O_CREAT, 00700);
        if (fd_new == -1)
        {
            errorf("error opening file %s", argv[2]);
            exit(1);
        }
        if (write(fd_new, res, BUFSIZ) == -1)
        {
            errorf("fail to write");
            exit(1);
        }

        close(fd_new);
    }

    if (strcmp(argv[1], "--decode") == 0)
    {
        size_t len = BUFSIZ;
        size_t * outLen = &len;
        unsigned char * res_decode = (unsigned char *)malloc(BUFSIZ * sizeof(unsigned char));
        base64decode(buf, BUFSIZ, res_decode, outLen);
    }

    free(buf);
    free(res);
    close(fd);

    return 0;
}
