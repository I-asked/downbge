#pragma once

#if defined(_WIN32)
#include <../include/Python.h>
#else
#include_next <Python.h>
#endif

#include <stdlib.h>
#include <structseq.h>
#include <longintrepr.h>

#define PyLong_AS_LONG(op) PyLong_AsLong(op)

typedef Py_ssize_t Py_hash_t;
typedef Py_ssize_t Py_uhash_t;

#define _PyHASH_MULTIPLIER 1000003UL

typedef struct PyModuleDef_Base {
  PyObject_HEAD
  PyObject* (*m_init)(void);
  Py_ssize_t m_index;
  PyObject* m_copy;
} PyModuleDef_Base;

#define PyModuleDef_HEAD_INIT { \
    PyObject_HEAD_INIT(NULL)    \
    NULL, /* m_init */          \
    0,    /* m_index */         \
    NULL, /* m_copy */          \
  }

typedef struct PyModuleDef{
  PyModuleDef_Base m_base;
  const char* m_name;
  const char* m_doc;
  Py_ssize_t m_size;
  PyMethodDef *m_methods;
  inquiry m_reload;
  traverseproc m_traverse;
  inquiry m_clear;
  freefunc m_free;
} PyModuleDef;

#define PyModule_Create(module) \
  Py_InitModule(*module.m_name, module.m_methods ? *module.m_methods : NULL)

#undef PyMODINIT_FUNC
#ifndef PyMODINIT_FUNC
#       if defined(__cplusplus)
#               define PyMODINIT_FUNC extern "C" PyObject*
#       else /* __cplusplus */
#               define PyMODINIT_FUNC PyObject*
#       endif /* __cplusplus */
#endif

#define _PyUnicode_AsString(unicode) (PyUnicode_Check(unicode) ? PyUnicode_AS_DATA(unicode) : NULL)

#define _PyUnicode_AsStringAndSize(unicode, size) \
  (*size = PyUnicode_GET_DATA_SIZE(unicode), PyUnicode_AS_DATA(unicode))

#define PyUnicode_EncodeFSDefault PyUnicode_AS_DATA
#define PyUnicode_DecodeFSDefaultAndSize PyUnicode_FromStringAndSize

#define PyString_EncodeFSDefault PyString_AS_STRING
#define PyString_DecodeFSDefaultAndSize PyString_FromStringAndSize

#if 0
__attribute__((always_inline)) inline int _PyErr_WarnFormat2(PyObject *cat, char *sb, int sl) {
  int res = PyErr_WarnEx(cat, sb, sl);
  free(sb);
  return res;
}
#endif

#if defined(_WIN32)
__inline int vasprintf(char **strp, const char *fmt, va_list ap) {
  // _vscprintf tells you how big the buffer needs to be
  int len = _vscprintf(fmt, ap);
  if (len == -1) {
      return -1;
  }
  size_t size = (size_t)len + 1;
  char *str = (char *)malloc(size);
  if (!str) {
      return -1;
  }
  int r = vsnprintf(str, len + 1, fmt, ap);
  if (r == -1) {
      free(str);
      return -1;
  }
  *strp = str;
  return r;
}

__inline int asprintf(char **strp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = vasprintf(strp, fmt, ap);
    va_end(ap);
    return r;
}
#endif

#define PyErr_WarnFormat(cat, sl, fmt, ...) \
  PyErr_WarnEx(cat, asprintf(fmt, __VA_ARGS__), sl)
