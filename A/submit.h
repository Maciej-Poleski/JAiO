#include <memory>

struct Node;

class RegularLanguage
{
public:
    RegularLanguage(const std::string& regExp); // dopuszczalne symbole: a, b, 0 (język pusty), 1 (singleton słowa pustego),
    // + (suma), * (gwiazdka Kleene'ego), (, ).
    // np. "a*b", "(a+b)b*", "ab***"
    // priorytet operatorów: suma < katenacja < gwiazdka
    // gramatyka dla wyrażeń (plus ewentalne dodatkowe spacje): S ::= S + S | SS | S* | (S) | a | b | 0 | 1
    // dla niepoprawnych argumentów rzuca wyjątek std::invalid_argument

    friend RegularLanguage operator+(RegularLanguage const & a, RegularLanguage const & b); // suma
    friend RegularLanguage operator*(RegularLanguage const & a, RegularLanguage const & b); // katenacja
    friend RegularLanguage operator*(RegularLanguage const & a); // gwiazdka Kleene'ego

    friend bool operator==(RegularLanguage const & a, RegularLanguage const & b);
    friend bool operator!=(RegularLanguage const & a, RegularLanguage const & b);
    friend bool operator<=(RegularLanguage const & a, RegularLanguage const & b); // inkluzja

    std::string ToString() const; // wypisuje wyrażenie opisujące, bez zbędnych nawiasów
    // i maksymalnie uproszczone (a** = a*, a + 0 = a, a1 = a, a0 = 0, 0* = 1, 1* = 1).

private:
    RegularLanguage(std::shared_ptr<const Node> node);
    
private:
    std::shared_ptr<const Node> _node;
};
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "RegularLanguage.h"

namespace
{
class CharProvider final
{
public:
    CharProvider(const std::string &string);

    bool accept(char c);

private:
    const std::string _string;
    std::size_t _i=0;
};

CharProvider::CharProvider(const std::string& string) :_string(string)
{
}

bool CharProvider::accept(char c)
{
    while(_i<_string.length() && _string[_i]==' ')
        ++_i;
    if(_i<_string.length() && _string[_i]==c)
    {
        ++_i;
        return true;
    }
    else
        return false;
}
};

struct Node
{
    virtual ~Node();

    virtual const int rank() const = 0;
    virtual std::string toString() const =0;

    virtual bool isSthStar() const =0;
};

Node::~Node()
{
}

namespace
{

struct PlusNode : public Node
{
    std::shared_ptr<const Node> left,right;

    PlusNode(std::shared_ptr<const Node> left,std::shared_ptr<const Node> right);
    virtual ~PlusNode() override;

    virtual const int rank() const override;
    virtual std::string toString() const override;
    virtual bool isSthStar() const override;
};

const int PlusNode::rank() const
{
    return 0;
}

bool PlusNode::isSthStar() const
{
    return false;
}

PlusNode::~PlusNode()
{
}

struct CatNode : public Node
{
    std::shared_ptr<const Node> left,right;

    CatNode(std::shared_ptr<const Node> left,std::shared_ptr<const Node> right);

    virtual bool isSthStar() const override;
    virtual const int rank() const override;
    virtual std::string toString() const override;
    virtual ~CatNode() override;
};

struct StarNode : public Node
{
    std::shared_ptr<const Node> node;

    StarNode(std::shared_ptr<const Node> node);

    virtual bool isSthStar() const override;
    virtual const int rank() const override;
    virtual std::string toString() const override;
    virtual ~StarNode() override;

};

struct ConstNode : public Node
{
    char c;

    ConstNode(char c);

