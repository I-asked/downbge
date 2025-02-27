/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * Contributor(s): Joseph Eagar, Campbell Barton
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/python/generic/idprop_py_api.c
 *  \ingroup pygen
 */

#include <Python.h>

#include "MEM_guardedalloc.h"

#include "BLI_utildefines.h"

#include "idprop_py_api.h"

#include "BKE_idprop.h"

#define USE_STRING_COERCE

#ifdef USE_STRING_COERCE
#include "py_capi_utils.h"
#endif

#include "python_utildefines.h"


/*********************** ID Property Main Wrapper Stuff ***************/

/* ----------------------------------------------------------------------------
 * static conversion functions to avoid duplicate code, no type checking.
 */

static PyObject *idprop_py_from_idp_string(const IDProperty *prop)
{
	if (prop->subtype == IDP_STRING_SUB_BYTE) {
		return PyBytes_FromStringAndSize(IDP_String(prop), prop->len);
	}
	else {
#ifdef USE_STRING_COERCE
		return PyC_StringFromByteAndSize(IDP_Array(prop), prop->len - 1);
#else
		return PyString_FromStringAndSize(IDP_String(prop), prop->len - 1);
#endif
	}
}

static PyObject *idprop_py_from_idp_int(const IDProperty *prop)
{
	return PyLong_FromLong((long)IDP_Int(prop));
}

static PyObject *idprop_py_from_idp_float(const IDProperty *prop)
{
	return PyFloat_FromDouble((double)IDP_Float(prop));
}

static PyObject *idprop_py_from_idp_double(const IDProperty *prop)
{
	return PyFloat_FromDouble(IDP_Double(prop));
}

static PyObject *idprop_py_from_idp_group(ID *id, IDProperty *prop, IDProperty *parent)
{
	BPy_IDProperty *group = PyObject_New(BPy_IDProperty, &BPy_IDGroup_Type);
	group->id = id;
	group->prop = prop;
	group->parent = parent; /* can be NULL */
	return (PyObject *)group;
}

static PyObject *idprop_py_from_idp_array(ID *id, IDProperty *prop)
{
	BPy_IDProperty *array = PyObject_New(BPy_IDProperty, &BPy_IDArray_Type);
	array->id = id;
	array->prop = prop;
	return (PyObject *)array;
}

static PyObject *idprop_py_from_idp_idparray(ID *id, IDProperty *prop)
{
	PyObject *seq = PyList_New(prop->len), *wrap;
	IDProperty *array = IDP_IDPArray(prop);
	int i;

	if (!seq) {
		PyErr_Format(PyExc_RuntimeError,
		             "%s: IDP_IDPARRAY: PyList_New(%d) failed",
		             __func__, prop->len);
		return NULL;
	}

	for (i = 0; i < prop->len; i++) {
		wrap = BPy_IDGroup_WrapData(id, array++, prop);

		if (!wrap) /* BPy_IDGroup_MapDataToPy sets the error */
			return NULL;

		PyList_SET_ITEM(seq, i, wrap);
	}

	return seq;
}

/* -------------------------------------------------------------------------- */

/* use for both array and group */
static Py_hash_t BPy_IDGroup_hash(BPy_IDProperty *self)
{
	return _Py_HashPointer(self->prop);
}

static PyObject *BPy_IDGroup_repr(BPy_IDProperty *self)
{
	return PyString_FromFormat("<bpy id prop: owner=\"%s\", name=\"%s\", address=%p>",
	                            self->id ? self->id->name : "<NONE>", self->prop->name, self->prop);
}

PyObject *BPy_IDGroup_WrapData(ID *id, IDProperty *prop, IDProperty *parent)
{
	switch (prop->type) {
		case IDP_STRING:   return idprop_py_from_idp_string(prop);
		case IDP_INT:      return idprop_py_from_idp_int(prop);
		case IDP_FLOAT:    return idprop_py_from_idp_float(prop);
		case IDP_DOUBLE:   return idprop_py_from_idp_double(prop);
		case IDP_GROUP:    return idprop_py_from_idp_group(id, prop, parent);
		case IDP_ARRAY:    return idprop_py_from_idp_array(id, prop);
		case IDP_IDPARRAY: return idprop_py_from_idp_idparray(id, prop); /* this could be better a internal type */
		default: Py_RETURN_NONE;
	}
}

#if 0 /* UNUSED, currently assignment overwrites into new properties, rather than setting in-place */
static int BPy_IDGroup_SetData(BPy_IDProperty *self, IDProperty *prop, PyObject *value)
{
	switch (prop->type) {
		case IDP_STRING:
		{
			char *st;
			if (!PyString_Check(value)) {
				PyErr_SetString(PyExc_TypeError, "expected a string!");
				return -1;
			}
			/* NOTE: if this code is enabled, bytes support needs to be added */
#ifdef USE_STRING_COERCE
			{
				int alloc_len;
				PyObject *value_coerce = NULL;

				st = (char *)PyC_StringAsByte(value, &value_coerce);
				alloc_len = strlen(st) + 1;

				st = PyString_AsString(value);
				IDP_ResizeArray(prop, alloc_len);
				memcpy(IDP_Array(prop), st, alloc_len);
				Py_XDECREF(value_coerce);
			}
#else
			st = PyString_AsString(value);
			IDP_ResizeArray(prop, strlen(st) + 1);
			strcpy(IDP_Array(prop), st);
#endif

			return 0;
		}

		case IDP_INT:
		{
			int ivalue = PyLong_AsSsize_t(value);
			if (ivalue == -1 && PyErr_Occurred()) {
				PyErr_SetString(PyExc_TypeError, "expected an int type");
				return -1;
			}
			IDP_Int(prop) = ivalue;
			break;
		}
		case IDP_FLOAT:
		{
			float fvalue = (float)PyFloat_AsDouble(value);
			if (fvalue == -1 && PyErr_Occurred()) {
				PyErr_SetString(PyExc_TypeError, "expected a float");
				return -1;
			}
			IDP_Float(self->prop) = fvalue;
			break;
		}
		case IDP_DOUBLE:
		{
			double dvalue = PyFloat_AsDouble(value);
			if (dvalue == -1 && PyErr_Occurred()) {
				PyErr_SetString(PyExc_TypeError, "expected a float");
				return -1;
			}
			IDP_Double(self->prop) = dvalue;
			break;
		}
		default:
			PyErr_SetString(PyExc_AttributeError, "attempt to set read-only attribute!");
			return -1;
	}
	return 0;
}
#endif

static PyObject *BPy_IDGroup_GetName(BPy_IDProperty *self, void *UNUSED(closure))
{
	return PyString_FromString(self->prop->name);
}

