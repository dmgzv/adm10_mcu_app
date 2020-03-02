import binascii
import socket
import struct
import sys
import traceback
import time

server_address = ('192.168.56.48', 50000)
if (len(sys.argv) == 3):
    server_address = (str(sys.argv[1]), int(sys.argv[2]))
elif (len(sys.argv) == 2):
    server_address = ('192.168.56.48', int(sys.argv[1]))

#schema_id='module-name_YYYY_MM_DD'
schema_id = "adm-10-2019-11-28"     # changes after hello message

msg_ver='1.0'

DT_NONE = 0
DT_BOOL = 1
DT_ENUM = 2
DT_INT8 = 3
DT_UINT8 = 4
DT_INT16 = 5
DT_UINT16 = 6
DT_INT32 = 7
DT_UINT32 = 8
DT_INT64 = 9
DT_UINT64 = 10
DT_FLOAT_1 = 11
DT_FLOAT_2 = 12
DT_FLOAT_3 = 13
DT_FLOAT_E1 = 14
DT_FLOAT_E18 = 15
DT_STRING_16 = 16
DT_STRING_32 = 17
DT_STRING_64 = 18
DT_STRING_128 = 19
DT_STRING_256 = 20

SCHEMA_NODE_TYPE_ROOT = 0
SCHEMA_NODE_TYPE_CONFIG = 1
SCHEMA_NODE_TYPE_INVENTORY = 2
SCHEMA_NODE_TYPE_MONITOR = 3
SCHEMA_NODE_TYPE_FAULT = 4
SCHEMA_NODE_TYPE_GAUGE = 5
SCHEMA_NODE_TYPE_COUNTER = 6
SCHEMA_NODE_TYPE_ATTRIBUTE = 7
SCHEMA_NODE_TYPE_OBJECT = 8

def pack_str(s):
    s += '\x00'
    return struct.pack('!%ds' % len(s), s)

def pack_ptv(s, datatype, format_str, val):
    s += '\x00'
    payload = struct.pack('!%dsB' % (len(s)), s, datatype)
    if (format_str == 's'):
        val = val + '\x00'
        payload += struct.pack('!%ds' % len(val), val)
    else:
        payload += struct.pack('!%s' % format_str, val)
    return payload

def unpack_str(s, idx):
    nt = s[idx:].index('\x00')
    return ((s[idx:idx+nt]), idx+nt+1)

def unpack_uint8(s, idx):
    return (int(struct.unpack('!B', s[idx:idx+1])[0]), idx+1)
    #return (int(s[idx:idx+1]), idx+1)

def unpack_uint16(s, idx):
    return (int(struct.unpack('!H', s[idx:idx+2])[0]), idx+2)

def unpack(s, idx, type):
    if type == DT_STRING_16 or \
        type == DT_STRING_32 or \
        type == DT_STRING_64 or \
        type == DT_STRING_128 or \
        type == DT_STRING_256:
        nt = s[idx:].index('\x00')
        return ((s[idx:idx+nt]), idx+nt+1)
    elif type == DT_UINT8 or type == DT_BOOL:
        return (int(struct.unpack('!B', s[idx:idx+1])[0]), idx+1)
    elif type == DT_INT8:
        return (int(struct.unpack('!b', s[idx:idx+1])[0]), idx+1)
    if type == DT_UINT16:
        return (int(struct.unpack('!H', s[idx:idx+2])[0]), idx+2)
    elif type == DT_INT16:
        return (int(struct.unpack('!h', s[idx:idx+2])[0]), idx+2)
    if type == DT_UINT32 or type == DT_ENUM:
        return (int(struct.unpack('!I', s[idx:idx+4])[0]), idx+4)
    elif type == DT_INT32 or type == DT_ENUM:
        return (int(struct.unpack('!i', s[idx:idx+4])[0]), idx+4)
    if type == DT_UINT64:
        return (int(struct.unpack('!Q', s[idx:idx+8])[0]), idx+8)
    elif type == DT_INT64:
        return (int(struct.unpack('!q', s[idx:idx+8])[0]), idx+8)
    elif type == DT_FLOAT_1 or type == DT_FLOAT_2 or type == DT_FLOAT_3 or \
        type == DT_FLOAT_E1 or type == DT_FLOAT_E18:
        return (float(struct.unpack('!f', s[idx:idx+4])[0]), idx+4)

