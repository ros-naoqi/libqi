#! /usr/bin/env python


from xml.etree import ElementTree as etree
import sys
import argparse
import tempfile
import os
import subprocess
import shutil
import re

TYPE_MAP = {
  'unsigned int': 'uint',
  'unsigned long': 'uint64',
  'unsigned short': 'ushort',
  'unsigned char': 'uchar',
  'long': 'int64',
  'char*': 'string',
  'GenericValue': 'dynamic',
}

def cxx_type_parse(t):
  pre = ''
  mid = ''
  post = ''
  substart = t.find('<')
  if substart != -1:
    #find matching
    count=1
    p = substart + 1
    while p < len(t) and count:
      if t[p] == '>':
        count = count - 1
      if t[p] == '<':
        count = count + 1
      p = p+1
    if count:
      throw ("Parse error in " + t)
    elem = t[substart+1:p-1]
    subres = cxx_type_parse(elem)
    after = t[p+1:]
    return (t[0:substart], subres, cxx_type_parse(after))
  return t

def cxx_parsed_to_sig(p):
  if (type(p) == tuple):
    if re.search('vector$', p[0]):
      return p[0][0:-6] + "[" + cxx_parsed_to_sig(p[1]) + "]" + cxx_parsed_to_sig(p[2])
    elif re.search('map$', p[0]):
      return p[0][0:-3] + "{" + cxx_parsed_to_sig(p[1]) + "}" + cxx_parsed_to_sig(p[2])
    else: # unknown template
      return p[0] + "<" + p[1] + ">" + p[2]
  else:
    return p


def cxx_type_to_signature(t):
  # Drop const and ref.
  # Drop namespace std (assume any class named vector is...a vector)
  t = t.replace('const ', '').replace("&", '').replace('std::', '')
  # Drop all spaces that do not separate identifiers
  t = re.sub(r"\s([^a-zA-Z])", r"\1", t)
  t = re.sub(r"([^a-zA-Z])\s", r"\1", t)
  t = t.strip()
  #Known type conversion
  for e in TYPE_MAP:
    t = re.sub(e, TYPE_MAP[e], t)
  #Container handling
  #For correct result in presence of containers of containers,
  #we need to parse the type almost fully
  #Huge hack, we do not realy parse 'a,b' in template
  parsed = cxx_type_parse(t)
  sig = cxx_parsed_to_sig(parsed)
  print(parsed)
  return sig

def run_doxygen(files):
  tmp_dir = tempfile.mkdtemp()
  # Create Doxyfile in there
  doxyfile_path = os.path.join(tmp_dir, "Doxyfile")
  doxy = open(doxyfile_path, "w")
  doxy.write("GENERATE_XML=YES\n"
    + "GENERATE_HTML=NO\n"
    + "GENERATE_LATEX=NO\n"
    + "QUIET=YES\n"
    + "WARN_IF_UNDOCUMENTED   = NO\n"
    + "INPUT= " + " ".join(files) + "\n"
    + "OUTPUT_DIRECTORY= " + tmp_dir + "\n")
  doxy.close()
  # Invoke doxygen
  subprocess.call(["doxygen", doxyfile_path])
  return tmp_dir

def doxyxml_to_raw(doxy_dir):
  # Parse the index to get all class names (and their functions)
  index_tree = etree.parse(os.path.join(doxy_dir, "xml", "index.xml"))
  class_index = dict()
  result = dict()
  for cls in index_tree.findall(".//compound[@kind='class']"):
    class_index[cls.find("name").text] = (cls.get('refid'), [f.find("name").text for f in cls.findall("member[@kind='function']")])
  for cls in class_index:
    class_id = class_index[cls][0]
    ctree = etree.parse(os.path.join(doxy_dir, "xml", class_id + ".xml"))
    class_root = ctree.find(".//compounddef[@id='" + class_id + "']")
    methods = dict()
    # Parse methods
    for m in class_root.findall("sectiondef[@kind='public-func']/memberdef[@kind='function']"):
      method_name = m.find("name").text
      rettype_raw = m.find("type").text
      rettype = cxx_type_to_signature(rettype_raw)
      arg_nodes = m.findall("param")
      argstype_raw = []
      if arg_nodes is not None:
        argstype_raw = [a.find('type').text for a in arg_nodes]
      argstype = map(cxx_type_to_signature, argstype_raw)
      methods[method_name] = (rettype, argstype)
    result[cls] = (methods,) #force tuple will add signals, doc
  return result

def raw_to_idl(dstruct):
  root = etree.Element('IDL')
  print(dstruct)
  for cls in dstruct:
    e = etree.SubElement(root, 'class', name=cls)
    print("--\n")
    print(dstruct[cls])
    for method_name in dstruct[cls][0]:
      m = etree.SubElement(e, 'method', name=method_name)
      method_raw = dstruct[cls][0][method_name]
      etree.SubElement(m, 'return', type= method_raw[0])
      for a in method_raw[1]:
        etree.SubElement(m, 'argument', type=a)
  return root

def raw_to_text(dstruct):
  result = ""
  for cls in dstruct:
    result += "class " + cls +"\n  methods\n"
    for method_name in dstruct[cls][0]:
      method_raw = dstruct[cls][0][method_name]
      result += "    " + method_raw[0] + " " + method_name +"(" + ",".join(method_raw[1]) + ")\n"
  return result

def main(args):
  parser = argparse.ArgumentParser()
  parser.add_argument("--output-file","-o", help="output file (stdout)")
  parser.add_argument("--output-mode","-m", default="idl", choices=["txt", "idl"], help="output mode (stdout)")
  parser.add_argument("input", nargs='+', help="input file(s)")
  pargs = parser.parse_args(args)
  doxy_dir = run_doxygen(pargs.input)
  raw = doxyxml_to_raw(doxy_dir)
  shutil.rmtree(doxy_dir)
  out = sys.stdout
  if pargs.output_file and pargs.output_file != "-" :
    out = open(pargs.output_file, "w")
  if pargs.output_mode == "txt":
    res = raw_to_text(raw)
  elif pargs.output_mode == "idl":
    res = etree.tostring(raw_to_idl(raw))
  out.write(res)

main(sys.argv)
