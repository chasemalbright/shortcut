from scapy.all import *


def main():
    """Driver function"""
    while True:
        print_menu()
        option = input('Choose a menu option: ')
        if option == '1':
            print("Creating and sending packets ...")
            c = input("Enter the number of packets to send: ")
            i = input("Enter the packet interval: ")
            send_pkt(int(c), int(i))
        elif option == '2':
            print("Listening to all traffic and show all ...")
            capture = sniff(filter="ICMP")
            print_pkt(capture)
        elif option == '3':
            print("Listening to ping command to the address 8.8.4.4 ...")
            ans = sr(IP(dst="8.8.4.4")/ICMP())
            print_pkt(ans)

            # TODO
        elif option == '4':
            print("Listening to telnet command executed from localhost ...")
            ans = sr(IP(dst="LOCALHOST")/ICMP())
            print_pkt(ans)
        elif option == '5':
            print("End")
            break
        else:
            print(f"\nInvalid entry\n")

def send_pkt(number, interval):
    """Send a custom packet"""
    # number = number of packets to send
    # interval = interval in seconds between sending of packets
    # p = scapy.IP(ttl=32)
    # p.src = "192.168.10.4"
    # p.dst = "192.168.6.12"

    send(IP(src="192.168.10.4", dst="192.168.6.12", ttl=32)/ICMP(type="echo-request"), count=number, inter=interval)


    pass

def print_pkt(pkt):
    """ Print Source IP, Destination IP, Protocol, TTL"""
    # TODO
    print(pkt.src)
    print(pkt.dst)
    print(pkt.proto)
    print(pkt.ttl) 
    print(pkt.summary())
    pass


def print_menu():
    """Prints the menu of options"""
    print("*******************Main Menu*******************")
    print('1. Create and send packets')
    print('2. Listen to all traffic and show all')
    print('3. Listen to ping command to the address 8.8.4.4')
    print('4. Listen to telnet command executed from localhost')
    print('5. Quit')
    print('***********************************************\n')


main()
