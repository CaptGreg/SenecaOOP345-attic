#include <ctype.h>  // isprint
#include <malloc.h>
#include <stdio.h>

void hexdump(FILE* fp, const void* b, const size_t count)
{
    for(size_t address = 0; count > address; address += 16 ) {
        const unsigned char* line = ((const unsigned char*)  b ) + address;

        size_t thisLineBytes = count - address;
        if(thisLineBytes > 16) thisLineBytes = 16;

        // Show the address
        fprintf(fp, "%08lx", address);

        // Show the hex codes
        for( int i = 0; i < 16; i++ ) {
            if( i % 8 == 0 ) fprintf(fp, " ");
            if( i < thisLineBytes )
                fprintf(fp, " %02x",  line[i]);
            else 
                fprintf(fp,  "   ");
        }

        // Show printable characters
        fprintf(fp,  "  ");
        for( size_t i = 0; i < thisLineBytes; i++) {
            // if( i % 8 == 0 ) fprintf(fp, " ");
            fprintf(fp, "%c", isprint(line[i]) ? line[i]: '.' ); 
        }

        fprintf(fp, "\n");
    }
}

int main(int argc, char**argv)
{
  if(argc == 1) {
    fprintf(stderr, "Usage: %s file-to-dump [file-to-dump ...]\n", argv[0]);
    return 1;
  }

  for(int arg = 1; arg < argc; arg++) {
    char* file = argv[arg];
    FILE* fp   = fopen(file, "rb");
    if(fp) {
      long size;
      char* b;

      // how large is file?
      fseek(fp,0, SEEK_END);
      size = ftell(fp);

      // allocate buffer
      b = malloc(size);
      if(b) {
        // read file
        fseek(fp,0, SEEK_SET);
        fread(b, 1, size, fp);

        // all done with file, close it
        fclose(fp);

        // dump file
        hexdump(stdout, b, size);

        // all done with buffer, delete it
        free(b);
      } else {
        fprintf(stderr, "cannot allocat buffer of size %ld for file \"%s\"\n", size, file);
      }
    } else {
      fprintf(stderr, "cannot open file \"%s\"\n",  file );
    }
  }
  return 0;
}
