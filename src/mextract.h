
/*
* Assorted macros.
*
* This file is part of MorphExtractor (mextract)
*
* Written and maintained by Stephen Ramsay <sramsay.unl@gmail.com>
*
* Last Modified: Sat Jan 12 19:17:14 CST 2013
*
* Copyright (c) 2013 Stephen Ramsay
*
* MorphExtractor is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option) any
* later version.
*
* MorphExtractor is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License
* along with MorphExtractor; see the file COPYING.  If not see
* <http://www.gnu.org/licenses/>.
*/

#define DEBUG 1

/* Enable debugging string */
#define debug_print(fmt, ...)					       \
	do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

/* Safe version of asprintf.  Slightly modified from Ben Klemens
 * version */
#define Sasprintf(write_to, ...) {				\
	int bytes = 0;						\
	char *tmp_string_for_extend = (write_to);		\
	bytes = asprintf(&(write_to), __VA_ARGS__);		\
	free(tmp_string_for_extend);				\
	if (bytes == -1) {					\
		printf("Fatal: Unable to assign string");	\
		exit(EXIT_FAILURE);				\
	}							\
}