static int BPy_IDGroup_SetName(BPy_IDProperty *self, PyObject *value, void *UNUSED(closure))
{
	const char *name;
	Py_ssize_t name_size;

	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "expected a string!");
		return -1;
	}

	PyString_AsStringAndSize(value, &name, &name_size);

	if (name_size > MAX_IDPROP_NAME) {
		PyErr_SetString(PyExc_TypeError, "string length cannot exceed 63 characters!");
		return -1;
	}

	memcpy(self->prop->name, name, name_size);
	return 0;
}

#if 0
static PyObject *BPy_IDGroup_GetType(BPy_IDProperty *self)
{
	return PyLong_FromLong(self->prop->type);
}
#endif

static PyGetSetDef BPy_IDGroup_getseters[] = {
	{(char *)"name", (getter)BPy_IDGroup_GetName, (setter)BPy_IDGroup_SetName, (char *)"The name of this Group.", NULL},
	{NULL, NULL, NULL, NULL, NULL}
};

static Py_ssize_t BPy_IDGroup_Map_Len(BPy_IDProperty *self)
{
	if (self->prop->type != IDP_GROUP) {
		PyErr_SetString(PyExc_TypeError, "len() of unsized object");
		return -1;
	}

	return self->prop->len;
}

static PyObject *BPy_IDGroup_Map_GetItem(BPy_IDProperty *self, PyObject *item)
{
	IDProperty *idprop;
	const char *name;

	if (self->prop->type != IDP_GROUP) {
		PyErr_SetString(PyExc_TypeError, "unsubscriptable object");
		return NULL;
	}

	name = PyString_AsString(item);

	if (name == NULL) {
		PyErr_SetString(PyExc_TypeError, "only strings are allowed as keys of ID properties");
		return NULL;
	}

	idprop = IDP_GetPropertyFromGroup(self->prop, name);

	if (idprop == NULL) {
		PyErr_SetString(PyExc_KeyError, "key not in subgroup dict");
		return NULL;
	}

	return BPy_IDGroup_WrapData(self->id, idprop, self->prop);
}

/* returns NULL on success, error string on failure */
static char idp_sequence_type(PyObject *seq_fast)
{
	PyObject **seq_fast_items = PySequence_Fast_ITEMS(seq_fast);
	PyObject *item;
	char type = IDP_INT;

	Py_ssize_t i, len = PySequence_Fast_GET_SIZE(seq_fast);

	for (i = 0; i < len; i++) {
		item = seq_fast_items[i];
		if (PyFloat_Check(item)) {
			if (type == IDP_IDPARRAY) { /* mixed dict/int */
				return -1;
			}
			type = IDP_DOUBLE;
		}
		else if (PyLong_Check(item)) {
			if (type == IDP_IDPARRAY) { /* mixed dict/int */
				return -1;
			}
		}
		else if (PyMapping_Check(item)) {
			if (i != 0 && (type != IDP_IDPARRAY)) { /* mixed dict/int */
				return -1;
			}
			type = IDP_IDPARRAY;
		}
		else {
			return -1;
		}
	}

	return type;
}

/**
 * \note group can be a pointer array or a group.
 * assume we already checked key is a string.
 *
 * \return success.
 */
bool BPy_IDProperty_Map_ValidateAndCreate(PyObject *name_obj, IDProperty *group, PyObject *ob)
{
	IDProperty *prop = NULL;
	IDPropertyTemplate val = {0};

	const char *name;

	if (name_obj) {
		Py_ssize_t name_size;
		PyString_AsStringAndSize(name_obj, &name, &name_size);

		if (name == NULL) {
			PyErr_Format(PyExc_KeyError,
			             "invalid id-property key, expected a string, not a %.200s",
			             Py_TYPE(name_obj)->tp_name);
			return false;
		}

		if (name_size > MAX_IDPROP_NAME) {
			PyErr_SetString(PyExc_KeyError, "the length of IDProperty names is limited to 63 characters");
			return false;
		}
	}
	else {
		name = "";
	}

	if (PyFloat_Check(ob)) {
		val.d = PyFloat_AsDouble(ob);
		prop = IDP_New(IDP_DOUBLE, &val, name);
	}
	else if (PyLong_Check(ob)) {
		val.i = _PyLong_AsInt(ob);
		if (val.i == -1 && PyErr_Occurred()) {
			return false;
		}
		prop = IDP_New(IDP_INT, &val, name);
	}
	else if (PyString_Check(ob)) {
#ifdef USE_STRING_COERCE
		PyObject *value_coerce = NULL;
		val.string.str = PyC_StringAsByte(ob, &value_coerce);
		val.string.subtype = IDP_STRING_SUB_UTF8;
		prop = IDP_New(IDP_STRING, &val, name);
		Py_XDECREF(value_coerce);
#else
		val.str = PyString_AsString(ob);
		prop = IDP_New(IDP_STRING, val, name);
#endif
	}
	else if (PyBytes_Check(ob)) {
		val.string.str = PyBytes_AS_STRING(ob);
		val.string.len = PyBytes_GET_SIZE(ob);
		val.string.subtype = IDP_STRING_SUB_BYTE;

		prop = IDP_New(IDP_STRING, &val, name);
		//prop = IDP_NewString(PyBytes_AS_STRING(ob), name, PyBytes_GET_SIZE(ob));
		//prop->subtype = IDP_STRING_SUB_BYTE;
	}
	else if (PySequence_Check(ob)) {
		PyObject *ob_seq_fast;
		PyObject **ob_seq_fast_items;
		PyObject *item;
		int i;

		if (!(ob_seq_fast = PySequence_Fast(ob, "py -> idprop"))) {
			return false;
		}

		ob_seq_fast_items = PySequence_Fast_ITEMS(ob_seq_fast);

		if ((val.array.type = idp_sequence_type(ob_seq_fast)) == (char)-1) {
			Py_DECREF(ob_seq_fast);
			PyErr_SetString(PyExc_TypeError, "only floats, ints and dicts are allowed in ID property arrays");
			return false;
		}

		/* validate sequence and derive type.
		 * we assume IDP_INT unless we hit a float
		 * number; then we assume it's */

		val.array.len = PySequence_Fast_GET_SIZE(ob_seq_fast);

		switch (val.array.type) {
			case IDP_DOUBLE:
			{
				double *prop_data;

				prop = IDP_New(IDP_ARRAY, &val, name);
				prop_data = IDP_Array(prop);
				for (i = 0; i < val.array.len; i++) {
					item = ob_seq_fast_items[i];
					if (((prop_data[i] = PyFloat_AsDouble(item)) == -1.0) && PyErr_Occurred()) {
						Py_DECREF(ob_seq_fast);
						return false;
					}
				}
				break;
			}
			case IDP_INT:
			{
				int *prop_data;
				prop = IDP_New(IDP_ARRAY, &val, name);
				prop_data = IDP_Array(prop);
				for (i = 0; i < val.array.len; i++) {
					item = ob_seq_fast_items[i];
					if (((prop_data[i] = _PyLong_AsInt(item)) == -1) && PyErr_Occurred()) {
						Py_DECREF(ob_seq_fast);
						return false;
					}
				}
				break;
			}
			case IDP_IDPARRAY:
			{
				prop = IDP_NewIDPArray(name);
				for (i = 0; i < val.array.len; i++) {
					item = ob_seq_fast_items[i];

					if (BPy_IDProperty_Map_ValidateAndCreate(NULL, prop, item) == false) {
						Py_DECREF(ob_seq_fast);
						return false;
					}
				}
				break;
			}
			default:
				/* should never happen */
				Py_DECREF(ob_seq_fast);
				PyErr_SetString(PyExc_RuntimeError, "internal error with idp array.type");
				return false;
		}

		Py_DECREF(ob_seq_fast);
	}
	else if (PyMapping_Check(ob)) {
		PyObject *keys, *vals, *key, *pval;
		int i, len;
		/*yay! we get into recursive stuff now!*/
		keys = PyMapping_Keys(ob);
		vals = PyMapping_Values(ob);

		/* we allocate the group first; if we hit any invalid data,
		 * we can delete it easily enough.*/
		prop = IDP_New(IDP_GROUP, &val, name);
		len = PyMapping_Length(ob);
		for (i = 0; i < len; i++) {
			key = PySequence_GetItem(keys, i);
			pval = PySequence_GetItem(vals, i);
			if (BPy_IDProperty_Map_ValidateAndCreate(key, prop, pval) == false) {
				IDP_FreeProperty(prop);
				MEM_freeN(prop);
				Py_XDECREF(keys);
				Py_XDECREF(vals);
				Py_XDECREF(key);
				Py_XDECREF(pval);
				/* error is already set */
				return false;
			}
			Py_XDECREF(key);
			Py_XDECREF(pval);
		}
		Py_XDECREF(keys);
		Py_XDECREF(vals);
	}
	else {
		PyErr_Format(PyExc_TypeError,
		             "invalid id-property type %.200s not supported",
		             Py_TYPE(ob)->tp_name);
		return false;
	}

	if (group->type == IDP_IDPARRAY) {
		IDP_AppendArray(group, prop);
		// IDP_FreeProperty(item);  /* IDP_AppendArray does a shallow copy (memcpy), only free memory */
		MEM_freeN(prop);
	}
	else {
		IDProperty *prop_exist;

		/* avoid freeing when types match in case they are referenced by the UI, see: T37073
		 * obviously this isn't a complete solution, but helps for common cases. */
		prop_exist = IDP_GetPropertyFromGroup(group, prop->name);
		if ((prop_exist != NULL) &&
		    (prop_exist->type == prop->type) &&
		    (prop_exist->subtype == prop->subtype))
		{
			/* Preserve prev/next links!!! See T42593. */
			prop->prev = prop_exist->prev;
			prop->next = prop_exist->next;

			IDP_FreeProperty(prop_exist);
			*prop_exist = *prop;
			MEM_freeN(prop);
		}
		else {
			IDP_ReplaceInGroup_ex(group, prop, prop_exist);
		}
	}

	return true;
}

