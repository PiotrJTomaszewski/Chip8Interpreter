#include "interpreter.h"

int main(int argc, char **argv) {
    interpreter_t *interpreter = interpreter_init();
    interpreter_run(interpreter);
    interpreter_free(interpreter);
    return 0;
}