#include<iostream>
#include <fstream>
#include "string.h"
#include <iomanip>
#include "math.h"
#include <stdio.h>
#include <string>

using namespace std;

struct node
{

        long int ID;                     //KEY
        int count;                       //Event Count
        int height;                      //Height of Node
        char color;                      //Color of node
        node *left;                      //pointer to left child
        node *right;                     //pointer to right child
        node *parent;                    //pointer to parent

};


class RBtree
{
public:

      node *root; // pointer to root of the tree
      node *q;    
      node *nill = new node; //sentinel node

      RBtree()
      {             
        nill->ID = -2;                   
        nill->count = -2;                      
        nill->height = -3;                      
        nill->color = 'b';                      
        nill->left = nill;                  //initialize sentinel node  
        nill->right = nill;                     
        nill->parent = nill;     

        root = nill;                       //root of tree will be a sentinel node when tree is empty       
      }


      node* initialize(long int arr[][2], long int start, long int end, node* father, int h, int check);

      void insert(long int key, long int count);

      void insertfix(node *);

      void leftrotate(node *);

      void rightrotate(node *);

      void del(long int key);

      void delfix(node *);                                      // Functions prototypes

      node* successor(node *);

      node* predecessor(node *x);

      void modifycounter(long int key, long int delta, int func);

      void search_pn(long int x, char func);

      void searchcount(long int x);

      long int inrange(node *root, long int low, long int high);

};

// Insert into tree
//parameters are TheID and value of count
void RBtree::insert(long int key, long int count)
{
          int i = 0;
          node *p, *q;

          node *t = new node;    // create new node

          t->ID = key;
          t->count = count;
          t->left = nill;
          t->right = nill;       //assign values of node: the color of node will 
          t->color = 'r';
          t->height = -1;

          p = root;

          q = nill;

          if (root == nill)
          {
                root = t;
                t->parent = nill;   //if tree is empty, insert at root
          }

          else
          {
                while (p != nill)
                {
                    q = p;
                    if (p->ID < t->ID)
                      p = p->right;
                    else
                      p = p->left;
                }                           // Normal binary tree insert

                t->parent = q;
                if (q->ID < t->ID)
                  q->right = t;
                else
                  q->left = t;
          }

          insertfix(t);               //call insert fix to fix any violation of RB Tree properties
}

