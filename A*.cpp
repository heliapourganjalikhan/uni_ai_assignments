
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>
#include <fstream>

// Informed- A* Search
using namespace std;

// Statespace		
class Statespace_Tree
{
      private:
      struct Node
      {
          // "pointer" to the parenet of this node
          Node* parent;
          
          // the 4 possible neighbor nodes of this node represented by "pointers"
          Node* anode;
          Node* bnode;
          Node* cnode;
          Node* dnode;
          
          vector<int> state;  // the state of the puzzle 
          
          int idx;            // the index of blank space in the puzzle
          int dist;           // the distance of the current node from the root
          int cost;           // the total cost associated with this node (heuristic + distance)
      };
      
      vector<Node*> S;        // an array container to hold all generated nodes (OPEN LIST)
      
      vector<int> initial;    // the starting state of the puzzle
      vector<int> goal;       // the target state of the puzzle
      
      Node* r;                // a pointer to the root of the tree
      int h;                  // represent the heuristic type  
      bool got;               // a flag indicating whether the goal state has been found
	
	  public:
      Statespace_Tree()       // the constructor of the tree , initializes the tree by setting the root to NULL
      {
          r = NULL;            
      }
      
      // Functions Prototypes
      void generate_root(vector<int>); // creates the root node using the initial state provided
      void generate();                 // generates child nodes from the current node based on possible moves
      void insert(Node*);              // insert a new node
      void input_state();              // input the puzzle's initial and goal state (from the user) 
      void print_state(Node*);         // prints the state of the puzzle at a given node
      void Move(Node*, Node*, char);   // executes a move between two nodes based on a given direction
      void check_goal(Node*);          // checks if a hiven node's state matches the goal state
      void sorting();                  // sorts the list of nodes(S) based on the total cost(heuristic+distance)
      int s1(Node*);                   // evaluates the heuristic cost for a given node
      
      // checks if the tree is empty (i.e., if the root node is NULL). 
      // this is critical for initialization of  the tree
      bool isEmpty() const             
      {
           return r==NULL;
      }
              	
};
/*  Notes for Further Implementation:
 >Move generation: generate() and Move() would implement the logic to determine valid puzzle moves (e.g., swapping tiles in a sliding puzzle).
 >Heuristics: s1(Node*) could calculate a heuristic like Manhattan distance or misplaced tiles.
 >Sorting: sorting() might order the nodes in S by their heuristic costs to prioritize the most promising paths.
 >Backtracking: Using parent pointers in Node to trace the solution path once the goal is reached.*/

// Functions Definitions

// so let initialize the root node :)
void Statespace_Tree::generate_root(vector<int> state)
{
     Node* n1 = new Node; // a pointer to a memory location that holds a dynamically allocated Node object
     
     // save the initial puzzle state in the root node
     for(int i=0; i<(signed)state.size(); i++)
     {
          n1->state.push_back(state[i]);    
     }
     
     n1->cost = s1(n1);  // calculating the heuristic cost of the root node
     n1->dist =1;        // setting the distance from the root to , meaning this is the root node of the tree
     
     // the root node has no parent in general
     n1->parent = NULL;
     // and at this point it has no neighbors yet
     n1->anode = NULL;
     n1->bnode = NULL;
     n1->cnode = NULL;
     n1->dnode = NULL;
     
     // finding the blank space index , which is critical for move generation
     for(int i=0; i<(signed)state.size(); i++)
     {
          if(state[i] == 0)
          {
              n1->idx=i;    
          } 
     }
     
     // if the the tree is empty, then add the root node to it and initialize the tree
     if(isEmpty())  
     {
         r = n1;
         S.push_back(r);         
     }   
}

// Generating tree
/*
   this function is responsible for building the "statespace" tree by processing nodes iteratively.
   It uses a "queue-like" behavior to explore states and build the tree until there are no more states to process. 
*/
void Statespace_Tree::generate()
{
     input_state();          // prompts the user to input the initial and goal state of the puzzle
     generate_root(initial); // creates the root node
     got=false;              // indicates that the goal state has not been found yet 
     
     Node* curr;             // a pointer to hold the node currently being processed
     h=1;                    // initialize the heuristic type
     
     // The depth tracker
     int t=0;                // Tracks the maximum depth (dist) of nodes processed so far.
     
     while(S.size() > 0)
     {
          curr = S.front();  // retrives the first node (FIFO behavior)
          S.erase(S.begin());// removes the processed node to avoid duplicate processing
          insert(curr);      // add the current node to the tree structure
          
          // update the max depth of the nodes processed so far
          if(curr->dist > t)
          {
              t = curr->dist;
          }
     }
}

