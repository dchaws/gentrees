// David C. Haws 2/2/2012
// Program to generate all binary trees with labeled leafs.

#include <iostream>
#include <list>
#include <math.h>

using namespace std;

int printdebug = 0;

class namedist 
{
public:
    int label;
    double dist;
};

class int2d
{
public:
    int2d () {arrayinit=0;};
    int2d (int n, int m);
    ~int2d ();
    void print ();
    void printupperdiagvector ();
    void printupperdiagvectorpauplin ();
    int & operator() (const int n, const int m);
private:
    int nlim,mlim;
    int **myarray;
    int arrayinit;
};

int2d::int2d(int n, int m)
{
    arrayinit = 1;
    nlim=n;
    mlim=m;
    myarray = new int *[n];
    for (int i=0;i<n;i++)
    {
        myarray[i] = new int [m];
        for (int j=0;j<m;j++)
        {
            myarray[i][j] = 0;
        }
    }
}

int2d::~int2d ()
{
    if (arrayinit == 1)
    {
        for (int i=0;i<nlim;i++)
        {
            delete myarray[i];
        }
        delete myarray;
    }
}

int & int2d::operator() (const int n, const int m)
{
    if (arrayinit == 0 || n < 0 || m < 0 || n >= nlim || m >= mlim)
    {
        cout << "int2d: Array not initialized or out of bounds. " << endl;
        exit(0);
    }
    else 
    {
        return myarray[n][m];
    }
}

void int2d::print ()
{
    if (arrayinit == 1)
    {
        for (int i=0;i<nlim;i++)
        {
            for (int j=0;j<mlim;j++)
            {
                cout << myarray[i][j] << " ";
            }
            cout << endl;
        }
    }
}

