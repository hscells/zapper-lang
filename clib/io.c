#include "../system.h"

object_t* z_slurp(list_t* args) {
  object_t* data = newObject();
  char* filename = z_first(args)->value->value.s;
  char* mode = z_nth(args, 1)->value->value.s;
  char *buffer = 0;
  long length;
  FILE *f;
  f = fopen(filename, mode);

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
  }

  if (buffer) {
    data->value->value.s = buffer;
    data->value->type = String;
  } else {
    exception("Could not read file", -1, filename);
    data->value->type = Bool;
    data->value->value.b = false;
    return data;
  }
  return data;
}

object_t* z_spit(list_t* args) {
  object_t* data = newObject();
  data->value->type = Bool;
  data->value->value.b = true;
  char* filename = z_first(args)->value->value.s;
  char* output = "";

  if (z_typeof(z_nth(args, 1)) == String) {
    output = z_nth(args, 1)->value->value.s;
  } else if(z_typeof(z_nth(args, 1)) == Int) {
    sprintf(output, "%d", z_nth(args, 1)->value->value.i);
  }

  char* mode = z_nth(args, 2)->value->value.s;
  FILE *f;
  f = fopen(filename, mode);

  if (f) {
    fprintf(f, "%s\n", output);
    fclose(f);
  } else {
    exception("Could not open file", -1, filename);
    data->value->value.b = false;
    return data;
  }
  return data;
}

void init_io() {

  object_t* (*slurp)(list_t* args) = &z_slurp;
  struct function* slurp_ref = newFunction(slurp,"slurp",2);
  addFunctionToSymbolTable(clib_functions, slurp_ref);

  object_t* (*spit)(list_t* args) = &z_spit;
  struct function* spit_ref = newFunction(spit,"spit",3);
  addFunctionToSymbolTable(clib_functions, spit_ref);

}
