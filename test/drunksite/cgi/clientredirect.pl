#!/usr/bin/perl

local ($buffer, @pairs, $pair, $name, $value, %FORM);
# Read in text
$ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;

if ($ENV{'REQUEST_METHOD'} eq "POST") {
   read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
} else {
   $buffer = $ENV{'QUERY_STRING'};
}

# Split information into name/value pairs
@pairs = split(/&/, $buffer);

foreach $pair (@pairs) {
   ($name, $value) = split(/=/, $pair);
   $value =~ tr/+/ /;
   $value =~ s/%(..)/pack("C", hex($1))/eg;
   $FORM{$name} = $value;
}

if( $FORM{like} ) {
   $like_flag ="on";
} else {
   $like_flag ="off";
}

if( $FORM{dontlike} ) {
   $dontlike_flag ="on";
} else {
   $dontlike_flag ="off";
}

if ( $like_flag eq "on" and $dontlike_flag eq "on" ) {
    print ("HTTP/1.1 200 OK\r\n");
    print ("Content-Type: text/html\r\n\r\n");
    print ("<!DOCTYPE html>\n");
    print ("<html>\n");
    print ("<head>\n");
    print ("<title>Coucouuu haan</title>\n");
    print ("</head>\n");
    print ("<body>\n");
    print ("<h2>Please click only one checkbox.</h2>");
    print ("<li><a href=\"/test_cgi.html\">retour</a></li>");
    print ("</body>\n");
    print ("</html>\n");
}
elsif ( $like_flag eq "on" ) {
    print ("HTTP/1.1 302 Found\r\n");
    print ("Location:https://youtu.be/rTgj1HxmUbg\r\n\r\n");
}
elsif ( $dontlike_flag eq "on" ) {
    print ("HTTP/1.1 302 Found\r\n");
    print ("Location:https://youtu.be/E4JY04ccAB4?t=32\r\n\r\n");
}
else {
    print ("HTTP/1.1 200 OK\r\n");
    print ("Content-Type: text/html\r\n\r\n");
    print ("<!DOCTYPE html>\n");
    print ("<html>\n");
    print ("<head>\n");
    print ("<title>Coucouuu haan</title>\n");
    print ("</head>\n");
    print ("<body>\n");
    print ("<h2>Please click to one checkbox at least.</h2>");
    print ("<li><a href=\"/test_cgi.html\">retour</a></li>");
    print ("</body>\n");
    print ("</html>\n");
}