void int2d::printupperdiagvector ()
{
    if (arrayinit == 1)
    {
        for (int i=0;i<nlim;i++)
        {
            for (int j=i+1;j<mlim;j++)
            {
                cout << myarray[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void int2d::printupperdiagvectorpauplin ()
{
    if (arrayinit == 1)
    {
        for (int i=0;i<nlim;i++)
        {
            for (int j=i+1;j<mlim;j++)
            {
                cout << "1/" << pow(2,myarray[i][j]-1) << " ";
            }
        }
        cout << endl;
    }
}

class treenode
{
public:
    treenode (int nodelabel) { mylabel = nodelabel;};
    treenode (list <treenode *> inherchildren, int nodelabel);
    ~treenode ();
    void print();
    void addchild (int nodelabel);
    list <namedist> distancetoleafs(int disttoroot, int2d &distmatrix);

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
        //cout << ")" << mylabel;
        cout << ")";
    }
    else {
        cout << mylabel;
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

list <namedist> treenode::distancetoleafs(int disttoroot, int2d &distmatrix)
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
        list <list <namedist> > listofnamedistlist;

        list <namedist> retnamedistlist;

        list <treenode *>::iterator tit = children.begin();
        for (;tit!=children.end();tit++)
        {
            list <namedist> childlist = (*tit)->distancetoleafs(disttoroot + 1,distmatrix);
            pushdistancetoleafslist (retnamedistlist,childlist);
            listofnamedistlist.push_back(childlist);
        }
        list <list <namedist> >::iterator llndone = listofnamedistlist.begin();
        for (;llndone!= listofnamedistlist.end();llndone++)
        {
            list <list <namedist> >::iterator llndtwo = listofnamedistlist.begin();
            for (;llndtwo!= listofnamedistlist.end();llndtwo++)
            {
                if (llndone!= llndtwo)
                {
                    list <namedist>::iterator nditone, ndittwo;
                    for (nditone = (*llndone).begin();nditone!= (*llndone).end();nditone++)
                    {
                        for (ndittwo = (*llndtwo).begin();ndittwo!= (*llndtwo).end();ndittwo++)
                        {
                            //print (); 
                            //cout << " (*nditone).label = " << (*nditone).label << ", (*ndittwo).label = " << (*ndittwo).label;
                            //cout << " (*nditone).dist = " << (*nditone).dist - disttoroot << ", (*ndittwo).dist = " << (*ndittwo).dist - disttoroot << endl;
                            distmatrix((*nditone).label - 1,(*ndittwo).label - 1) = (*nditone).dist + (*ndittwo).dist - 2*disttoroot;
                        }
                    }

                }
            }
        }

        return retnamedistlist;
    }
}


int **namedistlisttodistmatrix (list <namedist> mynamedistlist)
{
    int **distmatrix = new int *[mynamedistlist.size()];
    for (int i=0;i<(int)mynamedistlist.size();i++)
    {
        distmatrix[i] = new int [mynamedistlist.size()];
        for (int j=0;j<(int)mynamedistlist.size();j++)
        {
            distmatrix[i][j] = 0;
        }
    }
    //cout << "Matrix initialized." << endl;
    list <namedist>::iterator ndit;
    list <namedist>::iterator ndit2;
    for (ndit=mynamedistlist.begin();ndit!=mynamedistlist.end();ndit++)
    {
        for (ndit2=mynamedistlist.begin();ndit2!=mynamedistlist.end();ndit2++)
        {
            if ((*ndit).label != (*ndit2).label)
            {
                //cout << (*ndit).label << " " << (*ndit2).label << endl;
                distmatrix[(*ndit).label - 1][(*ndit2).label - 1] = (*ndit).dist + (*ndit2).dist;
                distmatrix[(*ndit2).label - 1][(*ndit).label - 1] = (*ndit).dist + (*ndit2).dist;
            }
        }
    }
    //cout << "Matrix calculated." << endl;
    return distmatrix;
}

void printdistmatrix(int **distmatrix,int n)
{
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            cout << distmatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void printnamedistlist (list <namedist> mynamedistlist)
{
    list <namedist>::iterator ndit;
    list <namedist>::iterator ndit2;
    cout << "[";
    for (ndit=mynamedistlist.begin();ndit!=mynamedistlist.end();ndit++)
    {
        cout << (*ndit).label << ":" << (*ndit).dist;
        ndit2=ndit;
        ndit2++;
        if (ndit2 != mynamedistlist.end())
        {
            cout << ",";
        }
    }
    cout << "]";
}


void insertleafs (treenode &origtreenode, treenode &sometreenode, int numtotalleafs, int numcurleafs, int indentlevel)
{
    if (printdebug == 1){
        for (int i=0;i<indentlevel;i++){ cout << " "; }
        cout << "insertleafs: numtotalleafs = " << numtotalleafs << ", numcurleafs = " << numcurleafs << endl;
        for (int i=0;i<indentlevel;i++){ cout << " "; }
        cout << "  Original Tree: ";
        origtreenode.print();
        cout << endl;
        for (int i=0;i<indentlevel;i++){ cout << " "; }
        cout << "  Some Tree Node: ";
        sometreenode.print();
        cout << endl;
    }
    if (numcurleafs == numtotalleafs){
        if (printdebug == 1){
            for (int i=0;i<indentlevel;i++){ cout << " "; }
            cout << "  Terminal tree." << endl;
            for (int i=0;i<indentlevel;i++){ cout << " "; }
            cout << "  ";
        }
        origtreenode.print();
        cout << endl;
        int2d mydistmatrix(numtotalleafs,numtotalleafs);
        origtreenode.distancetoleafs(0,mydistmatrix);
        mydistmatrix.print();
        cout << "Vector        : ";
        mydistmatrix.printupperdiagvector();
        cout << "Pauplin Vector: ";
        mydistmatrix.printupperdiagvectorpauplin();
        cout << endl;
    }
    if (numcurleafs >= numtotalleafs || sometreenode.children.empty()){
        if (printdebug == 1){
            for (int i=0;i<indentlevel;i++){ cout << " "; }
            cout << "  Exiting." << endl;
        }
        return;
    }

    list <treenode *>::iterator tit = sometreenode.children.begin();
    list <treenode *> newchildren;
    for (;tit!=sometreenode.children.end();tit++)
    {
        if (printdebug == 1){
            //cout << "   Current label: " << (*tit)->mylabel << endl;
            for (int i=0;i<indentlevel;i++){ cout << " "; }
            cout << "  Recursing without inserting new leaf." << endl;
        }
        insertleafs(origtreenode, *(*tit),numtotalleafs,numcurleafs,indentlevel+4);

        treenode *curchild = *tit;

        treenode *newtreeleaf = new treenode(numcurleafs+1);
        newchildren.clear();
        // Make new treenode with children *tit and a new node
        newchildren.push_back(*tit);
        newchildren.push_back(newtreeleaf);
        treenode *newtreenode = new treenode(newchildren,-1);

        sometreenode.children.erase(tit);
        tit++;
        //cout << "   New Current label: " << (*tit)->mylabel << endl;

        sometreenode.children.push_front(newtreenode);
        if (printdebug == 1){
            for (int i=0;i<indentlevel;i++){ cout << " "; }
            cout << "  Recursing inserting new leaf." << endl;
        }
        insertleafs(origtreenode, origtreenode, numtotalleafs,numcurleafs+1,indentlevel+4);

        // Should delete newtreenode and make sometree like nothing happened.
        sometreenode.children.pop_front();
        sometreenode.children.insert(tit,curchild);
        tit--;
        if (printdebug == 1){
            //cout << "   New Current label: " << (*tit)->mylabel << endl;
            //sometreenode.children.push_front(curchild);
            //cout << "   Current tree: ";
            //sometreenode.print();
            //cout << endl;
        }
        newtreenode->children.clear();
        delete newtreenode;
        delete newtreeleaf;
    }
    //cout << "    Ending the loop." << endl;
}

int main (int argc, char **argv)
{
    int numtaxa = 3; // default

    if (argc <= 1)
    {
        cout << "This program will generate all labeled binary trees." << endl;
        cout << "Usage: ./genbintrees <number of leafs>" << endl;
        cout << "       -d Print debug information." << endl;
        exit(0);
    }
    else
    {
        numtaxa = atoi(argv[1]);
        for (int i=2;i<=argc-1 && argc >= 3;i++)
        {
            if (!strcmp(argv[i],"-d"))
            {
                printdebug = 1;
            }
        }
    }

//    cout << "Number of taxa: " << numtaxa << endl;
    
    treenode firstnode(-1);
    firstnode.addchild(1);
    firstnode.addchild(2);
    firstnode.addchild(3);

//    firstnode.print();
//    cout << endl;
//
//    printnamedistlist(firstnode.distancetoleafs(0));

    insertleafs(firstnode,firstnode,numtaxa,3,0);

}
