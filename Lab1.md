# 1. Find the IP address of your computer. Try connecting to different service providers and notice the changes, if any, in the IP address of your machine.

`ipconfig`
`iconfig`
`ip a`

# 2. How to send exactly 4 packets of size 100 bytes to www.github.com?

`ping www.github.com -n 4 -l 100`
`ping www.github.com -c 4 -s 100`

# 3. Run traceroute via two or more service providers for www.kiit.ac.in and report your observations, like if some paths default to IPv6 then how can you force traceroute to use IPv4, any private IP addresses routers that do not reply to requests, etc.

`tracert www.kiit.ac.in`
`traceroute www.kiit.ac.in`

Force use IPv4
`tracert -4 www.kiit.ac.in`
`traceroute -4 www.kiit.ac.in`

# 4. How can you limit the number of hops to 10 in a traceroute command?

`tracert -h 10 www.example.com`
`traceroute -m 10 www.example.com`

# 5. How can you display statistics for all protocols using the netstat command?

`netstat -s`

# 6. Use nslookup to find the IP Address of www.kiit.ac.in and www.facebook.com

`nslookup www.kiit.ac.in`
`nslookup www.facebook.com`

# 7. How can you perform a reverse DNS lookup to find the domain name associated with the IP address 8.8.8.8?

`nslookup 8.8.8.8`

# 8. How can you use nslookup to query www.example.com using the DNS server at 8.8.8.8?

`nslookup www.example.com 8.8.8.8`

# 9. How do you use the ipconfig command to display all current TCP/IP network configurations?

`ipconfig /all`
`ifconfig`
`ip a`

# 10. What are the ipconfig commands to release and renew an IP address on a Windows machine?

    `ipconfig /release`
    `ipconfig /renew`

# 11. How do you use iptables to add a rule that allows all incoming HTTP traffic (port 80)?

    `sudo iptables -A INPUT -p tcp --dport 80 -j ACCEPT`

# 12. How do you use iptables to block all incoming traffic from the IP address 192.168.1.100?

`sudo iptables -A INPUT -s `