int BPy_Wrap_SetMapItem(IDProperty *prop, PyObject *key, PyObject *val)
{
	if (prop->type != IDP_GROUP) {
		PyErr_SetString(PyExc_TypeError, "unsubscriptable object");
		return -1;
	}

	if (val == NULL) { /* del idprop[key] */
		IDProperty *pkey;
		const char *name = PyString_AsString(key);

		if (name == NULL) {
			PyErr_Format(PyExc_KeyError,
			             "expected a string, not %.200s",
			             Py_TYPE(key)->tp_name);
			return -1;
		}

		pkey = IDP_GetPropertyFromGroup(prop, name);
		if (pkey) {
			IDP_FreeFromGroup(prop, pkey);
			return 0;
		}
		else {
			PyErr_SetString(PyExc_KeyError, "property not found in group");
			return -1;
		}
	}
	else {
		bool ok;

		ok = BPy_IDProperty_Map_ValidateAndCreate(key, prop, val);
		if (ok == false) {
			return -1;
		}

		return 0;
	}
}

static int BPy_IDGroup_Map_SetItem(BPy_IDProperty *self, PyObject *key, PyObject *val)
{
	return BPy_Wrap_SetMapItem(self->prop, key, val);
}

static PyObject *BPy_IDGroup_iter(BPy_IDProperty *self)
{
	BPy_IDGroup_Iter *iter = PyObject_New(BPy_IDGroup_Iter, &BPy_IDGroup_Iter_Type);
	iter->group = self;
	iter->mode = IDPROP_ITER_KEYS;
	iter->cur = self->prop->data.group.first;
	Py_XINCREF(iter);
	return (PyObject *)iter;
}

/* for simple, non nested types this is the same as BPy_IDGroup_WrapData */
static PyObject *BPy_IDGroup_MapDataToPy(IDProperty *prop)
{
	switch (prop->type) {
		case IDP_STRING:
			return idprop_py_from_idp_string(prop);
		case IDP_INT:
			return idprop_py_from_idp_int(prop);
		case IDP_FLOAT:
			return idprop_py_from_idp_float(prop);
		case IDP_DOUBLE:
			return idprop_py_from_idp_double(prop);
		case IDP_ARRAY:
		{
			PyObject *seq = PyList_New(prop->len);
			int i;

			if (!seq) {
				PyErr_Format(PyExc_RuntimeError,
				             "%s: IDP_ARRAY: PyList_New(%d) failed",
				             __func__, prop->len);
				return NULL;
			}

			switch (prop->subtype) {
				case IDP_FLOAT:
				{
					const float *array = (float *)IDP_Array(prop);
					for (i = 0; i < prop->len; i++) {
						PyList_SET_ITEM(seq, i, PyFloat_FromDouble(array[i]));
					}
					break;
				}
				case IDP_DOUBLE:
				{
					const double *array = (double *)IDP_Array(prop);
					for (i = 0; i < prop->len; i++) {
						PyList_SET_ITEM(seq, i, PyFloat_FromDouble(array[i]));
					}
					break;
				}
				case IDP_INT:
				{
					const int *array = (int *)IDP_Array(prop);
					for (i = 0; i < prop->len; i++) {
						PyList_SET_ITEM(seq, i, PyLong_FromLong(array[i]));
					}
					break;
				}
				default:
					PyErr_Format(PyExc_RuntimeError,
					             "%s: invalid/corrupt array type '%d'!",
					             __func__, prop->subtype);
					Py_DECREF(seq);
					return NULL;
			}

			return seq;
		}
		case IDP_IDPARRAY:
		{
			PyObject *seq = PyList_New(prop->len), *wrap;
			IDProperty *array = IDP_IDPArray(prop);
			int i;

			if (!seq) {
				PyErr_Format(PyExc_RuntimeError,
				             "%s: IDP_IDPARRAY: PyList_New(%d) failed",
				             __func__, prop->len);
				return NULL;
			}

			for (i = 0; i < prop->len; i++) {
				wrap = BPy_IDGroup_MapDataToPy(array++);

				if (!wrap) /* BPy_IDGroup_MapDataToPy sets the error */
					return NULL;

				PyList_SET_ITEM(seq, i, wrap);
			}
			return seq;
		}
		case IDP_GROUP:
		{
			PyObject *dict = PyDict_New(), *wrap;
			IDProperty *loop;

			for (loop = prop->data.group.first; loop; loop = loop->next) {
				wrap = BPy_IDGroup_MapDataToPy(loop);

				if (!wrap) /* BPy_IDGroup_MapDataToPy sets the error */
					return NULL;

				PyDict_SetItemString(dict, loop->name, wrap);
				Py_DECREF(wrap);
			}
			return dict;
		}
	}

	PyErr_Format(PyExc_RuntimeError,
	             "%s ERROR: '%s' property exists with a bad type code '%d'!",
	             __func__, prop->name, prop->type);
	return NULL;
}