/*
 This insert function is a critical part of building the state-space tree. 
 It generates child nodes for a given node (curr) by attempting all possible moves (up, right, down, left),
 calculates their properties, and manages memory for the tree structure.
*/
void Statespace_Tree::insert(Node* curr)
{
    // creating all the possble neighbors candidates
     Node* n1 = new Node;
     Move(curr, n1, 'u');
     n1->dist = curr->dist +1;
     n1->cost = s1(n1) + n1->dist;
     n1->parent = curr;
     n1->anode = NULL;
     n1->bnode = NULL;
     n1->cnode = NULL;
     n1->dnode = NULL;
     
     Node* n2 = new Node;
     Move(curr, n2, 'r');
     n2->dist = curr->dist +1;
     n2->cost = s1(n2) + n2->dist;
     n2->parent = curr;
     n2->anode = NULL;
     n2->bnode = NULL;
     n2->cnode = NULL;
     n2->dnode = NULL;
     
     Node* n3 = new Node;
     Move(curr, n3, 'd');
     n3->dist = curr->dist +1;
     n3->cost = s1(n3) + n3->dist;
     n3->parent = curr;
     n3->anode = NULL;
     n3->bnode = NULL;
     n3->cnode = NULL;
     n3->dnode = NULL;
     
     Node* n4 = new Node;
     Move(curr, n4, 'l');
     n4->dist = curr->dist +1;
     n4->cost = s1(n4) + n4->dist;
     n4->parent = curr;
     n4->anode = NULL;
     n4->bnode = NULL;
     n4->cnode = NULL;
     n4->dnode = NULL;
     
    // check if the neighbors are valid add them to (S)  	
	if((curr->anode == NULL) && (n1->state.size() > 0))
    {
         curr->anode = n1;
         if(!got) 
           S.push_back(curr->anode);
    }
    else
         delete n1;
         
    if((curr->bnode == NULL) && (n2->state.size() > 0))
    {
         curr->bnode = n2;
         if(!got) 
           S.push_back(curr->bnode);
    }
    else
         delete n2;
         
    if((curr->cnode == NULL) && (n3->state.size() > 0))
    {
         curr->cnode = n3;
         if(!got) 
           S.push_back(curr->cnode);
    }
    else
         delete n3;
         
    if((curr->dnode == NULL) && (n4->state.size() > 0))
    {
         curr->dnode = n4;
         if(!got)  
            S.push_back(curr->dnode);
    }
    else
         delete n4;            	
                 	
    sorting();        // sort the neighbors according to the total cost
    check_goal(curr); /* after expanding the cerrent node and adding its neighbors to (S),
                         checks if the current state is the goal state*/
} 
       	
//Move Function, To move blocks
void Statespace_Tree:: Move(Node* curr, Node* n, char move)
{
     int a, idx;
     bool Notcorrect = false;
     
    // copy current state to new node
     for(int i=0; i<(signed)curr->state.size(); i++)
     {
             if(curr->state[i]==0)
             {
                  idx = i;                
             }
             n->state.push_back(curr->state[i]);
     }
     
     n->idx = idx;

    // prevent undo moves 
	if(curr->dist > 2)
	{
        if(idx == curr->parent->idx)
        {
            Notcorrect = true;
        }         
    }
    
    // switch statement for moves		
    switch(move)
    {
       case 'r':
         if((idx%3 !=2) && (!Notcorrect))
         {
             a = n->state[idx];
             n->state[idx] = n->state[idx+1];
             n->state[idx+1]=a;       
         }
         else
             n->state.erase(n->state.begin(), n->state.end());
             break;
       case 'l':
       if((idx%3 != 0) && (!Notcorrect))
       {
             a = n->state[idx];
             n->state[idx] = n->state[idx-1];
             n->state[idx-1]=a;       
         }
         else
             n->state.erase(n->state.begin(), n->state.end());
             break;            
       case 'u' :
       if((idx>2) && (!Notcorrect))
       {
            a = n->state[idx];
            n->state[idx] = n->state[idx-3];
            n->state[idx-3] = a;
       }
       else
            n->state.erase(n->state.begin(), n->state.end());
            break;
       case 'd' :
       if((idx<6) && (!Notcorrect))
       {
            a = n->state[idx];
            n->state[idx] = n->state[idx+3];
            n->state[idx+3] = a;
       }
       else
            n->state.erase(n->state.begin(), n->state.end());
            break;                      
    }
}

// Inputting Initial and final states from Input File
void Statespace_Tree::input_state()
{
     ifstream in("A2in1.txt");
     
     string p; // store each line read from the file
     string q; // store each individual value separated by spaces in the line
     int n=0;  // counter to track the number of values processed
     
     while(getline(in, p))
     {    
        // linestream: Creates a stream object to process the contents of p (the current line).
         istringstream linestream(p);
         
         while(getline (linestream, q, ' '))
         {
               n++;
               
               if(n<=9)
               {
                   initial.push_back(atoi(q.c_str()));
               }
               else
               {
                   goal.push_back(atoi(q.c_str()));
               }
         }
     }                               
}

