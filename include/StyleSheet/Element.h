#ifndef _CSS_CSSELEMENT_H
#define _CSS_CSSELEMENT_H

#include "StyleSheet/Selector.h"
#include "StyleSheet/PropertySet.h"
#include <set>

namespace StyleSheet {

class CssElement
{
public:
	CssElement(const CssSelector& selector, const CssPropertySet& properties)
		: selector_(selector), properties_(properties)
	{}
	// static ctor
	static CssElement parse(const std::string& str)
	{
		std::size_t posPropStart = str.find('{');
		std::size_t posPropEnd = str.find('}');

		std::string properties = str.substr(posPropStart + 1, posPropEnd - posPropStart - 1);
		boost::algorithm::trim(properties);

		return CssElement(CssSelector::parse(str.substr(0, posPropStart)), CssPropertySet::parse(properties));
	}

	bool operator<(const CssElement& e) const { return selector_ < e.selector_; }

	const CssSelector& getSelector() const { return selector_; }
	std::size_t getPropertyCount() const { return properties_.size(); }
	const CssPropertySet& getProperties() const { return properties_; }

	void addProperty(const CssProperty& prop) { properties_.add(prop); }

	std::string toString() const {
		return selector_.toString() + " { " + properties_.toString() + "}";
	}

private:
	CssSelector selector_;
	CssPropertySet properties_;
};

} // namespace StyleSheet

#endif // _CSS_CSSELEMENT_H
