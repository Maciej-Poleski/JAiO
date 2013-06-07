#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "RegularLanguage.h"

#define override

namespace
{
class NFA;

typedef std::shared_ptr<NFA> NFAPtr;

class CharProvider
{
public:
    CharProvider(const std::string &string);

    bool accept(char c);

private:
    const std::string _string;
    std::size_t _i;
};

CharProvider::CharProvider(const std::string& string) :_string(string), _i(0)
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
    virtual NFAPtr buildNFA() const=0;

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
    virtual NFAPtr buildNFA() const override;
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
    virtual NFAPtr buildNFA() const override;
};

struct StarNode : public Node
{
    std::shared_ptr<const Node> node;

    StarNode(std::shared_ptr<const Node> node);

    virtual bool isSthStar() const override;
    virtual const int rank() const override;
    virtual std::string toString() const override;
    virtual ~StarNode() override;
    virtual NFAPtr buildNFA() const override;

};

struct ConstNode : public Node
{
    char c;

    ConstNode(char c);

    virtual const int rank() const override;
    virtual bool isSthStar() const override;
    virtual std::string toString() const override;
    virtual ~ConstNode() override;
    virtual NFAPtr buildNFA() const override;
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

/********************************** AUTOMAT ***********************************/

#include <vector>
#include <list>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <cassert>

#define check(x)

namespace
{

class State;

struct Edge
{
    State *dest;
    char c;
};

class NFA
{
public:
    NFA(const NFA&) = delete;
    NFA & operator=(const NFA&) = delete;
    ~NFA();

    State *newState(bool start=false,bool terminal=false);

    std::size_t getAllStatesCount() const;
    void transferStates(NFAPtr from);

    bool isTerminal(State *state);
    const std::vector<State*>& getStartStates() const;

