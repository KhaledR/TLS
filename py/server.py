#!/usr/bin/env python3

from datetime import datetime
import socket
import ssl
import sys

listen_addr = '127.0.0.1'
listen_port = 8082
server_cert = 'server.crt'
server_key = 'server.key'
client_certs = 'client.crt'

print ('Start the server at', datetime.now())

context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
context.verify_mode = ssl.CERT_REQUIRED # SSLError will be raised if no certificate is provided
context.load_cert_chain(certfile=server_cert, keyfile=server_key)#  SSLError is raised if the private key doesn’t match with the certificate
context.load_verify_locations(cafile=client_certs)

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
#server_address = ('localhost', 12012)
server_address = (listen_addr, listen_port)
print ('starting up on %s port %s' % server_address)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1) # The number of unaccepted connections that the system will allow before refusing new connectionis

while True:
    # Wait for a connection
    print ('Waitng for a client to call')
    connection, client_address = sock.accept()
    try:
        print ('Connection from Client host %s port %s' % client_address)
    
        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(16)
#            print ('received "%s"' % data)
            if data:
#                print ('sending data back to the client')
                connection.sendall(data)
            else:
                print ('no more data from', client_address)
                break
                    
    finally:
        # Clean up the connection
#        print ('close connection')
        connection.close()
