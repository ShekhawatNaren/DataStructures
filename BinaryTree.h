#pragma once
#include <memory>
#include <utility>

namespace CustomBinaryTree {
	template <typename KeyType,typename ValueType>
	/*
	Binary Node with smart pointers as left and right child node
	So that memory management is not a problem as smart pointer works on RAII concept and 
	takes care of freeing resources once they are no longer needed.
	Chosen Unique pointer instead of shared pointer as performance wise it is faster
	and as good as a raw pointer, also a node in binary tree is execlusive ownership of its parent node
	*/
	struct BinaryNode {
		KeyType key_;
		std::pair<KeyType, ValueType> value_;
		std::unique_ptr<BinaryNode<KeyType, ValueType>> left_;
		std::unique_ptr<BinaryNode<KeyType, ValueType>> right_;
		//parent_ cannot be a unique pointer as the node's ownership
		//which it will point to is with it's parent left_ or right_ unique pointer
		BinaryNode* parent_; 
		BinaryNode(KeyType k, ValueType v) :key_{ k }, left_{ nullptr }, right_{ nullptr }, parent_{ nullptr }
		{
			value_ = std::make_pair(k, v);
		}
	};

	//forward declare the iterator classes
	template<typename KeyType, typename ValueType>
	class binaryTree_iterator;

	template<typename KeyType, typename ValueType>
	class binaryTree_const_iterator;

	template <typename KeyType, typename ValueType>
	class BinaryTree {
	public:
		BinaryTree() :root_{ nullptr }
		{}
		//Only can be moved
		BinaryTree(BinaryTree&& tree) {
			root_ = std::move(tree.root_);
		}
		//Don't allowing copying the tree as we are using unique pointers
		BinaryTree(const BinaryTree& tree) = delete;
		BinaryTree& operator=(const BinaryTree& tree) = delete;

		//type defs
		using TreeNode = BinaryNode<KeyType, ValueType>;
		using iterator = binaryTree_iterator<KeyType, ValueType>;
		using const_iterator = binaryTree_const_iterator<KeyType, ValueType>;

		void insert(KeyType k, ValueType v) {
			std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>(k,v); //Only in C++14
			TreeNode* temp = root_.get();
			TreeNode* prev = nullptr;

			//post this loop prev will point to the node whose child this new node will become
			bool isLeft = false; //flag to tell if the new node will be left child or right child
			while (temp != nullptr) {
				prev = temp;
				if (k < temp->key_) {
					temp = temp->left_.get();
					isLeft = true;
				}
				else {
					temp = temp->right_.get();
					isLeft = false;
				}
			}
			node.get()->parent_ = prev;

			if (prev == nullptr) {
				root_ = std::move(node);
			}
			else if (isLeft) {
				prev->left_ = std::move(node);
			}
			else {
				prev->right_ = std::move(node);
			}
		}
		//Runs in O(h) time where h is the height of the tree (log(n))
		void erase(const KeyType& key) {
			iterator it = find(key);
			if (it != end()) {
				erase(it);
			}
		}

		//Runs in Constant time
		void erase(iterator position) {
			if (position.current_->left_ == nullptr) {
				//first node is the node which has to be replaced
				//second is the node which will replace the first node
				transplant(position.current_, position.current_->right_.get());
			}
			else if (position.current_->right_ == nullptr) {
				//first node is the node which has to be replaced
				//second is the node which will replace the first node
				transplant(position.current_, position.current_->left_.get());
			}
			else {
				iterator nextMin = Min(position.current_->right_.get());

				//Check if this min node is right node of the node to be deleted
				if (nextMin.current_->parent_ != position.current_) {
					//the minimum node found(nextMin) will definatly have only right child
					//because if there had been any left child then Min() would have returned that
					std::unique_ptr<TreeNode> replacedMin = transplant(nextMin.current_, nextMin.current_->right_.get());
					replacedMin.get()->right_ = std::move(position.current_->right_);
					replacedMin.get()->right_.get()->parent_ = replacedMin.get();

					//fix the left and parent of the new successor
					replacedMin.get()->left_ = std::move(position.current_->left_);
					replacedMin.get()->left_.get()->parent_ = replacedMin.get();
					replacedMin.get()->parent_ = position.current_->parent_;

					if (position.current_->parent_->right_.get() == position.current_) {
						position.current_->parent_->right_ = std::move(replacedMin);
					}
					else {
						position.current_->parent_->left_ = std::move(replacedMin);
					}

				}
				else {
					//if yes it means there is not left child for the min node and it can replace
					//the node to be deleted
					//Move the ownership of the leftChild of the node to be deleted
					std::unique_ptr<TreeNode> leftChild = std::move(position.current_->left_);
					transplant(position.current_, nextMin.current_);

					//After replacing the next minimum node at node to be deleted position
					//add the left child to the new node
					nextMin.current_->left_ = std::move(leftChild);
					nextMin.current_->left_.get()->parent_ = nextMin.current_;
				}
			}
		}