static PyObject *BPy_IDGroup_Pop(BPy_IDProperty *self, PyObject *value)
{
	IDProperty *idprop;
	PyObject *pyform;
	const char *name = PyString_AsString(value);

	if (!name) {
		PyErr_Format(PyExc_TypeError,
		             "pop expected at least a string argument, not %.200s",
		             Py_TYPE(value)->tp_name);
		return NULL;
	}

	idprop = IDP_GetPropertyFromGroup(self->prop, name);

	if (idprop) {
		pyform = BPy_IDGroup_MapDataToPy(idprop);

		if (!pyform) {
			/* ok something bad happened with the pyobject,
			 * so don't remove the prop from the group.  if pyform is
			 * NULL, then it already should have raised an exception.*/
			return NULL;
		}

		IDP_RemoveFromGroup(self->prop, idprop);
		return pyform;
	}

	PyErr_SetString(PyExc_KeyError, "item not in group");
	return NULL;
}

static PyObject *BPy_IDGroup_IterItems(BPy_IDProperty *self)
{
	BPy_IDGroup_Iter *iter = PyObject_New(BPy_IDGroup_Iter, &BPy_IDGroup_Iter_Type);
	iter->group = self;
	iter->mode = IDPROP_ITER_ITEMS;
	iter->cur = self->prop->data.group.first;
	Py_XINCREF(iter);
	return (PyObject *)iter;
}

/* utility function */
static void BPy_IDGroup_CorrectListLen(IDProperty *prop, PyObject *seq, int len, const char *func)
{
	int j;

	printf("%s: ID Property Error found and corrected!\n", func);

	/* fill rest of list with valid references to None */
	for (j = len; j < prop->len; j++) {
		PyList_SET_ITEM(seq, j, Py_INCREF_RET(Py_None));
	}

	/*set correct group length*/
	prop->len = len;
}

PyObject *BPy_Wrap_GetKeys(IDProperty *prop)
{
	PyObject *list = PyList_New(prop->len);
	IDProperty *loop;
	int i;

	for (i = 0, loop = prop->data.group.first; loop && (i < prop->len); loop = loop->next, i++)
		PyList_SET_ITEM(list, i, PyString_FromString(loop->name));

	/* if the id prop is corrupt, count the remaining */
	for ( ; loop; loop = loop->next, i++) {
		/* pass */
	}

	if (i != prop->len) { /* if the loop didnt finish, we know the length is wrong */
		BPy_IDGroup_CorrectListLen(prop, list, i, __func__);
		Py_DECREF(list); /*free the list*/
		/*call self again*/
		return BPy_Wrap_GetKeys(prop);
	}

	return list;
}

PyObject *BPy_Wrap_GetValues(ID *id, IDProperty *prop)
{
	PyObject *list = PyList_New(prop->len);
	IDProperty *loop;
	int i;

	for (i = 0, loop = prop->data.group.first; loop; loop = loop->next, i++) {
		PyList_SET_ITEM(list, i, BPy_IDGroup_WrapData(id, loop, prop));
	}

	if (i != prop->len) {
		BPy_IDGroup_CorrectListLen(prop, list, i, __func__);
		Py_DECREF(list); /*free the list*/
		/*call self again*/
		return BPy_Wrap_GetValues(id, prop);
	}

	return list;
}

PyObject *BPy_Wrap_GetItems(ID *id, IDProperty *prop)
{
	PyObject *seq = PyList_New(prop->len);
	IDProperty *loop;
	int i;

	for (i = 0, loop = prop->data.group.first; loop; loop = loop->next, i++) {
		PyObject *item = PyTuple_New(2);
		PyTuple_SET_ITEMS(item,
		        PyString_FromString(loop->name),
		        BPy_IDGroup_WrapData(id, loop, prop));
		PyList_SET_ITEM(seq, i, item);
	}

	if (i != prop->len) {
		BPy_IDGroup_CorrectListLen(prop, seq, i, __func__);
		Py_DECREF(seq); /*free the list*/
		/*call self again*/
		return BPy_Wrap_GetItems(id, prop);
	}

	return seq;
}


static PyObject *BPy_IDGroup_GetKeys(BPy_IDProperty *self)
{
	return BPy_Wrap_GetKeys(self->prop);
}

static PyObject *BPy_IDGroup_GetValues(BPy_IDProperty *self)
{
	return BPy_Wrap_GetValues(self->id, self->prop);
}

static PyObject *BPy_IDGroup_GetItems(BPy_IDProperty *self)
{
	return BPy_Wrap_GetItems(self->id, self->prop);
}

static int BPy_IDGroup_Contains(BPy_IDProperty *self, PyObject *value)
{
	const char *name = PyString_AsString(value);

	if (!name) {
		PyErr_Format(PyExc_TypeError,
		             "expected a string, not a %.200s",
		             Py_TYPE(value)->tp_name);
		return -1;
	}

	return IDP_GetPropertyFromGroup(self->prop, name) ? 1 : 0;
}

