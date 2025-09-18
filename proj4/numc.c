#include "numc.h"
#include <structmember.h>

PyTypeObject Matrix61cType;

/* Helper functions for initalization of matrices and vectors */

/*
 * Return a tuple given rows and cols
 */
PyObject* get_shape(int rows, int cols) {
  if (rows == 1 || cols == 1) {
    return PyTuple_Pack(1, PyLong_FromLong(rows * cols));
  } else {
    return PyTuple_Pack(2, PyLong_FromLong(rows), PyLong_FromLong(cols));
  }
}
/*
 * Matrix(rows, cols, low, high). Fill a matrix random double values
 */
int init_rand(PyObject* self, int rows, int cols, unsigned int seed, double low,
              double high) {
  matrix* new_mat;
  int alloc_failed = allocate_matrix(&new_mat, rows, cols);
  if (alloc_failed)
    return alloc_failed;
  rand_matrix(new_mat, seed, low, high);
  ((Matrix61c*)self)->mat = new_mat;
  ((Matrix61c*)self)->shape = get_shape(new_mat->rows, new_mat->cols);
  return 0;
}

/*
 * Matrix(rows, cols, val). Fill a matrix of dimension rows * cols with val
 */
int init_fill(PyObject* self, int rows, int cols, double val) {
  matrix* new_mat;
  int alloc_failed = allocate_matrix(&new_mat, rows, cols);
  if (alloc_failed)
    return alloc_failed;
  else {
    fill_matrix(new_mat, val);
    ((Matrix61c*)self)->mat = new_mat;
    ((Matrix61c*)self)->shape = get_shape(new_mat->rows, new_mat->cols);
  }
  return 0;
}

/*
 * Matrix(rows, cols, 1d_list). Fill a matrix with dimension rows * cols with 1d_list values
 */
int init_1d(PyObject* self, int rows, int cols, PyObject* lst) {
  if (rows * cols != PyList_Size(lst)) {
    PyErr_SetString(PyExc_ValueError, "Incorrect number of elements in list");
    return -1;
  }
  matrix* new_mat;
  int alloc_failed = allocate_matrix(&new_mat, rows, cols);
  if (alloc_failed)
    return alloc_failed;
  int count = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      set(new_mat, i, j, PyFloat_AsDouble(PyList_GetItem(lst, count)));
      count++;
    }
  }
  ((Matrix61c*)self)->mat = new_mat;
  ((Matrix61c*)self)->shape = get_shape(new_mat->rows, new_mat->cols);
  return 0;
}

/*
 * Matrix(2d_list). Fill a matrix with dimension len(2d_list) * len(2d_list[0])
 */
int init_2d(PyObject* self, PyObject* lst) {
  int rows = PyList_Size(lst);
  if (rows == 0) {
    PyErr_SetString(PyExc_ValueError,
                    "Cannot initialize numc.Matrix with an empty list");
    return -1;
  }
  int cols;
  if (!PyList_Check(PyList_GetItem(lst, 0))) {
    PyErr_SetString(PyExc_ValueError, "List values not valid");
    return -1;
  } else {
    cols = PyList_Size(PyList_GetItem(lst, 0));
  }
  for (int i = 0; i < rows; i++) {
    if (!PyList_Check(PyList_GetItem(lst, i)) ||
        PyList_Size(PyList_GetItem(lst, i)) != cols) {
      PyErr_SetString(PyExc_ValueError, "List values not valid");
      return -1;
    }
  }
  matrix* new_mat;
  int alloc_failed = allocate_matrix(&new_mat, rows, cols);
  if (alloc_failed)
    return alloc_failed;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      set(new_mat, i, j,
          PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(lst, i), j)));
    }
  }
  ((Matrix61c*)self)->mat = new_mat;
  ((Matrix61c*)self)->shape = get_shape(new_mat->rows, new_mat->cols);
  return 0;
}

/*
 * This deallocation function is called when reference count is 0
 */
void Matrix61c_dealloc(Matrix61c* self) {
  deallocate_matrix(self->mat);
  Py_TYPE(self)->tp_free(self);
}

/* For immutable types all initializations should take place in tp_new */
PyObject* Matrix61c_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  /* size of allocated memory is tp_basicsize + nitems*tp_itemsize*/
  Matrix61c* self = (Matrix61c*)type->tp_alloc(type, 0);
  return (PyObject*)self;
}

