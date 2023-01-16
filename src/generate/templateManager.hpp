#include <iostream>
#include <string>
#include "../3rd/mustache/mustache.hpp"

namespace Mustache = kainjow::mustache;

extern class P_Class;

/*
	管理模板
*/

class TemplateManager{


public:
	virtual void genClassRenderData(std::shared_ptr<P_Class> class_temp, Mustache::data& class_def);
};
