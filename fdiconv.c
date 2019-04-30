/* FDICONV -  Converter between Float FP - Double FP - Integer representation */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


void print_binary(unsigned long long value, int bits)
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
	printf("  (C) 2010 R.Bartosinski\n\n");
	printf(" usage:  fdiconv [-d|-h] -F|-D|-I <value>\n");
	printf(" options:   -F <value>     value is a float FP number\n");
	printf("            -D <value>     value is a double FP number\n");
	printf("            -I <value>     value is a integer number\n");
	printf("            -d             value behind -F|-D is integer (decimal) (default)\n");
	printf("            -h             value behind -F|-D is integer (hexadecimal)\n");
	printf("            -b             value behind -F|-D is integer (binary)\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	const char optstring[] = "F:D:I:dhb";
	int optch;
	int opterr = 0;
	int from = -1; /* 0 = float, 1 = double, 2 = integer */
	int form = -1; /* -1 = natural FP/integer value, 0 = decimal, 1 = hexadecimal, 2 = binary */

	char *inval = NULL;
	float fval;
	double dval;
	unsigned long long ival;
	unsigned long long idval;
	unsigned long ifval;

	float *pfv = &fval;
	double *pdv = &dval;
	unsigned long long *pidv;
	unsigned long *pifv;
	
	while ((optch = getopt(argc, argv, optstring)) != -1){
		switch (optch) {
			case 'F': /* float FP */
				from = 0;
				inval = optarg;
				break;
			case 'D': /* double FP */
				from = 1;
				inval = optarg;
				break;
			case 'I': /* integer value (not representation) */
				from = 2;
				inval = optarg;
				break;

			case 'd': /* integer representation of the value */
				form = 0;
				break;
			case 'h': /* hexadecimal integer representation of the value */
				form = 1;
				break;
			case 'b': /* binary integer representation of the value */
				form = 2;
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
		case 0: /* value is float */
			if (form==0 || form==1) { /* decimal/hexadecimal representation */
				if (form==1)
					ifval = strtoul(inval, NULL, 16);
				else
					ifval = strtoul(inval, NULL, 10);
				pifv = &ifval;
				pfv = (float *) pifv;
				fval = *pfv;
				dval = (double) fval;
				pidv = (unsigned long long *) pdv;
				ival = (unsigned long long) fval;
			} else if (form==2) { /* binary representation */
printf("\n\nTODO\n\n");
			} else { /* natural FP */
				fval = strtof(inval, NULL);
				dval = (double) fval;
				ival = (unsigned long long) fval;
				pifv = (unsigned long *) pfv;
				pidv = (unsigned long long *) pdv;
			}
			break;
		case 1: /* value is double */
			if (form==0 || form==1) { /* decimal/hexadecimal representation */
				if (form==1)
					idval = strtoull(inval, NULL, 16);
				else
					idval = strtoull(inval, NULL, 10);
				pidv = &idval;
				pdv = (double *) pidv;
				dval = *pdv;
				fval = (float) dval;
				pifv = (unsigned long *) pfv;
				ival = (unsigned long long) dval;
				
			} else if (form==2) { /* binary representation */
printf("\n\nTODO\n\n");
			} else { /* natural FP */
				dval = strtod(inval, NULL);
				fval = (float) dval;
				ival = (unsigned long long) fval;
				pifv = (unsigned long *) pfv;
				pidv = (unsigned long long *) pdv;
			}
			break;
		case 2: /* value is integer */
			if (form==0) { /* decimal */
				ival = strtoull(inval, NULL, 10);
			} else if (form==1) { /* hexadecimal */
				ival = strtoull(inval, NULL, 16);
			} else if (form==2) { /* binary */
printf("\n\nTODO\n\n");
			} else { /* as FP */
				ival = (unsigned long long) strtod(inval, NULL);
			}
			fval = (float) ival;
			dval = (double) ival;
			pifv = (unsigned long *) pfv;
			pidv = (unsigned long long *) pdv;
			break;
		default:
			usage();
			return -1;
	}

	/* print outputs */
	printf("Int value   : %llu\n", ival);
	printf("Int (hex)   : 0x%016llx\n", ival);
	printf("Int (bin)   : "); print_binary(ival, 64); printf("\n\n");

	printf("Flt value   : %g\n", fval);
	printf("Flt d.i.r.  : %lu\n", *pifv);
	printf("Flt h.i.r.  : 0x%08lx\n", *pifv);
	printf("Flt b.i.r.  : "); print_binary( *pifv, 32); printf("\n\n");
	printf("Dbl value   : %.40le\n", dval);
	printf("Dbl d.i.r.  : %llu\n", *pidv);
	printf("Dbl h.i.r.  : 0x%016llx\n", *pidv);
	printf("Dbl b.i.r.  : "); print_binary( *pidv, 64); printf("\n\n");
	return 0;
}
