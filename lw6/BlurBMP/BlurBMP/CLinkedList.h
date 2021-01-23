#pragma once
#include <iterator>
#include <string>
#include <memory>
#include <functional>

using namespace std;

template <typename T>
class CLinkedList
{
	struct Node
	{
		Node(const T& data, Node* prev, std::unique_ptr<Node>&& next)
			: data(data), prev(prev), next(std::move(next))
		{
		}
		T data;
		Node* prev;
		std::unique_ptr<Node> next;
	};
public:
	CLinkedList();
	size_t GetSize()const;
	void Append(const T& data);
	void AddToFront(const T& data);
	bool IsEmpty()const;
	void Clear();

	CLinkedList& operator=(const CLinkedList& other);
	CLinkedList& operator=(CLinkedList&& other);
	class CIterator : public std::iterator<std::bidirectional_iterator_tag, Node, ptrdiff_t>
	{
		friend CLinkedList<T>;
		CIterator(Node* node, bool isReverse = false);
	public:
		friend bool const operator==(typename CLinkedList<T>::CIterator const& it1, typename CLinkedList<T>::CIterator const& it2)
		{
			return it1.m_node == it2.m_node;
		}
		friend bool const operator!=(typename CLinkedList<T>::CIterator const& it1, typename CLinkedList<T>::CIterator const& it2)
		{
			return !(it1 == it2);
		}
		CIterator() = default;
		T& operator*()const;
		CIterator& operator++();
		CIterator& operator--();
	private:
		Node* m_node = nullptr;
		bool m_isReverse = false;

	};

	CIterator begin();
	CIterator end();
	const CIterator cbegin()const;
	const CIterator cend()const;

	CIterator rbegin();
	CIterator rend();
	const CIterator crbegin()const;
	const CIterator crend()const;

	CIterator Insert(CIterator& iter, T const& data);
	CIterator Erase(CIterator& iter);
	T& GetBackElement();
	T& GetFrontElement();
	T const& GetBackElement()const;
	T const& GetFrontElement()const;

	~CLinkedList();
private:
	void ResetList();
	size_t m_size = 0;
	std::unique_ptr<Node> m_imaginaryLastItem = nullptr;
	std::unique_ptr<Node> m_imaginaryFirstItem = nullptr;
	Node* m_firstNode = nullptr;
	Node* m_lastNode = nullptr;
};

template <typename T>
CLinkedList<T>::CLinkedList()
{
}

template <typename T>
CLinkedList<T>::~CLinkedList()
{
	while (m_imaginaryFirstItem)
	{
		auto tmp = move(m_imaginaryFirstItem->next);
		m_imaginaryFirstItem.reset();
		m_imaginaryFirstItem = move(tmp);
	}
	m_lastNode = nullptr;
}

template<typename T>
inline void CLinkedList<T>::ResetList()
{
	m_imaginaryFirstItem = make_unique<Node>(T(), nullptr, make_unique<Node>(T(), nullptr, nullptr));
	m_firstNode = m_imaginaryFirstItem->next.get();
	m_lastNode = m_firstNode;
	m_imaginaryLastItem = make_unique<Node>(T(), m_lastNode, nullptr);
	m_lastNode->next = move(m_imaginaryLastItem);
}


template <typename T>
size_t CLinkedList<T>::GetSize() const
{
	return m_size;
}

template <typename T>
void CLinkedList<T>::Append(const T& data)
{
	unique_ptr<Node> newNode;
	try
	{
		if (IsEmpty())
		{
			ResetList();
		}
		newNode = make_unique<Node>(data, m_lastNode, nullptr);
	}
	catch (...)
	{
		throw;
	}
	Node* newLastNode = newNode.get();
	m_imaginaryLastItem = move(m_lastNode->next);
	m_imaginaryLastItem->prev = newLastNode;
	if (m_lastNode->prev)
	{
		newNode->next = move(m_imaginaryLastItem);
		m_lastNode->next = move(newNode);
	}
	else
	{
		newNode->next = move(m_imaginaryLastItem);
		m_imaginaryFirstItem->next = move(newNode);
		m_firstNode = m_imaginaryFirstItem->next.get();
		m_firstNode->prev = m_imaginaryFirstItem.get();
	}
	m_lastNode = newLastNode;
	++m_size;
}