static PyObject *BPy_IDGroup_Update(BPy_IDProperty *self, PyObject *value)
{
	PyObject *pkey, *pval;
	Py_ssize_t i = 0;

	if (BPy_IDGroup_Check(value)) {
		BPy_IDProperty *other = (BPy_IDProperty *)value;
		if (UNLIKELY(self->prop == other->prop)) {
			Py_RETURN_NONE;
		}

		/* XXX, possible one is inside the other */
		IDP_MergeGroup(self->prop, other->prop, true);
	}
	else if (PyDict_Check(value)) {
		while (PyDict_Next(value, &i, &pkey, &pval)) {
			BPy_IDGroup_Map_SetItem(self, pkey, pval);
			if (PyErr_Occurred()) return NULL;
		}
	}
	else {
		PyErr_Format(PyExc_TypeError,
		             "expected a dict or an IDPropertyGroup type, not a %.200s",
		             Py_TYPE(value)->tp_name);
		return NULL;
	}


	Py_RETURN_NONE;
}

static PyObject *BPy_IDGroup_to_dict(BPy_IDProperty *self)
{
	return BPy_IDGroup_MapDataToPy(self->prop);
}

static PyObject *BPy_IDGroup_clear(BPy_IDProperty *self)
{
	IDP_ClearProperty(self->prop);
	Py_RETURN_NONE;
}

/* Matches python dict.get(key, [default]) */
static PyObject *BPy_IDGroup_Get(BPy_IDProperty *self, PyObject *args)
{
	IDProperty *idprop;
	const char *key;
	PyObject *def = Py_None;

	if (!PyArg_ParseTuple(args, "s|O:get", &key, &def))
		return NULL;

	idprop = IDP_GetPropertyFromGroup(self->prop, key);
	if (idprop) {
		PyObject *pyobj = BPy_IDGroup_WrapData(self->id, idprop, self->prop);
		if (pyobj)
			return pyobj;
	}

	Py_INCREF(def);
	return def;
}

static struct PyMethodDef BPy_IDGroup_methods[] = {
	{"pop", (PyCFunction)BPy_IDGroup_Pop, METH_O,
	 "pop an item from the group; raises KeyError if the item doesn't exist"},
	{"iteritems", (PyCFunction)BPy_IDGroup_IterItems, METH_NOARGS,
	 "iterate through the items in the dict; behaves like dictionary method iteritems"},
	{"keys", (PyCFunction)BPy_IDGroup_GetKeys, METH_NOARGS,
	 "get the keys associated with this group as a list of strings"},
	{"values", (PyCFunction)BPy_IDGroup_GetValues, METH_NOARGS,
	 "get the values associated with this group"},
	{"items", (PyCFunction)BPy_IDGroup_GetItems, METH_NOARGS,
	 "get the items associated with this group"},
	{"update", (PyCFunction)BPy_IDGroup_Update, METH_O,
	 "updates the values in the group with the values of another or a dict"},
	{"get", (PyCFunction)BPy_IDGroup_Get, METH_VARARGS,
	 "idprop.get(k[,d]) -> idprop[k] if k in idprop, else d.  d defaults to None"},
	{"to_dict", (PyCFunction)BPy_IDGroup_to_dict, METH_NOARGS,
	 "return a purely python version of the group"},
	{"clear", (PyCFunction)BPy_IDGroup_clear, METH_NOARGS,
	 "clear all members from this group"},
	{NULL, NULL, 0, NULL}
};

static PySequenceMethods BPy_IDGroup_Seq = {
	(lenfunc) BPy_IDGroup_Map_Len,      /* lenfunc sq_length */
	NULL,                               /* binaryfunc sq_concat */
	NULL,                               /* ssizeargfunc sq_repeat */
	NULL,                               /* ssizeargfunc sq_item */ /* TODO - setting this will allow PySequence_Check to return True */
	NULL,                               /* intintargfunc ***was_sq_slice*** */
	NULL,                               /* intobjargproc sq_ass_item */
	NULL,                               /* ssizeobjargproc ***was_sq_ass_slice*** */
	(objobjproc) BPy_IDGroup_Contains,  /* objobjproc sq_contains */
	NULL,                               /* binaryfunc sq_inplace_concat */
	NULL,                               /* ssizeargfunc sq_inplace_repeat */
};

static PyMappingMethods BPy_IDGroup_Mapping = {
	(lenfunc)BPy_IDGroup_Map_Len,           /*inquiry mp_length */
	(binaryfunc)BPy_IDGroup_Map_GetItem,    /*binaryfunc mp_subscript */
	(objobjargproc)BPy_IDGroup_Map_SetItem, /*objobjargproc mp_ass_subscript */
};

PyTypeObject BPy_IDGroup_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	/*  For printing, in format "<module>.<name>" */
	"IDPropertyGroup",       /* char *tp_name; */
	sizeof(BPy_IDProperty),     /* int tp_basicsize; */
	0,                          /* tp_itemsize;  For allocation */

	/* Methods to implement standard operations */

	NULL,                       /* destructor tp_dealloc; */
	NULL,                       /* printfunc tp_print; */
	NULL,                       /* getattrfunc tp_getattr; */
	NULL,                       /* setattrfunc tp_setattr; */
	NULL,                       /* cmpfunc tp_compare; */
	(reprfunc)BPy_IDGroup_repr,     /* reprfunc tp_repr; */

	/* Method suites for standard classes */

	NULL,                       /* PyNumberMethods *tp_as_number; */
	&BPy_IDGroup_Seq,           /* PySequenceMethods *tp_as_sequence; */
	&BPy_IDGroup_Mapping,       /* PyMappingMethods *tp_as_mapping; */

	/* More standard operations (here for binary compatibility) */

	(hashfunc)BPy_IDGroup_hash, /* hashfunc tp_hash; */
	NULL,                       /* ternaryfunc tp_call; */
	NULL,                       /* reprfunc tp_str; */
	NULL,                       /* getattrofunc tp_getattro; */
	NULL,                       /* setattrofunc tp_setattro; */

	/* Functions to access object as input/output buffer */
	NULL,                       /* PyBufferProcs *tp_as_buffer; */

	/*** Flags to define presence of optional/expanded features ***/
	Py_TPFLAGS_DEFAULT,         /* long tp_flags; */

	NULL,                       /*  char *tp_doc;  Documentation string */
	/*** Assigned meaning in release 2.0 ***/
	/* call function for all accessible objects */
	NULL,                       /* traverseproc tp_traverse; */

	/* delete references to contained objects */
	NULL,                       /* inquiry tp_clear; */

	/***  Assigned meaning in release 2.1 ***/
	/*** rich comparisons ***/
	NULL,                       /* richcmpfunc tp_richcompare; */

	/***  weak reference enabler ***/
	0,                          /* long tp_weaklistoffset; */

	/*** Added in release 2.2 ***/
	/*   Iterators */
	(getiterfunc)BPy_IDGroup_iter, /* getiterfunc tp_iter; */
	NULL,                       /* iternextfunc tp_iternext; */
	/*** Attribute descriptor and subclassing stuff ***/
	BPy_IDGroup_methods,        /* struct PyMethodDef *tp_methods; */
	NULL,                       /* struct PyMemberDef *tp_members; */
	BPy_IDGroup_getseters,       /* struct PyGetSetDef *tp_getset; */
};

