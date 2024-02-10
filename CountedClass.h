#pragma once
template<typename T>
class CountedClass
{
	int count = 0;
protected:
	CountedClass() { ++count; }
	~CountedClass() { --count; }

};
