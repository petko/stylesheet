#ifndef _CSS_CSSPARSER_H
#define _CSS_CSSPARSER_H

#include "StyleSheet/Document.h"
#include "StyleSheet/Style.h"

namespace StyleSheet {

class CssParser
{
public:
	CssParser(const CssDocument& doc) : doc_(doc) {}

	CssPropertySet getProperties(const CssStyle& style) const
	{
		CssPropertySet properties;
		// First add inline properties...
		properties.add(style.getInlineProperties());
		// ... then properties by id
		properties.add(doc_.getElement(style.getIdSelector()).getProperties());
		// ... then properties by type
		properties.add(doc_.getElement(style.getTypeSelector()).getProperties());
		// TODO: ... then properties by class
		CssSelectorList classSelectors = style.getClassSelectors();
		for (CssSelectorList::const_iterator it = classSelectors.begin(); it != classSelectors.end(); ++it)
			properties.add(doc_.getElement(*it).getProperties());
		return properties;
	}

private:
	CssDocument doc_;
};

} // namespace StyleSheet

#endif // _CSS_CSSPARSER_H