/********Array Wrapper********/

static PyTypeObject *idp_array_py_type(BPy_IDArray *self, bool *r_is_double)
{
	switch (self->prop->subtype) {
		case IDP_FLOAT:
			*r_is_double = false;
			return &PyFloat_Type;
		case IDP_DOUBLE:
			*r_is_double = true;
			return &PyFloat_Type;
		case IDP_INT:
			*r_is_double = false;
			return &PyLong_Type;
		default:
			*r_is_double = false;
			return NULL;
	}
}

static PyObject *BPy_IDArray_repr(BPy_IDArray *self)
{
	return PyString_FromFormat("<bpy id property array [%d]>", self->prop->len);
}

static PyObject *BPy_IDArray_GetType(BPy_IDArray *self)
{
	switch (self->prop->subtype) {
		case IDP_FLOAT:  return PyString_FromString("f");
		case IDP_DOUBLE: return PyString_FromString("d");
		case IDP_INT:    return PyString_FromString("i");
	}

	PyErr_Format(PyExc_RuntimeError,
	             "%s: invalid/corrupt array type '%d'!",
	             __func__, self->prop->subtype);

	return NULL;
}

static PyGetSetDef BPy_IDArray_getseters[] = {
	/* matches pythons array.typecode */
	{(char *)"typecode", (getter)BPy_IDArray_GetType, (setter)NULL, (char *)"The type of the data in the array, is an int.", NULL},
	{NULL, NULL, NULL, NULL, NULL},
};

static PyObject *BPy_IDArray_to_list(BPy_IDArray *self)
{
	return BPy_IDGroup_MapDataToPy(self->prop);
}

static PyMethodDef BPy_IDArray_methods[] = {
	{"to_list", (PyCFunction)BPy_IDArray_to_list, METH_NOARGS,
	 "return the array as a list"},
	{NULL, NULL, 0, NULL}
};

static int BPy_IDArray_Len(BPy_IDArray *self)
{
	return self->prop->len;
}

static PyObject *BPy_IDArray_GetItem(BPy_IDArray *self, int index)
{
	if (index < 0 || index >= self->prop->len) {
		PyErr_SetString(PyExc_IndexError, "index out of range!");
		return NULL;
	}

	switch (self->prop->subtype) {
		case IDP_FLOAT:
			return PyFloat_FromDouble(((float *)IDP_Array(self->prop))[index]);
		case IDP_DOUBLE:
			return PyFloat_FromDouble(((double *)IDP_Array(self->prop))[index]);
		case IDP_INT:
			return PyLong_FromLong((long)((int *)IDP_Array(self->prop))[index]);
	}

	PyErr_Format(PyExc_RuntimeError,
	             "%s: invalid/corrupt array type '%d'!",
	             __func__, self->prop->subtype);

	return NULL;
}

static int BPy_IDArray_SetItem(BPy_IDArray *self, int index, PyObject *value)
{
	if (index < 0 || index >= self->prop->len) {
		PyErr_SetString(PyExc_RuntimeError, "index out of range!");
		return -1;
	}

	switch (self->prop->subtype) {
		case IDP_FLOAT:
		{
			const float f = (float)PyFloat_AsDouble(value);
			if (f == -1 && PyErr_Occurred()) {
				return -1;
			}
			((float *)IDP_Array(self->prop))[index] = f;
			break;
		}
		case IDP_DOUBLE:
		{
			const double d = PyFloat_AsDouble(value);
			if (d == -1 && PyErr_Occurred()) {
				return -1;
			}
			((double *)IDP_Array(self->prop))[index] = d;
			break;
		}
		case IDP_INT:
		{
			const int i = _PyLong_AsInt(value);
			if (i == -1 && PyErr_Occurred()) {
				return -1;
			}

			((int *)IDP_Array(self->prop))[index] = i;
			break;
		}
	}
	return 0;
}

static PySequenceMethods BPy_IDArray_Seq = {
	(lenfunc) BPy_IDArray_Len,          /* inquiry sq_length */
	NULL,                               /* binaryfunc sq_concat */
	NULL,                               /* intargfunc sq_repeat */
	(ssizeargfunc)BPy_IDArray_GetItem,  /* intargfunc sq_item */
	NULL,                               /* intintargfunc sq_slice */
	(ssizeobjargproc)BPy_IDArray_SetItem, /* intobjargproc sq_ass_item */
	NULL,                               /* intintobjargproc sq_ass_slice */
	NULL,                               /* objobjproc sq_contains */
	/* Added in release 2.0 */
	NULL,                               /* binaryfunc sq_inplace_concat */
	NULL,                               /* intargfunc sq_inplace_repeat */
};



/* sequence slice (get): idparr[a:b] */
static PyObject *BPy_IDArray_slice(BPy_IDArray *self, int begin, int end)
{
	IDProperty *prop = self->prop;
	PyObject *tuple;
	int count;

	CLAMP(begin, 0, prop->len);
	if (end < 0) end = prop->len + end + 1;
	CLAMP(end, 0, prop->len);
	begin = MIN2(begin, end);

	tuple = PyTuple_New(end - begin);

	switch (prop->subtype) {
		case IDP_FLOAT:
		{
			const float *array = (float *)IDP_Array(prop);
			for (count = begin; count < end; count++) {
				PyTuple_SET_ITEM(tuple, count - begin, PyFloat_FromDouble(array[count]));
			}
			break;
		}
		case IDP_DOUBLE:
		{
			const double *array = (double *)IDP_Array(prop);
			for (count = begin; count < end; count++) {
				PyTuple_SET_ITEM(tuple, count - begin, PyFloat_FromDouble(array[count]));
			}
			break;
		}
		case IDP_INT:
		{
			const int *array = (int *)IDP_Array(prop);
			for (count = begin; count < end; count++) {
				PyTuple_SET_ITEM(tuple, count - begin, PyLong_FromLong(array[count]));
			}
			break;
		}
	}

	return tuple;
}
/* sequence slice (set): idparr[a:b] = value */
static int BPy_IDArray_ass_slice(BPy_IDArray *self, int begin, int end, PyObject *seq)
{
	IDProperty *prop = self->prop;
	bool is_double;
	const PyTypeObject *py_type = idp_array_py_type(self, &is_double);
	const size_t elem_size = is_double ? sizeof(double) : sizeof(float);
	size_t alloc_len;
	size_t size;
	void *vec;

	CLAMP(begin, 0, prop->len);
	CLAMP(end, 0, prop->len);
	begin = MIN2(begin, end);

	size = (end - begin);
	alloc_len = size * elem_size;

	vec = MEM_mallocN(alloc_len, "array assignment"); /* NOTE: we count on int/float being the same size here */
	if (PyC_AsArray(vec, seq, size, py_type, is_double, "slice assignment: ") == -1) {
		MEM_freeN(vec);
		return -1;
	}

	memcpy((void *)(((char *)IDP_Array(prop)) + (begin * elem_size)), vec, alloc_len);

	MEM_freeN(vec);
	return 0;
}

