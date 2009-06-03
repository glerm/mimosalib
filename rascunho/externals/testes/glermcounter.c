#include "m_pd.h"

static t_class *glermcounter_class;

typedef struct _glermcounter {
  t_object  x_obj;
  t_int i_count;
} t_glermcounter;

void glermcounter_bang(t_glermcounter *x)
{
  t_float f=x->i_count;
  x->i_count++;
  outlet_float(x->x_obj.ob_outlet, f);
}

void *glermcounter_new(t_floatarg f)
{
  t_glermcounter *x = (t_glermcounter *)pd_new(glermcounter_class);

  x->i_count=f;
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}

void glermcounter_setup(void) {
  glermcounter_class = class_new(gensym("glermcounter"),
        (t_newmethod)glermcounter_new,
        0, sizeof(t_glermcounter),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);

  class_addbang(glermcounter_class, glermcounter_bang);
}