def unpack_ptv(s, idx):
        #print 'unpack input str', repr(s[idx:]), 'type=', type
        (param, idx) = unpack_str(s, idx)
        (datatype, idx) = unpack_uint8(s, idx)
        (val, idx) = unpack(s, idx, datatype)
        return (param, datatype, val, idx)

def recv_resp(sock):
    global schema_id
    print "Receiving...."
    binstr = sock.recv(2)
    #print binascii.hexlify(binstr)
    (msg_len, junk) = unpack_uint16(binstr, 0)
    #print msg_len
    #msg_len = msg_len[0]
    print "Response size:", msg_len
    msg_len -= 2
    binstr = sock.recv(msg_len)
    if (len(binstr) != msg_len):
        print 'Received: %d, Expected: %d' % (len(binstr), msg_len)
    print 'Received:', binascii.hexlify(binstr)
    print 'Received:', repr(binstr)
    fmt = '!IBBBB'
    idx = struct.calcsize(fmt)
    (msg_id, msg_type, target_schema, id_notation, last_fragment) = struct.unpack(fmt, binstr[0:idx])
    print "Response:", (msg_len, msg_id, msg_type, target_schema, id_notation, last_fragment);
    (rcvd_schema_id, idx) = unpack_str(binstr, idx)
    print 'Response: schema_id:"%s"' % (rcvd_schema_id)
    schema_id = rcvd_schema_id
    if (idx < msg_len):
        (status, idx) = unpack_uint8(binstr, idx)
        print 'Received: status', status
        if (status != 0):
            (err, idx) = unpack_str(binstr, idx)
            print 'Received: error_msg', err
        elif (msg_type == 3): # HELLO_RESP
            (param, datatype, val, idx) = unpack_ptv(binstr, idx)
            print '\tReceived: param=', param, 'type=', datatype, 'val=', val
            (param, datatype, val, idx) = unpack_ptv(binstr, idx)
            print '\tReceived: param=', param, 'type=', datatype, 'val=', val
            (param, datatype, val, idx) = unpack_ptv(binstr, idx)
            print '\tReceived: param=', param, 'type=', datatype, 'val=', val
        elif (msg_type == 7 or msg_type == 9): # GET_PARAM_RESP or GET_UPDATED_PARAM_RESP
            (param, datatype, val, idx) = unpack_ptv(binstr, idx)
            print 'Received: num_objs=', val
            num_objs = int(val)
            for i in range(0, num_objs):
                (obj_id, idx) = unpack_str(binstr, idx)
                print '\tReceived obj_id[%d]: %s' % (i, obj_id)
                (param, datatype, val, idx) = unpack_ptv(binstr, idx)
                print '\t\tReceived: num_params=', val
                num_params = int(val)
                for i in range(0, num_params):
                    (param, datatype, val, idx) = unpack_ptv(binstr, idx)
                    print '\t\tReceived: param=', param, 'type=', datatype, 'val=', val
        elif (msg_type == 25):  # GET_SW_UPDATE_STATUS
            (param, datatype, val, idx) = unpack_ptv(binstr, idx)
            print "\tReceived ", param, datatype, val
            (param, datatype, val, idx) = unpack_ptv(binstr, idx)
            print "\tReceived ", param, datatype, ('"%s"' % val)
            
    if (not last_fragment):
        print '\nReceived partial response\n'
        recv_resp(sock)




# Create a TCP/IP socket
print "Connecting to ", server_address
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(server_address)

