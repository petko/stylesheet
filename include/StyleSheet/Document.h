#ifndef _CSS_CSSDOCUMENT_H
#define _CSS_CSSDOCUMENT_H

#include "StyleSheet/Element.h"
#include <boost/tokenizer.hpp>

namespace StyleSheet {

class CssDocument
{
public:
	CssDocument() {}
	// static ctor
	static CssDocument parse(const std::string& doc)
	{
		CssDocument document;
		boost::char_separator<char> sep("\n");
		boost::tokenizer<boost::char_separator<char> > tok(doc, sep);
		for (boost::tokenizer<boost::char_separator<char> >::iterator it = tok.begin(); it != tok.end(); ++it)
		{
			document.addElement(CssElement::parse(*it));
		}
		return document;
	}

	std::size_t getElementCount() const { return elements_.size(); }

	CssElement getElement(const CssSelector& selector) const
  {
		CssElementSet::const_iterator it = std::find_if(elements_.begin(), elements_.end(), SelectorFinder(selector));
		if (it == elements_.end())
			return CssElement(selector, CssPropertySet());
		return *it;
	}

	bool hasSelector(const CssSelector& selector) const {
		return std::find_if(elements_.begin(), elements_.end(), SelectorFinder(selector)) != elements_.end();
	}

	void addElement(const CssElement& element)
	{
		if (element.getPropertyCount() == 0)
			return;
		elements_.insert(element);
	}

	std::string toString() const
	{
		std::string res = "";
		for (CssElementSet::const_iterator it = elements_.begin(); it != elements_.end(); ++it)
		{
			res += it->toString() + "\n";
		}
		return res;
	}

private:
	typedef std::set<CssElement> CssElementSet;
	CssElementSet elements_;

private:
	class SelectorFinder
	{
	public:
		SelectorFinder(const CssSelector& selector)
			: selector_(selector)
		{}
		SelectorFinder(const SelectorFinder& other)
			: selector_(other.selector_)
		{}
		SelectorFinder& operator=(const SelectorFinder&);	// not implemented

		bool operator()(const CssElement& element) const {
			return element.getSelector() == selector_;
		}

	private:
		const CssSelector& selector_;
	};
};

} // namespace StyleSheet

#endif // _CSS_CSSDOCUMENT_H
