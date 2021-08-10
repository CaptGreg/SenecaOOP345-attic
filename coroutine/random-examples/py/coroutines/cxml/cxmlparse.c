#include "Python.h"
#include "expat.h"

#define TEXT_BUF_SIZE  65536

static PyObject *send_meth;
static char     *textbuf[TEXT_BUF_SIZE];
static int       textindex = 0;

static PyObject *START_EVENT;
static PyObject *END_EVENT;
static PyObject *TEXT_EVENT;

void send_text() {
  if (textindex) {
    PyObject *result = 0;
    PyObject *text;
    PyObject *args;
    PyObject *event;

    text = PyString_FromStringAndSize(textbuf,textindex);
    event = PyTuple_Pack(2, TEXT_EVENT,text);
    args = PyTuple_Pack(1, event);
    result = PyEval_CallObject(send_meth,args);
    Py_DECREF(args);
    Py_DECREF(event);
    Py_DECREF(text);
    Py_XDECREF(result);
  }
  textindex = 0;
}
    
void start_hndl(void *data, const char *el, const char **attrs) {
  PyObject *event,*evtvalue;
  PyObject *result =0;
  PyObject *args;
  PyObject *name;
  PyObject *attr;

  send_text();

  name      = PyString_FromString(el);
  attr      = PyDict_New();
  evtvalue  = PyTuple_Pack(2,name,attr);
  event     = PyTuple_Pack(2,START_EVENT,evtvalue);
  args      = PyTuple_Pack(1,event);

  result = PyEval_CallObject(send_meth,args);

  Py_DECREF(args);
  Py_DECREF(event);
  Py_DECREF(evtvalue);
  Py_DECREF(attr);
  Py_DECREF(name);
  Py_XDECREF(result);
}

void end_hndl(void *data, const char *el) {
  PyObject *event;
  PyObject *result = 0;
  PyObject *name;
  PyObject *args;

  send_text();
  name  = PyString_FromString(el);
  event = PyTuple_Pack(2,END_EVENT,name);
  args  = PyTuple_Pack(1,event);

  result = PyEval_CallObject(send_meth,args);

  Py_DECREF(args);
  Py_DECREF(event);
  Py_DECREF(name);
  Py_XDECREF(result);
}

void char_hndl(void *data, const char *text, int txtlen) {
  int remaining;
  remaining = TEXT_BUF_SIZE - textindex;
  if (remaining > txtlen) {
    memmove(textbuf+textindex,text,txtlen);
    textindex += txtlen;
    return;
  }
  memmove(textbuf+textindex,text,remaining);
  textindex += remaining;
  send_text();
  memmove(textbuf,text+remaining,txtlen-remaining);
  textindex = txtlen-remaining;
}

#define BUFSIZE 65536

PyObject *parse(PyObject *self, PyObject *args) {
  char     *filename;
  PyObject *target;

  XML_Parser p;
  if (!PyArg_ParseTuple(args,"sO",&filename,&target)) {
    return NULL;
  }
  send_meth = PyObject_GetAttrString(target,"send");
  p = XML_ParserCreate(NULL);
  XML_SetElementHandler(p,start_hndl,end_hndl);
  XML_SetCharacterDataHandler(p, char_hndl);
  
  /* Run the parser */
  {
    FILE *f = fopen(filename,"r");
    for (;;) {
      int bytes_read;
      void *buff = XML_GetBuffer(p,BUFSIZE);      
      if (buff == NULL) {
	break;
      }
      bytes_read = fread(buff,1,BUFSIZE,f);
      XML_ParseBuffer(p,bytes_read,bytes_read == 0);
      if (bytes_read == 0) {
	break;
      }
    }
    fclose(f);
  }
  Py_XDECREF(send_meth);
  return Py_BuildValue("i",0);
}

static PyMethodDef cxmlparsemethods[] = {
  {"parse",parse, METH_VARARGS},
  {NULL,NULL}
};

void initcxmlparse() {
  Py_InitModule("cxmlparse",cxmlparsemethods);
  START_EVENT = PyString_FromString("start");
  END_EVENT = PyString_FromString("end");
  TEXT_EVENT = PyString_FromString("text");
}

