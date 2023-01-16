


class AutoClassMeta{};


class MyClass{
	int ageNumber; // 调用一个能让他增加的方法 标记类型
};


template<typename t>
class MockClass{};


template<>
class MockClass<MyClass>: public AutoClassMeta{

	int GetField();

	int GetStaticFieldCOunt();

	int GetMethodCount();

	int GetStaticMethodCount();

	void GetClassName();

};
