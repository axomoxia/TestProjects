#ifndef _binary_tree_h
#define _binary_tree_h

#include <memory>
#include <iostream>
#include <stack>
#include <memory>

template<typename T>
class binary_tree
{


	template<typename T_NODE>
	struct tree_node
		{
				std::shared_ptr<tree_node<T_NODE> > left_;
				std::shared_ptr<tree_node<T_NODE> > right_;


				explicit tree_node(const T_NODE& value) :  value_(value)
				{

				}

				void preOrder(std::ostream& out)
				{
					out << value_ << " ";
					if (left_)
					{
						left_->preOrder(out);
					}
					if (right_)
					{
						right_->preOrder(out);
					}
				}

				void inOrder(std::ostream& out)
				{
					if (left_)
					{
						left_->inOrder(out);
					}
					out << value_ << " ";

					if (right_)
					{
						right_->inOrder(out);
					}
				}
				void postOrder(std::ostream& out)
				{
					if (left_)
					{
						left_->postOrder(out);
					}
					if (right_)
					{
						right_->postOrder(out);
					}
					out << value_ << " ";
				}

				static void iterativeTraversal(std::ostream& outStr, const std::shared_ptr<tree_node <T_NODE> > root )
				{
					// start left...
					std::shared_ptr<tree_node <T_NODE> > cursor = root;
					if (root)
					{
						bool done = false;
						std::stack<std::shared_ptr<tree_node <T_NODE> > > s;
						while (!done)
						{
							if (cursor)
							{

								// go down left branch and push onto stack
								s.push(cursor);
								// until we get to the end left point
								cursor = cursor->left_;
							}
							else
							{
								if (!s.empty())
								{
									// pop off the stack
									cursor = s.top();
									s.pop();
									outStr << cursor->value_ << " ";
									cursor = cursor->right_;
								}
								else
								{
									done = true;
								}
							}
						}
					}
				}

		/*		enum colour {
						red,
						black
				};*/

				T_NODE value_;
				//colour colour_;
		};
public:
	void add(const T& value)
	{
		add_internal(value,root_);
	}
	void add_internal(const T& value, std::shared_ptr<tree_node <T> >& node)
	{
		if (!node)
		{
			// construct root - tree is empty
			node = std::make_shared<tree_node<T> >(value);
		}
		else
		{
			if (value < node->value_)
			{
				add_internal(value, node->left_);
			}
			else
			{
				add_internal(value, node->right_);
			}
		}
	}

	void iterativeTraversal(std::ostream& outStr)
	{
		if (root_)
		{
			tree_node<T>::iterativeTraversal(outStr,root_);

		}
		else
		{
			outStr << "NULL";
		}
		outStr << std::endl;
	}

	void preOrder(std::ostream& outStr)
	{
		if (root_)
		{
			root_->preOrder(outStr);
		}
		else
		{
			outStr << "NULL";
		}
		outStr << std::endl;
	}
	void inOrder(std::ostream& outStr)
		{
			if (root_)
			{
				root_->inOrder(outStr);
			}
			else
			{
				outStr << "NULL";
			}
			outStr << std::endl;
		}
	void postOrder(std::ostream& outStr)
		{
			if (root_)
			{
				root_->postOrder(outStr);
			}
			else
			{
				outStr << "NULL";
			}
			outStr << std::endl;
		}

private:






	std::shared_ptr<tree_node<T> > root_;


};

#endif
