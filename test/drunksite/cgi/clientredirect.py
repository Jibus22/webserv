#!/usr/bin/python

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
like = form.getvalue('like')
dontlike  = form.getvalue('dontlike')


#print 'LoCatIon:/foo/jbcgi.pl/path/info?query=string\r\n\r\n'
#print 'LoCatIon:http://Bim/Bam/Boumm\r\n\r\n'

if like and dontlike:
    print 'HTTP/1.1 200 OK'
    print 'Content-Type: text/html\r\n\r\n'
    print '<!DOCTYPE html>'
    print '<html>'
    print '<head>'
    print '<title>Coucouuu haan</title>'
    print '</head>'
    print '<body>'
    print "<h2>Please click only one checkbox.</h2>"
    print "<li><a href=\"/test_cgi.html\">retour</a></li>"
    print '</body>'
    print '</html>'
elif like :
    print 'HTTP/1.1 302 Found'
    print "Location:https://www.youtube.com/watch?v=dQw4w9WgXcQ\r\n\r\n"
elif dontlike :
    print 'HTTP/1.1 302 Found'
    print "Location:https://youtu.be/E4JY04ccAB4?t=32\r\n\r\n"
else :
    print 'HTTP/1.1 200 OK'
    print 'Content-Type: text/html\r\n\r\n'
    print '<!DOCTYPE html>'
    print '<html>'
    print '<head>'
    print '<title>Coucouuu haan</title>'
    print '</head>'
    print '<body>'
    print "<h2>Please click to one checkbox at least.</h2>"
    print "<li><a href=\"/test_cgi.html\">retour</a></li>"
    print '</body>'
    print '</html>'
