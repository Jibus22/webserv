#!/usr/bin/python

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')


print 'HTTP/1.1 200 OK'
print 'Content-Type: text/html\r\n\r\n'
#print 'LoCatIon:/foo/jbcgi.pl/path/info?query=string\r\n\r\n'
#print 'LoCatIon:http://Bim/Bam/Boumm\r\n\r\n'

#print '<!DOCTYPE html>'
print '<html>'
print '<head>'
print '<title>Coucouuu haan</title>'
print '</head>'
print '<body>'
print '<h2>Hello you... Do you wonder what are your name & last name ? Check it out below</h2>'
print "<h2>Your name is %s %s. Try to remember</h2>" % (first_name, last_name)
print '</body>'
print '</html>'
