/**
 * package.c : wrapper class around pmpkg_t
 *
 *  Copyright (c) 2011 Rémy Oudompheng <remy@archlinux.org>
 *
 *  This file is part of pyalpm.
 *
 *  pyalpm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pyalpm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pyalpm.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <alpm.h>
#include <Python.h>
#include "package.h"

struct PyGetSetDef AlpmPackageGetSet[];
static void pyalpm_package_dealloc(AlpmPackage *self);

PyTypeObject AlpmPackageType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "alpm.Package",             /*tp_name*/
  sizeof(AlpmPackage),        /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  (destructor)pyalpm_package_dealloc,     /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  NULL,                       /*tp_reserved*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash */
  0,                          /*tp_call*/
  0,                          /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "Package object",            /* tp_doc */
  0,                          /* tp_traverse */
  0,                          /* tp_clear */
  0,                          /* tp_richcompare */
  0,                          /* tp_weaklistoffset */
  0,                          /* tp_iter */
  0,                          /* tp_iternext */
  0,                          /* tp_methods */
  0,                          /* tp_members */
  AlpmPackageGetSet,          /* tp_getset */
  0,                          /* tp_base */
  0,                          /* tp_dict */
  0,                          /* tp_descr_get */
  0,                          /* tp_descr_set */
  0,                          /* tp_dictoffset */
  0,        /* tp_init */
  0,                          /* tp_alloc */
  (newfunc)pyalpm_package_new,         /* tp_new */
};

/** Initializes Package class in module */
void init_pyalpm_package(PyObject *module) {
  PyObject *type;

  if (PyType_Ready(&AlpmPackageType) < 0)
    return;
  type = (PyObject*)&AlpmPackageType;
  Py_INCREF(type);
  PyModule_AddObject(module, "Package", type);
}

PyObject *pyalpm_package_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  AlpmPackage *self;
  self = (AlpmPackage*)type->tp_alloc(type, 0);
  if (self != NULL) {
    self->c_data = NULL;
  }

  return (PyObject *)self;
}

void pyalpm_package_dealloc(AlpmPackage *self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *pyalpm_package_get_filename(AlpmPackage *self, void *closure) {
  const char *filename;

  if (! self->c_data) {
    PyErr_SetString(PyExc_RuntimeError, "data is not initialized");
    return NULL;
  }

  filename = alpm_pkg_get_filename(self->c_data);
  if (filename == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get filename");
    return NULL;
  }
  return Py_BuildValue("s", filename);
}

static PyObject* pyalpm_package_get_name(AlpmPackage *self, void *closure) {
  const char *name;

  if (! self->c_data) {
    PyErr_SetString(PyExc_RuntimeError, "data is not initialized");
    return NULL;
  }

  name = alpm_pkg_get_name(self->c_data);
  if (name == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get name");
    return NULL;
  }
  return Py_BuildValue("s", name);
}

static PyObject* pyalpm_package_get_version(AlpmPackage *self, void *closure) {
  const char *version;

  if (! self->c_data) {
    PyErr_SetString(PyExc_RuntimeError, "data is not initialized");
    return NULL;
  }

  version = alpm_pkg_get_version(self->c_data);
  if (version == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get version");
    return NULL;
  }
  return Py_BuildValue("s", version);
}

static PyObject* pyalpm_package_get_desc(AlpmPackage *self, void *closure) {
  const char *desc;

  if (! self->c_data) {
    PyErr_SetString(PyExc_RuntimeError, "data is not initialized");
    return NULL;
  }

  desc = alpm_pkg_get_desc(self->c_data);
  if (desc == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "unable to get desc");
    return NULL;
  }
  return Py_BuildValue("s", desc);
}

struct PyGetSetDef AlpmPackageGetSet[] = {
  { "name", (getter)pyalpm_package_get_name, 0, "package name", NULL } ,
  { "version", (getter)pyalpm_package_get_version, 0, "package version", NULL } ,
  { "desc", (getter)pyalpm_package_get_desc, 0, "package desc", NULL } ,
  { "filename", (getter)pyalpm_package_get_filename, 0, "package filename", NULL } ,
  { NULL }
};