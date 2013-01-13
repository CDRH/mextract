/*
* A tool for extracting data from Morphadorner files.
*
* Written and maintained by Stephen Ramsay <sramsay.unl@gmail.com>
*
* Last Modified: Sat Jan 12 19:05:40 CST 2013

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
#include <getopt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdio.h>

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

int main(int argc, char **argv)
{
	if (argc <= 1 && stdin == NULL) {
		printf("Usage: %s [file.xml]\n", argv[0]);
		return (0);
	}
	
	static struct option long_options[] = {
		{"lem",  required_argument, 0, 'l'},
		{"pos",  required_argument, 0, 'p'},
		{0, 0, 0, 0}
	};

	int opt = 0;
	int option_index = 0;
	char *xpath_string = NULL;

	while ((opt = getopt_long(argc, argv, "p:", long_options,
				       	&option_index)) != -1) {
		if (opt == -1)
	       		break;

		switch (opt) {
		case 'l':
			if (xpath_string == NULL) {
				Sasprintf(xpath_string, "//w[@lem='%s'",
					       	optarg);
			} else {
				Sasprintf(xpath_string, "%s and @lem='%s'",
						xpath_string, optarg);
			}
			break;
		case 'p':
			if (xpath_string == NULL) {
				Sasprintf(xpath_string, "//w[@pos='%s'",
					       	optarg);
			} else {
				Sasprintf(xpath_string, "%s and @pos='%s'",
						xpath_string, optarg);
			}
			break;
		default:
			exit(EXIT_FAILURE);
		}
			

	}

	//xmlChar *xpath = (xmlChar*) "//w[starts-with(@pos, 'n')
	
	xmlChar *xpath;
	xmlDocPtr doc = NULL;
	char *file_argument = *(argv + optind);


	if (optind == 1) {
		xpath = (xmlChar *) "//w";
	} else {
		Sasprintf(xpath_string, "%s]", xpath_string);
		xpath = (xmlChar *) xpath_string;
	}

	if (!file_argument) {
		doc = get_doc("-");
	} else {
	       	doc = get_doc(file_argument);
	}


	xmlXPathObjectPtr result = get_nodeset(doc, xpath);

	xmlChar *token;

	if (result) {
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (int i = 0; i < nodeset->nodeNr; i++) {
			token =
			    xmlNodeListGetString(doc,
						 nodeset->nodeTab[i]->
						 xmlChildrenNode, 1);
			printf("%s\n", token);
			xmlFree(token);
		}
		xmlXPathFreeObject(result);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
	debug_print("%s\n", xpath_string);
	free(xpath_string);
	return (1);
}