    static NFAPtr acceptChar(char c);
    static NFAPtr acceptDot();
    static NFAPtr acceptCat(NFAPtr A, NFAPtr B);
    static NFAPtr acceptAlt(NFAPtr A, NFAPtr B);
    static NFAPtr acceptStar(NFAPtr A);
    static NFAPtr acceptEmpty();          // akceptuje pusty napis
    static NFAPtr acceptVoid();           // pusty język

private:
    NFA() {}    // to nie ma sensu w API
    static NFAPtr createEmpty();

private:
    std::list<State*> allStates;        // Ułatwia dealokacje
    std::vector<State*> startStates;
    std::vector<State*> terminalStates;
    static const NFAPtr emptyNFA;
};

const NFAPtr NFA::emptyNFA=NFA::createEmpty();

class State
{
    friend State *NFA::newState(bool start,bool terminal);

public:
    std::vector<Edge> edges;

public:

private:
    State() {}
};

NFA::~NFA()
{
    for(std::list<State*>::iterator i=allStates.begin(),e=allStates.end(); i!=e; ++i)
    {
        delete *i;
    }
}

State* NFA::newState(bool start, bool terminal)
{
    State *result=new State();
    allStates.push_back(result);
    if(start)
        startStates.push_back(result);
    if(terminal)
        terminalStates.push_back(result);
    return result;
}

NFAPtr NFA::acceptChar(char c)
{
    NFAPtr result(new NFA);
    State *start=result->newState(true,false);
    State *terminal=result->newState(false,true);
    start->edges.push_back( {terminal,c});
    return result;
}

NFAPtr NFA::acceptDot()
{
    NFAPtr result(new NFA);
    State *start=result->newState(true,false);
    State *terminal=result->newState(false,true);
    start->edges.push_back( {terminal,'a'});
    start->edges.push_back( {terminal,'b'});
    return result;
}

void NFA::transferStates(NFAPtr from)
{
    check(this!=from.get());
    allStates.splice(allStates.end(),from->allStates);
}

NFAPtr NFA::acceptCat(NFAPtr A, NFAPtr B)
{
    if(A==emptyNFA)
        return B;
    else if(B==emptyNFA)
        return A;
    NFAPtr result(new NFA);
    result->transferStates(A);
    result->transferStates(B);
    result->startStates=A->startStates;
    result->terminalStates=B->terminalStates;
    for(std::vector<State*>::iterator i=A->terminalStates.begin(),e=A->terminalStates.end(); i!=e; ++i)
    {
        for(std::vector<State*>::iterator j=B->startStates.begin(),ee=B->startStates.end(); j!=ee; ++j)
        {
            (*i)->edges.push_back( {*j,'\0'});
        }
    }
    return result;
}

NFAPtr NFA::acceptAlt(NFAPtr A, NFAPtr B)
{
    if(A==emptyNFA)
        A=createEmpty();
    if(B==emptyNFA)
        B=createEmpty();
    NFAPtr result(new NFA);
    result->transferStates(A);
    result->transferStates(B);
    State *start=result->newState(true,false);
    State *terminal=result->newState(false,true);
    for(std::vector<State*>::iterator i=A->startStates.begin(),e=A->startStates.end(); i!=e; ++i)
    {
        start->edges.push_back( {*i,'\0'});
    }
    for(std::vector<State*>::iterator i=B->startStates.begin(),e=B->startStates.end(); i!=e; ++i)
    {
        start->edges.push_back( {*i,'\0'});
    }
    for(std::vector<State*>::iterator i=A->terminalStates.begin(),e=A->terminalStates.end(); i!=e; ++i)
    {
        (*i)->edges.push_back( {terminal,'\0'});
    }
    for(std::vector<State*>::iterator i=B->terminalStates.begin(),e=B->terminalStates.end(); i!=e; ++i)
    {
        (*i)->edges.push_back( {terminal,'\0'});
    }
    return result;
}

NFAPtr NFA::acceptStar(NFAPtr A)
{
    if(A==emptyNFA)
        return A;
    NFAPtr result(new NFA);
    result->transferStates(A);
    State *aux=result->newState(true,true);
    for(std::vector<State*>::iterator i=A->terminalStates.begin(),e=A->terminalStates.end(); i!=e; ++i)
    {
        (*i)->edges.push_back( {aux,'\0'});
    }
    for(std::vector<State*>::iterator i=A->startStates.begin(),e=A->startStates.end(); i!=e; ++i)
    {
        aux->edges.push_back( {*i,'\0'});
    }
    return result;
    //     NFAPtr result(new NFA);
    //     result->transferStates(A);
    //     State *start=result->newState(true,false);
    //     State *terminal=result->newState(false,true);
    //     start->edges.push_back( {terminal,'\0'});
    //     for(std::vector<State*>::iterator i=A->terminalStates.begin(),e=A->terminalStates.end(); i!=e; ++i)
    //     {
    //         (*i)->edges.push_back( {terminal,'\0'});
    //     }
    //     for(std::vector<State*>::iterator i=A->startStates.begin(),e=A->startStates.end(); i!=e; ++i)
    //     {
    //         start->edges.push_back( {*i,'\0'});
    //         terminal->edges.push_back( {*i,'\0'});
    //     }
    return result;
}

NFAPtr NFA::createEmpty()
{
    NFAPtr result(new NFA);
    result->newState(true,true);
    return result;
}

NFAPtr NFA::acceptVoid()
{
    NFAPtr result(new NFA);
    return result;
}

NFAPtr NFA::acceptEmpty()
{
    return emptyNFA;
}

bool NFA::isTerminal(State* state)
{
    return std::find(terminalStates.begin(),terminalStates.end(),state)!=terminalStates.end();
}

const std::vector< State* >& NFA::getStartStates() const
{
    return startStates;
}

struct DetState
{
    uint32_t next[2];
    bool terminal;

    std::vector<uint32_t> back[2];

    DetState() : terminal(false) {}
    DetState(const DetState &detState) :
        next {detState.next[0],detState.next[1]},
         terminal(detState.terminal),
         back {detState.back[0],detState.back[1]}
    {}

