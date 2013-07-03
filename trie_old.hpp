#ifndef BOOST_TRIE_HPP
#define BOOST_TRIE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <map>
#include <iterator>
#include <utility>
#include <cstdio>

namespace boost { namespace tries {

namespace detail {

template <typename Key, typename Value>
class trie_node {
	typedef Key key_type;
	typedef Value value_type;
	typedef key_type *key_ptr;
	typedef trie_node<key_type, value_type> node_type;
	typedef trie_node<key_type, value_type> * node_ptr;
	typedef std::map<key_type, node_ptr> children_type;
	typedef typename children_type::iterator child_iter;

	children_type child;

public:
	value_type *value; // how to initiate
	size_t node_count;
	size_t value_count;

	trie_node() : value(NULL), node_count(0), value_count(0) {}

	template<typename Iter>
	std::pair<node_ptr, bool> insert_impl(Iter cur_iter, Iter iter_end, 
			const value_type &_value, const key_type &cur_key)
	{
		if (cur_iter == iter_end)
		{
			if (!value)
			{
				value = new value_type(_value);
			}
			return std::make_pair(this, true);
		}
		//key_type cur_key = *cur_iter;
		//printf("%c\n", cur_key);
		child_iter ci = child.find(cur_key);
		if (ci == child.end())
		{
			ci = child.insert(std::make_pair(cur_key, new node_type())).first;
		}
		++cur_iter;
		return ci->second->insert_impl(cur_iter, iter_end,
				_value, *cur_iter);
	}
	template<typename Iter>
	std::pair<node_ptr, bool> insert(Iter cur_iter, Iter iter_end, const value_type &_value)
	{
		// use cur_iter as trait
		return insert_impl(cur_iter, iter_end, _value, *cur_iter);
		/*
		if (cur_iter == iter_end)
		{
			if (!value)
			{
				value = new value_type(_value);
			}
			return make_pair(*this, true);
		}
		key_type cur_key = *cur_iter;
		//printf("%c\n", cur_key);
		child_iter ci = child.find(cur_key);
		if (ci == child.end())
		{
			ci = child.insert(std::make_pair(cur_key, new node_type())).first;
		}
		return ci->second->insert(++cur_iter, iter_end,
				_value);
				*/
	}

	/*
	pair<node_type &, bool> insert(key_ptr key_string, int len, const value_type &_value)
	{
		if (len == 0)
		{
			if (!value)
			{
				value = new value_type(_value);
			}
			return make_pair(*this, true);
		}
		key_type cur_key = *key_string;
		//printf("%d %c\n", len, cur_key);
		child_iter ci = child.find(cur_key);
		if (ci == child.end())
		{
			ci = child.insert(std::make_pair(cur_key, new node_type())).first;
		}
		return ci->second->insert(key_string + 1, 
				len - 1, 
				_value);
	}
	*/
	/*
	node_ptr find_node(key_type *key_string, int len)
	{
		if (len == 0)
			return this;
		key_type cur_key = *key_string;
		//printf("%d %c\n", len, cur_key);
		child_iter ci = child.find(cur_key);
		if (ci != child.end())
		{
			return ci->second->find_node(key_string + 1, len - 1);
		}
		return NULL;
	}
	*/

	template<typename Iter>
	node_ptr find_node(Iter cur_iter, Iter iter_end)
	{
		if (cur_iter == iter_end)
			return this;
		key_type cur_key = *cur_iter;
		//printf("%c\n", cur_key);
		child_iter ci = child.find(cur_key);
		if (ci != child.end())
		{
			return ci->second->find_node(++cur_iter, iter_end);
		}
		return NULL;
	}
	/*
	node_ptr find_node(key_type *key_string, int len)
	{
		if (len == 0)
			return this;
		key_type cur_key = *key_string;
		//printf("%d %c\n", len, cur_key);
		child_iter ci = child.find(cur_key);
		if (ci != child.end())
		{
			return ci->second->find_node(key_string + 1, len - 1);
		}
		return NULL;
	}
	*/
	/*
	value_type *find(key_type *key_string, int len)
	{
		node_ptr node = find_node(key_string, len);
		return node->value;
	}
	*/
	template<typename Iter>
	value_type *find(Iter first, Iter last)
	{
		node_ptr node = find_node(first, last);
		return node->value;
	}

	template<typename Iter>
	node_ptr find_prefix(Iter first, Iter last)
	{
		return find_node(first, last);
	}

	node_ptr get_left_most_child()
	{
		if (child.empty())
			return NULL;
		node_ptr node = child.begin()->second;
		while (node->value == NULL)
		{
			
		}
	}

	template<typename Iter>
	bool erase(Iter cur_iter, Iter last)
	{

		return false;
	}

	void destroy()
	{
		for (child_iter ci(child.begin()); ci != child.end(); ++ci)
		{
			ci->second->destroy();
			delete ci->second;
		}
		child.clear();
		if (value)
		{
			delete value;
		}
	}

	~trie_node()
	{
		//puts("delete a node");
		destroy();
	}
};

} // namespace detail

template <typename Key, typename Value>
class trie {
	typedef Key key_type;
	typedef Value value_type;
	typedef trie<key_type, value_type> trie_type;
	typedef detail::trie_node<key_type, value_type> node_type;
	typedef key_type * key_ptr;
	typedef node_type * node_ptr;

	node_ptr root;

public:

	// iterator
	class iterator {
	public:
		iterator();
		iterator(const iterator&);
		~iterator();

