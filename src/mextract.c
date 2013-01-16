/*
* A tool for extracting data from Morphadorner files.
*
* Written and maintained by Stephen Ramsay <sramsay.unl@gmail.com>
*
* Last Modified: Wed Jan 16 10:19:16 CST 2013
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

#include "mextract.h"
#include <config.h>
#include <getopt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdio.h>

void xpath_builder(char **xpath_string, char *format_string, char *optarg, int
		attr_count);
xmlDocPtr get_doc(const char *docname);
xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *xpath);
void version(void);
void help(void);

int main(int argc, char **argv)
{
	static struct option long_options[] = {
		{"eos",      no_argument,       NULL, 'e'},
		{"help",     no_argument,       NULL, 'h'},
		{"lemma",    no_argument,       NULL, 'l'},
		{"pos",      required_argument, 0,    'p'},
		{"regular",  no_argument,       NULL, 'r'},
		{"spelling", no_argument,       NULL, 's'},
		{"version",  no_argument,       NULL, 'V'},
		{0, 0, 0, 0}
	};

	int opt = 0;
	int option_index = 0;
	int switch_count = 0;
	int attr_count   = 0;
	char *xpath_string = NULL;
	Sasprintf(xpath_string, "//*[local-name()='w']");
	xmlChar *content_switch = NULL;

	while ((opt = getopt_long(argc, argv, "ehlp:rsV", long_options,
				       	&option_index)) != -1) {
		if (opt == -1)
	       		break;
		switch (opt) {
		case 'e':
			xpath_builder(&xpath_string, "eos", "1", attr_count);
			attr_count++;
			break;
		case 'h':
			help();
			exit(EXIT_SUCCESS);
		case 'l':
			content_switch = (xmlChar *) "lem";
			switch_count++;
			break;
		case 'p':
			xpath_builder(&xpath_string, "pos", optarg,
					attr_count);
			attr_count++;
			break;
		case 'r':
			content_switch = (xmlChar *) "reg";
			switch_count++;
			break;
		case 's':
			content_switch = (xmlChar *) "spe";
			switch_count++;
			break;
		case 'V':
			version();
			exit(EXIT_SUCCESS);
		default:
			exit(EXIT_FAILURE);
		}

		if (switch_count > 1) {
			fputs(
	"One of --lemma, --spelling, --regular permitted.\n", stdout);
			exit(EXIT_FAILURE);
		}

	}

	const xmlChar *xpath = NULL;

	if (attr_count > 0) {
		Sasprintf(xpath_string, "%s%s", xpath_string, "]");
		xpath = (xmlChar *) xpath_string;
	} else {
		xpath = (xmlChar *) xpath_string;
	}


	xmlDocPtr doc = NULL;
	const char *file_argument = *(argv + optind);

	if (!file_argument) {
		doc = get_doc("-");
	} else {
	       	doc = get_doc(file_argument);
	}

	const xmlXPathObjectPtr result = get_nodeset(doc, xpath);
	xmlChar *attrib = NULL;

	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {
			if (content_switch) {	
				attrib = xmlGetProp(nodeset->nodeTab[i],
					       	content_switch);
			} else {
				attrib = xmlGetProp(nodeset->nodeTab[i],
					       	(xmlChar *) "tok");
			}
			printf("%s\n", attrib);
			xmlFree(attrib);
		}
		xmlXPathFreeObject(result);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
	debug_print("%s\n", xpath);
	free(xpath_string);
	return (1);
}

/*
 * Some switches (lemma, special, regular) are selected directly 
 * from nodesets.  Others (eos, pos) are selected using an XPath expression.
 * This function constructs the necessary XPath.
 */
void xpath_builder(char **xpath_string, char *cl_switch, char *optarg,
	       	int attr_count)
{
	if (attr_count == 0) {
		Sasprintf(*xpath_string, "%s[@%s='%s'", *xpath_string,
				cl_switch, optarg);
	} else {
		Sasprintf(*xpath_string, "%s and @%s='%s'", *xpath_string,
				cl_switch, optarg);
	}
}


xmlDocPtr get_doc(const char *docname)
{
	xmlDocPtr doc = xmlParseFile(docname);

	if (doc == NULL) {
		fprintf(stderr, "Unable to parse document. \n");
		return NULL;
	}

	return doc;
}


xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *xpath)
{
	xmlXPathContextPtr context = xmlXPathNewContext(doc);

	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}

	xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);

	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}

	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;

	}

	return result;
}

/* 
 * Version info for the --version switch
 */
void version(void)
{
	printf("%s\n", PACKAGE_STRING);
	fputs("Copyright © 2013 by Stephen Ramsay", stdout);
	fputs("\
\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
\n\
", stdout);
}

/*
 * Help info for the --help switch
 */
void help(void)
{
	printf("Usage: %s [OPTIONS]... [FILE]\n", PACKAGE);
	fputs("\n\
Content Options (one of):\n\
   -l, --lemma		The lemma head word form(s) of the token.\n\
   -r, --regular	A standardized version of the spelling.\n\
   -s, --spelling	The spelling (without specialized meta-characters.\n\n\
XPath specifiers:\n\
   -p, --pos		Part of speech (NUPOS).\n\
   -e, --eos		At end of sentence.\n\n\
", stdout);

   	printf("By default, %s will return all tokens (i.e. //w/@tok)\n\n",
			PACKAGE);

	printf("Report bugs to: %s\n", PACKAGE_BUGREPORT);
}
