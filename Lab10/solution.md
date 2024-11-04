## Questions

1. Screenshot of your Wireshark application.

2. Display filter output of different protocols(screenshot the entire window). Use “dns”, "udp", "tcp", "icmp" etc. as filter.

- DNS : `dns`
- UDP : `udp`
- TCP : `tcp`
- ICMP : `icmp`

3. Capture filter output of a specific port (screenshot the entire window). Use “port 53” as filter.

- `udp.port == 53`

4. Capture filter output of a specific source IP and destination IP addresses (screenshot the entire window).

- `ip.src == 127.0.0.1 and ip.dst == 127.0.0.1`