template <typename T>
void CLinkedList<T>::AddToFront(const T& data)
{
	std::unique_ptr<Node> newNode;
	try
	{
		if (IsEmpty())
		{
			ResetList();
		}
		newNode = make_unique<Node>(data, nullptr, nullptr);
	}
	catch (...)
	{
		throw;
	}
	if (m_size != 0)
	{
		m_firstNode->prev = newNode.get();
		newNode->next = move(m_imaginaryFirstItem->next);
	}
	else
	{
		m_imaginaryLastItem = move(m_lastNode->next);
		m_lastNode = newNode.get();
		m_imaginaryLastItem->prev = m_lastNode;
		m_lastNode->next = move(m_imaginaryLastItem);
	}
	newNode->prev = m_imaginaryFirstItem.get();
	m_imaginaryFirstItem->next = move(newNode);
	m_firstNode = m_imaginaryFirstItem->next.get();
	++m_size;
}

template <typename T>
bool CLinkedList<T>::IsEmpty()const
{
	return m_size == 0;
}

template <typename T>
void CLinkedList<T>::Clear()
{
	while (m_imaginaryFirstItem)
	{
		auto tmp = move(m_imaginaryFirstItem->next);
		m_imaginaryFirstItem.reset();
		m_imaginaryFirstItem = move(tmp);
	}
	m_size = 0;
	ResetList();
}

template <typename T>
T& CLinkedList<T>::GetBackElement()
{
	if (IsEmpty())
	{
		throw runtime_error("List is empty.");
	}

	return  m_lastNode->data;
}

template <typename T>
T& CLinkedList<T>::GetFrontElement()
{
	if (IsEmpty())
	{
		throw runtime_error("List is empty.");
	}

	return m_firstNode->data;
}

template <typename T>
T const& CLinkedList<T>::GetBackElement()const
{
	if (IsEmpty())
	{
		throw runtime_error("List is empty.");
	}

	return  m_lastNode->data;
}

template <typename T>
T const& CLinkedList<T>::GetFrontElement()const
{
	if (IsEmpty())
	{
		throw runtime_error("List is empty.");
	}

	return m_firstNode->data;
}

template <typename T>
CLinkedList<T>::CIterator::CIterator(Node* node, bool isReverse)
	: m_node(node)
	, m_isReverse(isReverse)

{
};

template<typename T>
CLinkedList<T>& CLinkedList<T>::operator=(CLinkedList&& other)
{

}

template<typename T>
CLinkedList<T>& CLinkedList<T>::operator=(const CLinkedList& other)
{
	if (!other.IsEmpty())
	{
		CLinkedList<T> lst;
		try
		{
			lst.ResetList();
			lst.m_imaginaryLastItem = move(lst.m_lastNode->next);
			auto tempPtr = other.m_firstNode;
			lst.m_firstNode->data = tempPtr->data;
			auto current = lst.m_firstNode;
			while (tempPtr->next->next)
			{
				tempPtr = tempPtr->next.get();
				current->next = make_unique<Node>(tempPtr->data, current, nullptr);
				current = current->next.get();
			}
			lst.m_imaginaryLastItem->prev = current;
			current->next = move(lst.m_imaginaryLastItem);
			lst.m_lastNode = current;
			lst.m_size = other.m_size;
		}
		catch (...)
		{
			throw;
		}
		Clear();
		ResetList();
		m_imaginaryLastItem = move(m_lastNode->next);
		m_imaginaryFirstItem->next = std::move(lst.m_imaginaryFirstItem->next);
		m_firstNode = m_imaginaryFirstItem->next.get();
		m_lastNode = lst.m_lastNode;
		m_imaginaryLastItem->prev = m_lastNode;
		m_lastNode->next = move(m_imaginaryLastItem);
		m_size = lst.m_size;
	}
	else
	{
		Clear();
	}
	return *this;
}

template <typename T>
T& CLinkedList<T>::CIterator::operator*()const
{
	if (!m_node || (!m_node->next))
	{
		throw std::runtime_error("Iterator as invalid.");
	}

	return m_node->data;
}

