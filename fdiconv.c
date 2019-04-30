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

typedef union ieee_half {
	uint16_t ui;
	int16_t  si;
	//half   fp;
} ieee_half_t;

typedef union ieee_float {
	uint32_t ui;
	float    fp;
} ieee_float_t;

typedef union ieee_double {
	uint64_t ui;
	double   fp;
} ieee_double_t;

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
	printf("  v0.2 (C) 2010-2019 R.Bartosinski <bartosr@centrum.cz>\n\n");
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
	float fval;
	double dval;
	uint64_t ival;
	uint64_t idval;
	uint32_t ifval;

	float *pfv = &fval;
	double *pdv = &dval;
	uint64_t *pidv;
	uint32_t *pifv;

	float hval;
	uint16_t *pihv;
	
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
					ifval = strtoul(inval, NULL, 16);
				else
					ifval = strtoul(inval, NULL, 10);
				pifv = &ifval;
				pfv = (float *) pifv;
				fval = *pfv;
				dval = (double) fval;
				pidv = (uint64_t *) pdv;
				ival = (uint64_t) fval;
			} else if (form==FORM_BININT) { /* binary representation */
printf("\n\nTODO\n\n");
			} else { /* natural FP */
				fval = strtof(inval, NULL);
				dval = (double) fval;
				ival = (uint64_t) fval;
				pifv = (uint32_t *) pfv;
				pidv = (uint64_t *) pdv;
			}
			break;

		case FROM_DOUBLE: /* value is double */
			if (form==FORM_DECINT || form==FORM_HEXINT) { /* decimal/hexadecimal representation */
				if (form==FORM_HEXINT)
					idval = strtoull(inval, NULL, 16);
				else
					idval = strtoull(inval, NULL, 10);
				pidv = &idval;
				pdv = (double *) pidv;
				dval = *pdv;
				fval = (float) dval;
				pifv = (uint32_t *) pfv;
				ival = (uint64_t) dval;
				
			} else if (form==FORM_BININT) { /* binary representation */
printf("\n\nTODO\n\n");
			} else { /* natural FP */
				dval = strtod(inval, NULL);
				fval = (float) dval;
				ival = (uint64_t) fval;
				pifv = (uint32_t *) pfv;
				pidv = (uint64_t *) pdv;
			}
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
			fval = (float) ival;
			dval = (double) ival;
			pifv = (uint32_t *) pfv;
			pidv = (uint64_t *) pdv;
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
	printf(" HALF 1,5,10\n");
	printf("Hlf value   : %g\n", hval);
	printf("Hlt d.i.r.  : %u\n", *pihv);
	printf("Hlt h.i.r.  : 0x%08x\n", *pihv);
	printf("Hlt b.i.r.  : "); print_binary( *pihv, 16); printf("\n");
	printf("Hlt p(s|e|m): %u | %d | %u\n", (*pihv>>15)&1, ((int)((*pihv>>10)&((1<<5)-1)))-128, (*pihv>>0)&((1<<10)-1));
	printf("\n");
	printf(" FLOAT 1,8,23\n");
	printf("Flt value   : %g\n", fval);
	printf("Flt d.i.r.  : %u\n", *pifv);
	printf("Flt h.i.r.  : 0x%08x\n", *pifv);
	printf("Flt b.i.r.  : "); print_binary( *pifv, 32); printf("\n");
	printf("Flt p(s|e|m): %u | %d | %u\n", (*pifv>>31)&1, ((int)((*pifv>>23)&0xff))-128, (*pifv>>0)&((1<<23)-1));
	printf("\n");
	printf(" DOUBLE 1,11,52\n");
	printf("Dbl value   : %.40le\n", dval);
	printf("Dbl d.i.r.  : %lu\n", *pidv);
	printf("Dbl h.i.r.  : 0x%016lx\n", *pidv);
	printf("Dbl b.i.r.  : "); print_binary( *pidv, 64); printf("\n");
	printf("Dbl p(s|e|m): %lu | %d | %lu\n", (*pidv>>63)&1, ((int)((*pidv>>52)&0x7ff))-1024, (*pidv>>0)&((((uint64_t)1)<<52)-1));
	printf("\n");
	return 0;
}