		/* 
		Searches the given key in the binary tree
		Returns: An iterator to the element, if an element with specified key is found, or BinaryTree::end otherwise.
		*/
		iterator find(const KeyType& key) {
			TreeNode* temp = root_.get();
			if (root_ != nullptr){
				while (temp != nullptr ) {
					if (key == temp->key_) {
						break;
					}
					else if (key < temp->key_) {
						temp = temp->left_.get();
					}
					else {
						temp = temp->right_.get();
					}
				}
			}
			return iterator(temp);
		}
		iterator Min() {
			return Min(root_.get());
		}
		iterator Max() {
			if (root_ == nullptr) {
				return end();
			}
			TreeNode* temp = root_.get();
			while (temp->right_ != nullptr) {
				temp = temp->right_.get();
			}
			return iterator(temp);
		}
		
		//iterator related functionality
		//begin function returns iterator to the left most element of the tree
		//iteration using begin and end does an inorder traversal
		iterator begin() {
			TreeNode* temp = root_.get();
			while (temp != nullptr && temp->left_ != nullptr) {
				temp = temp->left_.get();
			}
			return iterator(temp);
		}
		iterator end() {
			return iterator(nullptr);
		}

		//cbegin function returns const iterator to the left most element of the tree
		//iteration using cbegin and cend does an inorder traversal
		const_iterator cbegin() {
			TreeNode* temp = root_.get();
			while (temp != nullptr && temp->left_ != nullptr) {
				temp = temp->left_.get();
			}
			return const_iterator(temp);
		}
		const_iterator cend() {
			return const_iterator(nullptr);
		}
	private:
		std::unique_ptr<TreeNode> root_;

		//This is to allow the iterator class the access to private section
		friend class binaryTree_iterator<KeyType, ValueType>;
		friend class binaryTree_const_iterator<KeyType, ValueType>;

		//first node is the node which has to be replaced
		//second is the node which will replace the first node
		std::unique_ptr<TreeNode> transplant(TreeNode* u, TreeNode* v) {
			//Save a pointer to u's parent as it might get deleted during
			//pointer movement later
			TreeNode* uParent = u->parent_;
			std::unique_ptr<TreeNode> nodeToBeDeleted;
			if (u->parent_ == nullptr) { //When u was root, now v becomes root
				if (u->right_.get() == v) {
					nodeToBeDeleted = std::move(u->right_);
				}
				else {
					nodeToBeDeleted = std::move(u->left_);
				}
				root_ = std::move(nodeToBeDeleted);
			}
			//depending on which side is u of its parent node
			//we need to either update left or right node of u's parent to v
			else if (u->parent_->left_.get() == u) {
				nodeToBeDeleted = std::move(u->parent_->left_);
				//two cases v either can be right to the node to be deleted or left
				//need to move the ownership as these are unique pointers
				if (nodeToBeDeleted.get()->right_.get() == v) {
					u->parent_->left_ = std::move(nodeToBeDeleted.get()->right_);
				}
				else {
					u->parent_->left_ = std::move(nodeToBeDeleted.get()->left_);
				}
			}
			else {
				nodeToBeDeleted = std::move(u->parent_->right_);
				//two cases v either can be right to the node to be deleted or left
				//need to move the ownership as these are unique pointers
				if (nodeToBeDeleted.get()->right_.get() == v) {
					u->parent_->right_ = std::move(nodeToBeDeleted.get()->right_);
				}
				else {
					u->parent_->right_ = std::move(nodeToBeDeleted.get()->left_);
				}
			}

			if (v != nullptr) {
				v->parent_ = uParent;
			}

			return std::move(nodeToBeDeleted);
		}

