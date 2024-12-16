## Todo
- Try sending files through sockets on localhost 127.0.0.1
- Verify validity through SHA checksum
- Implement working programs on 2 computers(do not forget to change ip_addr to 192.168.1.32(its set as static))
### About
2 Computers in the same LAN(so no need to worry about NAT) can send or receive data from eachother for quick and easy file transfer. 

It will work like this:

- PC A will start 'client.cpp' and send data
- PC B will start 'server.cpp' and receive data

It could be both, but in order for client to reach server.cpp everytime, I needed to assign a static IP, netmask and default gateway to client, and I do not want to do this on 2 computers, for fears that there are multiple PC's in the LAN that have DHCP enabled and i do not know if there could be duplicate IP, but I am quite certain, messages as Neighbor Solicitacion will prevent that.
