#ifndef _CSS_CSSSELECTOR_H
#define _CSS_CSSSELECTOR_H

#include <boost/algorithm/string.hpp>
#include <string>
#include <stdexcept>

namespace StyleSheet {

class CssSelector
{
public:
    // static ctors
    static CssSelector CssTypeSelector(const std::string& name)
    {
        if (name.empty())
            throw std::invalid_argument("empty CSS selector");
        return CssSelector(SELECTOR_TYPE, name);
    }
    static CssSelector CssClassSelector(const std::string& name)
    {
        if (name.empty())
            throw std::invalid_argument("empty CSS selector");
        return CssSelector(SELECTOR_CLASS, name);
    }
    static CssSelector CssIdSelector(const std::string& name)
    {
        if (name.empty())
            throw std::invalid_argument("empty CSS selector");
        return CssSelector(SELECTOR_ID, name);
    }
    static CssSelector parse(const std::string& str)
    {
        std::string selector = str;
        boost::algorithm::trim(selector);

        if (selector.empty())
            throw std::invalid_argument("empty CSS selector");

        if (selector[0] == '.')
            return CssSelector(SELECTOR_CLASS, selector.substr(1));

        if (selector[0] == '#')
            return CssSelector(SELECTOR_ID, selector.substr(1));
		
        return CssSelector(SELECTOR_TYPE, selector);
    }

public:
    bool operator==(const CssSelector& other) const { return type_ == other.type_ && name_ == other.name_; }
    bool operator!=(const CssSelector& other) const { return !(*this == other); }

    bool operator<(const CssSelector& s) const {
        return type_ < s.type_ || (type_ == s.type_ && name_ < s.name_);
    }

    const std::string getName() const { return name_; }

    bool isType() const { return type_ == SELECTOR_TYPE; }
    bool isClass() const { return type_ == SELECTOR_CLASS; }
    bool isId() const { return type_ == SELECTOR_ID; }

    std::string toString() const { return typeToString() + name_; }

private:
    enum Type {
        //SELECTOR_BAD,
        SELECTOR_TYPE,
        SELECTOR_CLASS,
        SELECTOR_ID
    };

private:
    CssSelector(Type type, const std::string& name)
        : type_(type), name_(name)
    {}

private:
    std::string typeToString() const
    {
        switch (type_)
        {
        case SELECTOR_TYPE: return "";
        case SELECTOR_CLASS: return ".";
        case SELECTOR_ID: return "#";
        default:
            throw std::runtime_error("Bad CssSelector type");
        }
    }

private:
    Type type_;
    std::string name_;
};

} // namespace StyleSheet

#endif // _CSS_CSSSELECTOR_H
