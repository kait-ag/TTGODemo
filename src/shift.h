/* GIMP RGBA C-Source image dump (shift.c) */

static const struct {
  int  	 width;
  int  	 height;
  int  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char 	 pixel_data[16 * 16 * 4 + 1];
} shift = {
  16, 16, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377"
  "\377\000\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\377"
  "\377\377\377\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\377"
  "\377\377\000\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\017\017\017\000\377\377"
  "\377\000\377\377\377\000\377\377\377\000\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377"
  "\377\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377\000\377\377\377\000\377\377\377\000"
  "\377\377\377\000\377\377\377\377\377\377\377\377\377\377\377\000\377\377\377"
  "\377\377\377\377\377\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377"
  "\000\000\000\000\000\000\000\000\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377"
  "\000\377\377\377\377\377\377\377\377\377\377\377\000\377\377\377\000DDD\000\377\377"
  "\377\377\377\377\377\377\000\000\000\000\377\377\377\000\377\377\377\000\000\000\000\000\000\000"
  "\000\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\377\377\377\377"
  "\377\377\377\377\377\000\000\000\000\377\377\377\000\377\377\377\000\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\000\377\377\377\000\000\000\000\000\000\000"
  "\000\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\000\000\000\000\377"
  "\377\377\377\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\377\377"
  "\377\377\000\000\000\000\000\377\377\377\000\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\377"
  "\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\377\377"
  "\377\377\000\377\377\377\000\000\000\000\000\377\377\377\377\377\377\377\000\377\377\377"
  "\000\377\377\377\000\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377"
  "\000\377\377\377\000\377\377\377\000\377\377\377\377\377\377\377\000\377\377\377"
  "\000\377\377\377\000\377\377\377\377\377\377\377\000\377\377\377\000\377\377\377"
  "\000\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377\000\377"
  "\377\377\000\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377\000\377"
  "\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377\377\000\377\377"
  "\377\000\377\377\377\000\377\377\377\000\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