		//given the root of the tree it returns iterator to the min element in the tree
		iterator Min(TreeNode* rootNode) {
			if (rootNode == nullptr) {
				return end();
			}
			while (rootNode->left_ != nullptr) {
				rootNode = rootNode->left_.get();
			}
			return iterator(rootNode);
		}

	};

	/******** Iterators *******/
	template<typename KeyType, typename ValueType>
	class binaryTree_iterator {
	public:
		using TreeNode = BinaryNode<KeyType, ValueType>;
		using iterator = binaryTree_iterator<KeyType, typename ValueType>;
		//Equaltiy check operators
		bool operator == (const iterator& rhs) const{
			return current_ == rhs.current_;
		}
		bool operator != (const iterator& rhs) const{
			return !(current_ == rhs.current_);
		}

		//de-reference operator
		auto& operator* () const{
			return current_->value_;
		}

		//pre-increment operator
		iterator& operator ++() {
			nextNode();
			return *this;
		}

		//post-increment operator
		iterator operator ++(int) {
			iterator temp = *this;
			nextNode();
			return temp;
		}

		//Necessary traits
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ValueType value_type;
		typedef int difference_type;
	protected:
		binaryTree_iterator(TreeNode* curr) {
			current_ = curr;
		}
	private:
		friend class BinaryTree<KeyType, ValueType>;
		TreeNode* current_;
		//Sets the current_ to next element in the in order traversal
		void nextNode() {
			if (current_->right_.get() != nullptr) {
				current_ = current_->right_.get();
				while (current_->left_.get() != nullptr) {
					current_ = current_->left_.get();
				}
			}
			else {
				TreeNode* p = current_->parent_;
				while (p != nullptr && current_ != p->left_.get()) {
					current_ = p;
					p = current_->parent_;
				}
				current_ = p;
			}
		}
	};

	template<typename KeyType, typename ValueType>
	class binaryTree_const_iterator {
	public:
		using TreeNode = BinaryNode<KeyType, ValueType>;
		using const_iterator = binaryTree_const_iterator<KeyType, typename ValueType>;
		//Equaltiy check operators
		bool operator == (const const_iterator& rhs) const {
			return current_ == rhs.current_;
		}
		bool operator != (const const_iterator& rhs) const {
			return !(*this == rhs);
		}

		//de-reference operator
		const auto& operator* () const {
			return current_->value_;
		}
		//pre-increment operator
		const const_iterator& operator ++() {
			nextNode();
			return *this;
		}

		//post-increment operator
		const const_iterator operator ++(int) {
			iterator temp = *this;
			nextNode();
			return temp;
		}

		//Necessary traits
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ValueType value_type;
		typedef int difference_type;
	protected:
		binaryTree_const_iterator(BinaryNode<KeyType,ValueType>* curr) {
			current_ = curr;
		}
	private:
		//Allow the Binary tree class to access private members
		friend class BinaryTree<KeyType,ValueType>;
		BinaryNode<KeyType,ValueType>* current_;

		//Sets the current_ to next element in the in order traversal
		void nextNode() {
			if (current_->right_.get() != nullptr) {
				current_ = current_->right_.get();
				while (current_->left_.get() != nullptr) {
					current_ = current_->left_.get();
				}
			}
			else {
				TreeNode* p = current_->parent_;
				while (p != nullptr && current_ != p->left_.get()) {
					current_ = p;
					p = current_->parent_;
				}
				current_ = p;
			}
		}
	};
}