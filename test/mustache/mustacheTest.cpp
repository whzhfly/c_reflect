#include <stdio.h>
#include <iostream>
#include <string>
#include "mustache.hpp"

using namespace kainjow;


// https://github.com/kainjow/Mustache
// https://github.com/no1msd/mstch


/*
	[1] {{data}}
		data 表示键名
	[2] {{#data}} {{/data}}
		#开始、以/结束表示区块，它会根据当前上下文中的键值来对区块进行一次或多次渲染，例
		类似if for的功能
	[3] {{^data}} {{/data}}
		2 的反例
	[4] {{.}}
		表示枚举，可以循环输出整个数组。例如
	[5] {{>partials}}
		>开始表示子模块，当结构比较复杂时，我们可以使用该语法将复杂的结构拆分成几个小的子模块。
*/

void SimpleTest(){
	kainjow::mustache::mustache tmpl{"Hello {{what}}!"};
	std::cout << tmpl.render({"what", "World"}) << std::endl;
};

void ListTest(){
	kainjow::mustache::mustache tmpl{"{{#employees}}{{name}} {{key}} {{class}}, {{/employees}}"};
	kainjow::mustache::data employees{kainjow::mustache::data::type::list};
	employees << kainjow::mustache::data{"name", "Steve"} << kainjow::mustache::data{"name", "Bill"};
	employees << kainjow::mustache::data{"key", "kk"};
	tmpl.render({"employees", employees}, std::cout);
	std::cout << std::endl;
}

void forTest(){
	kainjow::mustache::mustache tmpl{
		"{{#employees}} sda {{key}} {{class}},  {{#name}} {{childName}}  {{/name}}  {{/employees}}"};
	kainjow::mustache::data employees{kainjow::mustache::data::type::list};

	kainjow::mustache::data tmp;
	tmp.set("key", "Steve");
	tmp.set("key", "Steve2");
	tmp.set("class", "ak");
	employees.push_back(tmp);

	kainjow::mustache::data tmp2;
	tmp2.set("key", "Steve4");
	tmp2.set("key", "Steve3");
	tmp2.set("class", "ak");

	kainjow::mustache::data child;
	child.set("childName", "insight");
	tmp2.set("name", child);

	employees.push_back(tmp2);

	tmpl.render({"employees", employees}, std::cout);
	std::cout << std::endl;
}

void CustomTest(){
	kainjow::mustache::mustache tmpl("Hello {{what}}!");
	std::stringstream ss;
	tmpl.render({"what", "World"}, [&ss](const std::string& str) {
		ss << str;
	});
	// ss.str() == "Hello World!"
}

int main(int argc, char** argv){
	SimpleTest();
	ListTest();
	forTest();
};


