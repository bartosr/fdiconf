/* FDICONV -  Converter between Float FP - Double FP - Integer representation */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <stdint.h>

enum from_types {
  FROM_UNKNOWN,
  FROM_FLOAT,
  FROM_DOUBLE,
  FROM_HALF,
  FROM_INTEGER,
};

enum form_kinds {
  FORM_FP,
  FORM_DECINT,
  FORM_HEXINT,
  FORM_BININT,
};

typedef union ieee_float {
  uint32_t ui;
  float    fp;
} ieee_float_t;

typedef union ieee_double {
  uint64_t ui;
  double   fp;
} ieee_double_t;


uint16_t conv_fp_to_hpi(float x)
{
  ieee_float_t from;
  int e;
  uint16_t to;
  from.fp = x;
  to = (from.ui & 0x80000000)>>16; // sign
  to |= (from.ui >> (23-10)) & ((1<<10)-1);  // fraction
  // exponent
  e = ((from.ui>>23)&((1<<8)-1))-127;
  if (e<-14) {        /* out of range - zero */
    to = to & 0x8000;
  } else if (e>15) {  /* out of range - infinity */
    to = (to & 0x8000) | 0b111110000000000;
  } else {
    e = (e+15) & 0b11111;
    to |= e<<10;
  }
  return to;
}


void print_binary(uint64_t value, int bits)
{
  char i = bits-1;
  unsigned long long mask = 1;
  while(i--) mask <<= 1;

  while(bits--) {
    if (value & mask) printf("1"); else printf("0");
    mask >>= 1;
  }
}

void usage(void)
{
  printf("FDICONV is converter between float/double and their integer representations\n");
  printf("  v0.3 (C) 2010-2019 R.Bartosinski <bartosr@centrum.cz>\n\n");
  printf(" usage:  fdiconv [-d|-h|-b] -F|-D|-H|-I <value>\n");
  printf(" options:   -F <value>     value is a float FP number\n");
  printf("            -D <value>     value is a double FP number\n");
  printf("            -H <value>     value is a half FP number\n");
  printf("            -I <value>     value is a integer number\n");
  printf("            -d             value behind -F|-D|-H is integer (decimal) (default)\n");
  printf("            -h             value behind -F|-D|-H is integer (hexadecimal)\n");
  printf("            -b             value behind -F|-D|-H is integer (binary)\n");
  printf("\n");
}

