// Simple tool to roughly evaluate the quality encoding of a webp bitstream
//
// Result is a *rough* estimation of the quality. You should just consider
// the bucket it's in (q > 80? > 50? > 20?) and not take it for face value.
/*
 gcc -o webp_quality webp_quality.c -O3 -I../ -L. -L../imageio \
    -lexample_util -limagedec -lwebpextras -lwebp -L/opt/local/lib \
    -lpng -lz -ljpeg -ltiff -lm -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./extras.h"
#include "../imageio/imageio_util.h"

int main(int argc, const char *argv[]) {
  int c;
  int quiet = 0;
  for (c = 1; c < argc; ++c) {
    if (!strcmp(argv[c], "-quiet")) {
      quiet = 1;
    } else if (!strcmp(argv[c], "-help") || !strcmp(argv[c], "-h")) {
      printf("webp_quality [-h][-quiet] webp_files...\n");
      return 1;
    } else {
      const char* const filename = argv[c];
      const uint8_t* data = NULL;
      size_t data_size = 0;
      int q;
      const int ok = ImgIoUtilReadFile(filename, &data, &data_size);
      if (!ok) continue;
      q = VP8EstimateQuality(data, data_size);
      if (!quiet) printf("[%s] ", filename);
      if (q < 0) {
        fprintf(stderr, "Not a WebP file, or not a lossy WebP file.\n");
      } else {
        if (!quiet) {
          printf("Estimated quality factor: %d\n", q);
        } else {
          printf("%d\n", q);   // just print the number
        }
      }
      free((void*)data);
    }
  }
  return 1;
}