		iterator& operator=(const iterator&);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
		/*
        bool operator<(const iterator&) const; //optional
        bool operator>(const iterator&) const; //optional
        bool operator<=(const iterator&) const; //optional
        bool operator>=(const iterator&) const; //optional
		*/
		

        iterator& operator++();
        iterator operator++(int); //optional
        iterator& operator--(); //optional
        iterator operator--(int); //optional
        iterator& operator+=(size_type); //optional
        iterator operator+(size_type) const; //optional
        friend iterator operator+(size_type, const iterator&) //optional
        iterator& operator-=(size_type); //optional            
        iterator operator-(size_type) const; //optional
        difference_type operator-(iterator) const; //optional

        reference operator*() const;
        pointer operator->() const;
        reference operator[](size_type) const; //optional
    };
    class const_iterator {
    public:
        typedef A::difference_type difference_type;
        typedef A::value_type value_type;
        typedef A::reference const_reference;
        typedef A::pointer const_pointer;
        typedef ???? iterator_category

        const_iterator ();
        const_iterator (const const_iterator&);
        const_iterator (const iterator&);
        ~const_iterator();

        const_iterator& operator=(const const_iterator&);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;
        bool operator<(const const_iterator&) const; //optional
        bool operator>(const const_iterator&) const; //optional
        bool operator<=(const const_iterator&) const; //optional
        bool operator>=(const const_iterator&) const; //optional

        const_iterator& operator++();
        const_iterator operator++(int); //optional
        const_iterator& operator--(); //optional
        const_iterator operator--(int); //optional
        const_iterator& operator+=(size_type); //optional
        const_iterator operator+(size_type) const; //optional
        friend const_iterator operator+(size_type, const const_iterator&) //optional
        const_iterator& operator-=(size_type); //optional            
        const_iterator operator-(size_type) const; //optional
        difference_type operator-(const_iterator) const; //optional

        const_reference operator*() const;
        const_pointer operator->() const;
        const_reference operator[](size_type) const; //optional
    };

    typedef std::reverse_iterator<iterator> reverse_iterator //optional
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator //optional

    X();
    X(const X&);
    ~X();

    X& operator=(const X&);
    bool operator==(const X&) const;
    bool operator!=(const X&) const;
    bool operator<(const X&) const; //optional
    bool operator>(const X&) const; //optional
    bool operator<=(const X&) const; //optional
    bool operator>=(const X&) const; //optional

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin(); //optional
    const_reverse_iterator rbegin() const; //optional
    const_reverse_iterator crbegin() const; //optional
    reverse_iterator rend(); //optional
    const_reverse_iterator rend() const; //optional
    const_reverse_iterator crend() const; //optional

	}



	// iterators still unavailable here
	
	trie() {
		root = new node_type();
	}

	template<typename Iter>
	std::pair<node_ptr, bool> insert(Iter first, Iter last, const value_type &value)
	{
		return root->insert(first, last, value);
	}
	template<typename Container>
	std::pair<node_ptr, bool> insert(const Container &container, const value_type &value)
	{
		return root->insert(container.begin(), container.end(), value);
	}
	std::pair<node_ptr, bool> insert(key_ptr key_string, int len, const value_type &value)
	{
		return root->insert(key_string, key_string + len, value);
	}

	// no iterator, 
	// and is should be put into "trie_map" and "trie_set",
	template<typename Container>
	value_type & operator[] (const Container &container)
	{
		return *(root->insert(container.begin(), container.end(), 
					value_type()).first->value);
	}

	/*
	 the interface is not suitable for all pointers here
	value_type & operator[] (key_ptr key_string)
	{
		return *(root->insert(key_string, key_string + strlen(key_string), 
					value_type()).first->value);
	}
	*/

	template<typename Container>
	value_type find(const Container &container)
	{
		return *root->find(container.begin(), container.end());
	}

	value_type find(key_type* key_string, int len)
	{
		return *root->find(key_string, key_string + len);
		//return root->find(key_string, len);
	}

	template<typename Container>
	node_ptr find_prefix(const Container &container)
	{
		return root->find_prefix(container.begin(), container.end());
	}

	node_ptr find_prefix(key_type* key_string, int len)
	{
		return root->find_prefix(key_string, key_string + len);
		//return root->find(key_string, len);
	}

	// is a function that find "string" no matter the node is with value or not needed here?

	bool erase(key_type* key_string, int len)
	{
		// not delete the node, but clear the value instead,
		// maybe need to change here, 
		// I think of a recursive version of erase() that,
		// on the path of in trie, 
		// check if the current node is without value and children
		// if it is true, delete the node
		node_ptr node = root->find_node(key_string, len);
		if (node == NULL)
			return false;
		if (node->value)
		{
			delete node->value;
		}
		node->value = NULL;
		return true;
	}

	void swap(trie_type &other)
	{
		node_ptr tmp = this->root;
		this->root = other.root;
		other.root = tmp;
	}

	bool clear()
	{
		return root->destroy();
	}

	size_t count()
	{

	}
	
	size_t size()
	{

	}

	~trie()
	{
		root->destroy();
		delete root;
		//puts("delete the whole trie");
	}

	/*
	 * the count() and size() function are not defined here, 
	 * the function of size() and count() may give different return values,
	 * one is the real size of trie - the number of nodes,
	 * the other is the number of "string" - value pairs
	 */
};

}} // boostLLtries
#endif // BOOST_TRIE_HPP
