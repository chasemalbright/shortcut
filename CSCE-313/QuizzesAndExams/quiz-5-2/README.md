# Quiz 5.2: Packet sniffing using Scapy

Scapy is a Python-based interactive packet manipulation program and library. It can forge or decode packets of a wide number of protocols, send them on the wire, capture them, store or read them using pcap files, match requests and replies, and much more. It is designed to allow fast packet prototyping by using default values that work.

When we sniff packets, we are only interested specific packets. We can do that by setting filters in sniffing. 
Write a Python program that send packets or listen to traffic according to some filters.


## Options to implement
### 1. Create and send packets
Create and send the following multilayer packet using the function `send_pkt()`.

The `send_pkt()` function takes as input:
- The number of packets to be sent
- The number of seconds between each packet.

**Construct the packet as follow:**
#### IP
- Source address: `192.168.10.4`
- Destination address: `192.168.6.12`
- Protocol: `ICMP`
- TTL: 32

#### ICMP
- ICMP type: `echo-reply`

### 2. Listen to all traffic and show all
- Sniff all traffic on your interface
- Show a developed view of the packets sniffed

### 3. Listen to ping command to the address `8.8.4.4`
- Sniff all packets from a `ping` command to the address `8.8.4.4`
- Use the function `print_pkt()` to print a information about the packet sniffed

### 4. Listen to telnet command executed from `localhost`
- Sniff all packets from a telnet command executed from localhost
- Use the function `print_pkt()` to print a information about the packet sniffed


## Important notes:
1. For testing, you can run your programs inside docker containers. (Refer to the setup in PA5)
1. Add your interface ID in the sniff function. (Use ifconfig to find you interface ID)
1. Usage: `Usage: python3 sniffer.py`
1. Your program will be manually tested for correctness with additional test cases.
1. Your program should execute with no errors and warnings.