#include "fakengine.h"
#include "fstringapp.h"

bool fstringapp::ini( int argc, char *argv[] )
{
	return true;
}

bool fstringapp::heartbeat()
{
	stringc a;
	a+=124;
	stringc b(a.c_str(), a.size());
	
	stringc c;
	c = b;
	
	bool bc = (b == c);

	stringc d("abcdef");
	c += d;

	bc = (b == c);
	bool bcc = (b < c);
	bcc = false;

	int c2 = c[2];
	c2 = 0;

	stringc e = c.make_upper();

	bool ec = e.equals_ignore_case(c);
	ec = e.equalsn(c, 100);

	e.append(57);

	stringc f("ab");
	f.append("asfasfagf", 4);

	int ff = f.findFirstCharNotInList("b");
	ff = f.findLastCharNotInList("b");

	ff = f.findLast('b', 100);
	
	ff = f.find("ba", 1);

	stringc g = f.subString(1,2);
	g.replace('a','c');

	f.replace(stringc("a"),c);
	
	f.replace(stringc("12"),stringc("22"));
	
	f.replace(stringc("24"),stringc("2"));

	f.removeChars(stringc("22"));

	f += "aa ";
	f.trim();
	
	f.erase(1);
	f[0] = 0;
	f.validate();

	f="abc";
	farray<stringc,100> fv;
	f.split(fv,(uint8_t*)"b");

	f = (f + "aaa").c_str();
	
	return true;
}

bool fstringapp::exit()
{
	return true;
}

fstringapp::fstringapp() : mainapp("fstringapp")
{

}