/*
 * This matrix61c type is mutable, so needs init function. Return 0 on success otherwise -1
 */
int Matrix61c_init(PyObject* self, PyObject* args, PyObject* kwds) {
  /* Generate random matrices */
  if (kwds != NULL) {
    PyObject* rand = PyDict_GetItemString(kwds, "rand");
    if (!rand) {
      PyErr_SetString(PyExc_TypeError, "Invalid arguments");
      return -1;
    }
    if (!PyBool_Check(rand)) {
      PyErr_SetString(PyExc_TypeError, "Invalid arguments");
      return -1;
    }
    if (rand != Py_True) {
      PyErr_SetString(PyExc_TypeError, "Invalid arguments");
      return -1;
    }

    PyObject* low = PyDict_GetItemString(kwds, "low");
    PyObject* high = PyDict_GetItemString(kwds, "high");
    PyObject* seed = PyDict_GetItemString(kwds, "seed");
    double double_low = 0;
    double double_high = 1;
    unsigned int unsigned_seed = 0;

    if (low) {
      if (PyFloat_Check(low)) {
        double_low = PyFloat_AsDouble(low);
      } else if (PyLong_Check(low)) {
        double_low = PyLong_AsLong(low);
      }
    }

    if (high) {
      if (PyFloat_Check(high)) {
        double_high = PyFloat_AsDouble(high);
      } else if (PyLong_Check(high)) {
        double_high = PyLong_AsLong(high);
      }
    }

    if (double_low >= double_high) {
      PyErr_SetString(PyExc_TypeError, "Invalid arguments");
      return -1;
    }

    // Set seed if argument exists
    if (seed) {
      if (PyLong_Check(seed)) {
        unsigned_seed = PyLong_AsUnsignedLong(seed);
      }
    }

    PyObject* rows = NULL;
    PyObject* cols = NULL;
    if (PyArg_UnpackTuple(args, "args", 2, 2, &rows, &cols)) {
      if (rows && cols && PyLong_Check(rows) && PyLong_Check(cols)) {
        return init_rand(self, PyLong_AsLong(rows), PyLong_AsLong(cols),
                         unsigned_seed, double_low, double_high);
      }
    } else {
      PyErr_SetString(PyExc_TypeError, "Invalid arguments");
      return -1;
    }
  }
  PyObject* arg1 = NULL;
  PyObject* arg2 = NULL;
  PyObject* arg3 = NULL;
  if (PyArg_UnpackTuple(args, "args", 1, 3, &arg1, &arg2, &arg3)) {
    /* arguments are (rows, cols, val) */
    if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) &&
        (PyLong_Check(arg3) || PyFloat_Check(arg3))) {
      if (PyLong_Check(arg3)) {
        return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2),
                         PyLong_AsLong(arg3));
      } else
        return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2),
                         PyFloat_AsDouble(arg3));
    } else if (arg1 && arg2 && arg3 && PyLong_Check(arg1) &&
               PyLong_Check(arg2) && PyList_Check(arg3)) {
      /* Matrix(rows, cols, 1D list) */
      return init_1d(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), arg3);
    } else if (arg1 && PyList_Check(arg1) && arg2 == NULL && arg3 == NULL) {
      /* Matrix(rows, cols, 1D list) */
      return init_2d(self, arg1);
    } else if (arg1 && arg2 && PyLong_Check(arg1) && PyLong_Check(arg2) &&
               arg3 == NULL) {
      /* Matrix(rows, cols, 1D list) */
      return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), 0);
    } else {
      PyErr_SetString(PyExc_TypeError, "Invalid arguments");
      return -1;
    }
  } else {
    PyErr_SetString(PyExc_TypeError, "Invalid arguments");
    return -1;
  }
}

/*
 * List of lists representations for matrices
 */
PyObject* Matrix61c_to_list(Matrix61c* self) {
  int rows = self->mat->rows;
  int cols = self->mat->cols;
  PyObject* py_lst = NULL;
  if (self->mat->is_1d) {  // If 1D matrix, print as a single list
    py_lst = PyList_New(rows * cols);
    int count = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        PyList_SetItem(py_lst, count, PyFloat_FromDouble(get(self->mat, i, j)));
        count++;
      }
    }
  } else {  // if 2D, print as nested list
    py_lst = PyList_New(rows);
    for (int i = 0; i < rows; i++) {
      PyList_SetItem(py_lst, i, PyList_New(cols));
      PyObject* curr_row = PyList_GetItem(py_lst, i);
      for (int j = 0; j < cols; j++) {
        PyList_SetItem(curr_row, j, PyFloat_FromDouble(get(self->mat, i, j)));
      }
    }
  }
  return py_lst;
}

