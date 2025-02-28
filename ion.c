#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX(x, y) ((x) >= (y)) ? (x) : (y)

// Stretchy buffer
typedef struct BufHdr {
    size_t len;  // Length of the buffer
    size_t cap;  // How much the buffer can store
    char buf[0]; // Used as a placeholder, to determine how much memory to use later
} BufHdr;

#define BUF__HDR(b) ((BufHdr *)((char *)b - offsetof(BufHdr, buf)))
#define BUF__FITS(b, n) (BUF_LEN(b) + (n) <= BUF_CAP(b))
#define BUF__FIT(b, n) (BUF__FITS(b, n) ? 0 : ((b) = buf__grow((b), BUF_LEN(b) + (n), sizeof(*(b)))))
#define BUF_FREE(b) ((b) ? free(BUF__HDR(b)) : 0)

/*
Notes:
* `buf[0]` is a flexible array member, meaning it is a placeholder. The actual memory for
  `buf` is assigned dynamically and can grow as needed.
* `offsetof(BufHdr, buf)` gives the offset (in bytes) from the start of the structure to the
  beginning of `buf`. This is a fixed value and does not change when the buffer size changes.
    * Typically offsetof is used because of any padding applied by the compiler.
* Subtracting `(char *)buf` from the address of `buf` adjusts the pointer by the offset,
  effectively "moving" the pointer back to the beginning of the structure. This gives you
  the address of the `BufHdr` structure that contains `buf`, allowing you to access other members
  like `len` and `cap`.

*/

#define BUF_LEN(b) ((b) ? BUF__HDR(b)->len : 0)
#define BUF_CAP(b) ((b) ? BUF__HDR(b)->cap : 0)

/*
Notes
* BUF_LEN: If b (buffer) is NULL return 0, else go to the latest buffer (the new head), 
  and because BUF__HDR returns, BufHdr *, we can access the length of the buffer through the feild. 

*/

#define BUF_PUSH(b, x) (BUF__FIT(b, 1), b[BUF_LEN(b)] = (x), BUF__HDR(b)->len++)

/*
Notes
* The comma between the expression means it will happen sequentially.
* First BUF__FIT is called to make sure there is space in the buffer for at least one element, if not
  make the space for at least one elemnt.
* The b[BUF_LEN(b)++] = (x) expression will place the element at the index, then increment the index
  because of the post increment operator.

*/

void *buf__grow(const void *buf, size_t newLength, size_t elementSize) {
    size_t newCap = MAX(1 + 2*BUF_CAP(buf), newLength);
    assert(newLength <= newCap);
    size_t newSize = offsetof(BufHdr, buf) + newCap * elementSize;

    BufHdr *newHdr = NULL;

    if (buf != NULL) {
        newHdr = realloc(BUF__HDR(buf), newSize);
    } else {
        newHdr = malloc(newSize);
        newHdr->len = 0;
    }

    newHdr->cap = newCap;
    return newHdr->buf;

    /*
    TODO: Implement stronger bug fixing and implemenptation
    */
}

void bufTest() {
    int *buf = NULL;
    int n = 1024;

    for (int i = 0; i < n; i++) {
        BUF_PUSH(buf, i);
    }

    assert(BUF_LEN(buf) == n);

    for (int i = 0; i < n; i++) {
        assert(buf[i]== i);
    }


    BUF_FREE(buf);
}

typedef enum TokenKind {
    TOKEN_INT = 128,
    TOKEN_NAME,

} TokenKind;

typedef struct Token {
    TokenKind kind;
    union {
        uint64_t val;
        
        struct {
            const char *start;
            const char *end;
        };

        /*
        Notes:
        * It's better to use a union here because a union only allocates as much space as needed for the active field.
          Since tokens occur sequentially and only one token type is active at any given time, using a union reduces
          unnecessary overhead. Instead of allocating space for every possible token field, the union ensures that only
          the necessary space for the current token type is used, making the memory usage more efficient.

        */
    };

} Token;

Token token;

const char *stream;

void nextToken() {
    switch (*stream) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            uint64_t val = 0;

            while (isdigit(*stream)) {
                val *= 10;
                val += *stream++ - '0';
            }
            
            token.kind = TOKEN_INT;
            token.val = val;

            break;

            /*
            Notes:
            * Value parser works as follows: (lets say I have 12345)
                * val = 0 (I see a 1)
                * val = 1
                * val = 1 * 10 -> 10
                * val = 10 + 2 -> 12
                * val = 12 * 10 -> 120
                * val = 120 + 3 -> 123
                * val = 123 * 10 -> 1230
                * val = 1230 + 4 -> 1234
                * val = 1234 * 10 -> 12340
                * val = 12340 + 5 -> 12345
            */
        }
        case 'a': 
        case 'b': 
        case 'c': 
        case 'd': 
        case 'e': 
        case 'f': 
        case 'g': 
        case 'h': 
        case 'i': 
        case 'j': 
        case 'k': 
        case 'l': 
        case 'm': 
        case 'n': 
        case 'o': 
        case 'p': 
        case 'q': 
        case 'r': 
        case 's': 
        case 't': 
        case 'u': 
        case 'v': 
        case 'w': 
        case 'x': 
        case 'y': 
        case 'z': 
        case 'A': 
        case 'B': 
        case 'C': 
        case 'D': 
        case 'E': 
        case 'F': 
        case 'G': 
        case 'H': 
        case 'I': 
        case 'J': 
        case 'K': 
        case 'L': 
        case 'M': 
        case 'N': 
        case 'O': 
        case 'P': 
        case 'Q': 
        case 'R': 
        case 'S': 
        case 'T': 
        case 'U': 
        case 'V': 
        case 'W': 
        case 'X': 
        case 'Y':
        case 'Z': 
        case '_': {
            const char *start = stream++;
            
            while (isalnum(*stream) || *stream == '_') {
                stream++;
            }

            token.kind = TOKEN_NAME;
            token.start = start;
            token.end = stream;

            break;

            /*
            Notes
            * If we see a string of characters (a-z | A-Z | _ ), we keep moving the stream pointer
              until we no longer see accepted character set (a-z | A-Z | _ ).
            * Store the start pointer of where the stream pointer beings, which is the first accepted
              character in the set (a-z | A-Z | _ ).
            * Store the pointer of the last character in the accpeted set (a-z | A-Z | _ ).
            
            */
        }
        default:
            token.kind = *stream++;
    }
}

void printToken(Token token) {
    switch (token.kind) {
        case TOKEN_INT:
            printf("TOKEN -> INT: %llu\n", token.val);
            break;
        case TOKEN_NAME:
            printf("TOKEN -> NAME: %.*s\n", (int)(token.end - token.start), token.start);
            break;
        default:
            printf("TOKEN -> TYPE NOT SET: '%c'\n", token.kind);
            break;
    }
    
}

void lextTest() {
    char *soruce = "+()_HELLO1,234+FOO!994";
    stream = soruce;

    nextToken();

    while (token.kind) {
        printToken(token);
        nextToken();
    }
    
}

int main(int argc, char **argv) {
    bufTest();
    lextTest();

    return 0;
}