static PyObject *BPy_IDArray_subscript(BPy_IDArray *self, PyObject *item)
{
	if (PyIndex_Check(item)) {
		Py_ssize_t i;
		i = PyNumber_AsSsize_t(item, PyExc_IndexError);
		if (i == -1 && PyErr_Occurred())
			return NULL;
		if (i < 0)
			i += self->prop->len;
		return BPy_IDArray_GetItem(self, i);
	}
	else if (PySlice_Check(item)) {
		Py_ssize_t start, stop, step, slicelength;

		if (PySlice_GetIndicesEx(item, self->prop->len, &start, &stop, &step, &slicelength) < 0)
			return NULL;

		if (slicelength <= 0) {
			return PyTuple_New(0);
		}
		else if (step == 1) {
			return BPy_IDArray_slice(self, start, stop);
		}
		else {
			PyErr_SetString(PyExc_TypeError, "slice steps not supported with vectors");
			return NULL;
		}
	}
	else {
		PyErr_Format(PyExc_TypeError,
		             "vector indices must be integers, not %.200s",
		             __func__, Py_TYPE(item)->tp_name);
		return NULL;
	}
}

static int BPy_IDArray_ass_subscript(BPy_IDArray *self, PyObject *item, PyObject *value)
{
	if (PyIndex_Check(item)) {
		Py_ssize_t i = PyNumber_AsSsize_t(item, PyExc_IndexError);
		if (i == -1 && PyErr_Occurred())
			return -1;
		if (i < 0)
			i += self->prop->len;
		return BPy_IDArray_SetItem(self, i, value);
	}
	else if (PySlice_Check(item)) {
		Py_ssize_t start, stop, step, slicelength;

		if (PySlice_GetIndicesEx(item, self->prop->len, &start, &stop, &step, &slicelength) < 0)
			return -1;

		if (step == 1)
			return BPy_IDArray_ass_slice(self, start, stop, value);
		else {
			PyErr_SetString(PyExc_TypeError, "slice steps not supported with vectors");
			return -1;
		}
	}
	else {
		PyErr_Format(PyExc_TypeError,
		             "vector indices must be integers, not %.200s",
		             Py_TYPE(item)->tp_name);
		return -1;
	}
}

static PyMappingMethods BPy_IDArray_AsMapping = {
	(lenfunc)BPy_IDArray_Len,
	(binaryfunc)BPy_IDArray_subscript,
	(objobjargproc)BPy_IDArray_ass_subscript
};


PyTypeObject BPy_IDArray_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	/*  For printing, in format "<module>.<name>" */
	"IDPropertyArray",           /* char *tp_name; */
	sizeof(BPy_IDArray),       /* int tp_basicsize; */
	0,                          /* tp_itemsize;  For allocation */

	/* Methods to implement standard operations */

	NULL,                       /* destructor tp_dealloc; */
	NULL,                       /* printfunc tp_print; */
	NULL,     /* getattrfunc tp_getattr; */
	NULL,     /* setattrfunc tp_setattr; */
	NULL,                       /* cmpfunc tp_compare; */
	(reprfunc)BPy_IDArray_repr,     /* reprfunc tp_repr; */

	/* Method suites for standard classes */

	NULL,                       /* PyNumberMethods *tp_as_number; */
	&BPy_IDArray_Seq,           /* PySequenceMethods *tp_as_sequence; */
	&BPy_IDArray_AsMapping,     /* PyMappingMethods *tp_as_mapping; */

	/* More standard operations (here for binary compatibility) */

	NULL,                       /* hashfunc tp_hash; */
	NULL,                       /* ternaryfunc tp_call; */
	NULL,                       /* reprfunc tp_str; */
	NULL,                       /* getattrofunc tp_getattro; */
	NULL,                       /* setattrofunc tp_setattro; */

	/* Functions to access object as input/output buffer */
	NULL,                       /* PyBufferProcs *tp_as_buffer; */

	/*** Flags to define presence of optional/expanded features ***/
	Py_TPFLAGS_DEFAULT,         /* long tp_flags; */

	NULL,                       /*  char *tp_doc;  Documentation string */
	/*** Assigned meaning in release 2.0 ***/
	/* call function for all accessible objects */
	NULL,                       /* traverseproc tp_traverse; */

	/* delete references to contained objects */
	NULL,                       /* inquiry tp_clear; */

	/***  Assigned meaning in release 2.1 ***/
	/*** rich comparisons ***/
	NULL,                       /* richcmpfunc tp_richcompare; */

	/***  weak reference enabler ***/
	0,                          /* long tp_weaklistoffset; */

	/*** Added in release 2.2 ***/
	/*   Iterators */
	NULL,                       /* getiterfunc tp_iter; */
	NULL,                       /* iternextfunc tp_iternext; */

	/*** Attribute descriptor and subclassing stuff ***/
	BPy_IDArray_methods,        /* struct PyMethodDef *tp_methods; */
	NULL,                       /* struct PyMemberDef *tp_members; */
	BPy_IDArray_getseters,       /* struct PyGetSetDef *tp_getset; */
	NULL,                       /* struct _typeobject *tp_base; */
	NULL,                       /* PyObject *tp_dict; */
	NULL,                       /* descrgetfunc tp_descr_get; */
	NULL,                       /* descrsetfunc tp_descr_set; */
	0,                          /* long tp_dictoffset; */
	NULL,                       /* initproc tp_init; */
	NULL,                       /* allocfunc tp_alloc; */
	NULL,                       /* newfunc tp_new; */
	/*  Low-level free-memory routine */
	NULL,                       /* freefunc tp_free;  */
	/* For PyObject_IS_GC */
	NULL,                       /* inquiry tp_is_gc;  */
	NULL,                       /* PyObject *tp_bases; */
	/* method resolution order */
	NULL,                       /* PyObject *tp_mro;  */
	NULL,                       /* PyObject *tp_cache; */
	NULL,                       /* PyObject *tp_subclasses; */
	NULL,                       /* PyObject *tp_weaklist; */
	NULL
};

