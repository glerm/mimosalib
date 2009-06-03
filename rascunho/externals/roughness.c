/* Roughness library
 *  
 * Copyright (C) 2009 - André Pires, Alexandre Porres 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, 
 * USA.
 */

#include <string.h>
#include "m_pd.h"
#include "math.h"

t_class *roughness_class;


/**
 * Class roughness
 */
typedef struct roughness
{
    t_object x_ob;
    t_outlet *x_outlet;
    float *x_value;
	//config
    char *impl_choice;
	
	//main parameters
	int argc; 
	float *freqs;
	float *amps;
} t_roughness;


/*
 * min() for floats
 */
static float min(float a, float b) {
  return a<b ? a : b;
}

/*
 * max() for floats
 */
static float max(float a, float b) {
  return a>b ? a : b;
}

/**
 *
 */
static int rough_outlet_size(int size) {
	int sout = 0;
	int i;
	for (i = 0; i < size; i++) {
		sout += i;
	}
	return sout;
}
/*
 *  
 */
static float terh(float h) {
   return 13.3*atan(3*h/4000);
}


/*
 * 
 */
static float traun(float h) {
   float r = ((26.81*h)/(1960 + h)) - 0.53;
   if (r > 20.1) {
        r = r + 0.22*(r - 20.1);
   } 
   return r;
}

/*
 * 
 */
static float barks(float h) {
    if (h < 219.5) {
        return terh(h);
    } else {
        return traun(h);
    }
}

/**
 * Vassilakis function
 */
static float vassilakis(float amp1, float amp2) {
	float a1 = min(amp1, amp2);
	float a2 = max(amp1, amp2);
	float k1 = 0.1;
	float k2 = 1;
	float k3 = 3.11;
	return pow(a1*a2, k1) * k2 * pow((2*a1)/(a1+a2), k3);
}

/**
 * 
 */
static float parncutt(float freq1, float freq2) {
    float r = barks(freq1) - barks(freq2);
    r = fabs(r);
    if (r > 1.2) {
        r = 0;
    } else {
        r = pow((4*exp(1)*r)/(exp(4*r)), 2);    
    }
    return r;
}


/**
 * roughness function.
 * computes the roughness for each component
 */
static float roughness(t_roughness *x) {
	int isparncutt = (strcmp( x->impl_choice,"parncutt") == 0);
	int size = x->argc;
	int i;
	int sout = rough_outlet_size(size);
	t_atom rout[sout];
	float r;
	int c = 0;
	for (i = 0; i < size; i++) {
		int j;
		for (j = i + 1; j < size; j++) {
			if (isparncutt) {
				//parncutt
				r = parncutt(x->freqs[i], x->freqs[j]) * vassilakis(x->amps[i], x->amps[j]);
				post("roughness [%f, %f][%f, %f]: %f", x->freqs[i], x->amps[i], x->freqs[j], x->amps[j], r);
        		SETFLOAT(rout + c,  r);
				//outlet_float(x->x_outlet, r);
				c++;
			} else {
				//sethares
			}
		}
	}
	outlet_list (x->x_outlet, gensym ("list"), sout, rout);
}

/**
 * Hot inlet
 * Allocate frequency values and execute roughness method. 
 */
void roughness_freqs(t_roughness *x, t_symbol *s, int argc, t_atom *argv)
{
	x->freqs = (float *)getbytes(argc * sizeof(float));
	if (x->argc < argc) {
		x->argc = argc;
	}
	int i;
    for (i = 0; i < argc;i++) {
		x->freqs[i] = atom_getfloat(argv+i);
		post("freqs %f ", x->freqs[i]);
	}
	roughness(x);
}
/**
 * Cold inlet
 * Allocate amplitude values. 
 */
void roughness_amps(t_roughness *x, t_symbol *s, int argc, t_atom *argv)
{
    x->amps = (float *)getbytes(argc * sizeof(float));
	x->argc = argc;
	int i;
    for (i = 0; i < argc;i++) {
		x->amps[i] = atom_getfloat(argv+i);
		post("amps %f ", x->amps[i]);
	}
}
/*
 * New method
 */
void *roughness_new(t_symbol *s1)
{
    t_roughness *x = (t_roughness *)pd_new(roughness_class);
    inlet_new(&x->x_ob, &x->x_ob.ob_pd, gensym("list"), gensym("amps"));
    x->x_outlet = outlet_new(&x->x_ob, gensym("float"));
	x->impl_choice= s1->s_name;
	//these are the valid choices
	if (strcmp( x->impl_choice,"parncutt") != 0 && strcmp( x->impl_choice,"sethares") != 0) {
		post("Missing valid argument:  roughness [parncutt|sethares]. \n Invalid argument '%s'. Using default: 'parncutt'.", x->impl_choice);
		x->impl_choice = "parncutt";
	}
    post("roughness %s", x->impl_choice);
    return (void *)x;
}

/*
 * Setup method
 */
void roughness_setup(void)
{
    post("roughness_setup");
    roughness_class = class_new(gensym("roughness"), (t_newmethod)roughness_new, 0, sizeof(t_roughness), 0, 
			    A_DEFSYMBOL, A_NULL);
	class_addlist(roughness_class, roughness_freqs);
	class_addanything(roughness_class, roughness_amps);
}