PyObject* Matrix61c_class_to_list(Matrix61c* self, PyObject* args) {
  PyObject* mat = NULL;
  if (PyArg_UnpackTuple(args, "args", 1, 1, &mat)) {
    if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
      PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
      return NULL;
    }
    Matrix61c* mat61c = (Matrix61c*)mat;
    return Matrix61c_to_list(mat61c);
  } else {
    PyErr_SetString(PyExc_TypeError, "Invalid arguments");
    return NULL;
  }
}

/*
 * Add class methods
 */
PyMethodDef Matrix61c_class_methods[] = {
    {"to_list", (PyCFunction)Matrix61c_class_to_list, METH_VARARGS,
     "Returns a list representation of numc.Matrix"},
    {NULL, NULL, 0, NULL}};

/*
 * Matrix61c string representation. For printing purposes.
 */
PyObject* Matrix61c_repr(PyObject* self) {
  PyObject* py_lst = Matrix61c_to_list((Matrix61c*)self);
  return PyObject_Repr(py_lst);
}

/* NUMBER METHODS */

/*
 * Add the second numc.Matrix (Matrix61c) object to the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject* Matrix61c_add(Matrix61c* self, PyObject* args) {
  if (!PyObject_TypeCheck(args, &Matrix61cType)) {
    PyErr_SetString(PyExc_TypeError, "Argument must be of type numc.Matrix");
    return NULL;
  }
  Matrix61c* other = (Matrix61c*)args;
  if (self->mat->rows != other->mat->rows ||
      self->mat->cols != other->mat->cols) {
    PyErr_SetString(PyExc_ValueError,
                    "Matrices must have the same dimensions to be added");
    return NULL;
  }
  Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
  if (!result)
    return NULL;
  if (allocate_matrix(&result->mat, self->mat->rows, self->mat->cols) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  if (add_matrix(result->mat, self->mat, other->mat) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  result->shape = get_shape(result->mat->rows, result->mat->cols);
  return (PyObject*)result;
}

/*
 * Substract the second numc.Matrix (Matrix61c) object from the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject* Matrix61c_sub(Matrix61c* self, PyObject* args) {
  if (!PyObject_TypeCheck(args, &Matrix61cType)) {
    PyErr_SetString(PyExc_TypeError, "Argument must be of type numc.Matrix");
    return NULL;
  }
  Matrix61c* other = (Matrix61c*)args;
  if (self->mat->rows != other->mat->rows ||
      self->mat->cols != other->mat->cols) {
    PyErr_SetString(PyExc_ValueError,
                    "Matrices must have the same dimensions to be subtracted");
    return NULL;
  }
  Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
  if (!result)
    return NULL;
  if (allocate_matrix(&result->mat, self->mat->rows, self->mat->cols) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  if (sub_matrix(result->mat, self->mat, other->mat) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  result->shape = get_shape(result->mat->rows, result->mat->cols);
  return (PyObject*)result;
}

/*
 * NOT element-wise multiplication. The first operand is self, and the second operand
 * can be obtained by casting `args`.
 */
