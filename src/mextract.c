/*
* A tool for extracting data from Morphadorner files.
*
* Written and maintained by Stephen Ramsay <sramsay.unl@gmail.com>
*
* Last Modified: Tue Jan 15 15:35:04 CST 2013

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

void xpath_builder(char **xpath_string, char *format_string, char *optarg);
xmlDocPtr get_doc(char *docname);
xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, xmlChar * xpath);
void version(void);

int main(int argc, char **argv)
{
	static struct option long_options[] = {
		{"lem",     no_argument,       NULL, 'l'},
		{"pos",     required_argument, 0,    'p'},
		{"version", no_argument,       NULL, 'V'},
		{0, 0, 0, 0}
	};

	int opt = 0;
	int option_index = 0;
	int switch_count = 0;
	char *xpath_string = NULL;
	Sasprintf(xpath_string, "//w");
	xmlChar *content_switch = NULL;

	while ((opt = getopt_long(argc, argv, "lp:", long_options,
				       	&option_index)) != -1) {
		if (opt == -1)
	       		break;

		switch (opt) {
		case 'l':
			content_switch = (xmlChar *) "lem";
			switch_count++;
			break;
		case 'p':
			Sasprintf(xpath_string, "%s[@pos='%s']",
				       	xpath_string, optarg);
			break;
		case 'V':
			version();
			exit(EXIT_SUCCESS);
		default:
			exit(EXIT_FAILURE);
		}

		if (switch_count > 1)
			fputs(
	"One of --lem, --spe, --tok, --reg permitted.",
	stdout);

	}
	
	xmlDocPtr doc = NULL;
	char *file_argument = *(argv + optind);

	xmlChar *xpath = (xmlChar *) xpath_string;

	if (!file_argument) {
		doc = get_doc("-");
	} else {
	       	doc = get_doc(file_argument);
	}

	xmlXPathObjectPtr result = get_nodeset(doc, xpath);

	xmlChar *attrib = NULL;

	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {
			 //word = xmlNodeListGetString(doc, 
			 //	nodeset->nodeTab[i]->xmlChildrenNode); 
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
	debug_print("%s\n", xpath_string);
	free(xpath_string);
	return (1);
}

/*
 * Most of the command line switches provide information
 * necessary for constructing the appropriate XPath expression.
 * This function does the conversion.
 *
 * Note that libxml does *not* understand XPath 2.0.
 */
//void xpath_builder(char **xpath_string, char *cl_switch, char *optarg)
//{
//	if (*xpath_string == NULL) {
//	       	Sasprintf(*xpath_string, "//w");
//	} else {
//		Sasprintf(*xpath_string, "%s and", *xpath_string);
//       	}
//
//	if (! STREQ("lem", cl_switch)) {
//
//		Sasprintf(*xpath_string, "@%s
//	}
//
//
//
//
//	
//	// string(//div[@class='known']/@name)
//	if (*xpath_string == NULL) {
//		if (optarg == NULL) {
//        		Sasprintf(*xpath_string, "%s='%s'", *xpath_string,
//					"text()");
//		} else {
//        		Sasprintf(*xpath_string, "%s='%s'", *xpath_string,
//				       	optarg);
//		}
//        } else {
//		if (optarg == NULL) {
//        		Sasprintf(*xpath_string, "%s'%s'", *xpath_string,
//					"text()");
//		} else {
//        		Sasprintf(*xpath_string, "%s'%s'", *xpath_string,
//					optarg);
//		}
//	}
//}


xmlDocPtr get_doc(char *docname)
{
	xmlDocPtr doc = xmlParseFile(docname);

	if (doc == NULL) {
		fprintf(stderr, "Unable to parse document. \n");
		return NULL;
	}

	return doc;
}


xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, xmlChar * xpath)
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
