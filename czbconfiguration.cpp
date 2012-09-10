#include "czbconfiguration.h"

CZBConfiguration::CZBConfiguration()
{
  doc = NULL;
  root_element = NULL;

  //LIBXML_TEST_VERSION

  if(!ReadModelsFile("/etc/domoyic/models.xml"))
    {
      std::cerr << "cannot read models file /etc/domotoc/models.xml" << std::endl;
    }

}

void CZBConfiguration::print_element_names(xmlNode * a_node)
{
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
  {
    if (cur_node->type == XML_ELEMENT_NODE) {
			printf("node type: Element, name: %s\n", cur_node->name);
		}

		print_element_names(cur_node->children);
	}
}


bool CZBConfiguration::ReadModelsFile(const char *_file)
{
	std::cout << "parsing models file " << std::string(_file) << std::endl;
	doc = xmlParseFile(_file);
	if(doc==NULL)
	{
		//std::cerr << "XML file " << std::string(_file) << " empty" << std::endl;
		return false;
	}
	root_element = xmlDocGetRootElement(doc);

	for( xmlNode *level_1 = root_element->children ; level_1 != NULL ; level_1 = level_1->next)
	{
		// end_devices_models level
		if(!xmlStrcmp(level_1->name,(const char *)"end_devices_model"))
			for( xmlNode *level_2 = level_1->children ; level_2 != NULL; level_2 = level_2->next)
			{
				// model level
				if(!xmlStrcmp(level_2->name,(const char *)"model"))
					for( xmlNode *level_3 = level2->children; level_3 != NULL ; level_3 = level_3->next )
					{
						// channel level
					}

				if(!xmlStrcmp(level_2->name,(const char *)"models_version"))
				{
				
				}



			}
	}

	return true;	
}

bool CZBConfiguration::SaveModelsFile(const char *)
{
  return true;
}

bool CZBConfiguration::ReadFile(const char *)
{
  return true;
}

bool CZBConfiguration::SaveFile(const char *)
{
  return true;
}
