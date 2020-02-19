"""DmCodegenPlugin output plugin
"""

import pprint

import optparse
import sys
import re
import random
import string
from decimal import *

from pyang import plugin
from pyang import statements
from pyang.statements import search_child

dm_name = "agt"

pack_struct = ''

class SchemaHelper:


    @staticmethod
    def get_min_max_for_type_stmt(type_stmt, base_datatype):
        # returns (min, max) in number format for well known types
        def get_min_max_for_type(datatype):
            if (datatype == 'boolean'):
                return (0, 1)
            #elif (datatype == 'fault-severity-type'):
            #    return (-1, -1)
            elif (datatype == 'int8'):
                return (-128, 127)
            elif (datatype == 'uint8'):
                return (0, 255)
            elif (datatype == 'int16'):
                return (-32768, 32767)
            elif (datatype == 'uint16'):
                return (0, 65535)
            elif (datatype == 'int32'):
                return (-2147483648, 2147483647)
            elif (datatype == 'uint32'):
                return (0, 4294967295)
            elif (datatype == 'int64'):
                return (-9223372036854775808, 9223372036854775807)
            elif (datatype == 'uint64'):
                return (0, 18446744073709551615)
            #+----------------+-----------------------+----------------------+
            #| fraction-digit | min                   | max                  |
            #+----------------+-----------------------+----------------------+
            #| 1              | -922337203685477580.8 | 922337203685477580.7 |
            #| 2              | -92233720368547758.08 | 92233720368547758.07 |
            #| 3              | -9223372036854775.808 | 9223372036854775.807 |
            #| 18             | -9.223372036854775808 | 9.223372036854775807 |
            #+----------------+-----------------------+----------------------+
            elif (datatype == 'float-1' or datatype == 'float-e1'):
                return (Decimal('-922337203685477580.8'), Decimal('922337203685477580.7'))
            elif (datatype == 'float-2'):
                return (Decimal('-92233720368547758.08'), Decimal('92233720368547758.07'))
            elif (datatype == 'float-3'):
                return (Decimal('-9223372036854775.808'), Decimal('9223372036854775.807'))
            elif (datatype == 'float-e18'):
                return (Decimal('-9.223372036854775808'), Decimal('9.223372036854775807'))
            #elif (datatype == 'float-1' or datatype == 'float-2' or datatype == 'float-3'):
            #    return ('C_FLOAT_MIN', 'C_FLOAT_MAX')
            #elif (datatype == 'float-e1' or datatype == 'float-e18'):
            #    return ('C_FLOAT_E_MIN', 'C_FLOAT_E_MAX')
            #elif (datatype == u'enumeration'):
            #    return (self.min_range, self.max_range)    # fixme based on usage
            #elif (datatype == 'string-16'):
            #    return (0, 16)
            #elif (datatype == 'string-32'):
            #    return (0, 32)
            #elif (datatype == 'string-64'):
            #    return (0, 64)
            #elif (datatype == 'string-128'):
            #    return (0, 128)
            #elif (datatype == 'string-256'):
            #    return (0, 256)
            else:
                print >>sys.stderr, 'SCHEMA ERROR: Unknown datatype "%s"' % (datatype)
                return ('FIXME', 'FIXME')         # FIXME
        (type_min, type_max) = get_min_max_for_type(base_datatype)
        low = ''
        high = ''
        if (hasattr(type_stmt, 'i_ranges')):
            if (len(type_stmt.i_ranges) == 1):
                min_range = type_stmt.i_ranges[0][0]
                max_range = type_stmt.i_ranges[0][1]
                if (min_range == 'min'):
                    low = type_min
                else:
                    low = min_range
                if (max_range == 'max'):
                    high = type_max
                else:
                    high = max_range
            elif (len(type_stmt.i_ranges) > 1):
                print >>sys.stderr, 'SCHEMA ERROR: multiple "', type_stmt.search_one('range'), '" is not supported'
        return (low, high)

    # Input: "1 .. 2" or "1"   Returns: (min, max)
    @staticmethod
    def split_range(range):
        range = range.replace(' .. ', '..')
        range = range.replace(' ', '')
        range_split = range.split('..')
        if (len(range_split) == 1):
            return (range_split[0], range_split[0])
        elif (len(range_split) == 2):
            return range_split
        else:
            print >>sys.stderr, 'SCHEMA ERROR: range %s is not supported' % range
            return ('', '')

    @staticmethod
    def get_c_label(label):
        # remove -, . (present in enum label)
        return label.replace('-', '_').replace('.', '_')

    @staticmethod
    def get_c_xpath(xpath, remove_list_id=False):
        xpath = xpath.lstrip('/').replace('-', '_').replace('/', '__').replace('.', '_')
        if (remove_list_id):
            # remove [id] or [number]
            return re.sub('\[(id|\d+)\]', '', xpath)
        return re.sub('\[(id|\d+)\]', '___\\1___', xpath)

    @staticmethod
    def get_c_var_for_xpath(xpath, remove_list_id=False):
        c_var = xpath.lstrip('/').replace('-', '_').replace('.', '_').replace('/', '.')
        if (remove_list_id):
            # remove [id] or [number]
            c_var = re.sub('\[(id|\d+)\]', '', c_var)
        return c_var

    @staticmethod
    def get_c_var_for_enum_label(label):
        # NOTE: one way converstion from YANG label to C variable;
        # NOTE: there could be collisions.
        # in some cases enum label is like XPATH
        enum_label_var = SchemaHelper.get_c_xpath(label).upper()
        # in some cases enum label has "." in it
        enum_label_var = SchemaHelper.get_c_label(enum_label_var)
        # in some cases enum label has " " in it
        # in some cases enum label has "+" in it;
        # in some cases enum label has "(1.25G)" in it;
        enum_label_var = re.sub('[\(\)+ ]', '_', enum_label_var)
        return enum_label_var

class CHelper():
    MIN_VAL_DEFINED_BIT_MASK = 0b00000001
    MAX_VAL_DEFINED_BIT_MASK = 0b00000010
    RANGE_VALUES_DEFINED_BIT_MASK    = 0b00000100
    MIN_REF_DEFINED_BIT_MASK = 0b00001000
    MAX_REF_DEFINED_BIT_MASK = 0b00010000
    #DEF_VAL_DEFINED_BIT_MASK = 0b00001000

    cdatatype_define = {
                '' : ( 'SCHEMA_DATATYPE_NONE', 0),
                'boolean' : ( 'SCHEMA_DATATYPE_BOOL', 1),
                'enumeration' : ( 'SCHEMA_DATATYPE_ENUM', 2),
                'int8' : ( 'SCHEMA_DATATYPE_INT8', 3),
                'uint8' : ( 'SCHEMA_DATATYPE_UINT8', 4),
                'int16' : ( 'SCHEMA_DATATYPE_INT16', 5),
                'uint16' : ( 'SCHEMA_DATATYPE_UINT16', 6),
                'int32' : ( 'SCHEMA_DATATYPE_INT32', 7),
                'uint32' : ( 'SCHEMA_DATATYPE_UINT32', 8),
                'int64' : ( 'SCHEMA_DATATYPE_INT64', 9),
                'uint64' : ( 'SCHEMA_DATATYPE_UINT64', 10),
                'float-1' : ( 'SCHEMA_DATATYPE_FLOAT_1', 11),
                'float-2' : ( 'SCHEMA_DATATYPE_FLOAT_2', 12),
                'float-3' : ( 'SCHEMA_DATATYPE_FLOAT_3', 13),
                'float-e1' : ( 'SCHEMA_DATATYPE_FLOAT_E1', 14),
                'float-e18' : ( 'SCHEMA_DATATYPE_FLOAT_E18', 15),
                'string-16' : ( 'SCHEMA_DATATYPE_STRING_16', 16),
                'string-32' : ( 'SCHEMA_DATATYPE_STRING_32', 17),
                'string-64' : ( 'SCHEMA_DATATYPE_STRING_64', 18),
                'string-128' : ( 'SCHEMA_DATATYPE_STRING_128', 19),
                'string-256' : ( 'SCHEMA_DATATYPE_STRING_256', 20)
    }

    c_node_type_define = {
            'root': ('SCHEMA_NODE_TYPE_ROOT', 0),
            'config': ('SCHEMA_NODE_TYPE_CONFIG', 1),
            'inventory': ('SCHEMA_NODE_TYPE_INVENTORY', 2),
            'monitor': ('SCHEMA_NODE_TYPE_MONITOR', 3),
            'fault': ('SCHEMA_NODE_TYPE_FAULT', 4),
            'gauge': ('SCHEMA_NODE_TYPE_GAUGE', 5),
            'counter': ('SCHEMA_NODE_TYPE_COUNTER', 6),
            'attribute': ('SCHEMA_NODE_TYPE_ATTRIBUTE', 7),
            'obj_cont': ('SCHEMA_NODE_TYPE_OBJECT', 8),
            #hack 'obj_list': ('SCHEMA_NODE_TYPE_OBJECT_LIST', 9),
    }

    @staticmethod
    def emit_constants():
        #print '#define C_INT16_MIN -32768'
        #print '#define C_INT16_MAX 32767'
        #print '#define C_UINT16_MAX 65535'
        #print '#define C_INT32_MIN -2147483648'
        #print '#define C_INT32_MAX 2147483647'
        #print '#define C_UINT32_MAX 4294967295'
        #print '#define C_INT64_MIN -9223372036854775808L'
        #print '#define C_INT64_MAX 9223372036854775807L'
        #print '#define C_UINT64_MAX 18446744073709551615UL'
        #print

        #for i, k in enumerate(CHelper.cdatatype_define):
        #    print '#define %s %d' % (CHelper.cdatatype_define[k], i)
        #print
        for v in sorted(CHelper.cdatatype_define.values(), key=lambda x: x[1]):
            print '#define %s %d' % (v[0], v[1])

        print
        for v in sorted(CHelper.c_node_type_define.values(), key=lambda x: x[1]):
            print '#define %s %d' % (v[0], v[1])
        print

        print '#define SCHEMA_NODE_RANGETYPE_NONE 0'
        print '#define SCHEMA_NODE_RANGETYPE_FIXED 1'
        print '#define SCHEMA_NODE_RANGETYPE_REFS 2'
        print '#define SCHEMA_NODE_RANGETYPE_ENUMS 3'

        #print '#define SCHEMA_NODE_DEFTYPE_NONE 0'
        #print '#define SCHEMA_NODE_DEFTYPE_FIXED 1'
        #print '#define SCHEMA_NODE_DEFTYPE_REF 2'

        #print '/* Fault Severity */'
        #print '#define FAULT_SEV_CRITICAL 1'
        #print '#define FAULT_SEV_MAJOR 2'
        #print '#define FAULT_SEV_MINOR 3'
        #print '#define FAULT_SEV_WARNING 4'
        #print '#define FAULT_SEV_INFO 5'
        #print

    @staticmethod
    def get_fault_severity_c_label(yang_label):
        if (yang_label == 'critical'):
            return 'FAULT_SEV_CRITICAL'
        elif (yang_label == 'major'):
            return 'FAULT_SEV_MAJOR'
        elif (yang_label == 'minor'):
            return 'FAULT_SEV_MINOR'
        elif (yang_label == 'warning'):
            return 'FAULT_SEV_WARNING'
        elif (yang_label == 'info'):
            return 'FAULT_SEV_INFO'
        print >>sys.stderr, 'ERROR: Unknown fault severity', yang_label
        return 'FAULT_SEV_INFO'
        