template <typename T>
typename CLinkedList<T>::CIterator& CLinkedList<T>::CIterator::operator++()
{
	if (!m_node || (!m_node->next && !m_isReverse) || (!m_node->prev && m_isReverse))
	{
		throw (std::runtime_error("Iterator has not icrementable."));
	}
	m_isReverse ? m_node = m_node->prev : m_node = m_node->next.get();

	return *this;
}

template <typename T>
typename CLinkedList<T>::CIterator& CLinkedList<T>::CIterator::operator--()
{
	if (!m_node || (!m_node->prev && !m_isReverse) || (!m_node->next && m_isReverse))
	{
		throw (std::runtime_error("Iterator has not decrementable."));
	}
	m_isReverse ? m_node = m_node->next.get() : m_node = m_node->prev;

	return *this;
}

template <typename T>
typename CLinkedList<T>::CIterator CLinkedList<T>::begin()
{
	return CIterator(m_firstNode);
}

template <typename T>
typename CLinkedList<T>::CIterator CLinkedList<T>::end()
{
	return m_lastNode ? CIterator(m_lastNode->next.get()) : CIterator(m_imaginaryLastItem.get());
}

template <typename T>
typename const CLinkedList<T>::CIterator CLinkedList<T>::cbegin()const
{
	return CIterator(m_firstNode);
}

template <typename T>
typename const CLinkedList<T>::CIterator CLinkedList<T>::cend()const
{
	return CIterator(m_lastNode->next.get());
}

template <typename T>
typename CLinkedList<T>::CIterator CLinkedList<T>::rbegin()
{
	return CIterator(m_lastNode, true);
}

template <typename T>
typename CLinkedList<T>::CIterator CLinkedList<T>::rend()
{
	return CIterator(m_imaginaryFirstItem.get(), true);
}

template <typename T>
typename const CLinkedList<T>::CIterator CLinkedList<T>::crbegin()const
{
	return CIterator(m_lastNode, true);
}

template <typename T>
typename const CLinkedList<T>::CIterator CLinkedList<T>::crend()const
{
	return CIterator(m_imaginaryFirstItem.get(), true);
}

template <typename T>
typename CLinkedList<T>::CIterator CLinkedList<T>::Insert(CIterator& iter, T const& data)
{
	auto iterNode = iter.m_node;
	if (iterNode == m_firstNode)
	{
		AddToFront(data);
	}
	else if ((iterNode == m_lastNode->next.get()))
	{
		Append(data);
	}
	else
	{
		if (!iterNode)
		{
			throw invalid_argument("Iterator is not valid.");
		}
		unique_ptr<Node> newNode;
		try
		{
			newNode = make_unique<Node>(data, move(iterNode->prev)
				, move(make_unique<Node>(iterNode->data, nullptr, move(iterNode->next))));
		}
		catch (...)
		{
			if (newNode->prev)
			{
				iterNode->prev = move(newNode->prev);
			}
			if (newNode->next)
			{
				iterNode->next = move(newNode->next->next);
			}
			throw;
		}
		newNode->next->prev = newNode.get();
		iter.m_node = newNode->next.get();
		newNode->prev->next = move(newNode);
		++m_size;
	}
	return iter;
}

template <typename T>
typename CLinkedList<T>::CIterator CLinkedList<T>::Erase(CIterator& iter)
{
	if (!(iter.m_node) || (IsEmpty()) || (iter.m_node == m_lastNode->next.get()))
	{
		throw runtime_error("Dont erase it, because iter is null.");
	}
	if (iter.m_node == m_firstNode)
	{
		if (m_size > 1)
		{
			m_imaginaryFirstItem->next = move(m_firstNode->next);
			m_firstNode = m_imaginaryFirstItem->next.get();
			m_firstNode->prev = m_imaginaryFirstItem.get();
			iter = begin();
		}
		else
		{
			ResetList();
			iter = end();
		}
	}
	else if (iter.m_node == m_lastNode)
	{
		m_imaginaryLastItem = move(m_lastNode->next);
		m_lastNode = m_lastNode->prev;
		m_lastNode->next = move(m_imaginaryLastItem);
		iter = m_lastNode->next.get();
	}
	else
	{
		auto next = move(iter.m_node->next);
		auto prev = iter.m_node->prev;
		next->prev = prev;
		prev->next = move(next);
		iter = prev->next.get();
	}
	--m_size;

	return iter;
}