// To print each state and moves
// when the goal is founded it will be sent here
void Statespace_Tree::print_state(Node* n)
{
     vector<Node*> v;
     ofstream fout("out.txt");
     
     while(n != NULL)
     {
        v.push_back(n); // add the current node to v
        n = n->parent;  // add the parent of the current node to be processed next
     }     	
     for(int b=v.size()-1; b>=0; b--) // iterate over v array in reverse order , which means to start from the root node
     { 
        for(int a=0; a<(signed)v[b]->state.size(); a++)
        {
            fout << v[b]->state[a] << " "; // print the result in output file
            cout << v[b]->state[a] << " "; // print the result in the console

            //go to the next line after each 3 characters
            if(a%3 == 2)
            {
               fout<<endl; 
               cout<<endl;
            }         
        }
        fout<<endl;
        cout<<endl;
     }
     // showing the message that the goal is found
     cout<<"Goal state found."<<endl;
     
    ifstream infile("out.txt");
    ofstream ofile("a*_out.txt");

     if (!infile.is_open() || !ofile.is_open()) {
        cerr << "Error opening file." << endl;
     }

     vector<vector<int>> states; // Vector to hold all states
     vector<int> blank_positions; // Vector to hold positions of blank tiles (0)

     string line;
     int num_states = 0; // Track the number of states read
     while (infile) {
         vector<int> state(9); // Single puzzle state (3x3 flattened to a vector of size 9)
         int blank_index = -1;

         // Read a state (9 integers)
         for (int i = 0; i < 9 && infile >> state[i]; i++) {
             if (state[i] == 0) {
                 blank_index = i; // Find the position of the blank tile
             }
         }

         if (state.size() == 9) { // Valid state
            states.push_back(state);        // Add state to states
            blank_positions.push_back(blank_index); // Add blank index
            num_states++;
         }
     }

     // Compare consecutive states to determine moves
     for (int i = 1; i < num_states; i++) {
         int prev_blank = blank_positions[i - 1];
         int curr_blank = blank_positions[i];

         // Determine the move based on blank tile's new position
         if (curr_blank == prev_blank + 1) {
             ofile << states[i-1][curr_blank] << " left" << endl;
         } else if (curr_blank == prev_blank + 3) {
             ofile << states[i-1][curr_blank] << " up" << endl;
         } else if (curr_blank == prev_blank - 1) {
             ofile << states[i-1][curr_blank] << " right" << endl;
         } else if (curr_blank == prev_blank - 3) {
             ofile << states[i-1][curr_blank] << " down" << endl;
         }
     }
     ofile << "number of moves: " << num_states -2 << endl;
     ofile << "Goal state found." << endl;

     infile.close();
     ofile.close();

    
}

// The Hamming distance
// The given function s1 calculates the heuristic cost for a given node based on the number of tiles
//that are not in their correct positions compared to the goal state.
int Statespace_Tree::s1(Node* n)
{
    int q=0;
    
    for(int a=0; a<(signed)n->state.size(); a++)
    {
          if(n->state[a] != goal[a])
          {
                q++;
          }
    }
    
    return q;
}          
                                     	
void Statespace_Tree::sorting()
{
     Node* n;
     
     for(int i=0; i<(signed)S.size()-1; i++)
     {
         for(int j=0; j<(signed)S.size()-1; j++)
         {
             if(S[j]->cost > S[j+1]->cost)
             {
                 n = S[j];
                 S[j] = S[j+1];
                 S[j+1] = n;
             }
         }
     }
}

//To check whether we reach the goal or not.
void Statespace_Tree::check_goal(Node* curr)
{
     bool check1=true, check2=true, check3=true, check4=true;
     
     for(int a=0; a<(signed)goal.size(); a++)
     {
          if(curr->anode != NULL)
          {
              if(curr->anode->state[a] != goal[a])
                    check1 = false;
          }
          else
                  check1=false;
                  
          if(curr->bnode != NULL)
          {
              if(curr->bnode->state[a] != goal[a])
                  check2 = false;
          }
          else 
                  check2=false;
                  
          if(curr->cnode != NULL)
          {
              if(curr->cnode->state[a] != goal[a])
                  check3 = false;
          }
          else 
                  check3=false;
          
          if(curr->dnode != NULL)
          {
              if(curr->dnode->state[a] != goal[a])
                  check4 = false;
          }
          else 
                  check4=false;
     }
     
     if(check1)
     {
          got=true;
          print_state(curr->anode);
     }
     else if(check2)
     {                                                                                 
          got=true;
          print_state(curr->bnode);
     }
     else if(check3)
     {
          got=true;
          print_state(curr->cnode);
     }
     else if(check4)
     {
          got=true;
          print_state(curr->dnode);
     }
     
     if(got)
     {
         while(!S.empty())
         {
             S.erase(S.begin());             
         }
     }
}



int main()
{
    Statespace_Tree t;
    t.generate();
    
    system("pause");
    return 0;
}