int main(int argc, char *argv[])
{
  const char optstring[] = "F:D:H:I:dhb";
  int optch;
  int opterr = 0;
  int from = FROM_UNKNOWN; /* float/double/half/integer */
  int form = FORM_FP; /* -1 = natural FP/integer value, 0 = decimal, 1 = hexadecimal, 2 = binary */

  char *inval = NULL;

  uint64_t ival;

  ieee_float_t  flt_val;
  ieee_double_t dbl_val;

  ieee_float_t  hlf_val; /* half saved as float */
  uint16_t      hlf_int; /* true half in integer representation */
  
  while ((optch = getopt(argc, argv, optstring)) != -1){
    switch (optch) {
      case 'F': /* float FP */
        from = FROM_FLOAT;
        inval = optarg;
        break;
      case 'D': /* double FP */
        from = FROM_DOUBLE;
        inval = optarg;
        break;
      case 'H': /* half FP */
        from = FROM_HALF;
        inval = optarg;
        break;
      case 'I': /* integer value (not representation) */
        from = FROM_INTEGER;
        inval = optarg;
        break;

      case 'd': /* integer representation of the value */
        form = FORM_DECINT;
        break;
      case 'h': /* hexadecimal integer representation of the value */
        form = FORM_HEXINT;
        break;
      case 'b': /* binary integer representation of the value */
        form = FORM_BININT;
        break;

      default:
        printf("ERR: Unknown command line option\n");
        opterr = 1;
        break;
    }
    if (opterr) {
      usage();
      return -1;
    }
  } // while

  switch (from) {
    case FROM_FLOAT: /* value is float */
      if (form==FORM_DECINT || form==FORM_HEXINT) { /* decimal/hexadecimal representation */
        if (form==FORM_HEXINT)
          flt_val.ui = strtoul(inval, NULL, 16);
        else
          flt_val.ui = strtoul(inval, NULL, 10);
      } else if (form==FORM_BININT) { /* binary representation */
printf("\n\nTODO\n\n");
      } else { /* natural FP */
        flt_val.fp = strtof(inval, NULL);
      }
      dbl_val.fp = (double) flt_val.fp;
      ival = (uint64_t) flt_val.fp;
      hlf_int = conv_fp_to_hpi(flt_val.fp);
      break;

    case FROM_DOUBLE: /* value is double */
      if (form==FORM_DECINT || form==FORM_HEXINT) { /* decimal/hexadecimal representation */
        if (form==FORM_HEXINT)
          dbl_val.ui = strtoull(inval, NULL, 16);
        else
          dbl_val.ui = strtoull(inval, NULL, 10);
      } else if (form==FORM_BININT) { /* binary representation */
printf("\n\nTODO\n\n");
      } else { /* natural FP */
        dbl_val.fp = strtod(inval, NULL);
      }
      flt_val.fp = (float) dbl_val.fp;
      ival = (uint64_t) dbl_val.fp;
      hlf_int = conv_fp_to_hpi(flt_val.fp);
      break;

    case FROM_HALF: /* value is half */
printf("\n\nTODO\n\n");
      if (form==FORM_DECINT || form==FORM_HEXINT) {
      } else if (form==FORM_BININT) {
      } else { /* natural FP */
        
      }
      break;

    case FROM_INTEGER: /* value is integer */
      if (form==FORM_DECINT) { /* decimal */
        ival = strtoull(inval, NULL, 10);
      } else if (form==FORM_HEXINT) { /* hexadecimal */
        ival = strtoull(inval, NULL, 16);
      } else if (form==FORM_BININT) { /* binary */
printf("\n\nTODO\n\n");
      } else { /* as FP */
        ival = (uint64_t) strtod(inval, NULL);
      }
      flt_val.fp = (float) ival;
      dbl_val.fp = (double) ival;
      break;
    default:
      usage();
      return -1;
  }

  /* print outputs */
  printf("Int value   : %lu\n", ival);
  printf("Int (hex)   : 0x%016lx\n", ival);
  printf("Int (bin)   : "); print_binary(ival, 64); printf("\n");
  printf("\n");
  printf(" HALF (1,5,10)\n");
//  printf("Hlf value   : %g\n", hval);
  printf("Hlt d.i.r.  : %u\n", hlf_int);
  printf("Hlt h.i.r.  : 0x%04x\n", hlf_int);
  printf("Hlt b.i.r.  : "); print_binary( hlf_int, 16); printf("\n");
  printf("Hlt p(s|e|f): %u | %d | %u\n", (hlf_int>>15)&1, ((int)((hlf_int>>10)&((1<<5)-1)))-15, (hlf_int>>0)&((1<<10)-1));
  printf("Hlt m(s|e|f): x%04x | x%04x | x%04x\n", hlf_int&(1<<15), hlf_int&(0xff<<10), hlf_int&((1<<10)-1));
  printf("\n");
  printf(" FLOAT (1,8,23)\n");
  printf("Flt value   : %g\n", flt_val.fp);
  printf("Flt d.i.r.  : %u\n", flt_val.ui);
  printf("Flt h.i.r.  : 0x%08x\n", flt_val.ui);
  printf("Flt b.i.r.  : "); print_binary( flt_val.ui, 32); printf("\n");
  printf("Flt p(s|e|f): %u | %d | %u\n", (flt_val.ui>>31)&1, ((int)((flt_val.ui>>23)&0xff))-127, (flt_val.ui>>0)&((1<<23)-1));
  printf("Flt m(s|e|f): x%08x | x%08x | x%08x\n", flt_val.ui&(1<<31), flt_val.ui&(0xff<<23), flt_val.ui&((1<<23)-1));
  printf("\n");
  printf(" DOUBLE (1,11,52)\n");
  printf("Dbl value   : %.40le\n", dbl_val.fp);
  printf("Dbl d.i.r.  : %lu\n", dbl_val.ui);
  printf("Dbl h.i.r.  : 0x%016lx\n", dbl_val.ui);
  printf("Dbl b.i.r.  : "); print_binary( dbl_val.ui, 64); printf("\n");
  printf("Dbl p(s|e|f): %lu | %d | %lu\n", (dbl_val.ui>>63)&1, ((int)((dbl_val.ui>>52)&0x7ff))-1023, (dbl_val.ui>>0)&((1L<<52)-1));
  printf("Dbl m(s|e|f): x%016lx | x%016lx | x%016lx\n", dbl_val.ui&(1L<<63), dbl_val.ui&(0x7ffL<<52), dbl_val.ui&((1L<<52)-1));
  printf("\n");
  return 0;
}