def get_schema_payload():
    (ip, port, path) = ("10.11.12.13"+'\x00', 125, "/tmp/schema_dir"+'\x00')
    payload = struct.pack('!%dsB%ds' % (len('tftp_srv_ip')+1, len(ip)),
            'tftp_srv_ip' + '\x00', DT_STRING_16, ip)
    payload += struct.pack('!%dsBH' % (len('tftp_srv_port')+1),
            'tftp_srv_port' + '\x00', DT_UINT16, port)
    payload += struct.pack('!%dsB%ds' % (len('tftp_path')+1, len(path)),
            'tftp_path' + '\x00', DT_STRING_128, path)
    return payload

def get_slot_param_payload():
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 1)
    payload += pack_str('/__HIDDEN__')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    return payload
 
def get_card_param_payload(card):
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 1)
    payload += pack_str('/' + card)
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    return payload
 
def get_device_param_payload(card, device):
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 1)
    payload += pack_str('/' + card + '/' + device)
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    return payload
 
def get_interface_param_payload(card, device, iface):
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 1)
    payload += pack_str('/' + card + '/' + device + '/' + iface)
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('module-type')
    return payload
 
def get_module_param_payload(card, device, iface, module):
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 1)
    payload += pack_str('/' + card + '/' + device + '/' + iface + '/' + module)
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    return payload

def get_param_payload():
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 12)
    #payload += pack_str('/adm-10-clbr-2020-02-10')
    #payload += pack_ptv('num_of_params', DT_UINT16, 'H', 1)
    #payload += pack_str('serial-number')
    payload += pack_str('/__HIDDEN__')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('/adm-10-2019-11-28/adm')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('/adm-10-2019-11-28/adm/line[1]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    payload += pack_str('/adm-10-2019-11-28/adm/line[2]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    # iter 2
    payload += pack_str('/__HIDDEN__')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('/adm-10-2019-11-28/adm')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('/adm-10-2019-11-28/adm/line[1]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    payload += pack_str('/adm-10-2019-11-28/adm/line[2]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    # iter 3
    payload += pack_str('/__HIDDEN__')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('/adm-10-2019-11-28/adm')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    payload += pack_str('admin-state')
    payload += pack_str('service-state')
    payload += pack_str('/adm-10-2019-11-28/adm/client[1]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    payload += pack_str('/adm-10-2019-11-28/adm/client[2]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 3)
    payload += pack_str('vendor')
    payload += pack_str('serial-number')
    payload += pack_str('part-number')
    return payload

def get_updated_param_payload():
    payload = pack_ptv('param_category', DT_UINT8, 'B', SCHEMA_NODE_TYPE_MONITOR)
    payload = pack_ptv('param_category', DT_UINT8, 'B', SCHEMA_NODE_TYPE_CONFIG)
    payload = pack_ptv('param_category', DT_UINT8, 'B', SCHEMA_NODE_TYPE_INVENTORY)
    return payload

def set_config_payload():
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 1)
    payload += pack_str('/adm-10-2019-11-28/adm')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 1)
    payload += pack_ptv('admin-state', DT_ENUM, 'I', 3)
    #payload += pack_str('/adm-10-2019-11-28/adm/line[3]/sfp-plus')
    #payload += pack_ptv('num_of_params', DT_UINT16, 'H', 1)
    #payload += pack_ptv('freq-spacing', DT_ENUM, 'I', 1)
    #payload += pack_str('/adm-10-2019-11-28/adm/line[3]/sfp-plus/rx-port/channels[1]')
    #payload += pack_ptv('num_of_params', DT_UINT16, 'H', 2)
    #payload += pack_ptv('wavelength', DT_FLOAT_2, 'I', 22222.1)
    #payload += pack_ptv('frequency', DT_FLOAT_2, 'I', 22222.2)
    return payload

def tx_end_payload(commit):
    payload = pack_ptv('commit', DT_UINT8, 'B', commit)
    return payload

