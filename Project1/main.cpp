#include <iostream>
#include <string>
#include <vector>
#include <time.h>

/*
* Function to get a random integer used for intialisation of a Node
* Basic enough so that each node should be unique
*/

int RandomInt()
{
	return rand();
}

/*
* Function to get a random string used for intialisation of a Node
* Basic enough so that each node should be unique
*/
std::string RandomString(int size)
{
	const std::string character = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::string str;
	for (int i = 0; i < size; i++)
	{
		str += character[rand() % 63];
	}

	return str;
}

/*
*  A Base class of Node so we can use this class as a way of storing varying type of TreeNode within TreeNode
*	virtual function have to function will be overreden in the derived class
*/
class BaseNode
{
public:
	BaseNode() {}
	virtual ~BaseNode() {}
	virtual void Output() {}
	virtual void OutputChildren() {}
	virtual void SetParent(BaseNode*) {}
	virtual void Add(BaseNode*) {}
	virtual void CleanUp() {}
	virtual BaseNode* Find(char ID) { return nullptr; }
};


/*
* A templated class that inherits from the BaseNode class
*		- m_Name varible that will be used to find a specific Node and an identifier
*		- m_Data is of type T so we can store a variety of types (int and string etc.)
*		- m_Children this vector will hold pointers to BaseNode class so this node can have a variety of types 
*		  and not be limited by the current classes templated type using pointers so we can override the base class
*
*/
template<typename T>
class TreeNode : public BaseNode
{
private:
	char m_Name;
	T m_Data;

	std::vector<BaseNode*> m_Children;
public:
	/*
	* When the node is created we will call Output to display the nodes data
	* in the console so we can check the tree structure in console as its being created
	*/
	TreeNode(char name, T value) : m_Name(name), m_Data(value) 
	{ 
		Output(); 
	}

	/*
	* Called when we destroy because we are using pointers and initialising them with
	* new we have to clean up for safety
	*/
	~TreeNode()
	{
		/*
		* Iterate through the Children call the CleanUp function
		*/
		for (int i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->CleanUp();
		}
		/*
		* Finally clean up this Nodes Vector of BaseNode ptrs
		*/
		CleanUp();
	}

	/*
	* Output the Nodes name and value to the console
	*/
	void Output()
	{
		std::cout << "Node Name : " << m_Name << "\t"
			<< "Node Value : " << m_Data << std::endl;
	}

	/*
	* Output this current Node then Output each Children aswell
	*/
	void OutputChildren()
	{
		Output();
		for (int i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->OutputChildren();
		}
	}

	/*
	* Add a new BaseNode class to the m_Children vector
	*/
	void Add(BaseNode* child)
	{ 
		m_Children.push_back(child); 
	}

	/*
	* Function that used to find and return the node that has the same m_Name as the char ID
	*/
	BaseNode* Find(char ID)
	{
		/*
		* If this Node's m_Name value is equal to the char ID we return this
		*/
		if (m_Name == ID)
		{
			return this;
		}
		/*
		* Create a node initialised to nullptr so if we dont find a node with corresponding
		* tag we return null
		*/
		BaseNode* node = nullptr;
		/*
		* Iterate through the children to check if the node with the corresponding ID is found
		* only check the children if the node == null so we dont override the found ptr with null in 
		* any of the future children
		*/
		for (unsigned int i = 0; i < m_Children.size(); ++i)
		{
			if (node == nullptr)
			{
				node = m_Children[i]->Find(ID);
			}
		}
		return node;
	}

	/*
	* Iterate through the Children and clean up the pointers
	* Then clear the vector
	*/
	void CleanUp()
	{
		for (int i = 0; i < m_Children.size(); ++i)
		{
			delete (m_Children[i]);
		}
		m_Children.clear();
	}
};


int main()
{
	{
		/*
		* Initialise srand() so we should get differnt random result each time program is ran
		*/
		srand(time(nullptr));

		/*
		*  Setup up the tree from the root to the given design
		*         A
		*       / | \
		*      /  |  \
		*  	  /   |   \
		*  	 B    C    D
		*  / | \  |    | \
		*  E F G  H    I  J
		*             /|\
		*            K L M
		* To add nodes to the right node we first find the Node with the char tag then
		* add a child to that Node we intialise the now with the right char tage and a
		* random type alternating between int and strings as specified
		*/
		TreeNode<std::string>* Root = new TreeNode<std::string>('A', RandomString(10));
		Root->Add(new TreeNode<int>('B', RandomInt()));
		Root->Add(new TreeNode<std::string>('C', RandomString(10)));
		Root->Add(new TreeNode<int>('D', RandomInt()));
		Root->Find('B')->Add(new TreeNode<std::string>('E', RandomString(10)));
		Root->Find('B')->Add(new TreeNode<int>('F', RandomInt()));
		Root->Find('B')->Add(new TreeNode<std::string>('G', RandomString(10)));
		Root->Find('C')->Add(new TreeNode<int>('H', RandomInt()));
		Root->Find('D')->Add(new TreeNode<std::string>('I', RandomString(10)));
		Root->Find('D')->Add(new TreeNode<int>('J', RandomInt()));
		Root->Find('I')->Add(new TreeNode<std::string>('K', RandomString(10)));
		Root->Find('I')->Add(new TreeNode<int>('L', RandomInt()));
		Root->Find('I')->Add(new TreeNode<std::string>('M', RandomString(10)));

		/*
		* Search for a Node with a user inputed tag 
		* if we dont find them we dont output anything
		* user can keep entering a tag until they enter 0
		*/
		char idTag;
		int numberOfInputs(3);
		std::cout << "Enter Search Tag (Enter 0 to Exit Application) : ";
		std::cin >> idTag;
		while (idTag != '0')
		{
			if (Root->Find(idTag))
			{
				std::cout << "Search Result - " << "\t";
				Root->Find(idTag)->Output();
			}
			else
			{
				std::cout << "Didn't Find Node " << idTag << std::endl;
			}

			std::cout << "Enter Search Tag (Enter 0 to Exit Application) : ";
			std::cin >> idTag;
		}

		/*
		* Clean up the Root Pointer
		*/
		delete Root;
		Root = nullptr;
	}

	return 0;
}