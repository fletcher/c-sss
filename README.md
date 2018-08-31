## About ##

|            |                           |  
| ---------- | ------------------------- |  
| Title:     | c-SSS        |  
| Author:    | Fletcher T. Penney       |  
| Date:      | 2018-08-31 |  
| Copyright: | Copyright © 2015-2018 Fletcher T. Penney.    |  
| Version:   | 0.2.4      |  


## Introduction ##

This is an implementation of [Shamir's Secret Sharing][shamir].  It was
implemented in original c code, based on the javascript example from
Wikipedia, with a couple of utility routines based on code elsewhere
on the internet.

As long as it's compiled from source, and you verify the source, you can
be confident that nothing nefarious is being done with your information.

The idea is that a secret string (e.g. a password, a secret note, etc.)
is encoded and divided up into a specified number of shares (n). You also
specify the threshold number of shares required to be able to reconstruct
the original secret (t).

For example, you can split the secret into three shares, but require only
two of them to recreate the secret.  This means that if one of the shares
is lost, or otherwise unavailable, the secret can still be recovered. One
of the shares, without either of the other two, is useless.

This means that if someone gains access to one, but not two, of the shares,
they cannot recover any information about the information that was
encrypted.


## How is it used? ##

You have a few options:

	./shamir "Some secret to be encrypted" 3 2 > all-keys.txt

This will encrypt the string and generate 3 shares, any 2 of which are required 
to unlock the secret.  They are written to `all-keys.txt`.  This file should
immediately be separated, since all of the keys together can be used to 
decrypt the secret.

	./shamir 3 2 < secret.txt

This pulls the secret from a text file


The easiest way to unlock the information is to place the required keys in a
text file, with each key on a line by itself.  Each key should be at the
beginning of the line.

	./shamir < keys.txt

This reads the keys from `keys.txt` and uses them to decrypt the secret, 
which is then output on stdout.


## The Shares ##

Each share looks like this:

	0102AA05C0DF2BD6

The first two characters indicate the number of the share (in hex). The
next two characters indicate how many shares are required to decrypt the
secret (again in hex).  The next two characters (`AA`), are fake and enable
the shares to also be used on a website:

<http://www.christophedavid.org/w/c/w.php/Calculators/ShamirSecretSharing>

I cannot vouch for the security of this website -- it claims not to send
any of your information over the internet, but that may or may not be true.


## About the Source ##

This project was based on my [c-template] project.  It includes some boilerplate setup to make it easy to incorporate:

* [cmake] for build configuration
* [CuTest] for unit testing
* [doxygen] for creating documentation from source files

[c-template]:	https://github.com/fletcher/c-template
[cmake]:	http://www.cmake.org/
[CuTest]:	http://cutest.sourceforge.net
[doxygen]:	http://www.stack.nl/~dimitri/doxygen/


## Security Issues ##

I am not a cryptologist.  From what I can gather from the internet, Shamir's
algorithm is secure -- without a sufficient number of shares, you cannot
"crack the code."  I *believe* that I have implemented the algorithm correctly,
based on what I can read and the fact that it generates compatible results
with another implementation.

Because the secret is encrypted one character at a time, you can use the 
length of the shares to determine the length of the secret.  Depending on
your needs, you could pad the secret with spaces to obscure the actual
length.

Naturally, you have to protect the shares as you would the password, especially
when you have all of them together.

I do not do anything special with the code to guard against any attacks on 
your machine, such as trying to read memory that was used by the application.
If someone has (reasonable) suggestions, I am happy to implement them.  But 
my intent was not to create something that will guard against motivated
attackers with significant resources.

To the best of my knowledge, this program is reasonably safe to use.  You can
readily view the source to ensure that your information is not being sent over
the internet or saved to other files.  But ultimately, it's your responsibility
to use the software carefully.

If you discover any problems with the program, please let me know!


[shamir]:	http://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing


## Source ##

The source is available online:

<https://github.com/fletcher/c-sss>


## Documentation ##

Full documentation is available online:

<http://fletcher.github.io/c-sss/index.html>


## License ##

The `c-sss` project is released under the MIT License.

GLibFacade.c and GLibFacade.h are from the MultiMarkdown v4 project:

	https://github.com/fletcher/MultiMarkdown-4/

MMD 4 is released under both the MIT License and GPL.


CuTest is released under the zlib/libpng license. See CuTest.c for the text
of the license.


## The MIT License ##

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
