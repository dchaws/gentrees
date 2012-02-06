// David C. Haws 2/2/2012
// Program to generate all binary trees with labeled leafs.

#include <iostream>
#include <list>

using namespace std;

class namedist 
{
public:
    int label;
    double dist;
};

class treenode
{
public:
    treenode (int nodelabel) { mylabel = nodelabel;};
    treenode (list <treenode *> inherchildren, int nodelabel);
    ~treenode ();
    void print();
    void addchild (int nodelabel);
    list <namedist> distancetoleafs(int disttoroot);

    list <treenode *> children;
    int mylabel;
private:
};

treenode::treenode (list <treenode *> inherchildren, int nodelabel)
{
    children = inherchildren;
    mylabel = nodelabel;
}

treenode::~treenode()
{
    list <treenode *>::iterator tit = children.begin();
    for (;tit!=children.end();tit++)
    {
        delete *tit;
    }
}

void treenode::print()
{
    if (!children.empty())
    {
        cout << "(";
        list <treenode *>::iterator tit = children.begin();
        list <treenode *>::iterator tit2;
        for (;tit!=children.end();tit++)
        {
            (*tit)->print();
            tit2 = tit;
            tit2++;
            if (tit2 != children.end())
            {
                cout << ",";
            }
        }
        cout << ")" << mylabel << endl;
    }
    else {
        cout << mylabel ;
    }
}

void treenode::addchild(int nodelabel)
{
    treenode *newnode = new treenode(nodelabel);
    children.push_back(newnode);
}

void pushdistancetoleafslist (list <namedist> &listone, list <namedist> listtwo)
{
    list <namedist>::iterator ndit;
    for (ndit=listtwo.begin();ndit!=listtwo.end();ndit++)
    {
        listone.push_back(*ndit);
    }
}

list <namedist> treenode::distancetoleafs(int disttoroot)
{
    if (children.empty())
    {
        namedist mynamedist;
        mynamedist.label = mylabel;
        mynamedist.dist = disttoroot;
        list <namedist> mynamedistlist;
        mynamedistlist.push_back(mynamedist);
        return mynamedistlist;
    }
    else {
        list <namedist> retnamedistlist;

        list <treenode *>::iterator tit = children.begin();
        for (;tit!=children.end();tit++)
        {
            pushdistancetoleafslist (retnamedistlist,(*tit)->distancetoleafs(disttoroot + 1));
        }

        return retnamedistlist;
    }
}

void printnamedistlist (list <namedist> &mynamedistlist)
{
    list <namedist>::iterator ndit;
    for (ndit=mynamedistlist.begin();ndit!=mynamedistlist.end();ndit++)
    {
        cout << (*ndit).label << ":" << (*ndit).dist << endl;
    }

}

int main (int argc, char **argv)
{
    int numtaxa = 3; // default

    if (argc <= 1)
    {
        cout << "This program will generate all labeled binary trees." << endl;
        exit(0);
    }
    else
    {
        numtaxa = atoi(argv[1]);
    }

    cout << "Number of taxa: " << numtaxa << endl;
    
    treenode firstnode(-1);
    firstnode.addchild(1);
    firstnode.addchild(2);
    firstnode.addchild(3);

    firstnode.print();

}
