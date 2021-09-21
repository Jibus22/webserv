#!/usr/bin/python

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')


print 'HTTP/1.1 200'
print 'Content-Type: text/html\r\n\r\n'
#print 'LoCatIon:/foo/jbcgi.pl/path/info?query=string\r\n\r\n'
#print 'LoCatIon:http://Bim/Bam/Boumm\r\n\r\n'

print '<!DOCTYPE html>'
print '<html>'
print '<head>'
print '<title>Hello World - First CGI Program</title>'
print '</head>'
print '<body>'
print '<h2>Hello World! This is my first CGI program</h2>'
print "<h2>Hello %s %s</h2>" % (first_name, last_name)
print '</body>'
print '</html>'
