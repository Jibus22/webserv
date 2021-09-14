#!/usr/bin/python

import os

print 'Content-type:text/html\r\n\r\n'
print '<html>'
print '<head>'
print '<title>Hello World - First CGI Program</title>'
print '</head>'
print '<body>'
print '<h2>Hello World! This is my first CGI program</h2>'
print "<font size=+1>Environment</font><br>"
for param in os.environ.keys():
   print "<b>%20s</b>: %s<br>" % (param, os.environ[param])
print '</body>'
print '</html>'