PyObject* Matrix61c_multiply(Matrix61c* self, PyObject* args) {
  if (!PyObject_TypeCheck(args, &Matrix61cType)) {
    PyErr_SetString(PyExc_TypeError, "Argument must be of type numc.Matrix");
    return NULL;
  }
  Matrix61c* other = (Matrix61c*)args;
  if (self->mat->cols != other->mat->rows) {
    PyErr_SetString(PyExc_ValueError,
                    "Number of columns in the first matrix must equal the "
                    "number of rows in the second matrix");
    return NULL;
  }
  Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
  if (!result)
    return NULL;
  if (allocate_matrix(&result->mat, self->mat->rows, other->mat->cols) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  if (mul_matrix(result->mat, self->mat, other->mat) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  result->shape = get_shape(result->mat->rows, result->mat->cols);
  return (PyObject*)result;
}

/*
 * Negates the given numc.Matrix.
 */
PyObject* Matrix61c_neg(Matrix61c* self) {
  Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
  if (!result)
    return NULL;
  if (allocate_matrix(&result->mat, self->mat->rows, self->mat->cols) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  if (neg_matrix(result->mat, self->mat) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  result->shape = get_shape(result->mat->rows, result->mat->cols);
  return (PyObject*)result;
}

/*
 * Take the element-wise absolute value of this numc.Matrix.
 */
PyObject* Matrix61c_abs(Matrix61c* self) {
  Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
  if (!result)
    return NULL;
  if (allocate_matrix(&result->mat, self->mat->rows, self->mat->cols) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  if (abs_matrix(result->mat, self->mat) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  result->shape = get_shape(result->mat->rows, result->mat->cols);
  return (PyObject*)result;
}

/*
 * Raise numc.Matrix (Matrix61c) to the `pow`th power. You can ignore the argument `optional`.
 */
PyObject* Matrix61c_pow(Matrix61c* self, PyObject* pow, PyObject* optional) {
  if (!PyLong_Check(pow)) {
    PyErr_SetString(PyExc_TypeError, "Exponent must be an integer");
    return NULL;
  }
  long exponent = PyLong_AsLong(pow);
  if (self->mat->rows != self->mat->cols) {
    PyErr_SetString(PyExc_ValueError,
                    "Matrix must be square for exponentiation");
    return NULL;
  }
  Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
  if (!result)
    return NULL;
  if (allocate_matrix(&result->mat, self->mat->rows, self->mat->cols) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  if (pow_matrix(result->mat, self->mat, exponent) != 0) {
    Py_DECREF(result);
    return NULL;
  }
  result->shape = get_shape(result->mat->rows, result->mat->cols);
  return (PyObject*)result;
}

/*
 * Create a PyNumberMethods struct for overloading operators with all the number methods you have
 * define. You might find this link helpful: https://docs.python.org/3.6/c-api/typeobj.html
 */
PyNumberMethods Matrix61c_as_number = {
    (binaryfunc)Matrix61c_add,      /* nb_add */
    (binaryfunc)Matrix61c_sub,      /* nb_subtract */
    (binaryfunc)Matrix61c_multiply, /* nb_multiply */
    0,                              /* nb_remainder */
    0,                              /* nb_divmod */
    (ternaryfunc)Matrix61c_pow,     /* nb_power */
    (unaryfunc)Matrix61c_neg,       /* nb_negative */
    0,                              /* nb_positive */
    (unaryfunc)Matrix61c_abs,       /* nb_absolute */
    0,                              /* nb_bool */
    0,                              /* nb_invert */
    0,                              /* nb_lshift */
    0,                              /* nb_rshift */
    0,                              /* nb_and */
    0,                              /* nb_xor */
    0,                              /* nb_or */
    0,                              /* nb_int */
    0,                              /* nb_reserved */
    0,                              /* nb_float */
};

/* INSTANCE METHODS */

/*
 * Given a numc.Matrix self, parse `args` to (int) row, (int) col, and (double/int) val.
 * Return None in Python (this is different from returning null).
 */
PyObject* Matrix61c_set_value(Matrix61c* self, PyObject* args) {
  int row, col;
  double val;
  if (!PyArg_ParseTuple(args, "iid", &row, &col, &val)) {
    return NULL;
  }
  if (row >= self->mat->rows || col >= self->mat->cols || row < 0 || col < 0) {
    PyErr_SetString(PyExc_IndexError, "Index out of bounds");
    return NULL;
  }
  set(self->mat, row, col, val);
  Py_RETURN_NONE;
}

/*
 * Given a numc.Matrix `self`, parse `args` to (int) row and (int) col.
 * Return the value at the `row`th row and `col`th column, which is a Python
 * float/int.
 */
PyObject* Matrix61c_get_value(Matrix61c* self, PyObject* args) {
  int row, col;
  if (!PyArg_ParseTuple(args, "ii", &row, &col)) {
    return NULL;
  }
  if (row >= self->mat->rows || col >= self->mat->cols || row < 0 || col < 0) {
    PyErr_SetString(PyExc_IndexError, "Index out of bounds");
    return NULL;
  }
  return PyFloat_FromDouble(get(self->mat, row, col));
}

/*
 * Create an array of PyMethodDef structs to hold the instance methods.
 * Name the python function corresponding to Matrix61c_get_value as "get" and Matrix61c_set_value
 * as "set"
 * You might find this link helpful: https://docs.python.org/3.6/c-api/structures.html
 */
PyMethodDef Matrix61c_methods[] = {{"get", (PyCFunction)Matrix61c_get_value,
                                    METH_VARARGS, "Get value at (row, col)"},
                                   {"set", (PyCFunction)Matrix61c_set_value,
                                    METH_VARARGS, "Set value at (row, col)"},
                                   {NULL, NULL, 0, NULL}};

/* INDEXING */

/*
 * Given a numc.Matrix `self`, index into it with `key`. Return the indexed result.
 */
PyObject* Matrix61c_subscript(Matrix61c* self, PyObject* key) {
  if (PyLong_Check(key)) {
    long index = PyLong_AsLong(key);
    if (self->mat->is_1d) {
      if (index < 0)
        index += self->mat->rows * self->mat->cols;
      if (index < 0 || index >= self->mat->rows * self->mat->cols) {
        PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        return NULL;
      }
      return PyFloat_FromDouble(self->mat->data[0][index]);
    } else {
      if (index < 0)
        index += self->mat->rows;
      if (index < 0 || index >= self->mat->rows) {
        PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        return NULL;
      }
      Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
      if (!result)
        return NULL;
      allocate_matrix_ref(&result->mat, self->mat, index, 0, 1,
                          self->mat->cols);
      result->shape = get_shape(1, self->mat->cols);
      return (PyObject*)result;
    }
  } else if (PySlice_Check(key)) {
    Py_ssize_t start, stop, step, slicelength;
    if (PySlice_GetIndicesEx(key,
                             self->mat->is_1d
                                 ? self->mat->rows * self->mat->cols
                                 : self->mat->rows,
                             &start, &stop, &step, &slicelength) < 0) {
      return NULL;
    }
    if (step != 1) {
      PyErr_SetString(PyExc_ValueError,
                      "Slicing with step other than 1 is not supported");
      return NULL;
    }
    Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
    if (!result)
      return NULL;
    if (self->mat->is_1d) {
      allocate_matrix_ref(&result->mat, self->mat, 0, start, 1, slicelength);
      result->shape = get_shape(slicelength, 1);
    } else {
      allocate_matrix_ref(&result->mat, self->mat, start, 0, slicelength,
                          self->mat->cols);
      result->shape = get_shape(slicelength, self->mat->cols);
    }
    return (PyObject*)result;
  } else if (PyTuple_Check(key) && PyTuple_Size(key) == 2) {
    PyObject* o1 = PyTuple_GetItem(key, 0);
    PyObject* o2 = PyTuple_GetItem(key, 1);

    if (PyLong_Check(o1) && PyLong_Check(o2)) {
      long r = PyLong_AsLong(o1);
      long c = PyLong_AsLong(o2);
      if (r < 0)
        r += self->mat->rows;
      if (c < 0)
        c += self->mat->cols;
      if (r < 0 || r >= self->mat->rows || c < 0 || c >= self->mat->cols) {
        PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        return NULL;
      }
      return PyFloat_FromDouble(get(self->mat, r, c));
    } else if (PySlice_Check(o1) && PySlice_Check(o2)) {
      Py_ssize_t r_start, r_stop, r_step, r_len;
      Py_ssize_t c_start, c_stop, c_step, c_len;
      if (PySlice_GetIndicesEx(o1, self->mat->rows, &r_start, &r_stop, &r_step,
                               &r_len) < 0)
        return NULL;
      if (PySlice_GetIndicesEx(o2, self->mat->cols, &c_start, &c_stop, &c_step,
                               &c_len) < 0)
        return NULL;
      if (r_step != 1 || c_step != 1) {
        PyErr_SetString(PyExc_ValueError,
                        "Slicing with step other than 1 is not supported");
        return NULL;
      }
      Matrix61c* result = (Matrix61c*)Matrix61c_new(&Matrix61cType, NULL, NULL);
      if (!result)
        return NULL;
      allocate_matrix_ref(&result->mat, self->mat, r_start, c_start, r_len,
                          c_len);
      result->shape = get_shape(r_len, c_len);
      return (PyObject*)result;
    }
  }
  PyErr_SetString(PyExc_TypeError, "Invalid arguments for indexing");
  return NULL;
}

/*
 * Given a numc.Matrix `self`, index into it with `key`, and set the indexed result to `v`.
 */
int Matrix61c_set_subscript(Matrix61c* self, PyObject* key, PyObject* v) {
  if (v == NULL) {
    PyErr_SetString(PyExc_TypeError, "Cannot delete matrix elements");
    return -1;
  }

  if (PyLong_Check(key)) {
    long index = PyLong_AsLong(key);
    if (self->mat->is_1d) {
      if (!PyFloat_Check(v) && !PyLong_Check(v)) {
        PyErr_SetString(PyExc_TypeError, "Value must be a number");
        return -1;
      }
      if (index < 0)
        index += self->mat->rows * self->mat->cols;
      if (index < 0 || index >= self->mat->rows * self->mat->cols) {
        PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        return -1;
      }
      self->mat->data[0][index] = PyFloat_AsDouble(v);
      return 0;
    } else {
      PyErr_SetString(PyExc_TypeError,
                      "Single integer indexing for 2D matrix row assignment is "
                      "not supported for setitem. Try slicing.");
      return -1;
    }
  } else if (PySlice_Check(key)) {
    Py_ssize_t start, stop, step, slicelength;
    if (PySlice_GetIndicesEx(key,
                             self->mat->is_1d
                                 ? self->mat->rows * self->mat->cols
                                 : self->mat->rows,
                             &start, &stop, &step, &slicelength) < 0) {
      return -1;
    }
    if (step != 1) {
      PyErr_SetString(PyExc_ValueError,
                      "Slicing with step other than 1 is not supported");
      return -1;
    }
    if (self->mat->is_1d) {
      if (PyList_Check(v)) {
        if (PyList_Size(v) != slicelength) {
          PyErr_SetString(PyExc_ValueError, "Mismatched dimensions");
          return -1;
        }
        for (int i = 0; i < slicelength; i++) {
          PyObject* item = PyList_GetItem(v, i);
          if (!PyFloat_Check(item) && !PyLong_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "List elements must be numbers");
            return -1;
          }
          self->mat->data[0][start + i] = PyFloat_AsDouble(item);
        }
        return 0;
      } else if (PyObject_TypeCheck(v, &Matrix61cType)) {
        Matrix61c* mat_v = (Matrix61c*)v;
        if (mat_v->mat->rows * mat_v->mat->cols != slicelength) {
          PyErr_SetString(PyExc_ValueError, "Mismatched dimensions");
          return -1;
        }
        for (int i = 0; i < slicelength; i++) {
          self->mat->data[0][start + i] = mat_v->mat->data[0][i];
        }
        return 0;
      }
    } else {
      if (PyList_Check(v)) {
        if (PyList_Size(v) != slicelength) {
          PyErr_SetString(PyExc_ValueError, "Mismatched dimensions");
          return -1;
        }
        for (int i = 0; i < slicelength; i++) {
          PyObject* row_list = PyList_GetItem(v, i);
          if (!PyList_Check(row_list) ||
              PyList_Size(row_list) != self->mat->cols) {
            PyErr_SetString(PyExc_ValueError,
                            "Mismatched dimensions or invalid row data");
            return -1;
          }
          for (int j = 0; j < self->mat->cols; j++) {
            PyObject* item = PyList_GetItem(row_list, j);
            if (!PyFloat_Check(item) && !PyLong_Check(item)) {
              PyErr_SetString(PyExc_TypeError, "List elements must be numbers");
              return -1;
            }
            set(self->mat, start + i, j, PyFloat_AsDouble(item));
          }
        }
        return 0;
      } else if (PyObject_TypeCheck(v, &Matrix61cType)) {
        Matrix61c* mat_v = (Matrix61c*)v;
        if (mat_v->mat->rows != slicelength ||
            mat_v->mat->cols != self->mat->cols) {
          PyErr_SetString(PyExc_ValueError, "Mismatched dimensions");
          return -1;
        }
        for (int i = 0; i < slicelength; i++) {
          for (int j = 0; j < self->mat->cols; j++) {
            set(self->mat, start + i, j, get(mat_v->mat, i, j));
          }
        }
        return 0;
      }
    }
  } else if (PyTuple_Check(key) && PyTuple_Size(key) == 2) {
    PyObject* o1 = PyTuple_GetItem(key, 0);
    PyObject* o2 = PyTuple_GetItem(key, 1);

    if (PyLong_Check(o1) && PyLong_Check(o2)) {
      if (!PyFloat_Check(v) && !PyLong_Check(v)) {
        PyErr_SetString(PyExc_TypeError, "Value must be a number");
        return -1;
      }
      long r = PyLong_AsLong(o1);
      long c = PyLong_AsLong(o2);
      if (r < 0)
        r += self->mat->rows;
      if (c < 0)
        c += self->mat->cols;
      if (r < 0 || r >= self->mat->rows || c < 0 || c >= self->mat->cols) {
        PyErr_SetString(PyExc_IndexError, "Index out of bounds");
        return -1;
      }
      set(self->mat, r, c, PyFloat_AsDouble(v));
      return 0;
    } else if (PySlice_Check(o1) && PySlice_Check(o2)) {
      Py_ssize_t r_start, r_stop, r_step, r_len;
      Py_ssize_t c_start, c_stop, c_step, c_len;
      if (PySlice_GetIndicesEx(o1, self->mat->rows, &r_start, &r_stop, &r_step,
                               &r_len) < 0)
        return -1;
      if (PySlice_GetIndicesEx(o2, self->mat->cols, &c_start, &c_stop, &c_step,
                               &c_len) < 0)
        return -1;
      if (r_step != 1 || c_step != 1) {
        PyErr_SetString(PyExc_ValueError,
                        "Slicing with step other than 1 is not supported");
        return -1;
      }
      if (PyObject_TypeCheck(v, &Matrix61cType)) {
        Matrix61c* mat_v = (Matrix61c*)v;
        if (mat_v->mat->rows != r_len || mat_v->mat->cols != c_len) {
          PyErr_SetString(PyExc_ValueError, "Mismatched dimensions");
          return -1;
        }
        for (int i = 0; i < r_len; i++) {
          for (int j = 0; j < c_len; j++) {
            set(self->mat, r_start + i, c_start + j, get(mat_v->mat, i, j));
          }
        }
        return 0;
      } else if (PyList_Check(v)) {
        if (PyList_Size(v) != r_len) {
          PyErr_SetString(PyExc_ValueError, "Mismatched dimensions");
          return -1;
        }
        for (int i = 0; i < r_len; i++) {
          PyObject* row_list = PyList_GetItem(v, i);
          if (!PyList_Check(row_list) || PyList_Size(row_list) != c_len) {
            PyErr_SetString(PyExc_ValueError,
                            "Mismatched dimensions or invalid row data");
            return -1;
          }
          for (int j = 0; j < c_len; j++) {
            PyObject* item = PyList_GetItem(row_list, j);
            if (!PyFloat_Check(item) && !PyLong_Check(item)) {
              PyErr_SetString(PyExc_TypeError, "List elements must be numbers");
              return -1;
            }
            set(self->mat, r_start + i, c_start + j, PyFloat_AsDouble(item));
          }
        }
        return 0;
      }
    }
  }
  PyErr_SetString(PyExc_TypeError, "Invalid arguments for setting subscript");
  return -1;
}

PyMappingMethods Matrix61c_mapping = {
    NULL,
    (binaryfunc)Matrix61c_subscript,
    (objobjargproc)Matrix61c_set_subscript,
};

/* INSTANCE ATTRIBUTES*/
PyMemberDef Matrix61c_members[] = {
    {"shape", T_OBJECT_EX, offsetof(Matrix61c, shape), 0, "(rows, cols)"},
    {NULL} /* Sentinel */
};

PyTypeObject Matrix61cType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "numc.Matrix",
    .tp_basicsize = sizeof(Matrix61c),
    .tp_dealloc = (destructor)Matrix61c_dealloc,
    .tp_repr = (reprfunc)Matrix61c_repr,
    .tp_as_number = &Matrix61c_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "numc.Matrix objects",
    .tp_methods = Matrix61c_methods,
    .tp_members = Matrix61c_members,
    .tp_as_mapping = &Matrix61c_mapping,
    .tp_init = (initproc)Matrix61c_init,
    .tp_new = Matrix61c_new};

struct PyModuleDef numcmodule = {PyModuleDef_HEAD_INIT, "numc",
                                 "Numc matrix operations", -1,
                                 Matrix61c_class_methods};

/* Initialize the numc module */
PyMODINIT_FUNC PyInit_numc(void) {
  PyObject* m;

  if (PyType_Ready(&Matrix61cType) < 0)
    return NULL;

  m = PyModule_Create(&numcmodule);
  if (m == NULL)
    return NULL;

  Py_INCREF(&Matrix61cType);
  PyModule_AddObject(m, "Matrix", (PyObject*)&Matrix61cType);
  printf("CS61C Fall 2020 Project 4: numc imported!\n");
  fflush(stdout);
  return m;
}