void RBtree::insertfix(node *pt)
{

  node *parent_pt = nill;
  node *grand_parent_pt = nill;

  char temp;

  while ((pt != root) && (pt->color != 'b') && (pt->parent->color == 'r'))
  {

          parent_pt = pt->parent;
          grand_parent_pt = pt->parent->parent;

          // Case A: Parent of node is left child of the grand-parent of node
          if (parent_pt == grand_parent_pt->left)
          {
                node *uncle_pt = grand_parent_pt->right;

                // Case 1: The uncle of node is also red so only recoloring is required
                if (uncle_pt != nill && uncle_pt->color == 'r')
                {
                      grand_parent_pt->color = 'r';
                      parent_pt->color = 'b';
                      uncle_pt->color = 'b';
                      pt = grand_parent_pt;
                }


                else
                {
                  // Case 2:  The node is the right child of its parent so left rotation is required

                  if (pt == parent_pt->right)
                  {
                        leftrotate(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                  }

                  // Case 3:  The node is left child of its parent right rotation required
                  rightrotate(grand_parent_pt);


                  temp = parent_pt->color;
                  parent_pt->color = grand_parent_pt->color;    // swap parent and grandparents color
                  grand_parent_pt->color = temp;


                  pt = parent_pt;
                }
          }

          // Case B:  Parent of the node is right child of grand-parent of the node 

          else
          {
              node *uncle_pt = grand_parent_pt->left;

              // Case 1:  The uncle of the node is also red  so only recoloring is required 


              if ((uncle_pt != nill) && (uncle_pt->color == 'r'))
              {
                    grand_parent_pt->color = 'r';
                    parent_pt->color = 'b';
                    uncle_pt->color = 'b';
                    pt = grand_parent_pt;
              }
              else
              {
                // Case 2:  pt is left child of its parent right-rotation required 


                    if (pt == parent_pt->left)
                    {
                          rightrotate(parent_pt);
                          pt = parent_pt;
                          parent_pt = pt->parent;
                    }

                    // Case 3:  pt is right child of its parent left-rotation required 


                    leftrotate(grand_parent_pt);

                    temp = parent_pt->color;
                    parent_pt->color = grand_parent_pt->color;      // swap parent and grandparents color
                    grand_parent_pt->color = temp;

                    pt = parent_pt;
              }
          }
  }
        root->color = 'b';  //set root colour to black in case it was changed
}


// Delete from tree
//parameters is TheID
void RBtree::del(long int key)
{
            if (root == nill)
            {
              return;                 //Tree is empty ; do nothing
            }

            node *p;
            p = root;
            
            int found = 0;

            while (p != nill && found == 0)
            {
              if (p->ID == key)
                    found = 1;
              if (found == 0)                                     //search for the node
              {
                  if (p->ID < key)
                    p = p->right;
                  else
                    p = p->left;
              }
            }

            if (found == 0)
            {
              return;                                           // node does not exist 
            }

            else
            {
                    node * x = nill;
                    node * y = nill;

                    if (p->left == nill || p->right == nill)   //check if the node is degree 1
                      y = p;
                    else
                      y = successor(p);                        //if degree the node being deleted is a degree 2 node,replace the node being deleted 
                                                               // with it's sucessor and then delete the sucessor

                    if (y->left != nill)
                      x = y->left;
                    else
                      x = y->right;


                    if (x != nill)                               // the node being deleted is degree 1
                    {
                          x->parent = y->parent;
                          if (nill == y->parent)
                          {
                                root = x;                           //root to be delete
                          }
                          else
                          {
                                if (y == y->parent->left)
                                {
                                    x->parent->left = x;
                                }
                                else                                    //break the link and make the child of deleted node a child of the parent 
                                {                                       //of the node being deleted
                                    x->parent->right = x;
                                }
                          }
                    }

                    else                                          //the node being deleted is a leaf
                    { 
                        node *temp;

                        if (y->parent != nill)
                        { 
                            node * temp;

                            if (y == y->parent->left)
                            {
                                y->parent->left = nill;  //link the parent to the nill node
                            }
                            else
                            {
                                y->parent->right = nill;
                            }                       
                        }
                        else
                        {
                            return;         //deleting root, do nothing
                        }
                    }

                    if (y != p)
                    {
                        p->ID = y->ID;              //switch data
                        p->count = y->count;
                    }

                    if (y->color == 'b')
                    {
                        delfix(x);
                                        //if the deleted node is black we need to fix any violations that occur
                    }
            }
}

//Fix violations caused by deletion of node
void RBtree::delfix(node *x)
{

  while (x != root && x->color == 'b')   //move up the tree till the element that needs to be fixed becomes the root or it's colour becomes black
  {
            node * w = nill;
            if (x->parent->left == x)
            {
                  w = x->parent->right;

                  if (w->color == 'r')
                  {
                    w->color = 'b';
                    x->parent->color = 'r';
                    leftrotate(x->parent);
                    w = x->parent->right;
                  }

                  if (w->left->color == 'b' && w->right->color == 'b')
                  {
                    w->color = 'r';
                    x = x->parent;
                  }
                  else
                  {
                    if (w->right->color == 'b')                            //Check the four different cases and accordingly perform
                    {                                                       // LL, RR, RR followed by LL or LL followed by RR rotations
                      w->left->color = 'b';
                      w->color = 'r';
                      rightrotate(w);
                      w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 'b';
                    w->right->color = 'b';
                    leftrotate(x->parent);
                    x = root;
                  }
            }

            else
            {
                  w = x->parent->left;
                  if (w->color == 'r')
                  {
                    w->color = 'b';
                    x->parent->color = 'r';
                    rightrotate(x->parent);
                    w = x->parent->left;
                  }

                  if (w->right->color == 'b' && w->left->color == 'b')
                  {
                    w->color = 'r';
                    x = x->parent;
                  }
                  else
                  {
                    if (w->left->color == 'b')
                    {
                      w->right->color = 'b';
                      w->color = 'r';
                      leftrotate(w);
                      w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = 'b';
                    w->left->color = 'b';
                    rightrotate(x->parent);
                    x = root;
                  }
            }
  }
  x->color = 'b';         //Make root black in case it is red
}

void RBtree::leftrotate(node *p)   // represents LL rotation
{
  node *y = p->right;
  p->right = y->left;

  if (p->right != nill)
    p->right->parent = p;

  y->parent = p->parent;

  if (p->parent == nill)
    root = y;
  else if (p == p->parent->left)
    p->parent->left = y;
  else
    p->parent->right = y;

  y->left = p;
  p->parent = y;
}


void RBtree::rightrotate(node *p) //represents RR rotation
{
  node *y = p->left;
  p->left = y->right;

  if (p->left != nill)
    p->left->parent = p;

  y->parent = p->parent;

  if (p->parent == nill)
    root = y;
  else if (p == p->parent->left)
    p->parent->left = y;
  else
    p->parent->right = y;

  y->right = p;
  p->parent = y;
}


//returns the successor of the node
node* RBtree::successor(node *x)
{
  if (x->right != nill)
  {
    x = x->right;
    while (x->left != nill)     // If right subtree is not nill, successor is smallest element in the right subtree
      x = x->left;
    return x;
  }

  node * y = x->parent;           // if right subtree is nill, move up till we reach the root or the node becomes left child of it's parent
  while (y != nill && x == y->right)
  {
    x = y;
    y = y->parent;
  }

  return y;
}


//returns the predecessor of the node
node* RBtree::predecessor(node *x)
{
  if (x->left != nill)
  {
    x = x->left;
    while (x->right != nill)
      x = x->right;
    return x;
  }

  node * y = x->parent;

  while (y != nill && x == y->left)
  {
    x = y;
    y = y->parent;
  }
  return y;
}

// modifies the counter of a given key
//parameters are TheID, delta for the counter, and an integer func.
//if func = 1 increase counter; if func = 0 decrease counter
void RBtree::modifycounter(long int key, long int delta, int func)
{
  if (root == nill) //if tree is empty
  {
    if (func == 1)
    {
      insert(key, delta);
      cout << delta << "\n";        //If tree is empty insert into tree or print 0 
    }
    else
      cout << "0\n";
    return;
  }

  node *p = root;
  int found = 0;

  while (p != nill && found == 0) // find the node
  {
    if (p->ID == key)
      found = 1;
    if (found == 0)
    {
      if (p->ID<key)
        p = p->right;
      else
        p = p->left;
    }
  }

  if (found == 0) // if node doesn't exist
  {
    if (func == 1)
    {
      insert(key, delta);
      cout << delta << '\n';
      return;
    }
    else                                //if node doesn't exist insert into tree or print 0
    {
      cout << "0" << '\n';
    }
  }

  else // if node is found 
  {
    if (func == 1)
    {
      p->count = p->count + delta;
      cout << p->count << '\n';
    }
    else
    {
      p->count = p->count - delta;       // if node is found modify it's count value
      if (p->count <= 0)
      {
        del(key);
        cout << "0" << '\n';              //if value of count becomes less than 1 remove the node from the tree and print 0
        return;
      }
      cout << p->count << '\n';
    }
  }
}

// intializes the tree
//parameters = array with keys and counters; start; end; father->pointer to parent; h->height of tree; check->height of last level of tree

node* RBtree::initialize(long int arr[][2], long int start, long int end, node* father, int h, int check)
{
  if (start > end)
  {
    return nill;
  }
   
  //Get the middle element and make it root 
  int mid = (start + end) / 2;
  node *element = new node;
  h = h + 1;

  element->parent = father;
  element->ID = arr[mid][0];
  element->count = arr[mid][1];
  element->height = h;

  if (h > check)            //if the element is on the last level of tree make it red
    element->color = 'r';
  else
    element->color = 'b';

  // Recursively construct the left subtree and make it left child of root
  element->left = initialize(arr, start, mid - 1, element, h, check);

  // Recursively construct the right subtree and make it right child of root
  element->right = initialize(arr, mid + 1, end, element, h, check);

  return element;
}


//Returns the sum of counts of ID's within a given range
//Parameters = pointer to root; low and high of range
long int RBtree::inrange(node *root, long int low, long int high)
{
  // Base case
  if (root == nill)
    return 0;

  if (root->ID == high && root->ID == low)       // Special case to improve efficiency 
    return root->count;

  if (root->ID <= high && root->ID >= low)      // If current node is in range, then add it to count and recur for left and right children
    return root->count + inrange(root->left, low, high) + inrange(root->right, low, high);
  else if (root->ID < low)                      // If current node is smaller than low, then recur for right child
    return inrange(root->right, low, high);
  else                                          // Else recur for left child
    return inrange(root->left, low, high);
}


//Find a node and return it's count
void RBtree::searchcount(long int x)
{
  if (root == nill)
  {
    cout << "0" << '\n';      //print 0 if tree is empty
    return;
  }

  node *p = root;
  int found = 0;

  while (p != nill&& found == 0)
  {
    if (p->ID == x)
      found = 1;
    if (found == 0)
    {
      if (p->ID<x)
        p = p->right;
      else
        p = p->left;
    }
  }
  if (found == 0)
    cout << "0" << '\n';
  else                        //if not found print 0 else print the count
  {
    cout << p->count<<'\n';
  }
}


// search for a node and return it's sucessor or predecessor 
//if func = p ->return predecessor; func = n ->return sucessor
void RBtree::search_pn(long int x, char func)
{
  if (root == nill) // if tree is empty
  {
    cout << "0 0" << '\n';
    return;
  }

  node *p = root;
  node *fail = nill;
  node *print = nill;
  int found = 0;

  while (p != nill && found == 0) // find the node keeping track of failure node in case element is not present
  {
    if (p->ID == x)
      found = 1;
    if (found == 0)
    {
      if (p->ID<x)
      {
        if (p->right == nill)
        {
          fail = p;
        }
        p = p->right;
      }
      else
      {

        if (p->left == nill)
        {
          fail = p;
        }
        p = p->left;
      }
    }
  }

  if (found == 0) // if element is not found
  {
    if (func == 'p')
    {
      if (fail->ID < x) // if ID of failure node is less than TheID it will be the predecessor
      {
        if (fail != nill)
          cout << fail->ID << " " << fail->count << '\n';
        else
          cout << "0 0\n";
      }
      else // the predecessor will be the predecessor  of TheID
      {
        print = predecessor(fail);

        if (print == nill)
          cout << "0 0" << '\n';
        else
        {
          cout << print->ID << " " << print->count << '\n';
        }
      }
    }

    if (func == 'n')
    {

      if (fail->ID > x) // if ID of failure node is more than TheID it will be the sucessor
      {
        if (fail != nill)
          cout << fail->ID << " " << fail->count << '\n';
        else
          cout << "0 0\n";
      }
      else // the sucessor will be the sucessor of TheID
      {
        print = successor(fail);

        if (print == nill)
          cout << "0 0" << '\n';
        else
        {
          cout << print->ID << " " << print->count << '\n';
        }
      }
    }
  }

  if (found == 1) // if element is found
  {
    if (func == 'n')
    {
      print = successor(p);
      if (print != nill)
      {
        cout << print->ID << " ";
        cout << print->count << '\n';
      }
      else
        cout << "0 0\n";
    }

    if (func == 'p')
    {
      print = predecessor(p);
      if (print != nill)
      {
        cout << print->ID << " ";
        cout << print->count << '\n';
      }
      else
        cout << "0 0\n";
    }
    return;
  }
}

long int data[200000000][2]; //initialize global variable

int main(int argc, char** argv)
{
      int y = 0;
      long int k, delta, low, high, count, n;

      char* input = argv[1];
      std::string sbcd;

      ifstream inputfile;
      inputfile.open(input, ios::in);

      inputfile >> n;

      long int ck = log2(n); //find the maximum height of treee
      
      for (int i = 0; i<n; i++) // intialize the array
      {
        inputfile >> ::data[i][0];
        inputfile >> ::data[i][1];
      }


      node* init = new node;
      node* t = new node;
   
      RBtree obj;   // create object of rbtree class

      init = obj.nill;  // create sentinels
      t = obj.nill;

      t = obj.initialize(::data, 0, n - 1, init, 0, ck); // intialize the tree with root at t

      obj.root = t; // set root of the tree object as t

      // read commands using standard input and call appropriate functions
      // do
      // {
      //   cin >> sbcd;

      //   if (sbcd == "increase")
      //   {
      //     cin >> k >> delta;
      //     obj.modifycounter(k, delta, 1);
      //   }
      //   else if (sbcd == "reduce")
      //   {
      //     cin >> k >> delta;
      //     obj.modifycounter(k, delta, 0);
      //   }
      //   else if (sbcd == "inrange")
      //   {
      //     cin >> low;
      //     cin >> high;

      //     count = obj.inrange(obj.root, low, high);
      //     cout << count << '\n';
      //   }
      //   else if (sbcd == "next")
      //   {
      //     cin >> k;
      //     obj.search_pn(k, 'n');
      //   }
      //   else if (sbcd == "previous")
      //   {
      //     cin >> k;
      //     obj.search_pn(k, 'p');
      //   }
      //   else if (sbcd == "count")
      //   {
      //     cin >> k;
      //     obj.searchcount(k);
      //   }
      //   else if (sbcd == "quit")
      //   {
      //     y = 1;
      //   }
      //   else
      //   {
      //     cout << "Enter a valid command" << "\n";
      //   }

      // } while (y!= 1);
      return 1;
}