def reset_payload():
    payload = pack_ptv('num_of_objs', DT_UINT16, 'H', 3)
    payload += pack_str('/adm-10-2019-11-28/adm/client[1]')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 1)
    payload += pack_str('admin-state')
    payload += pack_str('/__HIDDEN__')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 1)
    payload += pack_str('admin-state')
    payload += pack_str('/adm-10-2019-11-28/adm/line[2]/sfp')
    payload += pack_ptv('num_of_params', DT_UINT16, 'H', 0)
    return payload

def sw_image_download_payload():
    (ip, port, path) = ("10.11.12.13"+'\x00', 125, "/tmp/schema_dir"+'\x00')
    payload = struct.pack('!%dsB%ds' % (len('tftp_srv_ip')+1, len(ip)),
            'tftp_srv_ip' + '\x00', DT_STRING_16, ip)
    payload += struct.pack('!%dsBH' % (len('tftp_srv_port')+1),
            'tftp_srv_port' + '\x00', DT_UINT16, port)
    payload += struct.pack('!%dsB%ds' % (len('tftp_path')+1, len(path)),
            'tftp_path' + '\x00', DT_STRING_128, path)
    return payload

def send_req(req_type, payload):
    global schema_id
    msg_len = 10 + len(schema_id)+1
    if payload:
        msg_len += len(payload)
    target_schema = 0
    last_fragment = 1
    values = (msg_len, 100+req_type, req_type, target_schema, 0, last_fragment, schema_id)
    packer = struct.Struct('!HIB B B B %ds' % (1+len(schema_id)))
    hdr_part = packer.pack(*values)
    # Send data
    print 'Sending req=', req_type, values, binascii.hexlify(hdr_part)
    sock.sendall(hdr_part)
    if (payload):
        print '        payload =', binascii.hexlify(payload)
        print '        payload =', repr(payload)
        sock.sendall(payload)

try:
    for j in range(4, 20, 2):
        print "j=", j
        payload = ''
        print '-' * 80
        if (j == 4):  # GET_SCHEMA
            send_req(4, get_schema_payload())
        elif (j == 6):  # GET_PARAM
            send_req(6, get_param_payload())
        elif (j == 8):  # GET_UPDATED_PARAM
            send_req(8, get_updated_param_payload())
        elif (j == 10): # TX_START
            send_req(10, None)
        elif (j == 12): # SET_CONFIG
            send_req(12, set_config_payload())
        elif (j == 14): # TX_END
            send_req(14, tx_end_payload(0))  # 0 = commit; 1 = rollback
        elif (j == 16): # REBOOT
            send_req(16, None)
        elif (j == 18): # RESET
            send_req(18, reset_payload())
        elif (j == 20): # SW_DOWNLOAD
            send_req(20, sw_image_download_payload())
        else:
            send_req(j, None)
        try:
            recv_resp(sock)
        except Exception as ex:
            print 'Caught exception', (str(ex))
            traceback.print_exc()
        finally:
            print 'final from recv_resp'
            if (j == 16 or j == 22):
                sock.close()
                print 'will connect again after 10 sec'
                for t in range(0,10):
                    print t, "." * t
                    time.sleep(1)
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sock.connect(server_address)
except KeyboardInterrupt:
    print 'caught KeyboardInterrupt'
    pass
#except Exception as ex:
#    print 'Caught exception', (str(ex))
#    pass
finally:
    print >>sys.stderr, 'closing socket'
    sock.close()

"""
set /adm-10-2019-11-28/adm/admin-state i32 4
set /adm-10-2019-11-28/adm/line[3]/sfp-plus/part-number s hello-world
set /__HIDDEN__/admin-state i32 3
get /adm-10-2019-11-28/adm/admin-state
get /adm-10-2019-11-28/adm/line[3]/sfp-plus/part-number
get /__HIDDEN__/admin-state
"""

