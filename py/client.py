#!/usr/bin/env python3
import socket
import sys
import ssl

'''
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('host', type=str, help='host')
parser.add_argument('port', type=int, help='port')
args = parser.parse_args()

hostname = args.host # HOST = args.host   
PORT = 12012 #args.port              
'''
host_addr = '127.0.0.1'
host_port = 8082
server_sni_hostname = 'example.com'
server_cert = 'server.crt'
client_cert = 'client.crt'
client_key = 'client.key'

context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH, cafile=server_cert)
context.load_cert_chain(certfile=client_cert, keyfile=client_key)

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


# Connect the socket to the port where the server is listening
#server_address =(HOST,args.port) #('localhost', 10000)
server_address =(host_addr,host_port) #('localhost', 10000)
print('connecting to %s port %s' % server_address)
sock.connect(server_address)

# while True: # To keep the connection on
try:
    # Send data
    #message = b'This is the message.  It will be repeated.'
    message = input('Enter the file name\n')
    with open (message, 'rb')as sou:
        dataIn = sou.read()
        #print('sending "%s"' % dataIn)
        sock.sendall(dataIn)
        sou.close()
        
        # Look for the response
        amount_received = 0
        amount_expected = len(dataIn)
    
    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        with open('out', 'ab')as out:
            out.write(data)
            #print('received "%s"' % data)
            out.close()       
finally:
    print('closing socket')
    sock.close()