    DetState(uint32_t a,uint32_t b, bool terminal) : next {a,b}, terminal(terminal) {}
};

uint32_t charToIndex(const char c)
{
    return c=='b';
}

std::size_t NFA::getAllStatesCount() const
{
    return allStates.size();
}

static std::size_t getNFASize(NFAPtr nfa)
{
    // można lepiej, ale na razie nie widzę sensu
    return nfa->getAllStatesCount();
}

static void travelByNull(std::unordered_set<State*> &result, State *state)
{
    result.insert(state);
    for(std::vector<Edge>::const_iterator i=state->edges.begin(),e=state->edges.end(); i!=e; ++i)
    {
        if(i->c=='\0' && result.find(i->dest)==result.end())
            travelByNull(result,i->dest);
    }
}

static void travelByChar(std::unordered_set<State*> &result, State *state,char c)
{
    for(std::vector<Edge>::const_iterator i=state->edges.begin(),e=state->edges.end(); i!=e; ++i)
    {
        if(i->c==c && result.find(i->dest)==result.end())
            travelByNull(result,i->dest);
    }
}

class DFAMaker
{
public:
    DFAMaker(NFAPtr nfa) : nfa(nfa), NFASize(getNFASize(nfa)),
        nextIntegerFromState(0) {
        check(NFASize<=64); // typ size_t (zastąpić przez std::vector<bool> gdy biblioteka zostanie poprawiona)
    }

    void makeDFA();
    bool isAccepted(const std::string &line);
    void minimalize();