    virtual const int rank() const override;
    virtual bool isSthStar() const override;
    virtual std::string toString() const override;
    virtual ~ConstNode() override;
};

static std::shared_ptr<const ConstNode> aConstNode(new ConstNode('a'));
static std::shared_ptr<const ConstNode> bConstNode(new ConstNode('b'));
static std::shared_ptr<const ConstNode> zeroConstNode(new ConstNode('0'));
static std::shared_ptr<const ConstNode> oneConstNode(new ConstNode('1'));

static std::shared_ptr<const Node> parseSum(CharProvider &cp);

static std::shared_ptr<const Node> parseConst(CharProvider &cp)
{
    if(cp.accept('a'))
        return aConstNode;
    else if(cp.accept('b'))
        return bConstNode;
    else if(cp.accept('0'))
        return zeroConstNode;
    else if(cp.accept('1'))
        return oneConstNode;
    else if(cp.accept('('))
    {
        auto result=parseSum(cp);
        if(cp.accept(')'))
            return result;
        throw 6;
    }
    throw std::invalid_argument("");
}

static std::shared_ptr<const Node> makeStarNode(std::shared_ptr<const Node> node)
{
    if(node->isSthStar())
        return node;
    else if(node==zeroConstNode || node==oneConstNode)
        return oneConstNode;
    else
        return std::make_shared<StarNode>(node);
}

static std::shared_ptr<const Node> parseStar(CharProvider &cp)
{
    auto result=parseConst(cp);
    if(cp.accept('*'))
        result=makeStarNode(result);
    while(cp.accept('*'))
        ;
    return result;
}

static std::shared_ptr<const Node> makeCatNode(std::shared_ptr<const Node> left,std::shared_ptr<const Node> right)
{
    if(left==zeroConstNode || right==zeroConstNode)
        return zeroConstNode;
    else if(left==oneConstNode)
        return right;
    else if(right==oneConstNode)
        return left;
    else
    {
        return std::make_shared<CatNode>(left,right);
    }
}

static std::shared_ptr<const Node> parseCat(CharProvider &cp)
{
    auto result=parseStar(cp);
    try
    {
        for(;;)
        {
            auto t=parseStar(cp);
            result=makeCatNode(result,t);
        }
    }
    catch(const std::invalid_argument &)
    {
    }
    return result;
}

static std::shared_ptr<const Node> makeSumNode(std::shared_ptr<const Node> left,std::shared_ptr<const Node> right)
{
    if(left==zeroConstNode)
        return right;
    else if(right==zeroConstNode)
        return left;
    else if(left==right)
        return left;
    else
    {
        return std::shared_ptr<const PlusNode>(new PlusNode(left,right));
    }
}

static std::shared_ptr<const Node> parseSum(CharProvider &cp)
{
    auto result=parseCat(cp);
    while(cp.accept('+'))
    {
        auto t=parseCat(cp);
        result=makeSumNode(result,t);
    }
    return result;
}

CatNode::CatNode(std::shared_ptr< const Node > left, std::shared_ptr< const Node > right) :
    left(left), right(right)
{
}

bool CatNode::isSthStar() const
{
    return false;
}

CatNode::~CatNode()
{

}

ConstNode::ConstNode(char c) : c(c)
{

}

bool ConstNode::isSthStar() const
{
    return false;
}

ConstNode::~ConstNode()
{

}

PlusNode::PlusNode(std::shared_ptr< const Node > left, std::shared_ptr< const Node > right) :
    left(left), right(right)
{

}

bool StarNode::isSthStar() const
{
    return true;
}

StarNode::StarNode(std::shared_ptr< const Node > node) : node(node)
{

}

StarNode::~StarNode()
{

}

const int CatNode::rank() const
{
    return 1;
}

const int StarNode::rank() const
{
    return 2;
}

const int ConstNode::rank() const
{
    return 3;
}

std::string CatNode::toString() const
{
    std::string result;
    if(left->rank()<rank())
        result+="(";
    result+=left->toString();
    if(left->rank()<rank())
        result+=")";

    if(right->rank()<rank())
        result+="(";
    result+=right->toString();
    if(right->rank()<rank())
        result+=")";
    return result;
}

std::string ConstNode::toString() const
{
    return std::string(1,c);
}

std::string PlusNode::toString() const
{
    std::string result;
    if(left->rank()<rank())
        result+="(";
    result+=left->toString();
    if(left->rank()<rank())
        result+=")";
    result+="+";
    if(right->rank()<rank())
        result+="(";
    result+=right->toString();
    if(right->rank()<rank())
        result+=")";
    return result;
}

std::string StarNode::toString() const
{
    std::string result;
    if(node->rank()<rank())
        result+="(";
    result+=node->toString();
    if(node->rank()<rank())
        result+=")";
    result+="*";
    return result;
}


};


RegularLanguage::RegularLanguage(const std::string& regExp)
{
    try
    {
        CharProvider cp(regExp);
        _node=parseSum(cp);
    }
    catch(int)
    {
        throw std::invalid_argument("");
    }
}

RegularLanguage::RegularLanguage(std::shared_ptr< const Node > node) : _node(node)
{

}

std::string RegularLanguage::ToString() const
{
    return _node->toString();
}

RegularLanguage operator*(const RegularLanguage& a, const RegularLanguage& b)
{
    return RegularLanguage(makeCatNode(a._node,b._node));
}

RegularLanguage operator+(const RegularLanguage& a, const RegularLanguage& b)
{
    return RegularLanguage(makeSumNode(a._node,b._node));
}

RegularLanguage operator*(const RegularLanguage& a)
{
    return RegularLanguage(makeStarNode(a._node));
}

bool operator==(const RegularLanguage& a, const RegularLanguage& b)
{
    return false;
}

bool operator!=(const RegularLanguage& a, const RegularLanguage& b)
{
    return !(a==b);
}

bool operator<=(const RegularLanguage& a, const RegularLanguage& b)
{
    return false;
}
