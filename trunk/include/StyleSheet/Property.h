#ifndef _CSS_CSSPROPERTY_H
#define _CSS_CSSPROPERTY_H

#include <boost/algorithm/string.hpp>
#include <string>
#include <stdexcept>

namespace StyleSheet {

class CssProperty
{
public:
    CssProperty(const std::string& name, const std::string& value)
            : name_(name), value_(value)
    {}
    // static ctor
    static CssProperty parse(const std::string& str)
    {
        std::size_t pos = str.find(':');
        if (pos == std::string::npos)
            throw std::invalid_argument("value not found");
        if ((pos + 1) == str.size())
            throw std::invalid_argument("value not found");
        std::string name = str.substr(0, pos);
        boost::algorithm::trim(name);

        std::size_t lastPos = str.find(';');
        std::string value = lastPos == std::string::npos ? str.substr(pos + 1) : str.substr(pos + 1, lastPos - pos - 1);
        boost::algorithm::trim(value);
        return CssProperty(name, value);
    }

    bool operator<(const CssProperty& p) const { return name_ < p.name_; }

    const std::string& getName() const { return name_; }
    const std::string& getValue() const { return value_; }

    std::string toString() const { return name_ + ": " + value_ + ";"; }

private:
    std::string name_;
    std::string value_;
};

} // namespace StyleSheet

#endif // _CSS_CSSPROPERTY_H