xl_sep = '\t'
class SchemaNode:
    def __init__(self, label):
        self.label = label
        self.c_label = SchemaHelper.get_c_label(label)
        self.yang_type = ''
        self.xpath = ''
        self.oid = ''
        self.node_type = ''
        self.obj_name = ''
        self.config = 0        #False
        self.datatype = ''          # YANG base datatype
        self.typedef_name = None    # if type is derived type (typedef)
        self.enum = {}
        self.def_val = None           # stores severity in the case of fault
        self.min_range = ''
        self.max_range = ''
        self.min_ref = ''
        self.max_ref = ''
        self.def_ref = ''
        self.description = ''
        self.must_stmt = ''
        self.when_stmt = ''
        self.cond_var = 0
        self.list_key = ''
        self.list_max_elements = ''
        self.children = []

    def __repr__(self):
        print 'Label: %s, oid: %s, xpath: %s' % (self.label, self.oid, self.xpath)

    def __str__(self):
        return 'Label: %s, oid: %s, xpath: %s' % (self.label, self.oid, self.xpath)

    @staticmethod
    def get_parent_xpath(xpath):
        return re.sub('/[^/]*$', '', xpath)

    def get_c_xpath(self, remove_list_id=False):
        return SchemaHelper.get_c_xpath(self.xpath, remove_list_id)

    def get_c_var_for_xpath(self, remove_list_id=False):
        return SchemaHelper.get_c_var_for_xpath(self.xpath)

    def is_param(self):
        return self.node_type in ['config', 'inventory', 'monitor', 'fault', 'gauge', 'counter', 'attribute']

    def is_singleton(self):
        return self.node_type in ['obj_cont', 'config', 'inventory', 'monitor', 'fault', 'gauge', 'counter', 'attribute']

    def is_obj_list(self):
        return self.node_type == 'obj_list'

    @staticmethod
    def is_well_known_type(type):
        return (type in [ 'boolean',
                            #'fault-severity-type',
                            'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64',
                            'float-1', 'float-2', 'float-3', 'float-e1', 'float-e18',
                            'enumeration',
                            'string-16', 'string-32', 'string-64', 'string-128', 'string-256' ])

    # returns (ctype, and size of array)
    def get_cdatatype(self):
        if (self.datatype == 'boolean'):
            return ('uint8_t', 0)
        #elif (self.datatype == 'fault-severity-type'):
        #    return ('uint8_t', 0)
        elif (self.datatype == 'int8'):
            return ('int8_t', 0)
        elif (self.datatype == 'uint8'):
            return ('uint8_t', 0)
        elif (self.datatype == 'int16'):
            return ('int16_t', 0)
        elif (self.datatype == 'uint16'):
            return ('uint16_t', 0)
        elif (self.datatype == 'int32'):
            return ('int32_t', 0)
        elif (self.datatype == 'uint32'):
            return ('uint32_t', 0)
        elif (self.datatype == 'int64'):
            return ('int64_t', 0)
        elif (self.datatype == 'uint64'):
            return ('uint64_t', 0)
        elif (self.datatype == 'float-1' or self.datatype == 'float-2' or self.datatype == 'float-3'):
            return ('float', 0)
        elif (self.datatype == 'float-e1' or self.datatype == 'float-e18'):
            return ('double', 0)
        elif (self.datatype == u'enumeration'):
            return ('int32_t', 0)
        elif (self.datatype == 'string-16'):
            return ('char', 16+1)
        elif (self.datatype == 'string-32'):
            return ('char', 32+1)
        elif (self.datatype == 'string-64'):
            return ('char', 64+1)
        elif (self.datatype == 'string-128'):
            return ('char', 128+1)
        elif (self.datatype == 'string-256'):
            return ('char', 256+1)
        elif (self.datatype == ''):
            return ('bug', 0)
        else:
            print >>sys.stderr, 'SCHEMA ERROR: Unknown datatype "%s"' % (self.datatype)
            return ('BUG', 0)

    def get_cdatatype_define(self):
        if (self.datatype in CHelper.cdatatype_define):
            return CHelper.cdatatype_define[self.datatype][0]
        else:
            print >>sys.stderr, 'SCHEMA ERROR: Unknown datatype "%s"' % (self.datatype)
            return 'FIXME datatype'

    def get_c_node_type_define(self):
        if (self.node_type in CHelper.c_node_type_define):
            return CHelper.c_node_type_define[self.node_type][0]
        elif (self.node_type == 'obj_list'):    # hack obj_list same as obj_cont
            return CHelper.c_node_type_define['obj_cont'][0]
        else:
            print >>sys.stderr, 'SCHEMA ERROR: Unknown node_type "%s"' % (self.node_type)
            return 'FIXME node_type'

    def get_data_size(self):
        if (self.datatype == 'boolean'):
            return 1
        #elif (self.datatype == 'fault-severity-type'):
        #    return 4
        elif (self.datatype == 'uint8' or self.datatype == 'int8'):
            return 1
        elif (self.datatype == 'uint16' or self.datatype == 'int16'):
            return 2
        elif (self.datatype == 'uint32' or self.datatype == 'int32'):
            return 4
        elif (self.datatype == 'uint64' or self.datatype == 'int64'):
            return 8
        elif (self.datatype == 'float-1' or self.datatype == 'float-2' or self.datatype == 'float-3'):
            return 4
        elif (self.datatype == 'float-e1' or self.datatype == 'float-e18'):
            return 4
        elif (self.datatype == u'enumeration'):
            return 4
        elif (self.datatype == 'string-16'):
            return 16
        elif (self.datatype == 'string-32'):
            return 32
        elif (self.datatype == 'string-64'):
            return 64
        elif (self.datatype == 'string-128'):
            return 128
        elif (self.datatype == 'string-256'):
            return 256
        elif (self.datatype == ''):
            return 0
        else:
            print >>sys.stderr, 'SCHEMA ERROR: Unknown datatype "%s"' % (self.datatype)
            return 0

    # returns C DEFINE if value matches, otherwise just returns input as value
    #@staticmethod
    #def get_c_value(value):
    #    if (value is None or value == ''):
    #        return ''
    #    val = str(value)
    #    if (val == '0'):
    #        return val
    #    elif val == '-32768':
    #        return 'C_INT16_MIN'
    #    elif val == '32767':
    #        return 'C_INT16_MAX'
    #    elif val == '65535':
    #        return 'C_UINT16_MAX'
    #    elif val == '-2147483648':
    #        return 'C_INT32_MIN'
    #    elif val == '2147483647':
    #        return 'C_INT32_MAX'
    #    elif val == '4294967295':
    #        return 'C_UINT32_MAX'
    #    elif val == '-9223372036854775808':
    #        return 'C_INT64_MIN'
    #    elif val == '9223372036854775807':
    #        return 'C_INT64_MAX'
    #    elif val == '18446744073709551615':
    #        return 'C_UINT64_MAX'
    #    elif val in [ 'C_FLOAT_MIN', 'C_FLOAT_MAX', 'C_FLOAT_E_MIN', 'C_FLOAT_E_MAX']:
    #        return val
    #    else:
    #        return val

    def emit_label_tree(self, indent=0):
        for i in range(indent):
            print '  ',
        print self.label
        for node in self.children:
            node.emit_label_tree(indent+1)


    def emit_obj_list_cnt(self):
        ''' Emit the count of list objects '''
        if (self.node_type in ['obj_list']):
            print '#define %s_CNT %d' % (self.get_c_xpath(remove_list_id=True).upper(), int(self.list_max_elements))
        if (self.node_type in ['root', 'obj_cont', 'obj_list']):
            for node in self.children:
                node.emit_obj_list_cnt()

    def has_param_child(self, param_type, check_def_val=False, check_def_ref=False,
                            check_fixed_range=False, check_ref_range=False):
        has_param = False
        for child in self.children:
            if (child.node_type in ['root', 'obj_cont', 'obj_list']):
                has_param = child.has_param_child(param_type, check_def_val,
                        check_def_ref, check_fixed_range, check_ref_range)
            elif (child.node_type == param_type):
                if check_def_val:
                    if (child.def_val is not None):
                        has_param = True
                elif check_def_ref:
                    if (child.def_ref):
                        has_param = True
                elif check_fixed_range:
                    if (child.datatype == 'enumeration'):
                        has_param = True
                    else:
                        min_max_range = child.get_min_max_range()
                        # NOTE: enum and range cannot mixed
                        if (min_max_range):
                            if ((min_max_range['valid_bits'] & CHelper.MIN_VAL_DEFINED_BIT_MASK) and
                                (min_max_range['valid_bits'] & CHelper.MAX_VAL_DEFINED_BIT_MASK)):
                                has_param = True
                elif check_ref_range:
                    min_max_range = child.get_min_max_range()
                    # NOTE: enum and range cannot mixed
                    if (min_max_range):
                        if ((min_max_range['valid_bits'] & CHelper.MIN_REF_DEFINED_BIT_MASK) and
                            (min_max_range['valid_bits'] & CHelper.MAX_REF_DEFINED_BIT_MASK)):
                            has_param = True
                else:
                    has_param = True
            if has_param:
                return True
        if not has_param:
            #if check_def_val:
            #    print 'skipping def_val', self.c_label, self.xpath
            #elif check_def_ref:
            #    print 'skipping def_ref', self.c_label, self.xpath
            #elif check_fixed_range:
            #    print 'skipping fixed_range', self.c_label, self.xpath
            #elif check_ref_range:
            #    print 'skipping ref_range', self.c_label, self.xpath
            #else:
            #    print 'skipping param', self.c_label, self.xpath
            pass
        return has_param

    def emit_agt_param_struct(self, param_type, indent_str=''):
        global pack_struct
        if (param_type not in ['config', 'fault', 'inventory', 'monitor', 'gauge', 'counter', 'attribute']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            (c_type, array_size) = self.get_cdatatype()
            if (array_size):
                print '%s%-7s %s[%s];' % (indent_str, c_type, c_label, array_size)
            else:
                print '%s%-7s %s;' % (indent_str, c_type, c_label);
            if (self.node_type not in ['config']):  # Support for _updated flag
                print '%s%-7s _%s_updated;' % (indent_str + one_indent, 'uint8_t', c_label);
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            has_param = True
            if (self.node_type == 'root'):
                print 'typedef struct %s %s_%s_values {' % (pack_struct, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type)
                if has_param:
                    print '%sstruct %s {  /* %s */' % (indent_str, pack_struct, self.xpath)
            if has_param:
                for node in self.children:
                    node.emit_agt_param_struct(param_type, indent_str+one_indent)
            if (self.node_type == 'obj_list'):
                if has_param:
                    print '%s} %s[%s];' % (indent_str, c_label, self.list_max_elements)
            elif (self.node_type == 'obj_cont'):
                if has_param:
                    print '%s} %s;' % (indent_str, c_label)
            elif (self.node_type == 'root'):
                print '} %s_%s_values_t;\n' % (dm_name, param_type)

    def emit_agt_param_fixed_defaults_struct(self, param_type, indent_str=''):
        global pack_struct
        # For fault, default stores the severity of fault.
        # As per new design fault default is not needed.
        if (param_type not in ['config', 'inventory', 'fault', 'attribute']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            if (self.def_val is None):
                if (not self.def_ref):
                    print >>sys.stderr, 'SCHEMA ERROR: no fixed default/def-ref for %s' % (self.xpath)
            else:
                (c_type, array_size) = self.get_cdatatype()
                if (param_type == 'fault'):
                    c_type = 'uint8_t'
                if (array_size):
                    print '%s%-7s %s[%s];' % (indent_str, c_type, c_label, array_size)
                else:
                    print '%s%-7s %s;' % (indent_str, c_type, c_label);
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            has_param = True
            if (self.node_type == 'root'):
                print 'typedef struct %s %s_%s_fixed_defaults {' % (pack_struct, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type, check_def_val=True)
                if has_param:
                    print '%sstruct %s {  /* %s */' % (indent_str, pack_struct, self.xpath)
            if has_param:
                for node in self.children:
                    node.emit_agt_param_fixed_defaults_struct(param_type, indent_str+one_indent)
            if (self.node_type != 'root'):
                if has_param:
                    print '%s} %s;' % (indent_str, c_label)
            else:
                print '} %s_%s_fixed_defaults_t;\n' % (dm_name, param_type)

    def get_def_val(self):
        # FIXME: If only one enum is possible or if only one element in range, then it has implicit default
        pass

    def emit_agt_param_fixed_defaults(self, param_type, indent_str='', list_output=None):
        # For fault, default stores the severity of fault.
        # As per new design fault default is not needed.
        if (param_type not in ['config', 'inventory', 'fault', 'attribute']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        """ Emit param structure with DEFAULT values """
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            if (self.def_val is not None):
                (c_type, array_size) = self.get_cdatatype()
                no_def_comment = ''
                def_val = self.def_val
                if (param_type == 'fault'):
                    def_val = CHelper.get_fault_severity_c_label(def_val)
                elif (self.datatype == 'enumeration'):
                    def_val = str(self.enum[def_val]) + '  /* ' + def_val + ' */'
                if (c_type == 'char'):
                    list_output.append('%s.%s = "%s",%s' % (indent_str, c_label, def_val, no_def_comment)),
                else:
                    list_output.append('%s.%s = %s,%s' % (indent_str, c_label, def_val, no_def_comment)),
            # FIXME: If only one enum is possible or if only one element in range, then it has implicit default
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            if (list_output is None):
                list_output = []
            one_struct_output = []
            has_param = True
            if (self.node_type == 'root'):
                print "const %s_%s_fixed_defaults_t %s_%s_fixed_defaults = {" % (dm_name, param_type, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type, check_def_val=True)
                if has_param:
                    one_struct_output.append('%s.%s = {  /* %s */' % (indent_str, self.c_label, self.xpath))
            if has_param:
                for node in self.children:
                    node.emit_agt_param_fixed_defaults(param_type, indent_str+one_indent, one_struct_output)
                list_output.extend(one_struct_output)
            if (self.node_type != 'root'):
                if has_param:
                    list_output.append('%s}%s  /* %s */' % (indent_str, ',', c_label))
            else:
                print '\n'.join(list_output)
                print '};  /* %s_%s_fixed_defaults */\n' % (dm_name, param_type)

    def emit_agt_param_ref_defaults_struct(self, param_type, indent_str=''):
        global pack_struct
        # For fault, default stores the severity of fault.
        # As per new design fault default is not needed.
        if (param_type not in ['config', 'inventory']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            if (self.def_ref):
                (c_type, array_size) = self.get_cdatatype()
                #if (array_size):
                #    print '%s%-7s* %s[%s];' % (indent_str, c_type, c_label, array_size)
                print '%s%-7s* %s;' % (indent_str, c_type, c_label);
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            has_param = True
            if (self.node_type == 'root'):
                print 'typedef struct %s %s_%s_ref_defaults {' % (pack_struct, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type, check_def_ref=True)
                if has_param:
                    print '%sstruct %s {  /* %s */' % (indent_str, pack_struct, self.xpath)
            if has_param:
                for node in self.children:
                    node.emit_agt_param_ref_defaults_struct(param_type, indent_str+one_indent)
            if (self.node_type == 'obj_list'):
                if has_param:
                    print '%s} %s[%s];' % (indent_str, c_label, self.list_max_elements)
            elif (self.node_type == 'obj_cont'):
                if has_param:
                    print '%s} %s;' % (indent_str, c_label)
            elif (self.node_type == 'root'):
                print '} %s_%s_ref_defaults_t;\n' % (dm_name, param_type)

    def emit_agt_param_ref_defaults(self, param_type, indent_str='', list_output=None):
        if (param_type not in ['config', 'inventory']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        """ Emit param structure with DEFAULT values """
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            if (self.def_ref):
                parent_c_xpath = re.sub('\w*$', '', self.get_c_var_for_xpath(remove_list_id=False))
                def_ref = re.sub('^.*/', '', self.def_ref)
                def_ref = parent_c_xpath + SchemaHelper.get_c_label(def_ref)
                list_output.append('%s&%s_attribute_values.%s,' % (indent_str, dm_name, def_ref))
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            if (list_output is None):
                list_output = []
            one_struct_output = []
            has_param = True
            if (self.node_type != 'root'):
                has_param = self.has_param_child(param_type, check_def_ref=True)
                if has_param:
                    list_output.append('%s.%s = {' % (indent_str, self.c_label))
            if has_param:
                for child in self.children:
                    child.emit_agt_param_ref_defaults(param_type, indent_str+one_indent, one_struct_output)
                if (self.node_type == 'obj_list'):
                    # Add indentation
                    one_struct_output = ['%s%s' % (one_indent, line) for line in one_struct_output]
                    for i in range(int(self.list_max_elements)):
                        # Expand id in XPATH to its index
                        one_struct_output_copy = [re.sub('(.*)\[id(\])', '\\1[%s]'%(str(i)), line) for line in one_struct_output]
                        list_output.append('%s%s[%d] = {' % (indent_str, one_indent, i))
                        list_output.extend(one_struct_output_copy)
                        list_output.append('%s%s},' % (indent_str, one_indent))
                else:
                    list_output.extend(one_struct_output)
            if (self.node_type != 'root'):
                if has_param:
                    list_output.append('%s},' % (indent_str))
            else:
                print "const %s_%s_ref_defaults_t %s_%s_ref_defaults = {" % (dm_name, param_type, dm_name, param_type)
                print '\n'.join(list_output)
                print '};  /* %s_%s_ref_defaults */\n' % (dm_name, param_type)

    def emit_agt_enum_defines(self):
        """
            Emit #define for all enums in the schema
        """
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type in ['config', 'inventory', 'monitor', 'fault', 'gauge', 'counter', 'attribute']):
            if (self.datatype != 'enumeration'):
                return
            c_xpath = self.get_c_xpath(remove_list_id=True)
            print '/* enum constants - %s */' % (c_xpath)
            prefix = c_xpath.upper()
            #if (self.node_type in ['inventory', 'fault', 'gauge', 'counter', 'attribute']):
            #    print >>sys.stderr, 'Enum type for type=%s  xpath=%s' % (self.node_type, c_xpath)
            print '#define %s_ENUM__COUNT\t%s' % (prefix, len(self.enum))
            for (label, number) in sorted(self.enum.iteritems(), key=lambda x: x[1]):
                enum_label = SchemaHelper.get_c_var_for_enum_label(label)
                print '#define %s_ENUM_%s\t%s' % (prefix, enum_label, number)
            print  # print empty line between each enum
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            for node in self.children:
                node.emit_agt_enum_defines()


    def get_min_max_range(self):
        if (self.node_type not in ['config', 'gauge']):
            return None
        valid_bits = 0
        (c_type, array_size) = self.get_cdatatype()
        if (c_type == 'char'):
            return None
        elif (self.datatype == 'enumeration'):
            # TODO: Shall we optimize enum to range?
            #if (len(self.enum) == 0):
            #    print >>sys.stderr, "ERROR: enum values are not there for", self.xpath
            #min_range = str(min(self.enum.values()))
            #max_range = str(max(self.enum.values()))
            #valid_bits |= CHelper.MIN_VAL_DEFINED_BIT_MASK
            #valid_bits |= CHelper.MAX_VAL_DEFINED_BIT_MASK
            #if (int(max_range) - int(min_range) + 1 > len(self.enum)):
            #    valid_bits |= CHelper.RANGE_VALUES_DEFINED_BIT_MASK
            return None
        else:
            # Check if calibration references (min-ref, max-ref) are defined?
            # If not then based on c_type, define the range.
            if (self.min_ref):
                min_range = '0'
                valid_bits |= CHelper.MIN_REF_DEFINED_BIT_MASK
                #print 'min_ref:%s:%s:' % (self.xpath, self.min_ref)
            else:
                #min_range = self.get_c_value(self.min_range)
                min_range = self.min_range
                if (not min_range):
                    min_range = '0'
                else:
                    valid_bits |= CHelper.MIN_VAL_DEFINED_BIT_MASK
            if (self.max_ref):
                max_range = '0'
                valid_bits |= CHelper.MAX_REF_DEFINED_BIT_MASK
                #print 'max_ref:%s:%s:' % (self.xpath, self.max_ref)
            else:
              #max_range = self.get_c_value(self.max_range)
              max_range = self.max_range
              if (not max_range):
                  max_range = '0'
              else:
                  valid_bits |= CHelper.MAX_VAL_DEFINED_BIT_MASK
        return { 'valid_bits' : valid_bits,
                 'fixed_min_range' : min_range,
                 'fixed_max_range' : max_range,
                 }

    def emit_agt_fixed_ranges_struct(self, param_type, indent_str='', list_output=None): # , xpath_inst_list=None):
        global pack_struct
        """  Emit filled range values for parameters """
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            c_type = self.get_cdatatype()[0];
            if (self.datatype == 'enumeration'):
                #TODO: Is it a rule that for enumeration types, range statement is not allowed?
                if (self.node_type != 'config'):
                    #FIXME Question to IPG: Confirm that range_struct and range_value required for Config parameters only
                    print >>sys.stderr, 'SCHEMA ERROR: enumeration type used for non-config parameter', c_xpath
                c_xpath = self.get_c_xpath(remove_list_id=True)
                prefix = c_xpath.upper()
                list_output.append('%s%s %s[1+%s_ENUM__COUNT];' % (indent_str, c_type, self.c_label, prefix))
                return
            min_max_range = self.get_min_max_range()
            # NOTE: enum and range cannot mixed
            if (not min_max_range):  # skip enum and string types
                return
            if ((min_max_range['valid_bits'] & CHelper.MIN_REF_DEFINED_BIT_MASK) and
                (min_max_range['valid_bits'] & CHelper.MAX_REF_DEFINED_BIT_MASK)):
                list_output.append('%s /* %s --- dynamic range */' % (indent_str,
                                self.label))
                return
            elif ((min_max_range['valid_bits'] & CHelper.MIN_VAL_DEFINED_BIT_MASK) and
                  (min_max_range['valid_bits'] & CHelper.MAX_VAL_DEFINED_BIT_MASK)):
                list_output.append('%s%s %s[2];' % (indent_str, c_type, self.c_label))
            else:
                list_output.append('%s /* %s --- no range */' % (indent_str, self.label))
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            if (list_output is None):
                list_output = []
            #if (xpath_inst_list is None):
            #    xpath_inst_list = []
            one_struct_output = []
            has_param = True
            if (self.node_type == 'root'):
                print "typedef struct %s %s_%s_fixed_ranges {" % (pack_struct, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type, check_fixed_range=True)
                if has_param:
                    one_struct_output.append('%sstruct %s {  /* %s */' % (indent_str, pack_struct, self.xpath))
            if has_param:
                for node in self.children:
                    node.emit_agt_fixed_ranges_struct(param_type, indent_str+one_indent, one_struct_output)
                list_output.extend(one_struct_output)
            if (self.node_type != 'root'):
                if has_param:
                    list_output.append('%s} %s;' % (indent_str, c_label))
            else:
                print '\n'.join(list_output)
                print '} %s_%s_fixed_ranges_t;' % (dm_name, param_type)
                print

    def emit_agt_fixed_ranges(self, param_type, indent_str='', list_output=None): # , xpath_inst_list=None):
        """  Emit filled range values for parameters """
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            min_max_range = self.get_min_max_range()
            if (self.datatype == 'enumeration'):
                #TODO: Is it a rule that for enumeration types, range statement is not allowed?
                list_output.append('%s{ ' % (indent_str))
                c_xpath = self.get_c_xpath(remove_list_id=True)
                prefix = c_xpath.upper()
                if (self.node_type != 'config'):
                    #FIXME Question to IPG: Confirm that range_struct and range_value required for Config parameters only
                    print >>sys.stderr, 'SCHEMA ERROR: enumeration type used for non-config parameter', c_xpath
                list_output.append('%s%s%s_ENUM__COUNT, /* first element is size */' % (indent_str, one_indent, prefix))
                for (label, number) in sorted(self.enum.iteritems(), key=lambda x: x[1]):
                    enum_label = SchemaHelper.get_c_var_for_enum_label(label)
                    list_output.append('%s%s%s_ENUM_%s,' % (indent_str, one_indent, prefix, enum_label))
                list_output.append('%s}, /* %s */' % (indent_str, self.label))
                return
            # NOTE: enum and range cannot mixed
            if (not min_max_range):  # skip enum and string types
                return
            if ((min_max_range['valid_bits'] & CHelper.MIN_REF_DEFINED_BIT_MASK) and
                (min_max_range['valid_bits'] & CHelper.MAX_REF_DEFINED_BIT_MASK)):
                list_output.append('%s/* %s --- dynamic range */' % (indent_str,
                                self.label))
                return
            elif ((min_max_range['valid_bits'] & CHelper.MIN_VAL_DEFINED_BIT_MASK) and
                  (min_max_range['valid_bits'] & CHelper.MAX_VAL_DEFINED_BIT_MASK)):
                list_output.append('%s{ %s, %s },  /* %s */' % (indent_str,
                                min_max_range['fixed_min_range'],
                                min_max_range['fixed_max_range'],
                                self.label))
            else:
                list_output.append('%s/* %s --- no range */' % (indent_str,
                                self.label))
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            if (list_output is None):
                list_output = []
            #if (xpath_inst_list is None):
            #    xpath_inst_list = []
            one_struct_output = []
            has_param = True
            if (self.node_type == 'root'):
                print "const %s_%s_fixed_ranges_t %s_%s_fixed_ranges = {" % (dm_name, param_type, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type, check_fixed_range=True)
                if has_param:
                    one_struct_output.append('%s.%s = {  /* %s */' % (indent_str, self.c_label, self.xpath))
            if has_param:
                for node in self.children:
                    node.emit_agt_fixed_ranges(param_type, indent_str+one_indent, one_struct_output)
                list_output.extend(one_struct_output)
            if (self.node_type != 'root'):
                if has_param:
                    list_output.append('%s}%s  /* %s */' % (indent_str, ',', c_label))
            else:
                print '\n'.join(list_output)
                print '};'
                print

    def emit_agt_ref_ranges_struct(self, param_type, indent_str=''):
        global pack_struct
        if (param_type not in ['config', 'gauge']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type == param_type):
            min_max_range = self.get_min_max_range()
            # NOTE: enum and range cannot mixed
            if (not min_max_range):  # skip enum and string types
                return
            if ((min_max_range['valid_bits'] & CHelper.MIN_REF_DEFINED_BIT_MASK) and
                (min_max_range['valid_bits'] & CHelper.MAX_REF_DEFINED_BIT_MASK)):
                (c_type, array_size) = self.get_cdatatype()
                print '%s%-7s* %s[2];' % (indent_str, c_type, c_label);
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            has_param = True
            if (self.node_type == 'root'):
                print 'typedef struct %s %s_%s_ref_ranges {' % (pack_struct, dm_name, param_type)
            else:
                has_param = self.has_param_child(param_type, check_ref_range=True)
                if has_param:
                    print '%sstruct %s {  /* %s */' % (indent_str, pack_struct, self.xpath)
            if has_param:
                for node in self.children:
                    node.emit_agt_ref_ranges_struct(param_type, indent_str+one_indent)
            if (self.node_type == 'obj_list'):
                if has_param:
                    print '%s} %s[%s];' % (indent_str, c_label, self.list_max_elements)
            elif (self.node_type == 'obj_cont'):
                if has_param:
                    print '%s} %s;' % (indent_str, c_label)
            elif (self.node_type == 'root'):
                print '} %s_%s_ref_ranges_t;' % (dm_name, param_type)
                print

    def emit_agt_ref_ranges(self, param_type, indent_str='', list_output=None):
        if (param_type not in ['config', 'gauge']):
            print >>sys.stderr, 'ERROR: unknown argument param type %s' % (param_type)
            return
        one_indent = '  '
        #print data_node.c_label, data_node.c_xpath_flat, data_node.c_xpath_hierachy
        if (self.node_type in ['root', 'obj_cont', 'obj_list']):
            if (list_output is None):
                list_output = []
            one_struct_output = []
            has_param = True
            if (self.node_type != 'root'):
                has_param = self.has_param_child(param_type, check_ref_range=True)
                if has_param:
                    list_output.append('%s.%s = {' % (indent_str, self.c_label))
            if has_param:
                for child in self.children:
                    child.emit_agt_ref_ranges(param_type, indent_str + one_indent, one_struct_output)
                if (self.node_type == 'obj_list'):
                    # Add indentation
                    one_struct_output = ['%s%s' % (one_indent, line) for line in one_struct_output]
                    for i in range(int(self.list_max_elements)):
                        # Expand id in XPATH to its index
                        one_struct_output_copy = [re.sub('(.*)\[id(\])', '\\1[%s]'%(str(i)), line) for line in one_struct_output]
                        list_output.append('%s%s[%d] = {' % (indent_str, one_indent, i))
                        list_output.extend(one_struct_output_copy)
                        list_output.append('%s%s},' % (indent_str, one_indent))
                else:
                    list_output.extend(one_struct_output)
            if (self.node_type != 'root'):
                if has_param:
                    list_output.append('%s},' % (indent_str))
            else:
                print 'const %s_%s_ref_ranges_t %s_%s_ref_ranges = {' % (dm_name, param_type, dm_name, param_type)
                print '\n'.join(list_output)
                print '}; /* %s_%s_ref_ranges */\n' % (dm_name, param_type)
        elif (self.node_type == param_type):
            min_max_range = self.get_min_max_range()
            # NOTE: enum and range cannot mixed
            if (not min_max_range):  # skip enum and string types
                return
            if ((min_max_range['valid_bits'] & CHelper.MIN_REF_DEFINED_BIT_MASK) and
                (min_max_range['valid_bits'] & CHelper.MAX_REF_DEFINED_BIT_MASK)):
                parent_c_xpath = re.sub('\w*$', '', self.get_c_var_for_xpath(remove_list_id=False))
                #min_ref = schema_d[data_node.s_node.min_ref].c_xpath
                #max_ref = schema_d[data_node.s_node.max_ref].c_xpath
                min_ref = re.sub('^.*/', '', self.min_ref)
                min_ref = parent_c_xpath + SchemaHelper.get_c_label(min_ref)
                max_ref = re.sub('^.*/', '', self.max_ref)
                max_ref = parent_c_xpath + SchemaHelper.get_c_label(max_ref)
                list_output.append('%s{ &%s_attribute_values.%s,' % (indent_str, dm_name, min_ref))
                list_output.append('%s  &%s_attribute_values.%s },' % (indent_str, dm_name, max_ref))


    def emit_extern_param(self, indent_str='', list_output=None):
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type in ['config', 'inventory', 'monitor', 'fault', 'gauge', 'counter', 'attribute']):
            list_output.append(self.xpath)
        elif (self.node_type in ['root', 'obj_cont', 'obj_list']):
            if (self.node_type == 'root'):
                list_output = []
            one_struct_output = []
            for node in self.children:
                node.emit_extern_param(indent_str+one_indent, one_struct_output)
            if (self.node_type in ['obj_list']):
                for i in range(int(self.list_max_elements)):
                    # Expand id in XPATH to its index
                    one_struct_output_copy = [re.sub('(.*)\[id\]', '\\1[%s]' % (str(i)), line) for line in one_struct_output]
                    list_output.extend(one_struct_output_copy)
            else:
                list_output.extend(one_struct_output)
            if (self.node_type == 'root'):
                for line in list_output:
                    c_var_name = SchemaHelper.get_c_xpath(line, remove_list_id=False)
                    print 'extern const schema_node_t sn_%s;' % (c_var_name)
                print 'extern const schema_node_t sn_%s_root_;' % (dm_name)


    def emit_xpath_to_oid(self, indent_str='', list_output=None):
        one_indent = '  '
        c_label = self.c_label
        if (self.node_type in ['config', 'inventory', 'monitor', 'fault', 'gauge', 'counter', 'attribute']):
            list_output.append('%s\t%s' % (self.xpath, self.oid))
        elif (self.node_type in ['obj_cont', 'obj_list']):
            if (list_output is None):
                list_output = []
            one_struct_output = []
            for node in self.children:
                node.emit_xpath_to_oid(indent_str+one_indent, one_struct_output)
            if (self.node_type in ['obj_list']):
                # Add indentation
                #one_struct_output = ['%s%s' % (one_indent, line) for line in one_struct_output]
                for i in range(int(self.list_max_elements)):
                    one_struct_output_copy = [re.sub('(.*)\[id\](.*)\[\d*;\d*\.\.\d*\]', '\\1[%s]\\2[%s]' %(str(i), str(i)), line) for line in one_struct_output]
                    list_output.extend(one_struct_output_copy)
            else:
                list_output.extend(one_struct_output)
            if indent_str == '':
                print '\n'.join(list_output)


class DataNode:
    def __init__(self, s_node, parent_c_xpath_hierachy, parent_c_xpath_flat, siblingIndex=None):
        """ SchemaNode s_node """
        self.s_node = s_node
        self.c_label = SchemaHelper.get_c_label(s_node.label)
        self.siblingIndex = siblingIndex  # None means singleton
        if (siblingIndex is None):
            xp_h = (parent_c_xpath_hierachy + '.') if parent_c_xpath_hierachy else ''
            xp_f = (parent_c_xpath_flat + '__') if parent_c_xpath_flat else ''
            if (s_node.node_type == 'root'):
                self.c_xpath_hierachy = ''
                self.c_xpath_flat = ''
            else:
                self.c_xpath_hierachy = xp_h + self.c_label
                self.c_xpath_flat = xp_f + self.c_label
        else:
            xp_h = (parent_c_xpath_hierachy + '.%s[%d]') if parent_c_xpath_hierachy else '%s[%d]'
            xp_f = (parent_c_xpath_flat + '__%s___%d___') if parent_c_xpath_flat else '%s___%d___'
            self.c_xpath_hierachy = xp_h % (self.c_label, siblingIndex)
            self.c_xpath_flat = xp_f % (self.c_label, siblingIndex)
        self.children = []

    def add_child(self, child_node):
        self.children.append(child_node)

    def get_parent_xpath_hierarchy(self):
        return re.sub('([^\.]*)$', '', self.c_xpath_hierachy)

    def get_def_ref_xpath_hierarchy(self):
        return self.get_parent_xpath_hierarchy() + SchemaHelper.get_c_label(re.sub('(^.*/)', '', self.s_node.def_ref))

    def get_min_ref_xpath_hierarchy(self):
        return self.get_parent_xpath_hierarchy() + SchemaHelper.get_c_label(re.sub('(^.*/)', '', self.s_node.min_ref))

    def get_max_ref_xpath_hierarchy(self):
        return self.get_parent_xpath_hierarchy() + SchemaHelper.get_c_label(re.sub('(^.*/)', '', self.s_node.max_ref))

def build_data_tree(s_node, parent_c_xpath_hierachy='', parent_c_xpath_flat='', siblingIndex=None):
    data_node = DataNode(s_node, parent_c_xpath_hierachy, parent_c_xpath_flat, siblingIndex)
    for child_snode in s_node.children:
        if child_snode.is_singleton():
            data_node.add_child(build_data_tree(child_snode, data_node.c_xpath_hierachy, data_node.c_xpath_flat, None))
        elif child_snode.is_obj_list():
            for i in range(int(child_snode.list_max_elements)):
                data_node.add_child(build_data_tree(child_snode, data_node.c_xpath_hierachy, data_node.c_xpath_flat, i))
    return data_node

def emit_data_tree_simple(data_node, indent_str=''):
    one_indent = '  '
    print indent_str, data_node.c_label, data_node.c_xpath_flat, data_node.c_xpath_hierachy
    indent_str += one_indent
    for child in data_node.children:
        emit_data_tree_simple(child, indent_str)

def emit_data_tree(data_node):
    one_indent = '  '
    #print data_node.c_label, data_node.c_xpath_flat, data_node.c_xpath_hierachy
    if (data_node.s_node.node_type in ['root', 'obj_cont', 'obj_list']):
        for child in data_node.children:
            emit_data_tree(child)
    if (data_node.s_node.node_type in ['root', 'obj_cont', 'obj_list']):
        c_xpath_flat = data_node.c_xpath_flat if (data_node.s_node.node_type != 'root') else '%s_root_' % dm_name
        print 'const schema_node_t *sn_%s_children[%d] = {' % (c_xpath_flat, len(data_node.children))
        for child in data_node.children:
            print '%s&sn_%s,' % (one_indent, child.c_xpath_flat)
        print('};')
    c_xpath_flat = data_node.c_xpath_flat if (data_node.s_node.node_type != 'root') else '%s_root_' % dm_name
    print 'const schema_node_t sn_%s = {' % c_xpath_flat
    array_index = ''
    rebase_comment = ''
    if (data_node.s_node.node_type == 'obj_list'):
        array_index = '[%d]' % (data_node.siblingIndex + int(data_node.s_node.min_range))
        if (int(data_node.s_node.min_range) != 0):
            rebase_comment = ' /* xpath%s refers to [%s] in data block */' % (array_index, data_node.siblingIndex)
    print '%s.label = "%s%s",%s' % (one_indent, data_node.s_node.label, array_index, rebase_comment)
    print('%s.category = %s,' % (one_indent, data_node.s_node.get_c_node_type_define()))
    if (data_node.s_node.node_type in ['root', 'obj_cont', 'obj_list']):
        print('%s.datatype = SCHEMA_DATATYPE_NONE,' % (one_indent))
    else:
        print('%s.datatype = %s,' % (one_indent, data_node.s_node.get_cdatatype_define()))
    if (data_node.s_node.node_type in ['root', 'obj_cont', 'obj_list']):
        print '%s.range_type = SCHEMA_NODE_RANGETYPE_NONE,' % one_indent
        print '// %s.def_type = SCHEMA_NODE_DEFTYPE_NONE,' % one_indent
        print('%s.range = NULL,' % one_indent)
        #print('%s.min_range = NULL,' % one_indent)
        #print('%s.max_range = NULL,' % one_indent)
        print('%s.def = NULL,' % one_indent)
        print('%s.val = &sn_%s_children[0],' % (one_indent, c_xpath_flat))
        print('%s.val_updated = NULL,' % (one_indent))
        print('%s.num_children = %d,' % (one_indent, len(data_node.children)))
        if (not len(data_node.children)):
            print 'SCHEMA WARNING: %s has no child' % data_node.s_node.xpath
    elif (data_node.s_node.is_param):
        range_type = 'SCHEMA_NODE_RANGETYPE_NONE'
        range_val = 'NULL'
        #min_range_val = 'NULL'
        #max_range_val = 'NULL'
        def_type = 'SCHEMA_NODE_DEFTYPE_NONE'
        def_val = 'NULL'
        if (data_node.s_node.node_type in ['config']):
            if (data_node.s_node.datatype == 'enumeration'):
                range_type = 'SCHEMA_NODE_RANGETYPE_ENUMS'
                range_val = '&%s_%s_fixed_ranges.%s' % (dm_name, data_node.s_node.node_type, SchemaHelper.get_c_var_for_xpath(data_node.s_node.xpath, remove_list_id=True))
            else:
                min_max_range = data_node.s_node.get_min_max_range()
                if min_max_range is None:
                    range_type = 'SCHEMA_NODE_RANGETYPE_NONE'
                elif ((min_max_range['valid_bits'] & CHelper.MIN_REF_DEFINED_BIT_MASK) and
                    (min_max_range['valid_bits'] & CHelper.MAX_REF_DEFINED_BIT_MASK)):
                    range_type = 'SCHEMA_NODE_RANGETYPE_REFS'
                    range_val = '&%s_%s_ref_ranges.%s' % (dm_name, data_node.s_node.node_type, data_node.c_xpath_hierachy)
                    #min_range_val = '&%s_attribute_values.%s' % (dm_name, data_node.get_min_ref_xpath_hierarchy())
                    #max_range_val = '&%s_attribute_values.%s' % (dm_name, data_node.get_max_ref_xpath_hierarchy())
                elif ((min_max_range['valid_bits'] & CHelper.MIN_VAL_DEFINED_BIT_MASK) and
                    (min_max_range['valid_bits'] & CHelper.MAX_VAL_DEFINED_BIT_MASK)):
                    range_type = 'SCHEMA_NODE_RANGETYPE_FIXED'
                    range_val = '&%s_%s_fixed_ranges.%s' % (dm_name, data_node.s_node.node_type, SchemaHelper.get_c_var_for_xpath(data_node.s_node.xpath, remove_list_id=True))
                    #min_range_val = '&%s_%s_fixed_ranges.%s[0]' % (dm_name, data_node.s_node.node_type, SchemaHelper.get_c_var_for_xpath(data_node.s_node.xpath, remove_list_id=True))
                    #max_range_val = '&%s_%s_fixed_ranges.%s[1]' % (dm_name, data_node.s_node.node_type, SchemaHelper.get_c_var_for_xpath(data_node.s_node.xpath, remove_list_id=True))
        if (data_node.s_node.node_type in ['config', 'inventory']):
            if (data_node.s_node.def_val is not None):
                def_type = 'SCHEMA_NODE_DEFTYPE_FIXED'
                #def_val = '&agt_%s_fixed_defaults.%s' % (data_node.s_node.node_type, data_node.c_xpath_hierachy)
                def_val = '&%s_%s_fixed_defaults.%s' % (dm_name, data_node.s_node.node_type, SchemaHelper.get_c_var_for_xpath(data_node.s_node.xpath, remove_list_id=True))
            elif (data_node.s_node.def_ref):
                # FIXME-DM def_type = 'SCHEMA_NODE_DEFTYPE_REF'
                # FIXME-DM def_val = '&%s_%s_ref_defaults.%s' % (dm_name, data_node.s_node.node_type, data_node.c_xpath_hierachy)
                def_type = 'SCHEMA_NODE_DEFTYPE_FIXED /* FIXME def_type not required */'
                def_val = '&%s_attribute_values.%s' % (dm_name, data_node.get_def_ref_xpath_hierarchy())
            else:
                def_val = 'NULL /* FIXME - SCHEMA ERROR: Schema does not define either fixed or def-ref attribute */'
                #TODO print >>sys.stderr, 'SCHEMA ERROR: no fixed default/def-ref for', data_node.s_node.xpath
        elif (data_node.s_node.node_type in ['attribute']):
            if (data_node.s_node.def_val is not None):
                #def_val = '&agt_%s_fixed_defaults.%s' % (data_node.s_node.node_type, data_node.c_xpath_hierachy)
                def_val = '&%s_%s_fixed_defaults.%s' % (dm_name, data_node.s_node.node_type, SchemaHelper.get_c_var_for_xpath(data_node.s_node.xpath, remove_list_id=True))
            else:
                def_val = 'NULL /* FIXME - SCHEMA ERROR: Attrbute does not have def-val in SCHEMA */'
                print >>sys.stderr, 'SCHEMA ERROR: Attribute %s does not have default' % data_node.s_node.xpath
        print '%s.range_type = %s,' % (one_indent, range_type)
        print '// %s.def_type = %s,' % (one_indent, def_type)
        print '%s.range = %s,' % (one_indent, range_val)
        #print '%s.min_range = %s,' % (one_indent, min_range_val)
        #print '%s.max_range = %s,' % (one_indent, max_range_val)
        print '%s.def = %s,' % (one_indent, def_val)
        print('%s.val = &%s_%s_values.%s,' % (one_indent, dm_name, data_node.s_node.node_type, data_node.c_xpath_hierachy))
        if (data_node.s_node.node_type in ['config']):  # Support for _updated flag
            print('%s.val_updated = NULL,' % (one_indent))
        else:
            updated_var = re.sub('(\w*$)', '_\\1_updated', data_node.c_xpath_hierachy)
            print('%s.val_updated = &%s_%s_values.%s,' % (one_indent, dm_name, data_node.s_node.node_type, updated_var))
        print('%s.num_children = 0,' % (one_indent))
    print('};')


class SchemaNodeList:
    def __init__(self):
        self.node_list = []
        self.root_node = SchemaNode('')
        self.root_node.node_type = 'root'
        self.append(self.root_node)

    def append(self, x):
        self.node_list.append(x)

    def get_root_node(self):
        return self.root_node

    def emit_tree(self):
        total_instance_params = 0
        total_data_size = 0
        total_instances = {}
        total_size = {}

        max_oid = 0
        max_xpath = 0
        print '\n--------------------------------------------------------------------------------'
        print 'Node Type | Label | instances | config | cond_var | datatype | def_val | min range/elem | max range/elem | oid | xpath | enum | validation(must) | condition(when) | description' 
        for node in self.node_list:
            parent_dots = re.sub('[^/]', '', node.xpath).replace('/', '.')[:-1]
            num_instances = 1
            for array_size_str in re.findall('\[\d*', node.oid):            # find strings [size;range]
                num_instances *= int(array_size_str.replace('[',''))        # string leading [
            data_size = node.get_data_size()
            all_inst_size = num_instances * data_size
            print '%-10s|%-30s|%-3d|%-3d|%-7d|%-2d|%2d|%-20s|%-10s|%-10s|%-10s|%-10s|%-80s|%-120s|%-80s|%-80s|%-80s|%s' % (node.node_type, parent_dots + node.label, num_instances, data_size, all_inst_size, node.config, node.cond_var, node.datatype, node.def_val, node.min_range if (node.yang_type == 'leaf') else node.list_max_elements, node.max_range if (node.yang_type == 'leaf') else node.list_max_elements, node.oid, node.xpath, node.enum, node.must_stmt, node.when_stmt, node.description)
            if (len(node.oid) > max_oid):
                max_oid = len(node.oid)
            if (len(node.xpath) > max_xpath):
                max_xpath = len(node.xpath)
            total_instance_params += num_instances
            total_data_size += all_inst_size
            if node.node_type in total_instances:
                total_instances[node.node_type] += num_instances
                total_size[node.node_type] += all_inst_size
            else:
                total_instances[node.node_type] = num_instances
                total_size[node.node_type] = all_inst_size
        print ''
        print 'max_oid: %d max_xpath: %d' % (max_oid, max_xpath)
        print 'total_instance_params: %d' % total_instance_params
        print 'total_data_size: %d' % total_data_size
        print


class SchemaTypedef:
    def __init__(self, stmt):
        self.stmt = stmt        # Class statement
        self.typedef_name = stmt.arg            # typedef name
        self.datatype = ''                  # base yang type
        self.enum = {}
        self.min_range = ''
        self.max_range = ''
        #
        ch = stmt.search_one('type')
        if (ch is not None):
            # FIXME: why is datatype inside typedef is also having ipg-cmn?
            # FIXME: If so, Need to recursively find base type
            self.datatype = ch.arg.replace('ipg-cmn:', '').strip()
            if (self.datatype == 'enumeration'):
                for enum_st in ch.substmts:
                    if enum_st.keyword == 'enum':
                        val_st = enum_st.search_one('value')
                        if (val_st is not None):
                            self.enum[enum_st.arg] = int(val_st.arg)
                        else:
                            print >>sys.stderr, 'SCHEMA ERROR: No val in enum', self.typedef_name
                            self.enum[enum_st.arg] = None
                #self.min_range = min(self.enum.values())
                #self.max_range = max(self.enum.values())
            else:
                if (hasattr(ch, 'i_ranges') and len(ch.i_ranges)):
                    (self.min_range, self.max_range) = SchemaHelper.get_min_max_for_type_stmt(ch, self.datatype)
        pass


schema_l = SchemaNodeList()
schema_d = {}  # dictionary on xpath
schema_d[schema_l.get_root_node().label] = schema_l.get_root_node()
schema_t = {}  # Typedefs in schema


def get_root_node():
    return schema_l.get_root_node()

def get_parent_schema_node(xpath):
    return schema_d[SchemaNode.get_parent_xpath(xpath)]

def get_schema_node(xpath):
    if (schema_d.has_key(xpath)):
      return schema_d[xpath]
    return None


def pyang_plugin_init():
    plugin.register_plugin(DmCodegenPlugin())

class DmCodegenPlugin(plugin.PyangPlugin):
    def __init__(self):
        plugin.PyangPlugin.__init__(self, 'dm_codegen_plugin')

    def add_output_format(self, fmts):
        self.multiple_modules = True
        fmts['dm_codegen_plugin'] = self

    def add_opts(self, optparser):
        optlist = [
            optparse.make_option('--dm_codegen_plugin-help',
                                 dest='tree_help',
                                 action='store_true',
                                 help='Print help on dm_codegen_plugin symbols and exit'),
            optparse.make_option('--dm-name',
                                 type='str',
                                 dest='dm_name',
                                 help='Name of the data model.'),
            optparse.make_option('--pack-struct',
                                 action='store_true',
                                 dest='pack_struct',
                                 help='Pack structures?')
            ]
        g = optparser.add_option_group('Tree output specific options')
        g.add_options(optlist)

    def setup_ctx(self, ctx):
        global pack_struct
        if ctx.opts.tree_help:
            print_help()
            sys.exit(0)
        if ctx.opts.pack_struct:
            pack_struct = '__attribute__((__packed__))'

    def setup_fmt(self, ctx):
        ctx.implicit_errors = False

    def emit(self, ctx, modules, fd):
        global dm_name

        path = None

        # emit_tree updates SchemaNode
        emit_tree(ctx, modules, fd, None,
                  None, path)
        print

        root_data_node = build_data_tree(get_root_node())

        #print '#if 0'
        #root_data_node.emit_tree()
        #root_data_node.emit_label_tree()
        #print '#endif  /* 0 */'

        if ctx.opts.dm_name is not None:
            dm_name = ctx.opts.dm_name
            if re.match('^[\w]+$', dm_name) is None:
                print 'Error: Bad data model name', ctx.opts.dm_name
                sys.exit(-1)
        else:
            dm_name = "agt"
        print 'Writing code to dm_%s.h and dm_%s.c' % (dm_name, dm_name)
        c_file = open('dm_%s.c' % dm_name, 'w')
        h_file = open('dm_%s.h' % dm_name, 'w')
        dm_types_h_file = open('dm_types.h', 'w')

        orig_stdout = sys.stdout
        one_indent = '  '

        print >>c_file, '/* Auto-generated with cmd-line: %s */' % ' '.join(sys.argv)
        print >>h_file, '/* Auto-generated with cmd-line: %s */' % ' '.join(sys.argv)
        print >>dm_types_h_file, '/* Auto-generated with cmd-line: %s */' % ' '.join(sys.argv)

        print >>c_file, '#include "dm_%s.h"\n' % dm_name

        sys.stdout = dm_types_h_file
        print '#ifndef _DM_TYPES_H_'
        print '#define _DM_TYPES_H_\n'
        print '#include <stdint.h>'
        print '#include <stddef.h>'
        print
        CHelper.emit_constants()
        print
        print 'typedef struct %s schema_node {' % pack_struct
        print '%s%s' % (one_indent, 'const char *label;')
        print '%s%s' % (one_indent, 'const void *range;')
        print '%s%s' % (one_indent, 'const void *def;')
        print '%s%s' % (one_indent, 'void *val;')
        print '%s%s' % (one_indent, 'uint8_t *val_updated;')
        print '%s%s' % (one_indent, 'const uint16_t num_children;')
        print '%s%s' % (one_indent, 'const uint8_t category;')
        print '%s%s' % (one_indent, 'const uint8_t datatype;')
        print '%s%s' % (one_indent, 'const uint8_t range_type;')
        print '// %s%s' % (one_indent, 'const uint8_t def_type;')
        print '} schema_node_t;'
        print
        print '#endif  /* _DM_TYPES_H_ */'

        sys.stdout = h_file
        print '#ifndef _DM_%s_H_' % dm_name.upper()
        print '#define _DM_%s_H_\n' % dm_name.upper()
        print '#include "dm_types.h"'
        print
        get_root_node().emit_extern_param()
        print
        print '/*******************************************************************************'
        print ' * OBJECT LIST ELEMENTS COUNT                                                  *'
        print '*******************************************************************************/'
        get_root_node().emit_obj_list_cnt()
        print
        print '/*******************************************************************************'
        print ' * ENUMERATION #DEFINES                                                        *'
        print '*******************************************************************************/'
        get_root_node().emit_agt_enum_defines()

        for param_type in ['attribute', 'config', 'fault', 'inventory', 'monitor', 'gauge', 'counter']:
            get_root_node().emit_agt_param_struct(param_type)
        #for param_type in ['attribute', 'config', 'fault', 'inventory', 'monitor', 'gauge', 'counter']:
        #    print 'extern %s_%s_values_t %s_%s_values;' % (dm_name, param_type, dm_name, param_type)

        sys.stdout = c_file
        for param_type in ['attribute', 'config', 'fault', 'inventory', 'monitor', 'gauge', 'counter']:
            print '%s_%s_values_t %s_%s_values;' % (dm_name, param_type, dm_name, param_type)
        print

        sys.stdout = h_file
        get_root_node().emit_agt_param_fixed_defaults_struct('attribute')
        sys.stdout = c_file
        get_root_node().emit_agt_param_fixed_defaults('attribute')
        sys.stdout = h_file
        get_root_node().emit_agt_param_fixed_defaults_struct('config')
        #get_root_node().emit_agt_param_ref_defaults_struct('config')
        sys.stdout = c_file
        get_root_node().emit_agt_param_fixed_defaults('config')
        #get_root_node().emit_agt_param_ref_defaults('config')
        sys.stdout = h_file
        get_root_node().emit_agt_param_fixed_defaults_struct('inventory')
        #get_root_node().emit_agt_param_ref_defaults_struct('inventory')
        sys.stdout = c_file
        get_root_node().emit_agt_param_fixed_defaults('inventory')
        #get_root_node().emit_agt_param_ref_defaults('inventory')
        # fault severity is not used by agent/HW Thread
        # As per new design fault default is not needed in agent.


        sys.stdout = h_file
        get_root_node().emit_agt_fixed_ranges_struct('config')
        get_root_node().emit_agt_ref_ranges_struct('config')

        sys.stdout = c_file
        get_root_node().emit_agt_fixed_ranges('config')
        get_root_node().emit_agt_ref_ranges('config')

        sys.stdout = c_file
        ##TODO: print C pointer to value param
        #print '#if 0'
        #emit_data_tree_simple(data_node)
        #print '#endif'
        #print
        emit_data_tree(root_data_node)

        #sys.stdout = orig_stdout
        #for node in schema_l.node_list:
        #    print node.xpath

        #dm_main_file = open('dm_%s_main.c' % dm_name, 'w')
        sys.stdout = c_file
        #emit_main()

        sys.stdout = h_file
        #for param_type in ['attribute', 'config', 'fault', 'inventory', 'monitor', 'gauge', 'counter']:
        #    print 'extern %s_%s_values_t %s_%s_values;' % (dm_type, param_type, dm_type, param_type);
        #print 'extern const schema_node_t sn_%s_root_;' % dm_name
        print
        print '#endif  /* _DM_%s_H_ */' % dm_name.upper()


def emit_main():
    print
    print '#include <stdio.h>'
    print 'int main() {'
    #for param_type in ['attribute', 'config', 'fault', 'inventory', 'monitor', 'gauge', 'counter']:
    #    print '\tprintf("sizeof %s_%s_values %s\\n", sizeof(%s_%s_values));' % (dm_name, param_type, '%d', dm_name, param_type)
    #print '\tprintf("sizeof values %s\\n", ' % '%d'
    #for param_type in ['attribute', 'config', 'fault', 'inventory', 'monitor', 'gauge', 'counter']:
    #    print '\t\tsizeof(%s_%s_values) +' % (dm_name, param_type)
    #print '\t\t0);'
    #for param_type in ['attribute', 'config', 'inventory']:
    #    print '\tprintf("sizeof %s_%s_fixed_defaults %s\\n", sizeof(%s_%s_fixed_defaults_t));' % (dm_name, param_type, '%d', dm_name, param_type)
    #print '\tprintf("sizeof fixed defaults %s\\n", ' % '%d'
    #for param_type in ['attribute', 'config', 'inventory']:
    #    print '\t\tsizeof(%s_%s_fixed_defaults_t) +' % (dm_name, param_type)
    #print '\t\t0);'
    #for param_type in ['config', 'inventory']:
    #    print '\tprintf("sizeof %s_%s_ref_defaults %s\\n", sizeof(%s_%s_ref_defaults_t));' % (dm_name, param_type, '%d', dm_name, param_type)
    #print '\tprintf("sizeof ref defaults %s\\n", ' % '%d'
    #for param_type in ['config', 'inventory']:
    #    print '\t\tsizeof(%s_%s_ref_defaults_t) +' % (dm_name, param_type)
    #print '\t\t0);'
    print '\t return 0;'

    print '}'

def print_help():
    print("""
Each node is printed as:

<status>--<flags> <name><opts> <type> <if-features>

  <status> is one of:
    +  for current
    x  for deprecated
    o  for obsolete

  <flags> is one of:
    rw  for configuration data
    ro  for non-configuration data, output parameters to rpcs
        and actions, and notification parameters
    -w  for input parameters to rpcs and actions
    -u  for uses of a grouping
    -x  for rpcs and actions
    -n  for notifications

  <name> is the name of the node
    (<name>) means that the node is a choice node
   :(<name>) means that the node is a case node

   If the node is augmented into the tree from another module, its
   name is printed as <prefix>:<name>.

  <opts> is one of:
    ?  for an optional leaf, choice, anydata or anyxml
    !  for a presence container
    *  for a leaf-list or list
    [<keys>] for a list's keys

    <type> is the name of the type for leafs and leaf-lists, or
           "<anydata>" or "<anyxml>" for anydata and anyxml, respectively

    If the type is a leafref, the type is printed as "-> TARGET", where
    TARGET is the leafref path, with prefixes removed if possible.

  <if-features> is the list of features this node depends on, printed
    within curly brackets and a question mark "{...}?"
""")

def emit_tree(ctx, modules, fd, depth, llen, path):
    for module in modules:
        printed_header = False

        def print_header():
            bstr = ""
            b = module.search_one('belongs-to')
            if b is not None:
                bstr = " (belongs-to %s)" % b.arg
            fd.write("%s: %s%s\n" % (module.keyword, module.arg, bstr))
            printed_header = True

        if (module.i_typedefs):
            #print "Found typedefs in ", module.keyword, module.arg, module.i_typedefs
            for typedef, definition in module.i_typedefs.items():
                schema_t[definition.arg] = SchemaTypedef(definition)

        chs = [ch for ch in module.i_children
               if ch.keyword in statements.data_definition_keywords]
        if path is not None and len(path) > 0:
            chs = [ch for ch in chs if ch.arg == path[0]]
            chpath = path[1:]
        else:
            chpath = path

        if len(chs) > 0:
            if not printed_header:
                print_header()
                printed_header = True
            print_children(chs, module, fd, '', chpath, 'data', depth, llen,
                           None, "")

        mods = [module]
        for i in module.search('include'):
            subm = ctx.get_module(i.arg)
            if subm is not None:
                mods.append(subm)
        for m in mods:
            section_delimiter_printed=False
            for augment in m.search('augment'):
                if (hasattr(augment.i_target_node, 'i_module') and
                    augment.i_target_node.i_module not in modules + mods):
                    if not section_delimiter_printed:
                        fd.write('\n')
                        section_delimiter_printed = True
                    # this augment has not been printed; print it
                    if not printed_header:
                        print_header()
                        printed_header = True
                    print_path("  augment", ":", augment.arg, fd, llen)
                    mode = 'augment'
                    if augment.i_target_node.keyword == 'input':
                        mode = 'input'
                    elif augment.i_target_node.keyword == 'output':
                        mode = 'output'
                    elif augment.i_target_node.keyword == 'notification':
                        mode = 'notification'
                    print_children(augment.i_children, m, fd,
                                   '  ', path, mode, depth, llen,
                                   None, "")


def unexpand_uses(i_children):
    res = []
    uses = []
    for ch in i_children:
        if hasattr(ch, 'i_uses'):
            g = ch.i_uses[0].arg
            if g not in uses:
                # first node from this uses
                uses.append(g)
                res.append(ch.i_uses[0])
        else:
            res.append(ch)
    return res

def print_path(pre, post, path, fd, llen):
    def print_comps(pre, p, is_first):
        line = pre + '/' + p[0]
        p = p[1:]
        if len(line) > llen:
            # too long, print it anyway; it won't fit next line either
            pass
        else:
            while len(p) > 0 and len(line) + 1 + len(p[0]) <= llen:
                if len(p) == 1 and len(line) + 1 + len(p[0]) + len(post) > llen:
                    # if this is the last component, ensure 'post' fits
                    break
                line += '/' + p[0]
                p = p[1:]
        if len(p) == 0:
            line += post
        line += '\n'
        fd.write(line)
        if len(p) > 0:
            if is_first:
                pre = ' ' * (len(pre) + 2) # indent next line
            print_comps(pre, p, False)

    line = pre + ' ' + path + post
    if llen is None or len(line) <= llen:
        fd.write(line + '\n')
    else:
        p = path.split('/')
        if p[0] == '':
            p = p[1:]
        pre += " "
        print_comps(pre, p, True)

def print_children(i_children, module, fd, prefix, path, mode, depth,
                   llen, no_expand_uses, p_oid, p_xpath="", width=0):
    if depth == 0:
        if i_children: fd.write(prefix + '     ...\n')
        return
    def get_width(w, chs):
        for ch in chs:
            if ch.keyword in ['choice', 'case']:
                nlen = 3 + get_width(0, ch.i_children)
            else:
                if ch.i_module.i_modulename == module.i_modulename:
                    nlen = len(ch.arg)
                else:
                    nlen = len(ch.i_module.i_prefix) + 1 + len(ch.arg)
            if nlen > w:
                w = nlen
        return w

    if no_expand_uses:
        i_children = unexpand_uses(i_children)

    if width == 0:
        width = get_width(0, i_children)

    #print "RAVI", type(i_children)

    for ch_idx,ch in enumerate(i_children):
        if ((ch.keyword == 'input' or ch.keyword == 'output') and
            len(ch.i_children) == 0):
            pass
        else:
            if (ch == i_children[-1] or
                (i_children[-1].keyword == 'output' and
                 len(i_children[-1].i_children) == 0)):
                # the last test is to detect if we print input, and the
                # next node is an empty output node; then don't add the |
                newprefix = prefix + '   '
            else:
                newprefix = prefix + '  |'
            if ch.keyword == 'input':
                mode = 'input'
            elif ch.keyword == 'output':
                mode = 'output'
            print_node(ch, module, fd, newprefix, path, mode, depth, llen,
                       no_expand_uses, width, p_oid, p_xpath, ch_idx)

def get_ipg_ext(s, module, ext):
    for stmt in s.substmts:
        if stmt.keyword == (module, ext):
            if ext == u'condition':   # condition extenation does not have value
                return "cond"
            elif ext == 'category':
                if stmt.arg in ['config', 'inventory', 'monitor', 'fault', 'gauge', 'counter', 'attribute']:
                    return stmt.arg
                else:
                    print >>sys.stderr, "ERROR: unknown category", stmt.arg
                    return None
            else:
                return stmt.arg
    return None

def print_node(s, module, fd, prefix, path, mode, depth, llen,
               no_expand_uses, width, p_oid, p_xpath, ch_idx):
    line = "%s%s--" % (prefix[0:-1], get_status_str(s))

    if (s.i_typedefs):
        print >>sys.stderr, 'SCHEMA ERROR: Found typedef in %s %s %s' % (s.keyword, s.arg, s.i_typedefs)

    brcol = len(line) + 4

    if s.i_module.i_modulename == module.i_modulename:
        name = s.arg
    else:
        name = s.i_module.i_prefix + ':' + s.arg


    flags = get_flags_str(s, mode)

    oid = p_oid
    if (p_oid) :
        oid += "."
    oid += str(ch_idx+1)
    #xpath = p_xpath + "/" + name
    xpath = p_xpath + "/" + s.arg

    print ">>>>>>>>>>>>>>>>>>", xpath

    #schema_node = SchemaNode(name)
    schema_node = SchemaNode(s.arg)
    schema_node.yang_type = s.keyword

    ipg_attrs = "{"
    if s.keyword == 'list' or s.keyword == 'container':
        obj_ext = get_ipg_ext(s, u'ipg-common', u'object')
        schema_node.node_type = "obj_" + s.keyword[:4]
        if obj_ext is not None:
            ipg_attrs += "%s%s%s" % ("o=", obj_ext, ',')
            schema_node.obj_name = obj_ext
        else:
            print "WARNING: container / list is not an object, XPATH=%s" % xpath
            obj_ext = "UNKNOWN_OBJECT"
            ipg_attrs += "%s%s%s" % ("o=", obj_ext, ',')
            schema_node.obj_name = obj_ext
    elif s.keyword == 'leaf':
        cat_ext = get_ipg_ext(s, u'ipg-common', u'category')
        if cat_ext is not None:
            ipg_attrs += "%s%s%s" % ("c=", cat_ext, ',')
            schema_node.node_type = cat_ext
            def_sev = get_ipg_ext(s, u'ipg-common', u'severity')
            if (def_sev is not None):
                if (cat_ext != 'fault'):
                    print >>sys.stderr, 'SCHEMA ERRROR: severity specified for %s which is not fault parameter' % (schema_node.label)
                else:
                    schema_node.def_val = def_sev
            min_ref = get_ipg_ext(s, u'ipg-common', u'min-ref')
            max_ref = get_ipg_ext(s, u'ipg-common', u'max-ref')
            def_ref = get_ipg_ext(s, u'ipg-common', u'def-ref')
            # min_ref, max_ref - only one of them cannot be specified
            if ((min_ref and not max_ref) or (not min_ref and max_ref)):
                print >>sys.stderr, 'SCHEMA ERRROR: both min_ref and max_ref should be specified for %s' % (schema_node.label)
            if min_ref is not None:
                if (cat_ext not in ('config', 'gauge')):
                    print >>sys.stderr, 'SCHEMA ERRROR: min_ref specified for %s which is of %s' % (schema_node.label, cat_ext)
                if (not min_ref.startswith('../')):
                    print >>sys.stderr, 'SCHEMA ERRROR: min_ref %s for %s is not XPATH' % (min_ref, schema_node.label)
                schema_node.min_ref = min_ref
            if max_ref is not None:
                if (cat_ext not in ('config', 'gauge')):
                    print >>sys.stderr, 'SCHEMA ERRROR: max_ref specified for %s which is of %s' % (schema_node.label, cat_ext)
                if (not max_ref.startswith('../')):
                    print >>sys.stderr, 'SCHEMA ERRROR: max_ref %s for %s is not XPATH' % (max_ref, schema_node.label)
                schema_node.max_ref = max_ref
            if def_ref is not None:
                if (cat_ext not in ('config', 'gauge')):
                    print >>sys.stderr, 'SCHEMA ERRROR: def_ref specified for %s which is of %s' % (schema_node.label, cat_ext)
                if (not def_ref.startswith('../')):
                    print >>sys.stderr, 'SCHEMA ERRROR: def_ref %s for %s is not XPATH' % (def_ref, schema_node.label)
                schema_node.def_ref = def_ref
            if cat_ext == 'attribute':
                def_val = get_ipg_ext(s, u'ipg-common', u'def-val')
                if (def_val is not None):
                    schema_node.def_val = def_val
                else:
                    print >>sys.stderr, 'SCHEMA ERROR: Attribute %s does not have default' % schema_node.label
                    print >>sys.stderr, def_val
    else:
        print >>sys.stderr, "ERROR: unsupported YANG node", xpath
    cond_ext = get_ipg_ext(s, u'ipg-common', u'condition')
    if cond_ext is not None:
            ipg_attrs += "%s%s" % (cond_ext, ',')
            schema_node.cond_var = 1
    ipg_attrs += "}"

    if s.keyword == 'list':
        name += '*'
        line += flags + " " + name
    elif s.keyword == 'container':
        p = s.search_one('presence')
        if p is not None:
            print "SCHEMA WARNING: 'presence container' is not allowed ", schema_node.label
            name += '!'
        line += flags + " " + name
    elif s.keyword  == 'choice':
        print "SCHEMA WARNING: 'choice' is not allowed ", schema_node.label
        m = s.search_one('mandatory')
        if m is None or m.arg == 'false':
            line += flags + ' (' + name + ')?'
        else:
            line += flags + ' (' + name + ')'
    elif s.keyword == 'case':
        print "SCHEMA WARNING: 'case' is not allowed ", schema_node.label
        line += ':(' + name + ')'
        brcol += 1
    else:
        if s.keyword == 'leaf-list':
            print "SCHEMA WARNING: 'leaf-list' is not allowed ", schema_node.label
            name += '*'
        elif (s.keyword == 'leaf' and not hasattr(s, 'i_is_key')
              or s.keyword == 'anydata' or s.keyword == 'anyxml'):
            m = s.search_one('mandatory')
            if m is None or m.arg == 'false':
                name += '?'
        t = get_typename(s)
        if t == '':
            line += "%s %s" % (flags, name)
        elif (llen is not None and
              len(line) + len(flags) + width+1 + len(t) + 4 > llen):
            # there's no room for the type name
            if (get_leafref_path(s) is not None and
                len(t) + brcol > llen):
                # there's not even room for the leafref path; skip it
                line += "%s %-*s   leafref" % (flags, width+1, name)
            else:
                line += "%s %s" % (flags, name)
                fd.write(line + '\n')
                line = prefix + ' ' * (brcol - len(prefix)) + ' ' + t
        else:
            line += "%s %-*s   %s" % (flags, width+1, name, t)
        schema_node.yang_type = t

    #RAVI
    #for stmt in s.substmts:
    #    print stmt.keyword, " => ", stmt.arg

    if s.keyword == 'list':
        if s.search_one('key') is not None:
            key_arg = re.sub('\s+', ' ', s.search_one('key').arg)
            if (key_arg != 'id'):
                print 'SCHEMA ERROR: list %s has non-standard key %s' % (schema_node.label, key_arg)
            #TODO: validation: key range should match max elements.
            keystr_no_lead = "[%s]" % key_arg
            keystr = " " + keystr_no_lead
            if (llen is not None and
                len(line) + len(keystr) > llen):
                fd.write(line + '\n')
                line = prefix + ' ' * (brcol - len(prefix))
            line += keystr
            xpath += keystr_no_lead
            schema_node.list_key = key_arg
        else:
            line += " []"

        oid += "["
        ch_max = s.search_one('max-elements')
        if ch_max is not None:
            oid += "%s;" % ch_max.arg
            schema_node.list_max_elements = ch_max.arg
            ch_min = s.search_one('min-elements')
            if ch_min is not None:
                if (ch_max.arg != ch_min.arg):
                    print "SCHEMA ERROR: 'min_elements != max_elements' for list", schema_node.label
            else:
                    print "WARNING: 'min_elements' missing for list. Setting min_elements to max_elements for ", schema_node.label
        else:
            print >>sys.stderr, "SCHEMA ERROR: 'max_elements' missing for list ", schema_node.label
        list_id_child = search_child(s.i_children,
                          s.i_module.i_modulename, "id")
        if list_id_child is not None:
            list_type = list_id_child.search_one("type")
            if list_type is not None:
                list_range = list_type.search_one("range")
                if list_range is not None:
                    range = list_range.arg.replace(" .. ", "..")
                    (range_min, range_max) = SchemaHelper.split_range(range)
                    oid += range_min + '..' + range_max
                    # set min_range and max_range in list node too.
                    schema_node.min_range = range_min
                    schema_node.max_range = range_max
                if (list_range is None or range_min is None or range_max is None):
                    print >>sys.stderr, 'SCHEMA ERROR: range for ', schema_node.label
                else:
                    if (int(schema_node.list_max_elements) != (1 + int(range_max) - int(range_min))):
                        print >>sys.stderr, 'SCHEMA ERROR: max-elements not matching with key range specification for', schema_node.label

            else:
                print >>sys.stderr, 'SCHEMA ERROR: type is missing for list', schema_node.label
        else:
            print >>sys.stderr, 'SCHEMA ERRROR: "id" missing for list', schema_node.label
        oid += "]"


    features = s.search('if-feature')
    featurenames = [f.arg for f in features]
    if hasattr(s, 'i_augment'):
        afeatures = s.i_augment.search('if-feature')
        featurenames.extend([f.arg for f in afeatures
                             if f.arg not in featurenames])

    if len(featurenames) > 0:
        fstr = " {%s}?" % ",".join(featurenames)
        if (llen is not None and len(line) + len(fstr) > llen):
            fd.write(line + '\n')
            line = prefix + ' ' * (brcol - len(prefix))
        line += fstr

    schema_node.oid = oid
    schema_node.xpath = xpath

    # adjust xpath for min_ref, max_ref, def_ref
    if (schema_node.min_ref):
        schema_node.min_ref = re.sub('([^/]*)$', '', xpath) + re.sub('\.\./', '', schema_node.min_ref)
        if (not get_schema_node(schema_node.min_ref)):
          print >>sys.stderr, 'SCHEMA ERROR: min_ref %s is not found for %s' % (schema_node.min_ref, xpath)
    if (schema_node.max_ref):
        schema_node.max_ref = re.sub('([^/]*)$', '', xpath) + re.sub('\.\./', '', schema_node.max_ref)
        if (not get_schema_node(schema_node.max_ref)):
          print >>sys.stderr, 'SCHEMA ERROR: max_ref %s is not found for %s' % (schema_node.max_ref, xpath)
    if (schema_node.def_ref):
        schema_node.def_ref = re.sub('([^/]*)$', '', xpath) + re.sub('\.\./', '', schema_node.def_ref)
        if (not get_schema_node(schema_node.def_ref)):
          print >>sys.stderr, 'SCHEMA ERROR: def_ref %s is not found for %s' % (schema_node.def_ref, xpath)
    #line = "%-95s|%-60s|%-20s|%s" % (line, oid, ipg_attrs, xpath)
    line = "%-95s|%-60s|%s" % (line, oid, xpath)
    #fd.write(line + '\n')

    # TODO: Add SCHEMA ERROR for having more ref and range defined in YANG

    if (s.keyword == 'container' or s.keyword == 'list' or s.keyword == 'leaf'):
        if (s.keyword == 'leaf'):
            parent_schema_node = get_parent_schema_node(xpath)
            if (parent_schema_node is None):
                print "WARNING: parent node not found for ", xpath
            ch = s.search_one('default')
            if (ch is not None):
                schema_node.def_val = ch.arg
            ch_type = s.search_one('type')
            if (ch_type is not None):
                schema_node.datatype = ch_type.arg.replace("ipg-cmn:", "").strip()
                # FIXME check if type also defining range
                if (ch_type.arg == "enumeration"):
                    for enum_st in ch_type.substmts:
                        if enum_st.keyword == "enum":
                            val_st = enum_st.search_one("value")
                            if (val_st is not None):
                                schema_node.enum[enum_st.arg] = int(val_st.arg)
                            else:
                                print >>sys.stderr, "SCHEMA ERROR: No val in enum %s" % ch_type.arg
                                schema_node.enum[enum_st.arg] = None
                        #schema_node.min_range = min(schema_node.enum.values())
                        #schema_node.max_range = max(schema_node.enum.values())
                elif (hasattr(ch_type, 'i_ranges') and len(ch_type.i_ranges)):
                    (schema_node.min_range, schema_node.max_range) = \
                            SchemaHelper.get_min_max_for_type_stmt(ch_type, schema_node.datatype)
                elif (not SchemaNode.is_well_known_type(schema_node.datatype)):
                    # datatype needs to be get from typedef.
                    # typedef can be in many places : module/submodule/container/list/grouping/etc
                    # typedef can be nested
                    # typedef can have default value.
                    # typedef may be for enumeration, so it defines default range
                    # FIXME: assumption: typedefs are defined in the module without nesting and default
                    #RAVI for attr in dir(s):
                    #RAVI    print("obj.%s = %r" % (attr, getattr(s, attr)))
                    if schema_node.datatype in schema_t:
                        typedef = schema_t[schema_node.datatype]
                        schema_node.typedef_name = typedef.typedef_name
                        schema_node.datatype = typedef.datatype
                        schema_node.enum = typedef.enum.copy()
                        schema_node.min_range = typedef.min_range
                        schema_node.max_range = typedef.max_range
                    else:
                        print schema_t
                        print >>sys.stderr, "SCHEMA ERROR: found unknown type ", ch_type.arg

            ch = s.search_one('config')
            if (ch is not None):
                if (ch.arg.lower() == "true"):
                    schema_node.config = 1
            else:
                schema_node.config = 1
        ch = s.search_one('description')
        if (ch is not None):
            schema_node.description = ch.arg
        ch = s.search_one('must')
        if (ch is not None):
            schema_node.must_stmt = ch.arg.replace("\n", " ").replace("\r", "");
        ch = s.search_one('when')
        if (ch is not None):
            schema_node.when_stmt = ch.arg.replace("\n", " ").replace("\r", "")
        #print 'xxpath: ', xpath  #RAVI
        schema_l.append(schema_node)
        schema_d[xpath] = schema_node
        parent_path = SchemaNode.get_parent_xpath(xpath)
        if (parent_path):
            parent_schema_node = schema_d[parent_path]
            if (parent_schema_node is not None):
                parent_schema_node.children.append(schema_node)
            else:
                print >>sys.stderr, "ERROR: Something wrong, no parent found for ", schema_node
        else:  # no parent mean, add as child to root
            #print xpath, "is root child"
            get_root_node().children.append(schema_node)

    if hasattr(s, 'i_children') and s.keyword != 'uses':
        if depth is not None:
            depth = depth - 1
        chs = s.i_children
        if path is not None and len(path) > 0:
            chs = [ch for ch in chs
                   if ch.arg == path[0]]
            path = path[1:]
        if s.keyword in ['choice', 'case']:
            print_children(chs, module, fd, prefix, path, mode, depth,
                           llen, no_expand_uses, oid, width - 3, xpath)
        else:
            print_children(chs, module, fd, prefix, path, mode, depth, llen,
                           no_expand_uses, oid, xpath)

def get_status_str(s):
    status = s.search_one('status')
    if status is None or status.arg == 'current':
        return '+'
    elif status.arg == 'deprecated':
        return 'x'
    elif status.arg == 'obsolete':
        return 'o'

def get_flags_str(s, mode):
    if mode == 'input':
        return "-w"
    elif s.keyword in ('rpc', 'action', ('tailf-common', 'action')):
        return '-x'
    elif s.keyword == 'notification':
        return '-n'
    elif s.keyword == 'uses':
        return '-u'
    elif s.i_config == True:
        return 'rw'
    elif s.i_config == False or mode == 'output' or mode == 'notification':
        return 'ro'
    else:
        return ''

def get_leafref_path(s):
    t = s.search_one('type')
    if t is not None:
        if t.arg == 'leafref':
            return t.search_one('path')
    else:
        return None

def get_typename(s):
    t = s.search_one('type')
    if t is not None:
        if t.arg == 'leafref':
            p = t.search_one('path')
            if p is not None:
                # Try to make the path as compact as possible.
                # Remove local prefixes, and only use prefix when
                # there is a module change in the path.
                target = []
                curprefix = s.i_module.i_prefix
                for name in p.arg.split('/'):
                    if name.find(":") == -1:
                        prefix = curprefix
                    else:
                        [prefix, name] = name.split(':', 1)
                    if prefix == curprefix:
                        target.append(name)
                    else:
                        target.append(prefix + ':' + name)
                        curprefix = prefix
                return "-> %s" % "/".join(target)
            else:
                return t.arg
        else:
            return t.arg
    elif s.keyword == 'anydata':
        return '<anydata>'
    elif s.keyword == 'anyxml':
        return '<anyxml>'
    else:
        return ''

# TODO how to verify YANG string pattern in syntax check.
# TODO: exit code on error; is model generated through build process or manually?
# TODO: include parent node pointer in schema_node to be able to print xpath in debugs