    bool equalsTo(const DFAMaker& dfaMaker) const;

private:
    std::size_t integerFromState(State *state);
    uint32_t detStateFromStateSet(std::vector<bool> stateSet);
    std::vector<bool> stateSetFromStateSet(const std::unordered_set<State*> &set);
    void removeNotReachable();
    void markReachable(std::size_t v, std::vector< std::uint32_t >& fromOldIdToNewId, std::size_t& nextId) const;

private:
    NFAPtr nfa;
    const std::size_t NFASize;
    std::unordered_map<State*,size_t> stateToInteger;
    std::size_t nextIntegerFromState;
    std::vector<State*> stateByInteger;
    std::unordered_map<std::vector<bool>,uint32_t> stateSetToDetState;
    // Wygląda na to, że w kompilatorze jest błąd. push_back na takim wektorze
    // składającym się z rzeczywistych obiektów powoduje dealokacje pamięci.
    // Jako obejście przechowuję w wektorze wskaźnik na rzeczywisty obiekt
    // GCC 4.7.2 (i satori) - nie działa
    // Clang 3.2 - działa
    std::vector<std::shared_ptr<DetState> > DFA;
};

void DFAMaker::makeDFA()
{
    std::stack<std::vector<bool>> detStatesToFollow;
    std::vector<bool> t;
    {
        const std::vector<State*> &startStates=nfa->getStartStates();
        std::unordered_set<State*> set;
        for(std::vector<State*>::const_iterator i=startStates.begin(),e=startStates.end(); i!=e; ++i)
        {
            travelByNull(set,*i);
        }
        t=stateSetFromStateSet(set);
    }
    detStatesToFollow.push(t);
    std::unordered_set<std::vector<bool>> stateSetEnqueued;
    stateSetEnqueued.insert(t);
    while(!detStatesToFollow.empty())
    {
        std::vector<bool> stateSet=detStatesToFollow.top();
        detStatesToFollow.pop();
        uint32_t detState=detStateFromStateSet(stateSet);

        // a
        {
            std::unordered_set<State*> set;
            for(size_t i=0; i<NFASize; ++i)
            {
                if(i<stateSet.size() && stateSet[i])
                {
                    travelByChar(set,stateByInteger[i],'a');
                }
            }
            auto a=stateSetFromStateSet(set);
            if(stateSetEnqueued.find(a)==stateSetEnqueued.end())
            {
                detStatesToFollow.push(a);
                stateSetEnqueued.insert(a);
            }
            uint32_t t=detStateFromStateSet(a);
            DFA[detState]->next[charToIndex('a')]=t;
            //DFA[t]->back[charToIndex('a')].push_back(detState);
        }

        // b
        {
            std::unordered_set<State*> set;
            for(size_t i=0; i<NFASize; ++i)
            {
                if(i<stateSet.size() && stateSet[i])
                {
                    travelByChar(set,stateByInteger[i],'b');
                }
            }
            auto b=stateSetFromStateSet(set);
            if(stateSetEnqueued.find(b)==stateSetEnqueued.end())
            {
                detStatesToFollow.push(b);
                stateSetEnqueued.insert(b);
            }
            uint32_t t=detStateFromStateSet(b);
            DFA[detState]->next[charToIndex('b')]=t;
            //DFA[t]->back[charToIndex('b')].push_back(detState);
        }
    }
}

size_t DFAMaker::integerFromState(State* state)
{
    check(stateByInteger.size()==nextIntegerFromState);
    check(nextIntegerFromState<=64);
    if(stateToInteger.find(state)!=stateToInteger.end())
        return stateToInteger[state];
    check(nextIntegerFromState<64);
    stateByInteger.push_back(state);
    return stateToInteger[state]=nextIntegerFromState++;
}

uint32_t DFAMaker::detStateFromStateSet(const std::vector<bool> stateSet)
{
    check(NFASize<=64);
    check(stateByInteger.size()==nextIntegerFromState);
    if(stateSetToDetState.find(stateSet)!=stateSetToDetState.end())
        return stateSetToDetState[stateSet];
    bool terminal=false;
    for(size_t i=0; i<NFASize; ++i)
    {
        if(i<stateSet.size() && stateSet[i])
        {
            check(i<nextIntegerFromState);
            if(nfa->isTerminal(stateByInteger[i]))
            {
                terminal=true;
                break;
            }
        }
    }
    std::shared_ptr<DetState> p(new DetState(0,0,terminal));
    DFA.push_back(p);
    return stateSetToDetState[stateSet]=DFA.size()-1;
}


std::vector<bool> DFAMaker::stateSetFromStateSet(const std::unordered_set< State* >& set)
{
    std::vector<bool> result;
    for(std::unordered_set< State* >::const_iterator i=set.begin(),e=set.end(); i!=e; ++i)
    {
        result.reserve(integerFromState(*i)+1);
        result[integerFromState(*i)]=true;
    }
    return result;
}

bool DFAMaker::isAccepted(const std::string &line)
{
    uint32_t state=0;
    for(std::string::const_iterator i=line.begin(),e=line.end(); i!=e; ++i)
    {
        check(*i=='a' || *i=='b');
        if(DFA[state]->terminal)
            return true;
        state=DFA[state]->next[charToIndex(*i)];
    }
    return DFA[state]->terminal;
}

void DFAMaker::markReachable(size_t v, std::vector< uint32_t > &fromOldIdToNewId,size_t &nextId) const
{
    if(fromOldIdToNewId[v]!=std::numeric_limits<uint32_t>::max())
        return;
    fromOldIdToNewId[v]=nextId++;
    markReachable(DFA[v]->next[charToIndex('a')],fromOldIdToNewId,nextId);
    markReachable(DFA[v]->next[charToIndex('b')],fromOldIdToNewId,nextId);
}

void DFAMaker::removeNotReachable()
{
    size_t oldSize=DFA.size();
    std::vector<uint32_t> fromOldIdToNewId(oldSize,std::numeric_limits<uint32_t>::max());
    size_t newSize=0;
    markReachable(0,fromOldIdToNewId,newSize);
    std::vector<std::shared_ptr<DetState> > newDFA(newSize);
    for(size_t i=0; i<newSize; ++i)
        newDFA[i].reset(new DetState);
    for(size_t i=0; i<oldSize; ++i)
    {
        if(fromOldIdToNewId[i]==std::numeric_limits<uint32_t>::max())
            continue;
        newDFA[fromOldIdToNewId[i]]->terminal=DFA[i]->terminal;
        for(char c='a'; c<='b'; ++c)
        {
            uint32_t next=fromOldIdToNewId[DFA[i]->next[charToIndex(c)]];
            check(next!=std::numeric_limits<uint32_t>::max());
            newDFA[fromOldIdToNewId[i]]->next[charToIndex(c)]=next;
            newDFA[next]->back[charToIndex(c)].push_back(fromOldIdToNewId[i]);
        }
    }
    using std::swap;
    swap(DFA,newDFA);
}

void DFAMaker::minimalize()
{
    {
        // free memory
        nfa.reset();
        std::unordered_map<State*,size_t> a;
        a.swap(stateToInteger);
        std::vector<State*> b;
        stateByInteger.swap(b);
        std::unordered_map<std::vector<bool>,uint32_t> c;
        stateSetToDetState.swap(c);
    }
    removeNotReachable();       // probably redundant
    const size_t detStatesCount=DFA.size();
    std::vector<std::vector<bool> > matrix;
    std::stack<std::pair<uint32_t,uint32_t> > setOfPairsToCheck;
    for(size_t i=0; i<detStatesCount; ++i)
    {
        std::vector<bool> row;
        for(size_t j=0; j<i; ++j)
        {
            bool t=DFA[i]->terminal!=DFA[j]->terminal;
            row.push_back(t);
            if(t)
            {
                setOfPairsToCheck.push(std::pair<uint32_t,uint32_t>(i,j));
            }
        }
        matrix.push_back(row);
    }
    while(!setOfPairsToCheck.empty())
    {
        uint32_t ii=setOfPairsToCheck.top().first;
        uint32_t jj=setOfPairsToCheck.top().second;
        setOfPairsToCheck.pop();
        for(char c='a'; c<='b'; ++c)
        {
            for(std::vector<uint32_t>::const_iterator i=DFA[ii]->back[charToIndex(c)].begin(),e=DFA[ii]->back[charToIndex(c)].end(); i!=e; ++i)
            {
                for(std::vector<uint32_t>::const_iterator j=DFA[jj]->back[charToIndex(c)].begin(),ej=DFA[jj]->back[charToIndex(c)].end(); j!=ej; ++j)
                {
                    std::pair<uint32_t,uint32_t> p(std::max(*i,*j),std::min(*i,*j));
                    if(!matrix[p.first][p.second])
                    {
                        matrix[p.first][p.second]=true;
                        setOfPairsToCheck.push(p);
                    }
                }
            }
        }
    }
    std::vector<uint32_t> fromOLdIdToNewId(detStatesCount);
    for(size_t i=0; i<detStatesCount; ++i)
    {
        uint32_t selected=i;
        for(size_t j=0; j<i; ++j)
        {
            if(!matrix[i][j])
            {
                selected=j;
                break;
            }
        }
        fromOLdIdToNewId[i]=selected;
    }
    for(size_t i=0; i<detStatesCount; ++i)
    {
        for(char c='a'; c<='b'; ++c)
        {
            DFA[i]->next[charToIndex(c)]=fromOLdIdToNewId[DFA[i]->next[charToIndex(c)]];
        }
    }
    removeNotReachable();       // this is not redundant
}

bool DFAMaker::equalsTo(const DFAMaker& dfaMaker) const
{
    // minimalize method is _very_ deterministic
    if(DFA.size()!=dfaMaker.DFA.size())
        return false;
    for(size_t i=0; i<DFA.size(); ++i)
    {
        if(DFA[i]->terminal!=dfaMaker.DFA[i]->terminal)
            return false;
        for(char c='a'; c<='b'; ++c)
        {
            if(DFA[i]->next[charToIndex(c)]!=dfaMaker.DFA[i]->next[charToIndex(c)])
                return false;
        }
    }
    return true;
}

NFAPtr CatNode::buildNFA() const
{
    return NFA::acceptCat(left->buildNFA(),right->buildNFA());
}

NFAPtr ConstNode::buildNFA() const
{
    if(c=='a' || c=='b')
        return NFA::acceptChar(c);
    else if(c=='1')
        return NFA::acceptEmpty();
    else
    {
        assert(c=='0');
        return NFA::acceptVoid();
    }
}

NFAPtr PlusNode::buildNFA() const
{
    return NFA::acceptAlt(left->buildNFA(),right->buildNFA());
}

NFAPtr StarNode::buildNFA() const
{
    return NFA::acceptStar(node->buildNFA());
}

};

bool operator==(const RegularLanguage& a, const RegularLanguage& b)
{
    DFAMaker leftDFA(a._node->buildNFA());
    leftDFA.makeDFA();
    leftDFA.minimalize();
    DFAMaker rightDFA(b._node->buildNFA());
    rightDFA.makeDFA();
    rightDFA.minimalize();
    return leftDFA.equalsTo(rightDFA);
}

bool operator!=(const RegularLanguage& a, const RegularLanguage& b)
{
    return !(a==b);
}

bool operator<=(const RegularLanguage& a, const RegularLanguage& b)
{
    return a==b;
}
