# webserv
webserv is a 42 project which aims to implement a http/1.1 server

This server is *UNIX* & *LINUX* compliant.

#### Installation
`git clone https://github.com/Jibus22/webserv.git webserv && cd webserv && make`
#### Usage:
`./webserv CONFIGFILE`

### Core
This webserv is implemented with `kqueue()` for unix and `epoll()` for linux.

### Documentation
Please check the wiki of the project [here](https://github.com/Jibus22/webserv/wiki)

### Stress test result
With siege:
