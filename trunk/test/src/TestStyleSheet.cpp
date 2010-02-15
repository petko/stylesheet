#include <StyleSheet/Parser.h>
#include <gtest/gtest.h>

using namespace StyleSheet;

TEST(TestStyleSheet, testTypeSelector)
{
	std::string selectorStr = "type";
	CssSelector selector = CssSelector::parse(selectorStr);
	ASSERT_TRUE(selector.isType());
	ASSERT_FALSE(selector.isClass());
	ASSERT_FALSE(selector.isId());
	ASSERT_EQ(std::string("type"), selector.getName());
	ASSERT_EQ(std::string("type"), selector.toString());
}

TEST(TestStyleSheet, testClassSelector)
{
	std::string selectorStr = ".class";
	CssSelector selector = CssSelector::parse(selectorStr);
	ASSERT_FALSE(selector.isType());
	ASSERT_TRUE(selector.isClass());
	ASSERT_FALSE(selector.isId());
	ASSERT_EQ(std::string("class"), selector.getName());
	ASSERT_EQ(std::string(".class"), selector.toString());
}

TEST(TestStyleSheet, testIdSelector)
{
	std::string selectorStr = "#main";
	CssSelector selector = CssSelector::parse(selectorStr);
	ASSERT_FALSE(selector.isType());
	ASSERT_FALSE(selector.isClass());
	ASSERT_TRUE(selector.isId());
	ASSERT_EQ(std::string("main"), selector.getName());

	ASSERT_EQ(std::string("#main"), selector.toString());
}

TEST(TestStyleSheet, testEmptySelector)
{
	std::string sel = "";
	ASSERT_THROW(CssSelector::parse(sel), std::invalid_argument);

	ASSERT_THROW(CssSelector::CssTypeSelector(sel), std::invalid_argument);
	ASSERT_THROW(CssSelector::CssIdSelector(sel), std::invalid_argument);
	ASSERT_THROW(CssSelector::CssClassSelector(sel), std::invalid_argument);
}

TEST(TestStyleSheet, testSelectorSorting)
{
	CssSelector selector1 = CssSelector::parse("main1");
	CssSelector selector2 = CssSelector::parse(".main1");
	CssSelector selector3 = CssSelector::parse(".main2");
	CssSelector selector4 = CssSelector::parse("#main2");
	

	ASSERT_TRUE(selector1 < selector2);
	ASSERT_FALSE(selector2 < selector1);
	ASSERT_TRUE(selector2 < selector3);
	ASSERT_FALSE(selector3 < selector2);
	ASSERT_TRUE(selector3 < selector4);
	ASSERT_FALSE(selector4 < selector3);

	CssSelector selector5 = CssSelector::parse("type_selector");
	CssSelector selector6 = CssSelector::parse(".class_selector");

	ASSERT_TRUE(selector5 < selector6);
	ASSERT_FALSE(selector6 < selector5);
}

TEST(TestStyleSheet, testSelectorEquality)
{
	CssSelector selector1 = CssSelector::parse("main1");
	CssSelector selector2 = CssSelector::parse(".main1");
	CssSelector selector3 = CssSelector::parse(".main1");
	CssSelector selector4 = CssSelector::parse("#main1");

	ASSERT_TRUE(selector1 != selector2);
	ASSERT_TRUE(selector2 == selector3);
	ASSERT_TRUE(selector3 != selector4);
}

TEST(TestStyleSheet, testProperty)
{
	std::string propertyStr = " property1 : val1 ; ";
	CssProperty prop = CssProperty::parse(propertyStr);
	ASSERT_EQ(std::string("property1"), prop.getName());
	ASSERT_EQ(std::string("val1"), prop.getValue());

	ASSERT_EQ(std::string("property1: val1;"), prop.toString());
}

TEST(TestStyleSheet, testPropertySorting)
{
	CssProperty prop1 = CssProperty::parse("prop1: val1;");
	CssProperty prop2 = CssProperty::parse("prop2: val1;");

	ASSERT_TRUE(prop1 < prop2);
	ASSERT_FALSE(prop2 < prop1);
}

TEST(TestStyleSheet, testPropertySet)
{
	std::string propertiesStr = "   property1: val1;   property2: val2, val3;   ";
	CssPropertySet propertySet = CssPropertySet::parse(propertiesStr);

	ASSERT_FALSE(propertySet.empty());
	ASSERT_EQ(std::size_t(2), propertySet.size());
}

TEST(TestStyleSheet, testElement)
{
	std::string elementStr = " type_selector  {  property1: val1;  property2: val2, val3;  } ";

	CssElement element = CssElement::parse(elementStr);
	ASSERT_TRUE(element.getSelector().isType());
	ASSERT_FALSE(element.getSelector().isClass());
	ASSERT_FALSE(element.getSelector().isId());

	ASSERT_EQ(std::size_t(2), element.getPropertyCount());
	ASSERT_EQ(std::string("type_selector { property1: val1; property2: val2, val3; }"), element.toString());
}

TEST(TestStyleSheet, testElementSorting)
{
	CssElement element1 = CssElement::parse("selector1 { property1: val1; }");
	CssElement element2 = CssElement::parse("selector2 { property1: val1; }");

	ASSERT_TRUE(element1 < element2);
	ASSERT_FALSE(element2 < element1);

	CssElement element3 = CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3;  } ");
	CssElement element4 = CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }");

	ASSERT_TRUE(element3 < element4);
	ASSERT_TRUE(!(element4 < element3));
}

TEST(TestStyleSheet, testAddEmptyProperty)
{
	CssElement element(CssSelector::CssClassSelector("main"), CssPropertySet());

	element.addProperty(CssProperty("prop_name", ""));

	ASSERT_EQ(std::size_t(0), element.getPropertyCount());
}

TEST(TestStyleSheet, testDocument)
{
	CssDocument doc;
	doc.addElement(CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3;  } "));
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));
	// Try to add a duplicate
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));

	ASSERT_EQ(std::size_t(3), doc.getElementCount());
}

TEST(TestStyleSheet, testQueryDocument)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3;  } "));
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));

	ASSERT_FALSE(doc.hasSelector(CssSelector::CssTypeSelector("type1")));
	ASSERT_TRUE(doc.hasSelector(CssSelector::CssTypeSelector("type_selector")));
	ASSERT_FALSE(doc.hasSelector(CssSelector::CssTypeSelector("class_selector")));

	ASSERT_FALSE(doc.hasSelector(CssSelector::CssClassSelector("class1")));
	ASSERT_TRUE(doc.hasSelector(CssSelector::CssClassSelector("class_selector")));
	ASSERT_FALSE(doc.hasSelector(CssSelector::CssClassSelector("id_selector")));

	ASSERT_FALSE(doc.hasSelector(CssSelector::CssIdSelector("id1")));
	ASSERT_TRUE(doc.hasSelector(CssSelector::CssIdSelector("id_selector")));
	ASSERT_FALSE(doc.hasSelector(CssSelector::CssIdSelector("type_selector")));
}

TEST(TestStyleSheet, testGetElement)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3; property3: val3; } "));
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));

	CssElement cssElement = doc.getElement(CssSelector::CssTypeSelector("type1"));
	ASSERT_EQ(std::size_t(0), cssElement.getPropertyCount());

	cssElement = doc.getElement(CssSelector::CssTypeSelector("type_selector"));
	ASSERT_EQ(std::size_t(3), cssElement.getPropertyCount());

	cssElement = doc.getElement(CssSelector::CssClassSelector("class1"));
	ASSERT_EQ(std::size_t(0), cssElement.getPropertyCount());

	cssElement = doc.getElement(CssSelector::CssClassSelector("class_selector"));
	ASSERT_EQ(std::size_t(2), cssElement.getPropertyCount());

	cssElement = doc.getElement(CssSelector::CssIdSelector("id1"));
	ASSERT_EQ(std::size_t(0), cssElement.getPropertyCount());

	cssElement = doc.getElement(CssSelector::CssIdSelector("id_selector"));
	ASSERT_EQ(std::size_t(1), cssElement.getPropertyCount());
}

TEST(TestStyleSheet, testAddEmptyElement)
{
	CssDocument doc;

	doc.addElement(CssElement(CssSelector::CssIdSelector("prova"), CssPropertySet()));
	ASSERT_EQ(std::size_t(0), doc.getElementCount());
}

TEST(TestStyleSheet, testCssStyle)
{
	CssStyle style("main", "empty_obj");

	ASSERT_EQ(std::string("main"), style.getIdSelector().getName());
	ASSERT_EQ(std::string("empty_obj"), style.getTypeSelector().getName());
	ASSERT_TRUE(style.getClassSelectors().empty());
	ASSERT_TRUE(style.getInlineProperties().empty());

	ASSERT_EQ(std::string("main"), style.getIdSelector().getName());

	style.setInlineProperties(CssPropertySet::parse("pro1: val1;"));
	ASSERT_FALSE(style.getInlineProperties().empty());
	ASSERT_EQ(std::size_t(1), style.getInlineProperties().size());
}

TEST(TestStyleSheet, testCssStyleClasses)
{
	CssStyle style("main", "empty_obj");

	ASSERT_TRUE(style.getClassSelectors().empty());
	ASSERT_TRUE(style.getInlineProperties().empty());

	style.addClass("c1");
	ASSERT_FALSE(style.getClassSelectors().empty());
	ASSERT_EQ(std::size_t(1), style.getClassSelectors().size());

	style.addClass("c2");
	ASSERT_EQ(std::size_t(2), style.getClassSelectors().size());

	style.addClass("c2");
	ASSERT_EQ(std::size_t(2), style.getClassSelectors().size());
}

TEST(TestStyleSheet, testCssStyleParseClasses)
{
	CssStyle style("main", "empty_obj");
	style.addClasses("c1 c3 c2 c2");

	ASSERT_EQ(std::size_t(3), style.getClassSelectors().size());
}

TEST(TestStyleSheet, testUseCase1)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3; property3: val3; } "));
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));

	CssStyle style("id_selector", "type");
	const CssElement& elemById = doc.getElement(style.getIdSelector());
	ASSERT_FALSE(elemById.getProperties().empty());
	ASSERT_TRUE(style.getIdSelector() == elemById.getSelector());

	const CssElement& elemByType = doc.getElement(style.getTypeSelector());
	ASSERT_TRUE(elemByType.getProperties().empty());
	ASSERT_TRUE(style.getTypeSelector() == elemByType.getSelector());
}

TEST(TestStyleSheet, testCssParser)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3; property3: val3; } "));
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));

	CssParser parser(doc);

	CssStyle style("id_selector", "type");
	style.setInlineProperties(CssPropertySet::parse("inline_prop: val1;"));

	CssPropertySet properties = parser.getProperties(style);
	ASSERT_EQ(std::size_t(2), properties.size());
}

TEST(TestStyleSheet, testOverridePropertyWithInline)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector { property1 : val1; property2 : val2, val3; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val1;  property2 : val2, val3; property3: val3; } "));
	doc.addElement(CssElement::parse("#id_selector { property1 : val1; }"));

	CssParser parser(doc);

	CssStyle style("id", "type_selector");
	style.setInlineProperties(CssPropertySet::parse("property2: val4;"));

	CssPropertySet properties = parser.getProperties(style);
	ASSERT_EQ(std::size_t(3), properties.size());

	ASSERT_EQ(std::string("val4"), properties.getProperty("property2").getValue());
}

TEST(TestStyleSheet, testCssParserMultipleSelection)
{
	CssDocument doc;

	doc.addElement(CssElement::parse("#id_selector { property1 : val1; property2: val1; }"));
	doc.addElement(CssElement::parse("type_selector  {  property1 : val2;  property2 : val2; property3: val2; } "));
	doc.addElement(CssElement::parse(".class_selector { property1 : val3; property2 : val3; property3: val3; property4: val3; }"));

	CssParser parser(doc);

	CssStyle style("id_selector", "type_selector");
	style.setInlineProperties(CssPropertySet::parse("property1: val0;"));
	style.addClass("class_selector");
	style.addClass("class_selector_not_found");

	CssPropertySet properties = parser.getProperties(style);
	ASSERT_EQ(std::size_t(4), properties.size());

	// First Inline
	ASSERT_EQ(std::string("val0"), properties.getProperty("property1").getValue());

	// Second IdSelector
	ASSERT_EQ(std::string("val1"), properties.getProperty("property2").getValue());

	// Third TypeSelector
	ASSERT_EQ(std::string("val2"), properties.getProperty("property3").getValue());

	// Then class selectors
	ASSERT_EQ(std::string("val3"), properties.getProperty("property4").getValue());
}

TEST(TestStyleSheet, testCssParserMultipleClasses1)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector1 { property1 : val1; property2: val1; }"));
	doc.addElement(CssElement::parse(".class_selector2  {  property1 : val2;  property2 : val2; property3: val2; } "));
	doc.addElement(CssElement::parse(".class_selector3 { property1 : val3; property2 : val3; property3: val3; property4: val3; }"));

	CssParser parser(doc);

	CssStyle style("id_selector", "type_selector");
	style.setInlineProperties(CssPropertySet::parse("property1: val0;"));
	style.addClass("class_selector1");
	style.addClass("class_selector_not_found");
	style.addClass("class_selector3");

	CssPropertySet properties = parser.getProperties(style);
	ASSERT_EQ(std::size_t(4), properties.size());

	// First Inline
	ASSERT_EQ(std::string("val0"), properties.getProperty("property1").getValue());

	// Second class selectors
	ASSERT_EQ(std::string("val1"), properties.getProperty("property2").getValue());

	ASSERT_EQ(std::string("val3"), properties.getProperty("property3").getValue());

	ASSERT_EQ(std::string("val3"), properties.getProperty("property4").getValue());
}

TEST(TestStyleSheet, testCssParserMultipleClasses2)
{
	CssDocument doc;

	doc.addElement(CssElement::parse(".class_selector1 { property1 : val1; property2: val1; }"));
	doc.addElement(CssElement::parse(".class_selector2  {  property1 : val2;  property2 : val2; property3: val2; } "));
	doc.addElement(CssElement::parse(".class_selector3 { property1 : val3; property2 : val3; property3: val3; property4: val3; }"));

	CssParser parser(doc);

	CssStyle style("id_selector", "type_selector");
	style.setInlineProperties(CssPropertySet::parse("property1: val0;"));
	style.addClass("class_selector3");
	style.addClass("class_selector_not_found");
	style.addClass("class_selector1");

	CssPropertySet properties = parser.getProperties(style);
	ASSERT_EQ(std::size_t(4), properties.size());

	// First Inline
	ASSERT_EQ(std::string("val0"), properties.getProperty("property1").getValue());

	// Second class selectors
	ASSERT_EQ(std::string("val3"), properties.getProperty("property2").getValue());

	ASSERT_EQ(std::string("val3"), properties.getProperty("property3").getValue());

	ASSERT_EQ(std::string("val3"), properties.getProperty("property4").getValue());
}

TEST(TestStyleSheet, testParseDocument)
{
	std::string cssDocument = "type_selector  {  property1 : val1;  property2 : val2, val3;  } \n"
		".class_selector { property1 : val1; property2 : val2, val3; }\n"
		"#id_selector { property1 : val1; property2 : val2, val3; }\n";

	CssDocument cssDoc = CssDocument::parse(cssDocument);

	ASSERT_EQ(std::size_t(3), cssDoc.getElementCount());

	std::string output =
		"type_selector { property1: val1; property2: val2, val3; }\n"
		".class_selector { property1: val1; property2: val2, val3; }\n"
		"#id_selector { property1: val1; property2: val2, val3; }\n";
	ASSERT_EQ(output, cssDoc.toString());
}

TEST(TestStyleSheet, testParseDocument1)
{
    std::string docStr =
    "body {"
        "font-family: Arial, Helvetica, sans-serif, Verdana, Geneva;"
        //"/*    font-size: small; */"
        "font-size: 75%;"
        "margin: 0;"
        "padding:0;"
        "font-weight: normal;"
    "}"
    "\n"
    "A{"
        "text-decoration: none;"
    "}"
    "\n"
    "IMG {"
        "border: 0px;"
    "}\n"
    "\n"
    ".riga_strumenti{"
        "text-align:right;"
        "color : Black;"
        "font-weight : bold;"
    "}\n"
    "#menu_strumenti{"
        "float: right;"
        "margin: 9px 55px 0px 0px;\n"
    "}\n";

    CssDocument doc = CssDocument::parse(docStr);
    ASSERT_EQ(std::size_t(5), doc.getElementCount());
    CssElement bodyElem = doc.getElement(CssSelector::CssTypeSelector("body"));
    ASSERT_EQ(std::size_t(5), bodyElem.getPropertyCount());
    CssProperty font_family = bodyElem.getProperties().getProperty("font-family");
    ASSERT_EQ(std::string("Arial, Helvetica, sans-serif, Verdana, Geneva"), font_family.getValue());

    CssElement riga_strumentiElem = doc.getElement(CssSelector::CssClassSelector("riga_strumenti"));
    ASSERT_EQ(std::size_t(3), riga_strumentiElem.getPropertyCount());

    CssElement menu_strumentiElem = doc.getElement(CssSelector::CssIdSelector("menu_strumenti"));
    ASSERT_EQ(std::size_t(2), menu_strumentiElem.getPropertyCount());
}
