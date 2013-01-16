
MorphExtractor
==============

Version 0.9.0-alpha

*This is pre-release software.*  It may fail to compile.  It might have undocumented features.  It certainly has unimplemented features.  It definitely has bugs.  You have been warned.  Watch this space for updates.

Bug reports may be filed in the [Issues](https://github.com/CDRH/mextract/issues) section of the [GitHub repository](https://github.com/CDRH/mextract).

MorphExtractor is a command-line tool for extracting tokens from [MorphAdorner](http://morphadorner.northwestern.edu/) files.  It is particularly useful in the context of a UNIX pipeline.

The data that can be extracted corresponds roughly to the attributes on the `<w>` tag in an adorned file.  For details on the meaning of these attributes and how they are derived, please consult the [documentation](http://morphadorner.northwestern.edu/morphadorner/documentation/) for MorphAdorner itself.

Downloading
-----------

The latest version of MorphExtractor is available [here](http://abbot.unl.edu/downloads/)

Usage:
------

mextract [OPTIONS]... [FILE]

Content Options (one of):
   -l, --lemma          The lemma head word form(s) of the token.
   -r, --regular        A standardized version of the spelling.
   -s, --spelling       The spelling (without specialized meta-characters.

XPath specifiers:
   -p, --pos            Part of speech (NUPOS).
   -e, --eos            At end of sentence.

By default, `mextract` will return all tokens (i.e. //w/@tok).

Compiling and Installing MorphExtractor
---------------------------------------

MorphExtractor uses [libxml2](http://xmlsoft.org/) and [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config) -- both of which are widely available through package managers for UNIX-like systems.

To compile and install:
	tar -xvzf mextract.tar.gz
	cd mextract
	./configure
	make
	make install
 
Copying
-------

MorphExtractor is written and maintained by [Stephen Ramsay](http://stephenramsay.us/).

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).