/*********** ID Property Group iterator ********/

static PyObject *IDGroup_Iter_repr(BPy_IDGroup_Iter *self)
{
	return PyString_FromFormat("(ID Property Group Iter \"%s\")", self->group->prop->name);
}

static PyObject *BPy_Group_Iter_Next(BPy_IDGroup_Iter *self)
{

	if (self->cur) {
		PyObject *ret;
		IDProperty *cur;

		cur = self->cur;
		self->cur = self->cur->next;

		if (self->mode == IDPROP_ITER_ITEMS) {
			ret = PyTuple_New(2);
			PyTuple_SET_ITEMS(ret,
			        PyString_FromString(cur->name),
			        BPy_IDGroup_WrapData(self->group->id, cur, self->group->prop));
			return ret;
		}
		else {
			return PyString_FromString(cur->name);
		}
	}
	else {
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}
}

PyTypeObject BPy_IDGroup_Iter_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	/*  For printing, in format "<module>.<name>" */
	"IDPropertyGroupIter",           /* char *tp_name; */
	sizeof(BPy_IDGroup_Iter),       /* int tp_basicsize; */
	0,                          /* tp_itemsize;  For allocation */

	/* Methods to implement standard operations */

	NULL,                       /* destructor tp_dealloc; */
	NULL,                       /* printfunc tp_print; */
	NULL,     /* getattrfunc tp_getattr; */
	NULL,     /* setattrfunc tp_setattr; */
	NULL,                       /* cmpfunc tp_compare; */
	(reprfunc) IDGroup_Iter_repr,     /* reprfunc tp_repr; */

	/* Method suites for standard classes */

	NULL,                       /* PyNumberMethods *tp_as_number; */
	NULL,                       /* PySequenceMethods *tp_as_sequence; */
	NULL,                       /* PyMappingMethods *tp_as_mapping; */

	/* More standard operations (here for binary compatibility) */

	NULL,                       /* hashfunc tp_hash; */
	NULL,                       /* ternaryfunc tp_call; */
	NULL,                       /* reprfunc tp_str; */
	NULL,                       /* getattrofunc tp_getattro; */
	NULL,                       /* setattrofunc tp_setattro; */

	/* Functions to access object as input/output buffer */
	NULL,                       /* PyBufferProcs *tp_as_buffer; */

	/*** Flags to define presence of optional/expanded features ***/
	Py_TPFLAGS_DEFAULT,         /* long tp_flags; */

	NULL,                       /*  char *tp_doc;  Documentation string */
	/*** Assigned meaning in release 2.0 ***/
	/* call function for all accessible objects */
	NULL,                       /* traverseproc tp_traverse; */

	/* delete references to contained objects */
	NULL,                       /* inquiry tp_clear; */

	/***  Assigned meaning in release 2.1 ***/
	/*** rich comparisons ***/
	NULL,                       /* richcmpfunc tp_richcompare; */

	/***  weak reference enabler ***/
	0,                          /* long tp_weaklistoffset; */

	/*** Added in release 2.2 ***/
	/*   Iterators */
	PyObject_SelfIter,                  /* getiterfunc tp_iter; */
	(iternextfunc) BPy_Group_Iter_Next, /* iternextfunc tp_iternext; */
};

void IDProp_Init_Types(void)
{
	PyType_Ready(&BPy_IDGroup_Type);
	PyType_Ready(&BPy_IDGroup_Iter_Type);
	PyType_Ready(&BPy_IDArray_Type);
}

/*----------------------------MODULE INIT-------------------------*/

/* --- */

static struct PyModuleDef IDProp_types_module_def = {
	PyModuleDef_HEAD_INIT,
	"idprop.types",  /* m_name */
	NULL,  /* m_doc */
	0,  /* m_size */
	NULL,  /* m_methods */
	NULL,  /* m_reload */
	NULL,  /* m_traverse */
	NULL,  /* m_clear */
	NULL,  /* m_free */
};

static PyObject *BPyInit_idprop_types(void)
{
	PyObject *submodule;

	submodule = PyModule_Create(&IDProp_types_module_def);

	IDProp_Init_Types();

#define MODULE_TYPE_ADD(s, t) \
	PyModule_AddObject(s, t.tp_name, (PyObject *)&t); Py_INCREF((PyObject *)&t)

	/* bmesh_py_types.c */
	MODULE_TYPE_ADD(submodule, BPy_IDGroup_Type);
	MODULE_TYPE_ADD(submodule, BPy_IDGroup_Iter_Type);
	MODULE_TYPE_ADD(submodule, BPy_IDArray_Type);

#undef MODULE_TYPE_ADD

	return submodule;
}

/* --- */

static PyMethodDef IDProp_methods[] = {
	{NULL, NULL, 0, NULL}
};


PyDoc_STRVAR(IDProp_module_doc,
"This module provides access id property types (currently mainly for docs)."
);
static struct PyModuleDef IDProp_module_def = {
	PyModuleDef_HEAD_INIT,
	"idprop",  /* m_name */
	IDProp_module_doc,  /* m_doc */
	0,  /* m_size */
	IDProp_methods,  /* m_methods */
	NULL,  /* m_reload */
	NULL,  /* m_traverse */
	NULL,  /* m_clear */
	NULL,  /* m_free */
};

PyObject *BPyInit_idprop(void)
{
	PyObject *mod;
	PyObject *submodule;
	PyObject *sys_modules = PyThreadState_GET()->interp->modules;

	mod = PyModule_Create(&IDProp_module_def);

	/* idprop.types */
	PyModule_AddObject(mod, "types", (submodule = BPyInit_idprop_types()));
	PyDict_SetItemString(sys_modules, PyModule_GetName(submodule), submodule);
	Py_INCREF(submodule);

	return mod;
}


#ifndef NDEBUG
/* -------------------------------------------------------------------- */
/* debug only function */

void IDP_spit(IDProperty *prop)
{
	if (prop) {
		PyGILState_STATE gilstate;
		bool use_gil = true; /* !PyC_IsInterpreterActive(); */
		PyObject *ret_dict;
		PyObject *ret_str;

		if (use_gil) {
			gilstate = PyGILState_Ensure();
		}

		/* to_dict() */
		ret_dict = BPy_IDGroup_MapDataToPy(prop);
		ret_str = PyObject_Repr(ret_dict);
		Py_DECREF(ret_dict);

		printf("IDProperty(%p): %s\n", prop, PyString_AsString(ret_str));

		Py_DECREF(ret_str);

		if (use_gil) {
			PyGILState_Release(gilstate);
		}
	}
	else {
		printf("IDProperty: <NIL>\n");
	}
}

#endif
