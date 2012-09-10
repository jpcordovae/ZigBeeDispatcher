#ifndef CZBCONFIGURATION_H
#define CZBCONFIGURATION_H

#include "common.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

class CZBConfiguration
{
public:
    CZBConfiguration();
    bool ReadModelsFile(const char *);
    bool SaveModelsFile(const char *);
    bool ReadFile(const char *);
    bool SaveFile(const char *);
private:
    xmlDoc *doc;
    xmlNode *root_element;
    void print_element_names(xmlNode * a_node);
};

#endif // CZBCONFIGURATION_H
